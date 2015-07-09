/*
 * The main purpose of this .cpp file is to support snmp 1.3.6.1.2.1.1 system node.
 *
 */

#include "SystemInclude.h"
#include "BcmController.h"

#include <ifaddrs.h> //getifaddrs()
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>  //IFNAMSIZ

#include <time.h>
#include <sys/sysinfo.h>
#include "Debug.h"
#include "SaveCfgFile.h"
#include "ChannelMgt.h"
#include "StatisticsMgt.h"
#include "SystemInfoMgt.h"

using namespace std;

#define SystemDescr    "Gospell CMC v.11.05"
#define SystemContact  "who@where"
#define SystemName     "Cable Media Converter"
#define SystemLocation "China"

//edit by leonli

#define MAX_TIMER_NUMBER 100

#define CPU_CHECK_FUNC 0x01
#define RAM_CHECK_FUNC 0x02
#define SNR_CHECK_FUNC 0x03
#ifdef SYS_TEMPERATURE_ALARM
#define TEMPERATURE_CHECK_FUNC 0x04
#endif

#define DEFAULT_CPU_USING 0.5
#define DEFAULT_RAM_USING 50
#define DEFAULT_US_SNR 40
#ifdef SYS_TEMPERATURE_ALARM
#define DEFAULT_TEMPERATURE 45
#endif

int i=0,t=1;
float max_cpu_using = DEFAULT_CPU_USING;
float cpu_using_num = 0;
float last_time_cpu_using_num = 0;

unsigned long max_ram_using = DEFAULT_RAM_USING;
unsigned long ram_using = 0;
unsigned long last_time_ram_using = 0;


#ifdef SYS_TEMPERATURE_ALARM
unsigned int max_temperature = DEFAULT_TEMPERATURE;
unsigned int temperature = 0;
#endif

uint_t rebootLevel = RebootLevelRebootSw;

struct Timer {
    int total_time;
    int left_time;
    int func;
} SystemCheckTimer[MAX_TIMER_NUMBER];

//edit by leonli
//add a callback function alerm to mgt layer
//used for snmp trap
AlarmCpuUsingHandlerPoint theAlarmCpuUsingHandler = NULL;
CpuUsingInfoT CpuUsingInfoT_Entry;

AlarmRamUsingHandlerPoint theAlarmRamUsingHandler = NULL;
RamUsingInfoT RamUsingInfoT_Entry;

AlarmUsSigHandlerPoint theAlarmUsSigHandler = NULL;
UsSignalQualityAlarmT UsSigAlarmEntry;
unsigned int last_time_snr[4] = {0,0,0,0};


#ifdef SYS_TEMPERATURE_ALARM
AlarmTemperatureHandlerPoint theAlarmTemperatureHandler = NULL;
TemperatureT TemperatureAlarmEntry;
#endif

int RegisterCpuUsingCallbackMgt(AlarmCpuUsingHandlerPoint AlarmCpuUsingHandler)
{
    theAlarmCpuUsingHandler = AlarmCpuUsingHandler;
    return 0;
}

int RegisterRamUsingCallbackMgt(AlarmRamUsingHandlerPoint AlarmRamUsingHandler)
{
    theAlarmRamUsingHandler = AlarmRamUsingHandler;
    return 0;
}

int RegisterUsSigCallbackMgt(AlarmUsSigHandlerPoint AlarmUsSigHandler)
{
    theAlarmUsSigHandler = AlarmUsSigHandler;
    return 0;
}

#ifdef SYS_TEMPERATURE_ALARM
int RegisterTemperatureCallbackMgt(AlarmTemperatureHandlerPoint AlarmTemperatureHandler)
{
    theAlarmTemperatureHandler = AlarmTemperatureHandler;
    return 0;
}
#endif
/*************************************************************************/

//system temperature api
//get cpu using function
#ifdef SYS_TEMPERATURE_ALARM
unsigned int GetSysTempreture()
{
    unsigned int now_temperature = 36;
    //need new hardware
    return now_temperature;
}

BASE_ErrCode SetMaxSysTempretureMgt(unsigned int set_max_sys_temperature)
{
    max_temperature = set_max_sys_temperature;
    return BASE_OK;
}

BASE_ErrCode GetMaxSysTempretureMgt(unsigned int *get_max_sys_temperature)
{
    assert(get_max_sys_temperature);
    *get_max_sys_temperature = max_temperature;
    return BASE_OK;
}

BASE_ErrCode SaveMaxSysTempretureMgt()
{
    unsigned int get_max_sys_temperature;
    ostringstream os;

    os.str("");
    if(GetMaxSysTempretureMgt(&get_max_sys_temperature) < 0)
        return BASE_ERROR;

    if(get_max_sys_temperature == DEFAULT_TEMPERATURE)
        return BASE_OK;

    os << "set max system temperature " << (float)get_max_sys_temperature << endl;

    if(SaveCfg(os.str().c_str() ,CONTINUE )<0) {
        dbgstrm << "save error"<< endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

//used for higher layer call such as cli or snmp
BASE_ErrCode GetSysTempretureMgt(int *get_sys_temperature)
{
    assert(get_sys_temperature);
    *get_sys_temperature = temperature;
    return BASE_OK;
}
#endif
/*************************************************************************/

//cpu using api
//get cpu using function
float GetCpuUsage()
{
    FILE *fp;
    char buf[128];
    char cpu[16];
    long int user,nice,sys,idle,iowait,irq,softirq;

    long int all1,all2,idle1,idle2;
    float usage = 0;

    fp = fopen("/proc/stat","r");
    if(fp == NULL) {
        fprintf(stderr, "Failed to open /proc/stat \n");
        return 0.0;
    }

    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

    all1 = user+nice+sys+idle+iowait+irq+softirq;
    idle1 = idle;
    rewind(fp);

    sleep(1);
    memset(buf,0,sizeof(buf));
    cpu[0] = '\0';
    user=nice=sys=idle=iowait=irq=softirq=0;
    fgets(buf,sizeof(buf),fp);

    sscanf(buf,"%s%ld%ld%ld%ld%ld%ld%ld",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

    all2 = user+nice+sys+idle+iowait+irq+softirq;
    idle2 = idle;

    //usage = (float)((all2-idle2)-(all1-idle1)) / (all2-all1) * 100;
    usage = (float)((all2-idle2)-(all1-idle1)) / (all2-all1);
    //printf("all2=%ld, idle2=%ld, all1=%ld, idle1=%ld, usage=%f\n",all2, idle2, all1, idle2, usage);

    fclose(fp);
    return usage;
}

BASE_ErrCode SetMaxCpuUsingMgt(float set_max_cpu_using_number)
{
    max_cpu_using = set_max_cpu_using_number;
    return BASE_OK;
}

BASE_ErrCode GetMaxCpuUsingMgt(float *get_max_cpu_using_number)
{
    assert(get_max_cpu_using_number);
    *get_max_cpu_using_number = max_cpu_using;
    return BASE_OK;
}

BASE_ErrCode SaveMaxCpuUsingMgt()
{
    float get_max_cpu_using_number;
    ostringstream os;

    os.str("");
    if(GetMaxCpuUsingMgt(&get_max_cpu_using_number) < 0)
        return BASE_ERROR;

    if(get_max_cpu_using_number == DEFAULT_CPU_USING)
        return BASE_OK;

    os << "set max cpu using " << (float)get_max_cpu_using_number << endl;

    if(SaveCfg(os.str().c_str() ,CONTINUE )<0) {
        dbgstrm << "save error"<< endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

//used for higher layer call such as cli or snmp
BASE_ErrCode GetCpuUsingMgt(float *get_cpu_using_number)
{
    assert(get_cpu_using_number);
    *get_cpu_using_number = cpu_using_num;
    return BASE_OK;
}
/*************************************************************************/

//ram using mgt api
//get ram function
unsigned long GetRamUsage()
{
    unsigned long ram_using = 0;
    struct sysinfo si;
    sysinfo(&si);

    ram_using = (si.totalram - si.freeram)*100 / si.totalram;
    //cout << "The memory using persent is : " << ram_using << " persent" << endl;
    return ram_using;
}

BASE_ErrCode SetMaxRamUsingMgt(unsigned long set_max_ram_using_number)
{
    max_ram_using = set_max_ram_using_number;
    return BASE_OK;
}

BASE_ErrCode GetMaxRamUsingMgt(unsigned long *get_max_ram_using_number)
{
    assert(get_max_ram_using_number);
    *get_max_ram_using_number = max_ram_using;
    return BASE_OK;
}

BASE_ErrCode SaveMaxRamUsingMgt()
{
    unsigned long get_max_ram_using_number;
    ostringstream os;

    os.str("");
    if(GetMaxRamUsingMgt(&get_max_ram_using_number) < 0)
        return BASE_ERROR;

    if(get_max_ram_using_number == DEFAULT_RAM_USING)
        return BASE_OK;

    os << "set max ram using " << get_max_ram_using_number << endl;

    if(SaveCfg(os.str().c_str() ,CONTINUE )<0) {
        dbgstrm << "save error"<< endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

//used for higher layer call such as cli or snmp
BASE_ErrCode GetRamUsingMgt(unsigned long *get_ram_using_number)
{
    assert(get_ram_using_number);
    *get_ram_using_number = ram_using;
    return BASE_OK;
}
/*************************************************************************/

//us channel snr mgt api
//get us snr function
BASE_ErrCode GetUsSigMgt(unsigned int channel, unsigned int* snr)
{
    assert((channel>0)&&(channel<=4));//1.2.3.4
    assert(snr);
    *snr = UsSigAlarmEntry.us_sig[channel-1].snr;
    return BASE_OK;
}

BASE_ErrCode SetMaxUsSigMgt(unsigned int channel, unsigned int max_snr)
{
    assert((channel>=0)&&(channel<=4));//0.1.2.3.4
    if(channel == 0) { //0
        for(int i = 0; i < 4; i++)
            UsSigAlarmEntry.us_sig[i].max_snr = max_snr;
    } else //1.2.3.4
        UsSigAlarmEntry.us_sig[channel-1].max_snr = max_snr;

    return BASE_OK;
}

BASE_ErrCode GetMaxUsSigMgt(unsigned int channel, unsigned int* max_snr)
{
    assert((channel > 0)&&(channel<=4));//1.2.3.4
    assert(max_snr);
    *max_snr = UsSigAlarmEntry.us_sig[channel -1].max_snr;
    return BASE_OK;
}

BASE_ErrCode SaveUsSigMgt()
{
    unsigned int max_snr;
    ostringstream os;

    os.str("");
    for(int i = 1; i <= 4; i++) {
        if(GetMaxUsSigMgt(i, &max_snr) < 0)
            return BASE_ERROR;

        if(max_snr == DEFAULT_US_SNR)
            continue;

        os << "set max us channel " << i  << " snr_db " << max_snr << endl;
    }

    if(SaveCfg(os.str().c_str() ,CONTINUE )<0) {
        dbgstrm << "save error"<< endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}
/*************************************************************************/

//time check and alarm pthread
void setTimer(int t,int f)
{
    struct Timer new_timer_entry;
    new_timer_entry.total_time=t;
    new_timer_entry.left_time=t;
    new_timer_entry.func=f;
    if(i < MAX_TIMER_NUMBER)
        SystemCheckTimer[i++] = new_timer_entry;
    else
        printf("No timer to user\n");
}

void timeout()
{
    int j;
    int alarm_flag = 0;
    for(j=0; j<i; j++) {
        if(SystemCheckTimer[j].left_time!=0)
            SystemCheckTimer[j].left_time--;
        else {
            switch(SystemCheckTimer[j].func) {
                case CPU_CHECK_FUNC:
                    cpu_using_num = GetCpuUsage();
                    //--
                    if(cpu_using_num < max_cpu_using) {
                        if(last_time_cpu_using_num > max_cpu_using)
                            alarm_flag = 1;//alarm
                    } else {
                        //++
                        if(cpu_using_num > max_cpu_using) {
                            if(last_time_cpu_using_num < max_cpu_using)
                                alarm_flag = 1;//alarm
                        }
                    }
                    last_time_cpu_using_num = cpu_using_num;
                    if((theAlarmCpuUsingHandler != NULL)&&(alarm_flag == 1)) {
                        CpuUsingInfoT_Entry.cpu_using = cpu_using_num;
                        theAlarmCpuUsingHandler(&CpuUsingInfoT_Entry);
                    }
                    break;

                case RAM_CHECK_FUNC:
                    ram_using = GetRamUsage();
                    //--
                    if(ram_using < max_ram_using) {
                        if(last_time_ram_using > max_ram_using)
                            alarm_flag = 1;//alarm
                    } else {
                        //++
                        if(ram_using > max_ram_using) {
                            if(last_time_ram_using < max_ram_using)
                                alarm_flag = 1;//alarm
                        }
                    }
                    last_time_ram_using = ram_using;
                    if((theAlarmRamUsingHandler != NULL)&&(alarm_flag == 1)) {
                        RamUsingInfoT_Entry.ram_using = cpu_using_num;
                        theAlarmRamUsingHandler(&RamUsingInfoT_Entry);
                    }
                    break;

                case SNR_CHECK_FUNC: {
                    BASE_ErrCode ret;
                    unsigned char flag = 0;
                    int i =0;
                    CMCOAM_UpstreamIfcQualT UsSigdata;
                    UsSignalQualityT * UsSigAlarmPoint = UsSigAlarmEntry.us_sig;

                    //cout << "\n+++++++++++++++++++++++++++++++++++++++++++++" << endl;
                    for (ret = MgtChannelGetFirstUsSignalQuality(&UsSigdata);ret == BASE_OK;ret = MgtChannelGetNextUsSignalQuality(&UsSigdata, &UsSigdata)) 
                    {
                        i++;
                        UsSigAlarmPoint->snr = UsSigdata.snr;
                        //cout << "Us Signal Check channel " << (unsigned int)UsSigAlarmPoint->ch_id  << " snr : " << (float)(UsSigAlarmPoint->snr/10.0) << " max snr : " << UsSigAlarmPoint->max_snr << endl;

                        if(UsSigAlarmPoint->snr < UsSigAlarmPoint->max_snr) {
                            if(last_time_snr[i] > UsSigAlarmPoint->max_snr)
                            {
                                if(flag == 0)
                                    flag = 1;//alarm
                            }
                        } else {
                            //++
                            if(UsSigAlarmPoint->snr > UsSigAlarmPoint->max_snr) {
                                if(last_time_snr[i] < UsSigAlarmPoint->max_snr)
                                {
                                    if(flag == 0)
                                        flag = 1;//alarm
                                }
                            }
                        }
                        last_time_snr[i] = UsSigAlarmPoint->snr ;
                        ++UsSigAlarmPoint;
                    }
                    
                    if((theAlarmUsSigHandler != NULL)&&(flag == 1)) {
                        theAlarmUsSigHandler(&UsSigAlarmEntry);
                    }
                }
                break;

#ifdef SYS_TEMPERATURE_ALARM
                case TEMPERATURE_CHECK_FUNC:
                    temperature = GetSysTempreture();
                    if(temperature > max_temperature) {
                        TemperatureAlarmEntry. temperature= temperature;
                        if(theAlarmTemperatureHandler != NULL)
                            theAlarmTemperatureHandler(&TemperatureAlarmEntry);
                    }
                    break;
#endif
            }
            SystemCheckTimer[j].left_time=SystemCheckTimer[j].total_time;
        }
    }
}

static void* SystemInfoUtilization(void*)
{
    setTimer(5,CPU_CHECK_FUNC);
    setTimer(5,RAM_CHECK_FUNC);
    setTimer(5,SNR_CHECK_FUNC);
#ifdef SYS_TEMPERATURE_ALARM
    setTimer(5,TEMPERATURE_CHECK_FUNC);
#endif
    memset(&UsSigAlarmEntry, 0, sizeof(UsSignalQualityAlarmT));
    for(int i = 0; i < 4 ; i++) {
        UsSigAlarmEntry.us_sig[i].ch_id = i+1;
        UsSigAlarmEntry.us_sig[i].max_snr = DEFAULT_US_SNR;//default max snr 35 db
    }

    while(1) {
        sleep(1);
        timeout();
    }
    exit(0);
}

void StartSystemInfoCherk()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, SystemInfoUtilization, NULL);
}
/*************************************************************************/

//reboot level api
uint_t GetRebootLevel()
{
    return rebootLevel;
}

void SetRebootLevel(uint_t level)
{
    assert(level >= RebootLevelNoReboot && level <= RebootLevelRebootCmc);
    rebootLevel = level;
}

BASE_ErrCode SaveRebootLevelConfig()
{
    ostringstream os;

    os << "reboot_level  " << rebootLevel << endl;
    SaveCfg(os.str().c_str(), InsertToEnd);
    return BASE_OK;
}
/*************************************************************************/

//system info
void GetSystemDescr(char* sysDescr, size_t len)
{
    strncpy(sysDescr, SystemDescr, len);
}

void GetSystemContact(char* sysContact, size_t len)
{
    strncpy(sysContact, SystemContact, len);
}

void GetSystemName(char* sysName, size_t len)
{
    strncpy(sysName, SystemName, len);
}

void GetSystemLocation(char* sysLocation, size_t len)
{
    strncpy(sysLocation, SystemLocation, len);
}

int GetSystemServices()
{
    return 3;
}

BASE_ErrCode GetIntfAddrInfo(const char* ifName,
                             struct in_addr* ip, struct in_addr* mask,
                             uint8_t* mac)
{
    struct ifaddrs * ifAddr = NULL;

    assert(!(ip == NULL && mask == NULL && mac == NULL));

    int socketFd = socket(AF_INET,SOCK_STREAM,0);
    assert(socketFd > 0);

    if (getifaddrs(&ifAddr) < 0)
        return BASE_ERROR;

    for (; ifAddr != NULL; ifAddr = ifAddr->ifa_next) {
        if (strcmp(ifAddr->ifa_name, ifName) != 0) {
            continue;
        }

        // check ipv4 address only.
        if (ifAddr->ifa_addr->sa_family != AF_INET) {
            continue;
        }

        if (ip != NULL) {
            *ip = ((struct sockaddr_in *)ifAddr->ifa_addr)->sin_addr;
        }

        if (mask != NULL) {
            *mask = ((struct sockaddr_in *)ifAddr->ifa_netmask)->sin_addr;
        }

        if (mac != NULL) {
            struct ifreq ifreq;
            strncpy(ifreq.ifr_name, ifAddr->ifa_name, IFNAMSIZ);
            if(ioctl(socketFd, SIOCGIFHWADDR, &ifreq) < 0) {
                return BASE_ERROR;
            }

            memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
        }
        return BASE_OK;
    }

    return BASE_ERROR;
}


BASE_ErrCode GetFirstInterface(InterfaceInfo* intf)
{
    assert(intf != NULL);
    dbgstrm << "Start." << endl;
    intf->ifIndex = 1;
    strncpy(intf->ifDescr, "vlan1", sizeof(intf->ifDescr));
    intf->ifType = 6;
    intf->ifMtu  = 1500;
    intf->ifSpeed = 1000 * 1000 * 1000;
    GetIntfAddrInfo("vlan1", NULL, NULL, intf->ifPhysAddress);
    intf->ifAdminStatus = 1;
    intf->ifOperStatus = 1;
    intf->ifLastChange = 0;
    intf->ifOutQLen = 1000;
    WAN_PORT_STAT_CFG stat;
    BASE_ErrCode errCode = MgtGetWanPortStat(&stat);
    if(errCode != BASE_OK) {
        errstrm << "MgtGetWanPortStat() failed." << endl;
    }
    intf->ifInOctets = static_cast<uint32_t>(stat.snmpIfInOctets);
    intf->ifInUcastPkts = static_cast<uint32_t>(stat.snmpIfInUcastPkts);
    intf->ifInNUcastPkts = static_cast<uint32_t>(stat.snmpIfInNUcastPkts);
    intf->ifInDiscards = static_cast<uint32_t>(stat.snmpIfInDiscards);
    intf->ifInErrors = static_cast<uint32_t>(stat.snmpIfInErrors);
    intf->ifInUnknownProtos = static_cast<uint32_t>(stat.snmpIfInUnknownProtos);
    intf->ifOutOctets = static_cast<uint32_t>(stat.snmpIfOutOctets);
    intf->ifOutUcastPkts = static_cast<uint32_t>(stat.snmpIfOutUcastPkts);
    intf->ifOutNUcastPkts = static_cast<uint32_t>(stat.snmpIfOutNUcastPkts);
    intf->ifOutDiscards = static_cast<uint32_t>(stat.snmpIfOutDiscards);
    intf->ifOutErrors = static_cast<uint32_t>(stat.snmpIfOutErrors);

    return BASE_OK;
}

BASE_ErrCode GetNextInterface(int ifIndex, InterfaceInfo* intf)
{
    return BASE_ERROR;
}

BASE_ErrCode FindInterface(int ifIndex, InterfaceInfo* intf)
{
    if (ifIndex == 1) {
        return GetFirstInterface(intf);
    }

    return BASE_ERROR;
}
/*************************************************************************/

static list<SyslogServer> syslogServers;
BASE_ErrCode AddSyslogServer(SyslogServer const* server)
{
    list<SyslogServer>::iterator iter;
    if (syslogServers.size() >= MaxSyslogServer)
    {
        return BASE_ERROR;
    }

    for (iter = syslogServers.begin(); iter != syslogServers.end(); ++iter)
    {
        if (server->addr.s_addr == iter->addr.s_addr)
        {
            /* entry existed before. */
            return BASE_ERROR;
        }
    }

    syslogServers.push_back(*server);
    return BASE_OK;
}

BASE_ErrCode DelSyslogServer(struct in_addr addr)
{
    list<SyslogServer>::iterator iter;
    for (iter = syslogServers.begin(); iter != syslogServers.end(); ++iter)
    {
        if (addr.s_addr == iter->addr.s_addr)
        {
            syslogServers.erase(iter);
            return BASE_OK;
        }
    }
    return BASE_ERROR;
}

BASE_ErrCode UpdateSyslogServer(SyslogServer const* server)
{
    list<SyslogServer>::iterator iter;
    for (iter = syslogServers.begin(); iter != syslogServers.end(); ++iter)
    {
        if (server->addr.s_addr == iter->addr.s_addr)
        {
            iter->port = server->port;
            return BASE_OK;
        }
    }
    return BASE_ERROR;
}

BASE_ErrCode FindSyslogServer(SyslogServer *server)
{
    list<SyslogServer>::iterator iter;
    for (iter = syslogServers.begin(); iter != syslogServers.end(); ++iter)
    {
        if (server->addr.s_addr == iter->addr.s_addr)
        {
            server->port = iter->port;
            return BASE_OK;
        }
    }
    return BASE_ERROR;
}

BASE_ErrCode GetFirstSyslogServer(SyslogServer* server)
{
    list<SyslogServer>::iterator iter;
    iter = syslogServers.begin();
    if (iter != syslogServers.end())
    {
        server->addr.s_addr = iter->addr.s_addr;
        server->port = iter->port;
        return  BASE_OK;
    }
    return BASE_ERROR;
}

BASE_ErrCode GetNextSyslogServer(SyslogServer* current, SyslogServer* next)
{
    list<SyslogServer>::iterator iter;

    for (iter = syslogServers.begin();
        iter != syslogServers.end();
        ++iter)
    {
        if (iter->addr.s_addr == current->addr.s_addr)
        {
            iter++;
            break;
        }
    }
    if (iter != syslogServers.end())
    {
        next->addr.s_addr = iter->addr.s_addr;
        next->port = iter->port;
        return BASE_OK;
    }

    return BASE_ERROR;
}

BASE_ErrCode SaveSyslogConfig()
{
    ostringstream os;

    if (syslogServers.size() == 0)
        return BASE_OK;

    list<SyslogServer>::iterator iter;
    for (iter = syslogServers.begin(); iter != syslogServers.end(); ++iter)
    {
        os << "syslog " << inet_ntoa(iter->addr)
            << " " << iter->port << endl;
    }

    SaveCfg(os.str().c_str(), InsertToEnd);

    return BASE_OK;
}

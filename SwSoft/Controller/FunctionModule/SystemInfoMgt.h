#ifndef _SystemInfoMgt_h_
#define _SystemInfoMgt_h_

#include <net/if.h>  //IFNAMSIZ

#define RebootLevelNoReboot   0
#define RebootLevelRebootSw   1
#define RebootLevelRebootCmc  2

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//#define SYS_TEMPERATURE_ALARM

#ifdef SYS_TEMPERATURE_ALARM 
BASE_ErrCode SetMaxSysTempretureMgt(unsigned int set_max_sys_temperature);
BASE_ErrCode GetMaxSysTempretureMgt(unsigned int *get_max_sys_temperature);
BASE_ErrCode SaveMaxSysTempretureMgt();
BASE_ErrCode GetSysTempretureMgt(int *get_sys_temperature);
#endif

BASE_ErrCode SetMaxCpuUsingMgt(float set_max_cpu_using_number);
BASE_ErrCode GetMaxCpuUsingMgt(float *get_max_cpu_using_number);
BASE_ErrCode GetCpuUsingMgt(float *get_cpu_using_number);
BASE_ErrCode SaveMaxCpuUsingMgt();

BASE_ErrCode SetMaxRamUsingMgt(unsigned long set_max_ram_using_number);
BASE_ErrCode GetMaxRamUsingMgt(unsigned long *get_max_ram_using_number);
BASE_ErrCode GetRamUsingMgt(unsigned long *get_ram_using_number);
BASE_ErrCode SaveMaxRamUsingMgt();

BASE_ErrCode GetUsSigMgt(unsigned int channel, unsigned int* snr);
BASE_ErrCode SetMaxUsSigMgt(unsigned int channel, unsigned int max_snr);
BASE_ErrCode GetMaxUsSigMgt(unsigned int channel, unsigned int* max_snr);
BASE_ErrCode SaveUsSigMgt();

void GetSystemDescr(char* sysDescr, size_t len);
void GetSystemContact(char* sysContact, size_t len);
void GetSystemName(char* sysName, size_t len);
void GetSystemLocation(char* sysLocation, size_t len);
int GetSystemServices();

BASE_ErrCode GetIntfAddrInfo(const char* ifName,
                             struct in_addr* ip, struct in_addr* mask,
                             uint8_t* mac);

typedef uint64_t        Counter;
typedef uint_t          TimeTicks;

/* mib2_ifEntry */
struct InterfaceInfo
{
    int32_t        ifIndex;    /* ifEntry 1 */
    char           ifDescr[IFNAMSIZ];    /* ifEntry 2 */
    int32_t        ifType;     /* ifEntry 3 */
    int32_t        ifMtu;      /* ifEntry 4 */
    uint32_t       ifSpeed;    /* ifEntry 5 */
    uchar_t        ifPhysAddress[6];      /* ifEntry 6 */
    int32_t        ifAdminStatus;      /* ifEntry 7 */
    int32_t        ifOperStatus;       /* ifEntry 8 */
    TimeTicks      ifLastChange;       /* ifEntry 9 */
    Counter        ifInOctets; /* ifEntry 10 */
    Counter        ifInUcastPkts;      /* ifEntry 11 */
    Counter        ifInNUcastPkts;     /* ifEntry 12 */
    Counter        ifInDiscards;       /* ifEntry 13 */
    Counter        ifInErrors; /* ifEntry 14 */
    Counter        ifInUnknownProtos;  /* ifEntry 15 */
    Counter        ifOutOctets;        /* ifEntry 16 */
    Counter        ifOutUcastPkts;     /* ifEntry 17 */
    Counter        ifOutNUcastPkts;    /* ifEntry 18 */
    Counter        ifOutDiscards;      /* ifEntry 19 */
    Counter        ifOutErrors;        /* ifEntry 20 */
    uint32_t       ifOutQLen;          /* ifEntry 21 */
};

BASE_ErrCode GetFirstInterface(struct InterfaceInfo* intf);
BASE_ErrCode GetNextInterface(int index, struct InterfaceInfo* intf);
BASE_ErrCode FindInterface(int index, struct InterfaceInfo* intf);

//edit by leonli
void StartSystemInfoCherk();

//edit by leonli
/*add a callback function to add  cm ip to Mgt layer*/
typedef struct {
    float   cpu_using;
} CpuUsingInfoT;

typedef struct {
    float   ram_using;
} RamUsingInfoT;

typedef struct
{
	unsigned char	ch_id;		
	unsigned int snr;
	unsigned int max_snr;
} UsSignalQualityT;

typedef struct
{
	UsSignalQualityT	us_sig[4];
}UsSignalQualityAlarmT;

#ifdef SYS_TEMPERATURE_ALARM 
typedef struct {
    unsigned int   temperature;
} TemperatureT;
#endif

typedef int(*AlarmCpuUsingHandlerPoint)(CpuUsingInfoT*);
typedef int(*AlarmRamUsingHandlerPoint)(RamUsingInfoT*);
typedef int(*AlarmUsSigHandlerPoint)(UsSignalQualityAlarmT*);
#ifdef SYS_TEMPERATURE_ALARM 
typedef int(*AlarmTemperatureHandlerPoint)(TemperatureT*);
#endif


int RegisterCpuUsingCallbackMgt(AlarmCpuUsingHandlerPoint AlarmCpuUsingHandler);
int RegisterRamUsingCallbackMgt(AlarmRamUsingHandlerPoint AlarmRamUsingHandler);
int RegisterUsSigCallbackMgt(AlarmUsSigHandlerPoint AlarmUsSigHandler);
#ifdef SYS_TEMPERATURE_ALARM 
int RegisterTemperatureCallbackMgt(AlarmTemperatureHandlerPoint AlarmTemperatureHandler);
#endif


uint_t GetRebootLevel();
void SetRebootLevel(uint_t level);
BASE_ErrCode SaveRebootLevelConfig();


#define MaxSyslogServer 4
#define DefaultSyslogServerPort 154
typedef struct
{
    struct in_addr addr;
    uint32_t       port;
}SyslogServer;

BASE_ErrCode AddSyslogServer(SyslogServer const* server);
BASE_ErrCode DelSyslogServer(struct in_addr addr);
BASE_ErrCode UpdateSyslogServer(SyslogServer const* server);
BASE_ErrCode FindSyslogServer(SyslogServer *server);
BASE_ErrCode GetFirstSyslogServer(SyslogServer* server);
BASE_ErrCode GetNextSyslogServer(SyslogServer* current, SyslogServer* next);

BASE_ErrCode SaveSyslogConfig();

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

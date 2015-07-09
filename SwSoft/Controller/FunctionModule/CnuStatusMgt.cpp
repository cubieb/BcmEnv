#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"
#include "AutoLock.h"
#include "SaveCfgFile.h"
#include "DsQosConfigMgt.h"
#include "CnuStatusDB.h"
#include "CnuStatusMgt.h"

using namespace std;

pthread_mutex_t CnuStatusMgtMutex = PTHREAD_MUTEX_INITIALIZER;

int set_max_cm_online_number = 240;
int cm_online_number =0;

//map cm-mac to cm-ip.
//add entry in dhcp relay module , delete entry when cm offline or dhcp release.
//why add in dhcp relay because some cm don't have ip address when register online.
//this map is used for ds qos bandwidth setting switch field process rules(need dest ip address)
BASE_ErrCode AddMacToIpMapMgt(MacAddressT cnu_mac , U32 cnu_ip)
{
    AutoLock autoLock(CnuStatusMgtMutex);
    if(AddMacToIpMap(cnu_mac, cnu_ip) != BASE_OK) {
        errstrm << "Add cnu info db function fail." <<  endl;
        return BASE_ERROR;
    } else
        return BASE_OK;
}

BASE_ErrCode DeleteMacToIpMapMgt(MacAddressT cnu_mac)
{
    AutoLock autoLock(CnuStatusMgtMutex);
    if(DeleteMacToIpMap(cnu_mac) != BASE_OK) {
        errstrm << "Delete cnu info db function fail." <<  endl;
        return BASE_ERROR;
    } else
        return BASE_OK;
}

BASE_ErrCode SearchCnuMacToIpMapMgt(MacAddressT cnu_mac , U32* cnu_ip)
{
    assert(cnu_ip);
    AutoLock autoLock(CnuStatusMgtMutex);
    U32* cnu_ip_point = NULL;
    if(SearchCnuMacToIpMap(cnu_mac, &cnu_ip_point) != BASE_OK) {
        dbgcstrm<< "Search cnu info db function fail." <<  endl;
        return BASE_ERROR;
    }
    if(cnu_ip_point != NULL)
        *cnu_ip = *cnu_ip_point;
    return BASE_OK;
}

void DumpCnuMacToIpMapMgt()
{
    AutoLock autoLock(CnuStatusMgtMutex);
    DumpCnuMacToIpMap();
}
/**************************************************************************/

//map mac to bandnwidth
//used for stone each cm's bandnwidth
//get first and next used for snmp update table
BASE_ErrCode AddMacToBandnwidthMapMgt(DS_QoS_InfoT* new_ds_qos)
{
    assert(new_ds_qos);
    AutoLock autoLock(CnuStatusMgtMutex);
    DS_QoS_InfoDBT new_ds_qos_db;
    memset(&new_ds_qos_db, 0, sizeof(DS_QoS_InfoDBT));

    new_ds_qos_db.ds_qos_cnu_mac = new_ds_qos->ds_qos_cnu_mac;
    new_ds_qos_db.bandwidth = new_ds_qos->bandwidth;

    if(AddMacToBandnWidthMap(&new_ds_qos_db) != BASE_OK) {
        errstrm << "Add Mac To Bandnwidth Map Mgt fail." <<  endl;
        return BASE_ERROR;
    } else
        return BASE_OK;
}

BASE_ErrCode DeleteMacToBandnwidthMapMgt(MacAddressT cnu_mac)
{
    AutoLock autoLock(CnuStatusMgtMutex);
    if(DeleteMacToBandnWidthMap(cnu_mac) != BASE_OK) {
        errstrm << "Delete Mac To Bandnwidth Map Mgt fail." <<  endl;
        return BASE_ERROR;
    } else
        return BASE_OK;
}

BASE_ErrCode SearchCnuMacToBandnwidthMapMgt(MacAddressT cnu_mac , DS_QoS_InfoT* ds_qos)
{
    assert(ds_qos);
    AutoLock autoLock(CnuStatusMgtMutex);
    DS_QoS_InfoDBT ds_qos_db;
    memset(&ds_qos_db, 0, sizeof(DS_QoS_InfoDBT));

    if(SearchCnuMacToBandnWidthMap(cnu_mac, &ds_qos_db) != BASE_OK) {
        dbgstrm<< "Search Mac To Bandnwidth Map Mgt fail." <<  endl;
        return BASE_ERROR;
    } else {
        ds_qos->ds_qos_cnu_mac = ds_qos_db.ds_qos_cnu_mac;
        ds_qos->bandwidth = ds_qos_db.bandwidth;
    }
    return BASE_OK;
}

void DumpCnuMacToBandnwidthMapMgt()
{
    AutoLock autoLock(CnuStatusMgtMutex);
    DumpCnuMacToBandnWidthMap();
}

BASE_ErrCode GetFirstDsQosMgt(DS_QoS_InfoT* first_ds_qos)
{
    assert(first_ds_qos);
    DS_QoS_InfoDBT ds_qos_db;
    memset(&ds_qos_db, 0, sizeof(DS_QoS_InfoDBT));

    if(GetCmFirstDownStreamQos(&ds_qos_db)<0) {
        dbgstrm << "Get First Qos Fail !" << endl;
        return BASE_ERROR;
    } else {
        memcpy(&(first_ds_qos->ds_qos_cnu_mac), &(ds_qos_db.ds_qos_cnu_mac), sizeof(MacAddressT));
        //first_ds_qos->ds_qos_cnu_mac = ds_qos_db.ds_qos_cnu_mac;
        first_ds_qos->bandwidth = ds_qos_db.bandwidth;
    }

    return BASE_OK;
}

BASE_ErrCode GetNextDsQosMgt(DS_QoS_InfoT* now_ds_qos, DS_QoS_InfoT* next_ds_qos)
{
    assert(now_ds_qos);
    assert(next_ds_qos);
    DS_QoS_InfoDBT now_ds_qos_db,next_ds_qos_db;
    memset(&now_ds_qos_db, 0, sizeof(DS_QoS_InfoDBT));
    memset(&next_ds_qos_db, 0, sizeof(DS_QoS_InfoDBT));

    memcpy(&(now_ds_qos_db.ds_qos_cnu_mac), &(now_ds_qos->ds_qos_cnu_mac), sizeof(MacAddressT));
    //now_ds_qos_db.ds_qos_cnu_mac = now_ds_qos->ds_qos_cnu_mac;
    now_ds_qos_db.bandwidth = now_ds_qos->bandwidth;

    if(GetCmNextDownStreamQos(now_ds_qos_db, &next_ds_qos_db)<0) {
        dbgstrm << "Get Next Qos Fail ! " << endl;
        return BASE_ERROR;
    } else {
        memcpy(&(next_ds_qos->ds_qos_cnu_mac), &(next_ds_qos_db.ds_qos_cnu_mac), sizeof(MacAddressT));
        //next_ds_qos->ds_qos_cnu_mac = next_ds_qos_db.ds_qos_cnu_mac;
        next_ds_qos->bandwidth = next_ds_qos_db.bandwidth;
    }

    return BASE_OK;
}
/**************************************************************************/

//cnu status info function is used for SNMP
//display all cm info detail in snmp
BASE_ErrCode GetFirstCMInfoMgt(CMCOAM_CnuStatusT* FirstCMInfoP)
{
    assert(FirstCMInfoP);
    AutoLock autoLock(CnuStatusMgtMutex);
    CMCOAM_CnuStatusT * CnuStatusdata = NULL;
    U32 ip_address;
    MacAddressT mac_address;

    if(GetFirstCmInfo(&CnuStatusdata) != BASE_OK) {
        return BASE_ERROR;
    }

    if(CnuStatusdata != NULL) {
        ip_address = (CnuStatusdata->ip_addr[0] << 24) | (CnuStatusdata->ip_addr[1] << 16) | (CnuStatusdata->ip_addr[2] << 8) | CnuStatusdata->ip_addr[3];
        if(ip_address == 0) {
            memcpy(mac_address.addr, CnuStatusdata->cnu_mac, 6);
            if(SearchCnuMacToIpMapMgt(mac_address , &ip_address) != BASE_OK)
                return BASE_ERROR;
            else {
                CnuStatusdata->ip_addr[0] = (unsigned char)((ip_address >>24) & 0x000000ff);
                CnuStatusdata->ip_addr[1] = (unsigned char)((ip_address >>16) & 0x000000ff);
                CnuStatusdata->ip_addr[2] = (unsigned char)((ip_address >>8) & 0x000000ff);
                CnuStatusdata->ip_addr[3] = (unsigned char)((ip_address) & 0x000000ff);
            }
        }
        memcpy(FirstCMInfoP, CnuStatusdata, sizeof(CMCOAM_CnuStatusT));
    } else {
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode GetNextCMInfoMgt(CMCOAM_CnuStatusT *NowCMInfoP,CMCOAM_CnuStatusT *NextCMInfoP)
{
    assert(NowCMInfoP);
    assert(NextCMInfoP);
    AutoLock autoLock(CnuStatusMgtMutex);
    CMCOAM_CnuStatusT * CnuStatusdata;
    if(GetNextCmInfo(NowCMInfoP, &CnuStatusdata) != BASE_OK) {
        return BASE_ERROR;
    }

    if(CnuStatusdata != NULL)
        memcpy(NextCMInfoP, CnuStatusdata, sizeof(CMCOAM_CnuStatusT));
    else
        return BASE_ERROR;
    return BASE_OK;
}

BASE_ErrCode FindCMInfoMgt(MacAddressT cnu_mac,CMCOAM_CnuStatusT *FindCMInfoP)
{
    assert(FindCMInfoP);
    AutoLock autoLock(CnuStatusMgtMutex);
    CMCOAM_CnuStatusT * CnuStatusdata;
    if(FindCmInfo(cnu_mac, &CnuStatusdata) != BASE_OK) {
        return BASE_ERROR;
    }
    if(CnuStatusdata != NULL)
        memcpy(FindCMInfoP, CnuStatusdata, sizeof(CMCOAM_CnuStatusT));
    else
        return BASE_ERROR;
    return BASE_OK;
}
/**************************************************************************/

//control max cnu online numbers
//we can set , get or save this value
//if our cm online numbers beyond the max we call a trap to snmp
BASE_ErrCode SetMaxCnuOnlieMgt(int  set_cm_online_numberr)
{
    AutoLock autoLock(CnuStatusMgtMutex);
    set_max_cm_online_number = set_cm_online_numberr;
    return BASE_OK;
}

BASE_ErrCode GetMaxCnuOnlieMgt(int *get_cm_online_number)
{
    assert(get_cm_online_number);
    AutoLock autoLock(CnuStatusMgtMutex);
    *get_cm_online_number = set_max_cm_online_number;
    return BASE_OK;
}

BASE_ErrCode SaveMaxCnuOnlieMgt()
{
    int get_cm_online_number;
    AutoLock autoLock(CnuStatusMgtMutex);
    ostringstream os;

    if(GetMaxCnuOnlieMgt(&get_cm_online_number) < 0)
        return BASE_ERROR;

    os << "set max cm online number " << (int)get_cm_online_number << endl;

    if(SaveCfg(os.str().c_str() ,CONTINUE )<0) {
        dbgstrm << "save error"<< endl;
        return BASE_ERROR;
    }
    os.str("");

    return BASE_OK;
}

BASE_ErrCode GetCnuOnlieMgt(int *get_cm_online_number)
{
    *get_cm_online_number = cm_online_number;
    return BASE_OK;
}

/*add a callback function to judge max onlie cm numbers to upper layer*/
JudgeMaxCmOnlineNumberHandlerPoint theJudgeMaxCmOnlineNumberHandler = NULL;
JudgeMaxCmOnlineNumberInfoT JudgeMaxCmOnlineNumberInfoT_Entry;
int last_time_online_cm_number = 0;

//call Register
int RegisterJudgeMaxCmOnlineNumberCallbackMgt(JudgeMaxCmOnlineNumberHandlerPoint JudgeMaxCmOnlineNumberHandler)
{
    theJudgeMaxCmOnlineNumberHandler = JudgeMaxCmOnlineNumberHandler;
    return 0;
}

//callback handle function is used for cm online number
//it's associate with the max cnu number (MaxCnuOnlieMgt APIs)
//when cm online or offline callback this function to change now cm online numbers
int UpdateCmOnlineNumberHandlerMgt(CmOnlineNumberInfoT* CmOnlineNumberInfo)
{
    assert(CmOnlineNumberInfo);
    AutoLock autoLock(CnuStatusMgtMutex);
    MacAddressT Cm_mac;

    int now_cm_number;
    int now_max_cm_number;
    int cm_online_number_alarm_flag;

    //U32 Cm_ip_address =0;

    memcpy(Cm_mac.addr, CmOnlineNumberInfo->mac_addr , 6);

    if(CmOnlineNumberInfo->onlineoroffline == 1) {
        cm_online_number++;
        //cout << "test UpdateCmOnlineStatusHandlerMgt  CM ONLINE number ++ : "<< cm_online_number << endl;
    } else if(CmOnlineNumberInfo->onlineoroffline == 2) {
        cm_online_number--;
        //cout << "test UpdateCmOnlineStatusHandlerMgt CM OFFLINE number -- : "<< cm_online_number << endl;
    }

#if 1

    now_cm_number = cm_online_number;
    now_max_cm_number = set_max_cm_online_number;    

    JudgeMaxCmOnlineNumberInfoT_Entry.cm_online_number = now_cm_number;
    //JudgeMaxCmOnlineNumberInfoT_Entry.max_cm_online_number = now_max_cm_number;

    //--
    if(now_cm_number < now_max_cm_number) {
        if(last_time_online_cm_number > now_max_cm_number)
            cm_online_number_alarm_flag = 1;//alarm
    }
    else
    {
    //++    
        if(now_cm_number > now_max_cm_number) {
            if(last_time_online_cm_number < now_max_cm_number)
                cm_online_number_alarm_flag = 1;//alarm
        }
    }

   //cout << "---------------->>>test call back now_cm_number is :" << now_cm_number << endl; 
   //cout << "---------------->>>test call back now_max_cm_number is :" << now_max_cm_number << endl; 
   //cout << "---------------->>>test call back last_time_online_cm_number is :" << last_time_online_cm_number << endl; 
   //cout << "---------------->>>test call back cm_online_number_alarm_flag is :" << cm_online_number_alarm_flag << endl; 
    
    last_time_online_cm_number = now_cm_number;
    //JudgeMaxCmOnlineNumberInfoT_Entry.cm_online_number_alarm_flag = cm_online_number_alarm_flag;
    if((theJudgeMaxCmOnlineNumberHandler != NULL)&&(cm_online_number_alarm_flag == 1))
        theJudgeMaxCmOnlineNumberHandler(&JudgeMaxCmOnlineNumberInfoT_Entry);
    
#endif


    return 0;
}

int initUpdateCmOnlineNumberHandlerMgt()
{
    return RegisterCmOnlineNumberCallbackMgt(UpdateCmOnlineNumberHandlerMgt);
}
/**************************************************************************/

//callback handle function is used for CM online/offline
//if cm has been set ds qos when cm reonline we need auto add this cm ds qos rules
//if cm offline we need to delete this cm ds qos rules
int UpdateCmIpHandlerMgt(CMCOAM_CmIpInfoT* CmIpInfo_point)
{
    assert(CmIpInfo_point);
    AutoLock autoLock(CnuStatusMgtMutex);
    MacAddressT Cm_mac;
    U32 Cm_ip_address =0;

    memcpy(Cm_mac.addr, CmIpInfo_point->mac_addr , 6);
    if(SearchCnuMacToIpMapMgt(Cm_mac , &Cm_ip_address) != BASE_OK) {
        dbgstrm << "Search Cnu Mac To Ip Map Mgt Fail." <<  endl;
        return -1;
    }
    //search mac to bandwidht map
    DS_QoS_InfoT temp_ds_qos;
    memset(&temp_ds_qos, 0, sizeof(DS_QoS_InfoT));

    SearchCnuMacToBandnwidthMapMgt(Cm_mac , &temp_ds_qos);

    if((CmIpInfo_point->opcode == 1)&&(temp_ds_qos.bandwidth!= 0)) { //add cm
        if(MgtAddDsQosARule(Cm_ip_address, temp_ds_qos.bandwidth) != BASE_OK) {
            errstrm << "Search Cnu Mac To Ip Map Mgt Fail." <<  endl;
            return -1;
        }
    } else {
        if(CmIpInfo_point->opcode == 2) { //delete cm
            if((Cm_ip_address!=0)&&(temp_ds_qos.bandwidth!=0)) {
                if(MgtDeleteDsQosARule(Cm_ip_address) != BASE_OK) {
                    errstrm << "Delete Ds Qos Rules Fail." <<  endl;
                    return -1;
                }

                if(DeleteMacToIpMapMgt(Cm_mac) != BASE_OK) {
                    errstrm << "Delete Cnu Mac To Ip Map Mgt Fail." <<  endl;
                    return -1;
                }
            }
        }
    }

    return 0;
}

int initUpdateCmIpHandlerMgt()
{
    return RegisterContallerCallbackMgt(UpdateCmIpHandlerMgt);
}
/**************************************************************************/

//callback handle function is used for CM online/offline to add ip address to cm info struct
//because some company 's cm don't take ip address info when register
//we need to add ip through map cm-mac to cm-ip by this function
int UpdateCmOnlineStatusHandlerMgt(CmOnlineStatusInfoT* CmOnlineStatusInfo)
{
    assert(CmOnlineStatusInfo);
    AutoLock autoLock(CnuStatusMgtMutex);
    MacAddressT Cm_mac;
    //U32 Cm_ip_address =0;

    memcpy(Cm_mac.addr, CmOnlineStatusInfo->mac_addr , 6);

    if(CmOnlineStatusInfo->online_status == 1)
        cout << "test UpdateCmOnlineStatusHandlerMgt  CM ONLINE!" << endl;
    else if(CmOnlineStatusInfo->online_status == 2)
        cout << "test UpdateCmOnlineStatusHandlerMgt CM OFFLINE!" << endl;

    return 0;
}

int initUpdateCmOnlineStatusHandlerMgt()
{
    return RegisterCmOnlineStatusCallbackMgt(UpdateCmOnlineStatusHandlerMgt);
}
/**************************************************************************/


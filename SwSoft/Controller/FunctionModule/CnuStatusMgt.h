#ifndef _CnuStatusMgt_h_
#define _CnuStatusMgt_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
    MacAddressT ds_qos_cnu_mac;
    unsigned int bandwidth;
} DS_QoS_InfoT;

typedef struct {
    //int max_cm_online_number;
    int cm_online_number;
    //int cm_online_number_alarm_flag;	
} JudgeMaxCmOnlineNumberInfoT;

typedef int(*JudgeMaxCmOnlineNumberHandlerPoint)(JudgeMaxCmOnlineNumberInfoT*);

int RegisterJudgeMaxCmOnlineNumberCallbackMgt(JudgeMaxCmOnlineNumberHandlerPoint JudgeMaxCmOnlineNumberHandler);

int initUpdateCmIpHandlerMgt();
int initUpdateCmOnlineStatusHandlerMgt();
int initUpdateCmOnlineNumberHandlerMgt();

BASE_ErrCode SetMaxCnuOnlieMgt(int  set_cm_online_numberr);
BASE_ErrCode GetMaxCnuOnlieMgt(int *get_cm_online_number);
BASE_ErrCode GetCnuOnlieMgt(int *get_cm_online_number);
BASE_ErrCode SaveMaxCnuOnlieMgt();

//used for cnu status
BASE_ErrCode GetFirstCMInfoMgt(CMCOAM_CnuStatusT* FirstCMInfoP);
BASE_ErrCode GetNextCMInfoMgt(CMCOAM_CnuStatusT *NowCMInfoP,CMCOAM_CnuStatusT *NextCMInfoP);
BASE_ErrCode FindCMInfoMgt(MacAddressT cnu_mac,CMCOAM_CnuStatusT *FindCMInfoP);

//used for dhcp
BASE_ErrCode AddMacToIpMapMgt(MacAddressT cnu_mac , U32 cnu_ip);
BASE_ErrCode DeleteMacToIpMapMgt(MacAddressT cnu_mac);
BASE_ErrCode SearchCnuMacToIpMapMgt(MacAddressT cnu_mac , U32* cnu_ipaddress);
void DumpCnuMacToIpMapMgt();

//used for qos
BASE_ErrCode AddMacToBandnwidthMapMgt(DS_QoS_InfoT* new_ds_qos);
BASE_ErrCode DeleteMacToBandnwidthMapMgt(MacAddressT cnu_mac);
BASE_ErrCode SearchCnuMacToBandnwidthMapMgt(MacAddressT cnu_mac , DS_QoS_InfoT* ds_qos);
void DumpCnuMacToBandnwidthMapMgt();
BASE_ErrCode GetFirstDsQosMgt(DS_QoS_InfoT* first_ds_qos);
BASE_ErrCode GetNextDsQosMgt(DS_QoS_InfoT* now_ds_qos, DS_QoS_InfoT* next_ds_qos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


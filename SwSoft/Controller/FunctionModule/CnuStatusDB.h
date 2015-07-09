#ifndef _CnuStatusDB_h_
#define _CnuStatusDB_h_

#ifndef SUPPORT_DHCP_RELAY
typedef struct {
    U32         ipv4_addr;
} CMCOAM_CabModemInfoT;

#endif

typedef struct {
    MacAddressT ds_qos_cnu_mac;
    unsigned int bandwidth;
} DS_QoS_InfoDBT;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//map funciton for dhcp
BASE_ErrCode AddMacToIpMap(MacAddressT cnu_mac , U32 cnu_ip);
BASE_ErrCode DeleteMacToIpMap(MacAddressT cnu_mac);
BASE_ErrCode SearchCnuMacToIpMap(MacAddressT cnu_mac , U32** cnu_ipaddress);
void DumpCnuMacToIpMap();

//map function for qos
BASE_ErrCode AddMacToBandnWidthMap(DS_QoS_InfoDBT* cnu_ds_qos);
BASE_ErrCode DeleteMacToBandnWidthMap(MacAddressT cnu_mac);
BASE_ErrCode SearchCnuMacToBandnWidthMap(MacAddressT cnu_mac , DS_QoS_InfoDBT* cnu_ds_qos);
void DumpCnuMacToBandnWidthMap();
BASE_ErrCode GetCmFirstDownStreamQos(DS_QoS_InfoDBT* first_ds_qos);
BASE_ErrCode GetCmNextDownStreamQos(DS_QoS_InfoDBT now_ds_qos, DS_QoS_InfoDBT* next_ds_qos);

//cm status info
BASE_ErrCode GetFirstCmInfo(CMCOAM_CnuStatusT **FirstCMInfoP);
BASE_ErrCode GetNextCmInfo(CMCOAM_CnuStatusT *NowCMInfoP,CMCOAM_CnuStatusT **NextCMInfoP);
BASE_ErrCode FindCmInfo(MacAddressT cnu_mac,CMCOAM_CnuStatusT **FindCMInfoP);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


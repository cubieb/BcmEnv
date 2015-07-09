#ifndef _CnuHostStatusDB_h_
#define _CnuHostStatusDB_h_

typedef struct {
    MacAddressT host_mac;
    U32         ipv4_addr;
    S32         cpe_type;
} CMCOAM_CmHostInfoT;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//host info
BASE_ErrCode UpdateCmHost(MacAddressT Cm_mac , CMCOAM_CmHostInfoT Cm_host);
BASE_ErrCode DeleteCmHost(MacAddressT Cm_mac , CMCOAM_CmHostInfoT Cm_host);
BASE_ErrCode GetCmFirstHost(CMCOAM_CnuStatusT *NowCMInfoP ,CMCOAM_CmHostInfoT **First_Cm_host);
BASE_ErrCode GetCmNextHost(CMCOAM_CnuStatusT *NowCMInfoP ,CMCOAM_CmHostInfoT *Now_Cm_host,CMCOAM_CmHostInfoT **Next_Cm_host);
BASE_ErrCode FindCmHost(MacAddressT Cm_mac ,MacAddressT Host_mac,CMCOAM_CmHostInfoT **Find_Cm_host);
BASE_ErrCode ShowAllCmHost();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


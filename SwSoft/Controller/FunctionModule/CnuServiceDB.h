#ifndef _CnuServiceDB_h_
#define _CnuServiceDB_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//service flow
BASE_ErrCode GetCmFirstServiceFlow(CMCOAM_CnuStatusT *NowCMInfoP , CMCOAM_ServiceFlowT **FirstServiceFlowP);
BASE_ErrCode GetCmNextServiceFlow(CMCOAM_CnuStatusT *NowCMInfoP , CMCOAM_ServiceFlowT * NowServiceFlowP,CMCOAM_ServiceFlowT **NextServiceFlowP);
BASE_ErrCode FindCMServiceFlowDB(MacAddressT cnu_mac , U32 sf_id,CMCOAM_ServiceFlowT **FindServiceFlowP);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


#ifndef _CnuServiceMgt_h_
#define _CnuServiceMgt_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BASE_ErrCode GetCMFirstServiceFlowMgt(CMCOAM_CnuStatusT *NowCMInfoP , CMCOAM_ServiceFlowT * FirstServiceFlowP);
BASE_ErrCode GetCMNextServiceFlowMgt(CMCOAM_CnuStatusT *NowCMInfoP , CMCOAM_ServiceFlowT * NowServiceFlowP,CMCOAM_ServiceFlowT * NextServiceFlowP);
BASE_ErrCode FindCMServiceFlowMgt(MacAddressT cnu_mac , U32 sf_id,CMCOAM_ServiceFlowT *FindServiceFlowP);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


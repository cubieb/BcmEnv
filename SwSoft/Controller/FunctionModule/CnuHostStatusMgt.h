#ifndef _CnuHostStatusMgt_h_
#define _CnuHostStatusMgt_h_

#include "CnuHostStatusDB.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BASE_ErrCode Update_Cm_HostMgt(MacAddressT Cm_mac , CMCOAM_CmHostInfoT Cm_host);
BASE_ErrCode Del_Cm_HostMgt(MacAddressT Cm_mac , CMCOAM_CmHostInfoT Cm_host);
BASE_ErrCode GetFirst_Cm_HostMgt(CMCOAM_CnuStatusT *NowCMInfoP ,CMCOAM_CmHostInfoT *First_Cm_host);
BASE_ErrCode GetNext_Cm_HostMgt(CMCOAM_CnuStatusT *NowCMInfoP ,CMCOAM_CmHostInfoT *Now_Cm_host,CMCOAM_CmHostInfoT *Next_Cm_host);
BASE_ErrCode Find_Cm_HostMgt(MacAddressT Cm_mac ,MacAddressT Host_mac,CMCOAM_CmHostInfoT *Find_Cm_host);
BASE_ErrCode Show_All_Cm_HostMgt();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


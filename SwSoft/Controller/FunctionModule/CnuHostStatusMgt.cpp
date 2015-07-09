#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"
#include "AutoLock.h"
#include "CnuHostStatusDB.h"
#include "CnuHostStatusMgt.h"

using namespace std;

pthread_mutex_t CnuHostMgtMutex = PTHREAD_MUTEX_INITIALIZER;

//Host mgt funciton
BASE_ErrCode Update_Cm_HostMgt(MacAddressT Cm_mac, CMCOAM_CmHostInfoT Cm_host)
{
    AutoLock autoLock(CnuHostMgtMutex);
    if(UpdateCmHost(Cm_mac, Cm_host) != BASE_OK) {
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode Del_Cm_HostMgt(MacAddressT Cm_mac, CMCOAM_CmHostInfoT Cm_host)
{
    AutoLock autoLock(CnuHostMgtMutex);
    if(DeleteCmHost(Cm_mac, Cm_host) != BASE_OK) {
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode GetFirst_Cm_HostMgt(CMCOAM_CnuStatusT *NowCMInfoP,CMCOAM_CmHostInfoT *First_Cm_host)
{
    assert(NowCMInfoP);
    assert(First_Cm_host);
    AutoLock autoLock(CnuHostMgtMutex);
    CMCOAM_CmHostInfoT * Hostdata = NULL;
    if(GetCmFirstHost(NowCMInfoP, &Hostdata) != BASE_OK) {
        return BASE_ERROR;
    }
    if(Hostdata != NULL)
        memcpy(First_Cm_host, Hostdata, sizeof(CMCOAM_CmHostInfoT));
    else
        return BASE_ERROR;
    return BASE_OK;
}

BASE_ErrCode GetNext_Cm_HostMgt(CMCOAM_CnuStatusT *NowCMInfoP,
                                CMCOAM_CmHostInfoT *Now_Cm_host,CMCOAM_CmHostInfoT *Next_Cm_host)
{
    assert(NowCMInfoP);
    assert(Now_Cm_host);
    assert(Next_Cm_host);
    AutoLock autoLock(CnuHostMgtMutex);
    CMCOAM_CmHostInfoT * Hostdata;
    if(GetCmNextHost(NowCMInfoP, Now_Cm_host, &Hostdata) != BASE_OK) {
        return BASE_ERROR;
    }
    if(Hostdata != NULL)
        memcpy(Next_Cm_host, Hostdata, sizeof(CMCOAM_CmHostInfoT));
    else
        return BASE_ERROR;
    return BASE_OK;
}

BASE_ErrCode Find_Cm_HostMgt(MacAddressT Cm_mac, MacAddressT Host_mac,
                             CMCOAM_CmHostInfoT *Find_Cm_host)
{
    assert(Find_Cm_host);
    AutoLock autoLock(CnuHostMgtMutex);
    CMCOAM_CmHostInfoT * Hostdata;
    if(FindCmHost(Cm_mac, Host_mac, &Hostdata) != BASE_OK) {
        return BASE_ERROR;
    }
    if(Hostdata != NULL)
        memcpy(Find_Cm_host, Hostdata, sizeof(CMCOAM_CmHostInfoT));
    else
        return BASE_ERROR;
    return BASE_OK;
}

BASE_ErrCode Show_All_Cm_HostMgt()
{
    AutoLock autoLock(CnuHostMgtMutex);
    if(ShowAllCmHost() != BASE_OK) {
        return BASE_ERROR;
    }
    return BASE_OK;
}


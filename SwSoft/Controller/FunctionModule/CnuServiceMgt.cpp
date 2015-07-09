#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"
#include "AutoLock.h"
#include "CnuServiceDB.h"
#include "CnuServiceMgt.h"

using namespace std;

pthread_mutex_t CnuServiceMgtMutex = PTHREAD_MUTEX_INITIALIZER;

//Cnu Service Mgt Function
BASE_ErrCode GetCMFirstServiceFlowMgt(CMCOAM_CnuStatusT *NowCMInfoP,
                                      CMCOAM_ServiceFlowT * FirstServiceFlowP)
{
    assert(NowCMInfoP);
    assert(FirstServiceFlowP);
    AutoLock autoLock(CnuServiceMgtMutex);
    CMCOAM_ServiceFlowT *Sfdata;

    if(GetCmFirstServiceFlow(NowCMInfoP, &Sfdata) != BASE_OK) {
        return BASE_ERROR;
    }
    if(Sfdata != NULL)
        memcpy(FirstServiceFlowP, Sfdata, sizeof(CMCOAM_ServiceFlowT));
    else
        return BASE_ERROR;
    return BASE_OK;
}

BASE_ErrCode GetCMNextServiceFlowMgt(CMCOAM_CnuStatusT *NowCMInfoP,
                                     CMCOAM_ServiceFlowT * NowServiceFlowP,
                                     CMCOAM_ServiceFlowT * NextServiceFlowP)
{
    assert(NowCMInfoP);
    assert(NowServiceFlowP);
    assert(NextServiceFlowP);
    AutoLock autoLock(CnuServiceMgtMutex);
    CMCOAM_ServiceFlowT * Sfdata;

    if(GetCmNextServiceFlow(NowCMInfoP, NowServiceFlowP, &Sfdata) != BASE_OK) {
        return BASE_ERROR;
    }
    if(Sfdata != NULL)
        memcpy(NextServiceFlowP, Sfdata, sizeof(CMCOAM_ServiceFlowT));
    else
        return BASE_ERROR;
    return BASE_OK;
}

BASE_ErrCode FindCMServiceFlowMgt(MacAddressT cnu_mac, U32 sf_id,
                                  CMCOAM_ServiceFlowT *FindServiceFlowP)
{
    assert(FindServiceFlowP);    
    AutoLock autoLock(CnuServiceMgtMutex);
    CMCOAM_ServiceFlowT * Sfdata;

    if(sf_id==0) {
        return BASE_ERROR;
    }
    if(FindCMServiceFlowDB(cnu_mac, sf_id, &Sfdata) != BASE_OK) {
        return BASE_ERROR;
    }
    if(Sfdata != NULL)
        memcpy(FindServiceFlowP, Sfdata, sizeof(CMCOAM_ServiceFlowT));
    else
        return BASE_ERROR;
    return BASE_OK;
}


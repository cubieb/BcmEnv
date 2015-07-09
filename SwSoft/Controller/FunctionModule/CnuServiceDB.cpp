#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"

#include "CnuServiceDB.h"

using namespace std;

CMCOAM_ServiceFlowT Global_CMCOAM_ServiceFlowEntry;

//extern "C" void BASE_P_DbgBuf(int dbgLevel, unsigned char *p, unsigned int len, unsigned short width);

//Service DB function
BASE_ErrCode 
GetCmFirstServiceFlow(CMCOAM_CnuStatusT *NowCMInfoP , CMCOAM_ServiceFlowT ** Sfdata)
{
    assert(NowCMInfoP);
    assert(Sfdata);
    CMCCTRL_CmcEntryT*  pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_ServiceFlowT*    pSf = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;
    if(NULL == pCmcEntry) {
        errstrm<< "CMC not Exist." << endl;
        CMCCTRL_ReleaseCmcEntry();
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CMC not Exist." << endl;
        return BASE_ERROR;
    }

    //judge and get this cm's first sf
    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&(NowCMInfoP->cnu_mac), &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            pNode = (pCmEntry->srvFlow)->ptr;
            if(pNode != NULL) {
                pSf = (CMCOAM_ServiceFlowT *)(pNode->date);
                *Sfdata = pSf;
                break;
            } else {
                CMCCTRL_ReleaseCmcEntry();
                errstrm << "Service Flow not Exist" << endl;
                return BASE_ERROR;
            }
        }
    }
    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}

BASE_ErrCode 
GetCmNextServiceFlow(CMCOAM_CnuStatusT *NowCMInfoP , CMCOAM_ServiceFlowT * NowServiceFlowP,CMCOAM_ServiceFlowT ** NextServiceFlowP)
{
    assert(NowCMInfoP);
    assert(NowServiceFlowP);
    assert(NextServiceFlowP);
    CMCCTRL_CmcEntryT*      pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_ServiceFlowT*    pSf = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;
    if(NULL == pCmcEntry) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CMC not Exist." << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CM not Exist." << endl;
        return BASE_ERROR;
    }

    //judge and get the next sf
    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&(NowCMInfoP->cnu_mac), &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            for(pNode = (pCmEntry->srvFlow)->ptr; pNode != NULL; pNode = pNode->next) {
                pSf = (CMCOAM_ServiceFlowT *)(pNode->date);

                if(pSf == NULL) {
                    CMCCTRL_ReleaseCmcEntry();
                    errstrm << "Service Flow not Exist" << endl;
                    return BASE_ERROR;
                }

                if(NowServiceFlowP->sf_id == pSf->sf_id) {
                    pNode = pNode->next;
                    if(pNode != NULL) {
                        pSf = (CMCOAM_ServiceFlowT *)(pNode->date);
                        *NextServiceFlowP = pSf;
                        break;
                    } else {
                        CMCCTRL_ReleaseCmcEntry();
                        dbgstrm << "This service flow is the last one,no more sf." << endl;
                        return BASE_ERROR;
                    }
                }
            }
            break;
        }
    }
    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}

BASE_ErrCode 
FindCMServiceFlowDB(MacAddressT cnu_mac , U32 sf_id,CMCOAM_ServiceFlowT **FindServiceFlowP)
{
    assert(FindServiceFlowP);
    CMCCTRL_CmcEntryT*      pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_ServiceFlowT*    pSf = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;
    if(NULL == pCmcEntry) {
        errstrm << "CMC not Exist." << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CM not Exist." << endl;
        return BASE_ERROR;
    }

    //judge and find sf
    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&cnu_mac, &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            for(pNode = (pCmEntry->srvFlow)->ptr; pNode != NULL; pNode = pNode->next) {
                pSf = (CMCOAM_ServiceFlowT *)(pNode->date);

                if(pSf == NULL) {
                    CMCCTRL_ReleaseCmcEntry();
                    errstrm << "Service Flow not Exist." << endl;
                    return BASE_ERROR;
                }

                if(sf_id == pSf->sf_id) {
                    *FindServiceFlowP = pSf;
                    break;
                } else {
                    if(pNode->next == NULL) {
                        CMCCTRL_ReleaseCmcEntry();
                        errstrm << "Don't have this SF in this CM." << endl;
                        return BASE_ERROR;
                    }
                }
            }
            break;
        } else {
            if(pNode->next == NULL) {
                CMCCTRL_ReleaseCmcEntry();
                errstrm << "Don't have this mac CM." << endl;
                return BASE_ERROR;
            }
        }
    }
    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}


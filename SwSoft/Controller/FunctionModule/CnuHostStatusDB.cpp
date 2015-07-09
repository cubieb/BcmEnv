#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"

#include "CnuHostStatusDB.h"

using namespace std;

//Host DB function
BASE_ErrCode 
UpdateCmHost(MacAddressT Cm_mac , CMCOAM_CmHostInfoT Cm_host)
{
    CMCCTRL_CmcEntryT*  pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_CmHostInfoT* pHostInfo = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;

    if(NULL == pCmcEntry) {
        CMCCTRL_ReleaseCmcEntry();
        dbgstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        dbgstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }

    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&(Cm_mac), &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            for(pNode = (pCmEntry->pHost)->ptr; pNode != NULL; pNode = pNode->next) {
                pHostInfo = (CMCOAM_CmHostInfoT *)(pNode->date);
                if(!memcmp(&(pHostInfo->host_mac), &(Cm_host.host_mac), sizeof(MacAddressT))) {
                    //Already exit, delete.
                    delete_node(pCmEntry->pHost,pNode);
                    break;
                }
            }
            //Add to list.
            if(NULL == add_node(pCmEntry->pHost,(void *)&Cm_host,sizeof(CMCOAM_CmHostInfoT))) {
                CMCCTRL_ReleaseCmcEntry();
                return BASE_ERROR;
            } else {
                dbgstrm << "add new host success" << endl;
                break;
            }
        } else {
            if(pNode->next == NULL) {
                CMCCTRL_ReleaseCmcEntry();
                errstrm << "Don't have this mac CM" << endl;
                return BASE_ERROR;
            }
        }
    }
    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}

BASE_ErrCode 
DeleteCmHost(MacAddressT Cm_mac , CMCOAM_CmHostInfoT Cm_host)
{
    CMCCTRL_CmcEntryT*  pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_CmHostInfoT* pHostInfo = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;

    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;

    pNode = NULL;

    if(NULL == pCmcEntry) {
        CMCCTRL_ReleaseCmcEntry();
        dbgstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        dbgstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }

    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&(Cm_mac), &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            for(pNode = (pCmEntry->pHost)->ptr; pNode != NULL; pNode = pNode->next) {
                pHostInfo = (CMCOAM_CmHostInfoT *)(pNode->date);
                if(!memcmp(&(pHostInfo->host_mac), &(Cm_host.host_mac), sizeof(MacAddressT))) {
                    //Already exit, delete.
                    delete_node(pCmEntry->pHost,pNode);
                    CMCCTRL_ReleaseCmcEntry();
                    return BASE_OK;
                } else {
                    if(pNode->next == NULL) {
                        CMCCTRL_ReleaseCmcEntry();
                        errstrm << "Don't have this Host" << endl;
                        return BASE_ERROR;
                    }
                }
            }
            break;
        } else {
            if(pNode->next == NULL) {
                CMCCTRL_ReleaseCmcEntry();
                errstrm << "Don't have this mac CM" << endl;
                return BASE_ERROR;
            }
        }
    }
    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}

BASE_ErrCode 
GetCmFirstHost(CMCOAM_CnuStatusT *NowCMInfoP ,CMCOAM_CmHostInfoT **First_Cm_host)
{
    assert(NowCMInfoP);
    assert(First_Cm_host);
    CMCCTRL_CmcEntryT*  pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_CmHostInfoT* pHostInfo = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;
    if(NULL == pCmcEntry) {
        CMCCTRL_ReleaseCmcEntry();;
        dbgstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        dbgstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }

    //judge and get this cm's first host info
    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&(NowCMInfoP->cnu_mac), &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            pNode = (pCmEntry->pHost)->ptr;
            if(pNode != NULL) {
                pHostInfo = (CMCOAM_CmHostInfoT *)(pNode->date);
                *First_Cm_host = pHostInfo;
            } else {
                dbgstrm << "Cm:" << NowCMInfoP->cnu_mac << " have no host entry" << endl;
            }
            break;
        } else {
            if(pNode->next == NULL) {
                CMCCTRL_ReleaseCmcEntry();
                errstrm << "Don't have this mac CM" << endl;
                return BASE_ERROR;
            }
        }
    }

    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}

BASE_ErrCode 
GetCmNextHost(CMCOAM_CnuStatusT *NowCMInfoP ,CMCOAM_CmHostInfoT *Now_Cm_host,CMCOAM_CmHostInfoT **Next_Cm_host)
{
    assert(NowCMInfoP);
    assert(Now_Cm_host);
    assert(Next_Cm_host);
    CMCCTRL_CmcEntryT*  pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_CmHostInfoT* pHostInfo = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;
    if(NULL == pCmcEntry) {
        errstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }

    //judge and get this cm's next host info
    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&(NowCMInfoP->cnu_mac), &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            for(pNode = (pCmEntry->pHost)->ptr; pNode != NULL; pNode = pNode->next) {
                pHostInfo = (CMCOAM_CmHostInfoT *)(pNode->date);
                if(!memcmp(&(Now_Cm_host->host_mac), &(pHostInfo->host_mac), sizeof(MacAddressT))) {
                    pNode = pNode->next;
                    if(pNode != NULL) {
                        pHostInfo = (CMCOAM_CmHostInfoT *)(pNode->date);
                        *Next_Cm_host = pHostInfo;
                    } else {
                        dbgstrm << "at the host list last" << endl;
                        CMCCTRL_ReleaseCmcEntry();
                        return BASE_ERROR;
                    }
                    break;
                } else {
                    if(pNode->next == NULL) {
                        CMCCTRL_ReleaseCmcEntry();
                        errstrm << "Don't have this mac Host" << endl;
                        return BASE_ERROR;
                    }
                }
            }
            break;
        } else {
            if(pNode->next == NULL) {
                CMCCTRL_ReleaseCmcEntry();
                return BASE_ERROR;
            }
        }
    }
    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}

BASE_ErrCode 
FindCmHost(MacAddressT Cm_mac ,MacAddressT Host_mac,CMCOAM_CmHostInfoT **Find_Cm_host)
{
    assert(Find_Cm_host);
    CMCCTRL_CmcEntryT*  pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_CmHostInfoT* pHostInfo = NULL;
    list_node*              pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;
    if(NULL == pCmcEntry) {
        errstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }

    //judge and find this cm's host info
    for(pNode =(pCmcEntry->cmList)->ptr; pNode != NULL; pNode = pNode->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode->date);
        if(!memcmp(&Cm_mac, &(pCmEntry->cm_mac), BRCM_MAC_LEN)) {
            for(pNode = (pCmEntry->pHost)->ptr; pNode != NULL; pNode = pNode->next) {
                pHostInfo = (CMCOAM_CmHostInfoT *)(pNode->date);
                if(!memcmp(&Host_mac, &(pHostInfo->host_mac), sizeof(MacAddressT))) {
                    if(pNode != NULL) {
                        pHostInfo = (CMCOAM_CmHostInfoT *)(pNode->date);
                        *Find_Cm_host = pHostInfo;
                    }
                    break;
                } else {
                    if(pNode->next == NULL) {
                        CMCCTRL_ReleaseCmcEntry();
                        errstrm << "Don't have this mac Host" << endl;
                        return BASE_ERROR;
                    }
                }
            }
            break;
        } else {
            if(pNode->next == NULL) {
                CMCCTRL_ReleaseCmcEntry();
                errstrm << "Don't have this mac CM" << endl;
                return BASE_ERROR;
            }
        }
    }

    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}

BASE_ErrCode 
ShowAllCmHost()
{
    CMCCTRL_CmcEntryT*  pCmcEntry = NULL;
    CMCCTRL_CmInfoT*        pCmEntry = NULL;
    CMCOAM_CmHostInfoT* pHostInfo = NULL;
    list_node*              pNode = NULL;
    list_node*              pNode1 = NULL;
    list_node*              pNode2 = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    pNode = NULL;
    if(NULL == pCmcEntry) {
        //BASE_ERR("CMC not Exit");
        errstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }

    //show all hosts
    if((pCmcEntry->cmList)->ptr == NULL) {
        cout << " NO CM REGISTER ONLINE !!! " << endl;
        return BASE_OK;
    }
    for(pNode1 =(pCmcEntry->cmList)->ptr; pNode1 != NULL; pNode1 = pNode1->next) {
        pCmEntry = (CMCCTRL_CmInfoT *)(pNode1->date);
        CMCOAM_PRINT("Cm Mac Address : %02x%02x.%02x%02x.%02x%02x\n", pCmEntry->cm_mac.addr[0], pCmEntry->cm_mac.addr[1], pCmEntry->cm_mac.addr[2], pCmEntry->cm_mac.addr[3], pCmEntry->cm_mac.addr[4], pCmEntry->cm_mac.addr[5]);
        for(pNode2 = (pCmEntry->pHost)->ptr; pNode2 != NULL; pNode2 = pNode2->next) {
            pHostInfo = (CMCOAM_CmHostInfoT *)(pNode2->date);
            CMCOAM_PRINT("--Host Mac Address : %02x%02x.%02x%02x.%02x%02x ", pHostInfo->host_mac.addr[0], pHostInfo->host_mac.addr[1], pHostInfo->host_mac.addr[2], pHostInfo->host_mac.addr[3], pHostInfo->host_mac.addr[4], pHostInfo->host_mac.addr[5]);
            CMCOAM_PRINT("--Host Ip Address : 0x%x\n",pHostInfo->ipv4_addr);
        }
    }
    CMCCTRL_ReleaseCmcEntry();
    return BASE_OK;
}


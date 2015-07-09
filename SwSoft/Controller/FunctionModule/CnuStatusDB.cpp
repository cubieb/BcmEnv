#include "SystemInclude.h"
#include <map>

#include "BcmController.h"
#include "Debug.h"
#include "Util.h"
#include "CnuStatusDB.h"

using namespace std;

//Mac To Cnu Status Function
//we can add,clear this map
typedef map<string,CMCOAM_CnuStatusT> CnuMacToStatusMap_ListT;
CnuMacToStatusMap_ListT         CnuMacToStatusMap_List;

BASE_ErrCode 
ClearCnuMacToStatusMap()
{
    CnuMacToStatusMap_List.clear();
    return BASE_OK;
}

BASE_ErrCode 
AddCnuMacToStatusMap(MacAddressT cnu_mac , CMCOAM_CnuStatusT cnu_status_info)
{
    string mac_string = MacToString(cnu_mac.addr);
    pair<map<string,CMCOAM_CnuStatusT>::iterator,bool> p;

    CnuMacToStatusMap_ListT::iterator m2s_iter=CnuMacToStatusMap_List.find(mac_string);
    if(m2s_iter!=CnuMacToStatusMap_List.end()) {
        m2s_iter->second=cnu_status_info;
    } else {
        p = CnuMacToStatusMap_List.insert((make_pair(mac_string,cnu_status_info)));
        if(p.second == 0) {
            errstrm << "insert this clinet in mac string to cnu status map false" << endl;
            return BASE_ERROR;
        } else
            dbgstrm << "insert this client  in mac string to cnu status map success." << endl;
    }
    return BASE_OK;
}

//get fisrt and netx cnu status is used for snmp
BASE_ErrCode 
FirstCnuMacToStatusMap(CMCOAM_CnuStatusT **now_cnu_status_info_p)
{
    assert(now_cnu_status_info_p);
    CnuMacToStatusMap_ListT::iterator m2s_iter=CnuMacToStatusMap_List.begin();

    if(m2s_iter!=CnuMacToStatusMap_List.end()) {
        *now_cnu_status_info_p = &(m2s_iter->second);
    } else {
        dbgstrm << "don't have any clinet in mac string to cnu status map" << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode 
NextCnuMacToStatusMap(MacAddressT cnu_mac,CMCOAM_CnuStatusT **next_cnu_status_info_p)
{
    assert(next_cnu_status_info_p);
    string mac_string = MacToString(cnu_mac.addr);

    CnuMacToStatusMap_ListT::iterator m2s_iter=CnuMacToStatusMap_List.find(mac_string);
    m2s_iter++;
    if(m2s_iter!=CnuMacToStatusMap_List.end()) {
        *next_cnu_status_info_p = &(m2s_iter->second);
    } else {
        dbgstrm << "don't have next clinet in mac string to cnu status map" << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode 
SearchCnuMacToStatusMap(MacAddressT cnu_mac,CMCOAM_CnuStatusT **cnu_status_info_p)
{
    assert(cnu_status_info_p);
    string mac_string = MacToString(cnu_mac.addr);

    CnuMacToStatusMap_ListT::iterator m2s_iter=CnuMacToStatusMap_List.find(mac_string);
    if(m2s_iter!=CnuMacToStatusMap_List.end()) {
        *cnu_status_info_p = &(m2s_iter->second);
    } else {
        dbgstrm << "don't have this clinet in mac string to cnu status map" << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

//when get data over time we need call this funciton to update map
BASE_ErrCode 
UpdateCnuStatusMapData()
{
    CMCOAM_GetCnuStatusRspT rsp;
    CMCOAM_CmLstT           cmList;
    int i=0;
    memset(&rsp, 0, sizeof(rsp));
    memset(&cmList, 0, sizeof(cmList));
    if(BASE_OK!=CMCOAM_Cm_GetCnuList(&cmList, cliGetCmc()))
        return BASE_OK;

    MacAddressT *pMac = (MacAddressT*)BASE_Malloc(cmList.num_of_cm * sizeof(MacAddressT));
    if(NULL == pMac)
        return BASE_OK;

    for(i=0; i<cmList.num_of_cm; i++) {
        memcpy((pMac+i)->addr, cmList.cm[i].mac.addr, sizeof(MacAddressT));
    }

    if(BASE_OK == CMCOAM_Cm_GetCnuListStatus(pMac, cmList.num_of_cm, &rsp, cliGetCmc())) {
        CMCOAM_GetCnuStatusRspT *pRsp = &rsp;
        CMCOAM_CnuStatusT *pnCnu = NULL;
        CMCOAM_CnuStatusT cnu_statusinfo_entry;
        memset(&cnu_statusinfo_entry , 0 , sizeof(CMCOAM_CnuStatusT));

        for(int n=0; n<pRsp->cnu_count; n++) {
            pnCnu = pRsp->pCnus + n;

            memcpy(&cnu_statusinfo_entry , pnCnu , sizeof(CMCOAM_CnuStatusT));

            MacAddressT cnu_mac_address;
            for(int i=0; i<BRCM_MAC_LEN; i++) {
                cnu_mac_address.addr[i]=cnu_statusinfo_entry.cnu_mac[i];
            }

            if(AddCnuMacToStatusMap(cnu_mac_address , cnu_statusinfo_entry) != BASE_OK) {
                errstrm << "Add Cnu Mac To Status Map fail" << endl;
                CMCOAM_Cm_FreeCnuListStatus(&rsp);
                BASE_Free(pMac);
                return BASE_ERROR;
            }
        }
        CMCOAM_Cm_FreeCnuListStatus(&rsp);
    }

    BASE_Free(pMac);
    return BASE_OK;
}
/**************************************************************************/

//Cnu Status Function called by mgt layer
BASE_ErrCode 
GetFirstCmInfo(CMCOAM_CnuStatusT** FirstCMInfoP)
{
    assert(FirstCMInfoP);
    CMCCTRL_CmcEntryT*          pCmcEntry = NULL;
    list_node*                  pNode = NULL;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    if(NULL == pCmcEntry) {
        dbgstrm << "CMC not Exist" << endl;
        CMCCTRL_ReleaseCmcEntry();
        return BASE_ERROR;
    }

    //judge cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        dbgstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }
    CMCCTRL_ReleaseCmcEntry();
    //send oam message to get this mac cun status info
    static time_t last_call_time,now_time;
    double cost_time = 0;
    time(&now_time);

    cost_time = difftime(now_time , last_call_time);

    if((cost_time > 5)||(last_call_time == 0)) {
        dbgstrm << "Get cnu status over timer" << endl;

        ClearCnuMacToStatusMap();

        if(UpdateCnuStatusMapData()!=BASE_OK) {
            errstrm << "Update Cnu Status Map Data Error" << endl;
            return BASE_ERROR;
        }
    }

    time(&last_call_time);

    CMCOAM_CnuStatusT* temp_cnu_status_info_point = NULL;
    if(BASE_OK == FirstCnuMacToStatusMap(&temp_cnu_status_info_point)) {
        *FirstCMInfoP = temp_cnu_status_info_point;
        dbgstrm << "get first cnu status info success" << endl;
    } else {
        errstrm << "First Cnu Ma cTo Status Map Error" << endl;
        return BASE_ERROR;
    }

    return BASE_OK;

}

BASE_ErrCode 
GetNextCmInfo(CMCOAM_CnuStatusT *NowCMInfoP,CMCOAM_CnuStatusT **NextCMInfoP)
{
    assert(NowCMInfoP);
    assert(NextCMInfoP);
    CMCCTRL_CmcEntryT*          pCmcEntry = NULL;
    list_node*                  pNode = NULL;
    MacAddressT                      macAddr;

    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    if(NULL == pCmcEntry) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    CMCCTRL_ReleaseCmcEntry();
    memcpy(&macAddr, &(NowCMInfoP->cnu_mac), sizeof(macAddr));
    CMCOAM_CnuStatusT* temp_cnu_status_info_point = NULL;

    if(BASE_OK == NextCnuMacToStatusMap(macAddr,&temp_cnu_status_info_point)) {
        *NextCMInfoP = temp_cnu_status_info_point;
        dbgstrm << "get next cnu status info success" << endl;
    } else {
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode 
FindCmInfo(MacAddressT cnu_mac,CMCOAM_CnuStatusT **FindCMInfoP)
{
    assert(FindCMInfoP);
    CMCCTRL_CmcEntryT*          pCmcEntry = NULL;
    list_node*                  pNode = NULL;
    CMCCTRL_AcquireCmcEntry();

    //judge cmc exist
    pNode=CMCOAM_Platform_GetHandler()->cmcLst->ptr;
    pCmcEntry = (CMCCTRL_CmcEntryT*)pNode->date;
    if(NULL == pCmcEntry) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CMC not Exist" << endl;
        return BASE_ERROR;
    }

    //jude cm exist
    pNode = (pCmcEntry->cmList)->ptr;
    if(pNode == NULL) {
        CMCCTRL_ReleaseCmcEntry();
        errstrm << "CM not Exist" << endl;
        return BASE_ERROR;
    }

    CMCCTRL_ReleaseCmcEntry();
    CMCOAM_CnuStatusT* temp_cnu_status_info_point = NULL;
    if(BASE_OK == SearchCnuMacToStatusMap(cnu_mac,&temp_cnu_status_info_point)) {
        *FindCMInfoP = temp_cnu_status_info_point;
        dbgstrm << "search next cnu status info success" << endl;
    } else {
        return BASE_ERROR;
    }
    return BASE_OK;
}
/**************************************************************************/

//Mac To Ip Function used for qos module
typedef map<std::string,U32> MacToIpMap_ListT;
MacToIpMap_ListT        MacToIpMap_List;

BASE_ErrCode 
ClearMacToIpMap()
{
    MacToIpMap_List.clear();
    return BASE_OK;
}

BASE_ErrCode 
AddMacToIpMap(MacAddressT cnu_mac , U32 cnu_ip)
{
    string mac_string = MacToString(cnu_mac.addr);

    pair<std::map<std::string,unsigned int>::iterator,bool> p;

    MacToIpMap_ListT::iterator m2i_iter=MacToIpMap_List.find(mac_string);
    if(m2i_iter!=MacToIpMap_List.end()) {
        m2i_iter->second=cnu_ip;
    } else {
        p = MacToIpMap_List.insert((make_pair(mac_string,cnu_ip)));
        if(p.second == 0) {
            errstrm << "insert this cnu ip in map false" << endl;
            return BASE_ERROR;
        } else
            dbgstrm << "insert this client  to map success" << endl;
    }
    return BASE_OK;
}

BASE_ErrCode 
DeleteMacToIpMap(MacAddressT cnu_mac)
{
    string mac_string = MacToString(cnu_mac.addr);

    MacToIpMap_ListT::iterator m2i_iter=MacToIpMap_List.find(mac_string);
    if(m2i_iter!=MacToIpMap_List.end()) {
        MacToIpMap_List.erase(m2i_iter);
        dbgstrm << "delete map entry success" << endl;
    } else {
        errstrm << "this mac is not in mac2ip list" << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode 
SearchCnuMacToIpMap(MacAddressT cnu_mac , U32** cnu_ipaddress)
{
    assert(cnu_ipaddress);
    string mac_string = MacToString(cnu_mac.addr);

    MacToIpMap_ListT::iterator m2i_iter=MacToIpMap_List.find(mac_string);
    if(m2i_iter!=MacToIpMap_List.end()) {
        *cnu_ipaddress = &(m2i_iter->second);
    } else {
        dbgstrm << "this mac is not in mac2ip " << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

void 
DumpCnuMacToIpMap()
{
    MacToIpMap_ListT::iterator m2i_iter=MacToIpMap_List.begin();
    cout << "\n\r ~~~~~~~~~~~~~~~~Dump IP~~~~~~~~~~~~~~~~" << endl;
    while(m2i_iter!=MacToIpMap_List.end()) {
        cout << m2i_iter->first << "    |    ip = 0x" << hex <<m2i_iter->second << endl;
        m2i_iter++;
    }
}
/**************************************************************************/


//Mac To Bandwidth Function
typedef map<std::string,DS_QoS_InfoDBT> MacToBandnwidthMap_ListT;
MacToBandnwidthMap_ListT        MacToBandnwidthMap_List;

BASE_ErrCode 
ClearMacToBandnWidthMap()
{
    MacToBandnwidthMap_List.clear();
    return BASE_OK;
}

BASE_ErrCode 
AddMacToBandnWidthMap(DS_QoS_InfoDBT* cnu_ds_qos)
{
    assert(cnu_ds_qos);
    string mac_string = MacToString(cnu_ds_qos->ds_qos_cnu_mac.addr);

    pair<std::map<std::string,DS_QoS_InfoDBT>::iterator,bool> p;

    MacToBandnwidthMap_ListT::iterator m2b_iter=MacToBandnwidthMap_List.find(mac_string);
    if(m2b_iter!=MacToBandnwidthMap_List.end()) {
        memcpy(&(m2b_iter->second), cnu_ds_qos, sizeof(DS_QoS_InfoDBT));
    } else {
        DS_QoS_InfoDBT temp_cnu_ds_entry;
        memset(&temp_cnu_ds_entry, 0, sizeof(DS_QoS_InfoDBT));
        memcpy(&temp_cnu_ds_entry, cnu_ds_qos, sizeof(DS_QoS_InfoDBT));
        p = MacToBandnwidthMap_List.insert((make_pair(mac_string,temp_cnu_ds_entry)));
        if(p.second == 0) {
            cout << "insert this cnu Bandnwidth in map false" << endl;
            return BASE_ERROR;
        } else
            dbgstrm << "insert this Bandnwidth  to map success" << endl;
    }
    return BASE_OK;
}

BASE_ErrCode 
DeleteMacToBandnWidthMap(MacAddressT cnu_mac)
{
    string mac_string = MacToString(cnu_mac.addr);

    MacToBandnwidthMap_ListT::iterator m2b_iter=MacToBandnwidthMap_List.find(mac_string);
    if(m2b_iter!=MacToBandnwidthMap_List.end()) {
        MacToBandnwidthMap_List.erase(m2b_iter);
        dbgstrm << "delete map Bandnwidth entry success" << endl;
    } else {
        errstrm << "this mac is not in mac2Bandnwidth list" << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode 
SearchCnuMacToBandnWidthMap(MacAddressT cnu_mac , DS_QoS_InfoDBT* cnu_ds_qos)
{
    assert(cnu_ds_qos);
    string mac_string = MacToString(cnu_mac.addr);

    MacToBandnwidthMap_ListT::iterator m2b_iter=MacToBandnwidthMap_List.find(mac_string);
    if(m2b_iter!=MacToBandnwidthMap_List.end()) {
        memcpy(cnu_ds_qos, &(m2b_iter->second), sizeof(DS_QoS_InfoDBT));
    } else {
        dbgstrm << "this mac is not in mac2Bandnwidth " << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

void 
DumpCnuMacToBandnWidthMap()
{
    MacToBandnwidthMap_ListT::iterator m2b_iter=MacToBandnwidthMap_List.begin();
    cout << "\n\r~~~~~~~~~~~~~~~~Dump Bandnwidth~~~~~~~~~~~~~~~~" << endl;
    while(m2b_iter!=MacToBandnwidthMap_List.end()) {
        cout << m2b_iter->first << "    |    Bandnwidth = " << m2b_iter->second.bandwidth << endl;
        m2b_iter++;
    }
}

BASE_ErrCode 
GetCmFirstDownStreamQos(DS_QoS_InfoDBT* first_ds_qos)
{
    assert(first_ds_qos);
    MacToBandnwidthMap_ListT::iterator m2b_iter=MacToBandnwidthMap_List.begin();
    if(m2b_iter!=MacToBandnwidthMap_List.end()) {
        memcpy(first_ds_qos, &(m2b_iter->second), sizeof(DS_QoS_InfoDBT));
    } else {
        dbgstrm << "there is no qos been setted " << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode 
GetCmNextDownStreamQos(DS_QoS_InfoDBT now_ds_qos, DS_QoS_InfoDBT* next_ds_qos)
{
    assert(next_ds_qos);
    string mac_string = MacToString(now_ds_qos.ds_qos_cnu_mac.addr);

    MacToBandnwidthMap_ListT::iterator m2b_iter=MacToBandnwidthMap_List.find(mac_string);
    m2b_iter++;
    if(m2b_iter!=MacToBandnwidthMap_List.end()) {
        memcpy(next_ds_qos, &(m2b_iter->second), sizeof(DS_QoS_InfoDBT));
    } else {
        dbgstrm << "there is no more ds qos " << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}
/**************************************************************************/


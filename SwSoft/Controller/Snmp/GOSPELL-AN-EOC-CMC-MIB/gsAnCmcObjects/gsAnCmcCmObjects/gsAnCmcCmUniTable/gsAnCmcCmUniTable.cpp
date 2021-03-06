/*
 * Note: this file originally auto-generated by mib2c using
 *  $
 */
#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "Util.h"
#include "Debug.h"
#include "CnuStatusMgt.h"
#include "gsAnCmcCmUniTable.h"
using namespace std;

CxxBeginNameSpace(Snmp)
CxxBeginNameSpace(GsAnCmcCmUniTalbe)

#define MaxCmUuiEntryNum 256

#define SingleKeysIndex
struct CmcCmUniEntryIndex
{
    int32_t cmStausIndex;
    uint8_t cmUserMac[6];
};

inline ostream& operator << (ostream& os, CmcCmUniEntryIndex const& index)
{
    os << "Index = " << index.cmStausIndex << ", "
        << "Mac = " <<  MacToString(index.cmUserMac);
    return os;
}

CmcCmUniEntryIndex indexList[MaxCmUuiEntryNum];

typedef int (*SnmpOperationHandler)(netsnmp_mib_handler*,
    netsnmp_handler_registration*,
    netsnmp_agent_request_info*,
    netsnmp_request_info*);

static int GsAnCmcCmUniGet(netsnmp_mib_handler*, netsnmp_handler_registration*,
    netsnmp_agent_request_info*, netsnmp_request_info*);
static int GsAnCmcCmUniSetReserve1(netsnmp_mib_handler*,
    netsnmp_handler_registration*,
    netsnmp_agent_request_info*,
    netsnmp_request_info*);
static int GsAnCmcCmUniSetReserve2(netsnmp_mib_handler*,
    netsnmp_handler_registration*,
    netsnmp_agent_request_info*,
    netsnmp_request_info*);
static int GsAnCmcCmUniSetFree(netsnmp_mib_handler*,
    netsnmp_handler_registration*,
    netsnmp_agent_request_info*,
    netsnmp_request_info*);
static int GsAnCmcCmUniSetAction(netsnmp_mib_handler*,
    netsnmp_handler_registration*,
    netsnmp_agent_request_info*,
    netsnmp_request_info*);
static int GsAnCmcCmUniSetUndo(netsnmp_mib_handler*,
    netsnmp_handler_registration*,
    netsnmp_agent_request_info*,
    netsnmp_request_info*);
static int GsAnCmcCmUniSetCommit(netsnmp_mib_handler*,
    netsnmp_handler_registration*,
    netsnmp_agent_request_info*,
    netsnmp_request_info*);

typedef void (*SnmpGetHandler)(netsnmp_agent_request_info*,
    netsnmp_request_info*,
    CMCOAM_CnuStatusT*);

static void SnmpGetCmUserMacAddr(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmUserIpAddrType(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmUserIpAddr(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmConnectState(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmOnlineTime(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmDocsisVersion(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmTimeOffSet(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmMicroReflect(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmPrimaryUsChannel(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmPrimaryDsChannel(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmUsSNR(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmUsPower(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmDsSNR(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmDsPower(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmNumberOfCPEs(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmConfigFile(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpCmResetNow(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmDeleteNow(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmUsBandWidth(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);
static void SnmpGetCmDsBandWidth(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry);

static map<int, SnmpOperationHandler> snmpHandler;
static map<int, SnmpGetHandler> snmpGetValueHandler;

/** Initializes the gsAnCmcCmUniTable module */
/** Initialize the gsAnCmcCmUniTable table by defining its contents and how it's structured */
void InitGsAnCmcCmUniTable(void)
{
    const oid cmUniTableOid[] = {1, 3, 6, 1, 4, 1, 22764, 1015, 9026, 2, 1, 2};
    const size_t cmUniTableOidLen = OID_LENGTH(cmUniTableOid);

    netsnmp_handler_registration *reg;
    reg = netsnmp_create_handler_registration("gsAnCmcCmUniTable",
        GsAnCmcCmUniTableHandler, cmUniTableOid, cmUniTableOidLen, HANDLER_CAN_RWRITE);

    netsnmp_table_registration_info *tableInfo;
    tableInfo = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);

#ifdef SingleKeysIndex
    /* index: docsIfCmtsCmStatusIndex, gsAnCmcCmUserMacAddress */
    netsnmp_table_helper_add_indexes(tableInfo, ASN_OCTET_STR, 0);
#else
    /* index: docsIfCmtsCmStatusIndex, gsAnCmcCmUserMacAddress */
    netsnmp_table_helper_add_indexes(tableInfo, ASN_INTEGER, ASN_OCTET_STR, 0);
#endif
    tableInfo->min_column = ColCmUserMacAddr;
    tableInfo->max_column = ColCmDsBandWidth;

    netsnmp_iterator_info *iinfo;
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = GsAnCmcCmUniTableGetFirst;
    iinfo->get_next_data_point = GsAnCmcCmUniTableGetNext;
    iinfo->table_reginfo = tableInfo;

    netsnmp_register_table_iterator(reg, iinfo);

    /* Initialize the contents of the table here */
    snmpHandler.insert(make_pair(MODE_GET, GsAnCmcCmUniGet));
    snmpHandler.insert(make_pair(MODE_SET_RESERVE1, GsAnCmcCmUniSetReserve1));
    snmpHandler.insert(make_pair(MODE_SET_RESERVE2, GsAnCmcCmUniSetReserve2));
    snmpHandler.insert(make_pair(MODE_SET_FREE, GsAnCmcCmUniSetFree));
    snmpHandler.insert(make_pair(MODE_SET_ACTION, GsAnCmcCmUniSetAction));
    snmpHandler.insert(make_pair(MODE_SET_UNDO, GsAnCmcCmUniSetUndo));
    snmpHandler.insert(make_pair(MODE_SET_COMMIT, GsAnCmcCmUniSetCommit));

    snmpGetValueHandler.insert(make_pair(ColCmUserMacAddr, SnmpGetCmUserMacAddr));
    snmpGetValueHandler.insert(make_pair(ColCmUserIpAddrType, SnmpGetCmUserIpAddrType));
    snmpGetValueHandler.insert(make_pair(ColCmUserIpAddr, SnmpGetCmUserIpAddr));
    snmpGetValueHandler.insert(make_pair(ColCmConnectState, SnmpGetCmConnectState));
    snmpGetValueHandler.insert(make_pair(ColCmOnlineTime, SnmpGetCmOnlineTime));
    snmpGetValueHandler.insert(make_pair(ColCmDocsisVersion, SnmpGetCmDocsisVersion));
    snmpGetValueHandler.insert(make_pair(ColCmTimeOffSet, SnmpGetCmTimeOffSet));
    snmpGetValueHandler.insert(make_pair(ColCmMicroReflect, SnmpGetCmMicroReflect));
    snmpGetValueHandler.insert(make_pair(ColCmPrimaryUsChannel, SnmpGetCmPrimaryUsChannel));
    snmpGetValueHandler.insert(make_pair(ColCmPrimaryDsChannel, SnmpGetCmPrimaryDsChannel));
    snmpGetValueHandler.insert(make_pair(ColCmUsSNR, SnmpGetCmUsSNR));
    snmpGetValueHandler.insert(make_pair(ColCmUsPower, SnmpGetCmUsPower));
    snmpGetValueHandler.insert(make_pair(ColCmDsSNR, SnmpGetCmDsSNR));
    snmpGetValueHandler.insert(make_pair(ColCmDsPower, SnmpGetCmDsPower));
    snmpGetValueHandler.insert(make_pair(ColCmNumberOfCPEs, SnmpGetCmNumberOfCPEs));
    snmpGetValueHandler.insert(make_pair(ColCmConfigFile, SnmpGetCmConfigFile));
    snmpGetValueHandler.insert(make_pair(ColCmResetNow, SnmpCmResetNow));
    snmpGetValueHandler.insert(make_pair(ColCmDeleteNow, SnmpGetCmDeleteNow));
    snmpGetValueHandler.insert(make_pair(ColCmUsBandWidth, SnmpGetCmUsBandWidth));
    snmpGetValueHandler.insert(make_pair(ColCmDsBandWidth, SnmpGetCmDsBandWidth));
}

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *GsAnCmcCmUniTableGetFirst(void **loopContext,
    void **dataContext,
    netsnmp_variable_list *indexData,
    netsnmp_iterator_info *data)
{
    BASE_ErrCode errCode;
    dbgstrm << "Start." << endl;

    CMCOAM_CnuStatusT cmInfo;
    memset(&cmInfo, 0, sizeof(CMCOAM_CnuStatusT));
    errCode = GetFirstCMInfoMgt(&cmInfo);
    if (BASE_OK != errCode)
    {
        dbgstrm << "GetFirstCMInfo() failed." << endl;
        *loopContext = NULL;
        *dataContext = NULL;
        return NULL;
    }
    CmcCmUniEntryIndex* index = indexList;
    index->cmStausIndex = (int32_t) cmInfo.cnu_index;
    CopyMacAddr(index->cmUserMac, cmInfo.cnu_mac);

#ifdef SingleKeysIndex
    snmp_set_var_value(indexData, index->cmUserMac, 6);
#else
    snmp_set_var_value(indexData, &index->cmStausIndex, sizeof(int32_t));
    snmp_set_var_value(indexData->next_variable, index->cmUserMac, 6);
#endif

    dbgstrm << "index information: " << *index << endl;

    *loopContext = (void *) index;
    *dataContext = (void *) index;
    return indexData;
}

netsnmp_variable_list *GsAnCmcCmUniTableGetNext(void **loopContext,
    void **dataContext,
    netsnmp_variable_list *indexData,
    netsnmp_iterator_info *data)
{
    BASE_ErrCode errCode;
    CmcCmUniEntryIndex* index = (CmcCmUniEntryIndex*) *loopContext;

    dbgstrm << "Start." << endl;
    if (index + 1 == indexList + MaxCmUuiEntryNum)
    {
        dbgstrm << "Exceeded max entry number." << endl;
        *loopContext = NULL;
        *dataContext = NULL;
        return NULL;
    }

    CMCOAM_CnuStatusT cmInfo;
    cmInfo.cnu_index = (int32_t) index->cmStausIndex;
    CopyMacAddr(cmInfo.cnu_mac, index->cmUserMac);

    errCode = GetNextCMInfoMgt(&cmInfo, &cmInfo);
    if (BASE_OK != errCode)
    {
        dbgstrm << "GetNextCMInfoMgt() failed." << endl;
        *loopContext = NULL;
        *dataContext = NULL;
        return NULL;
    }
    ++index;
    index->cmStausIndex = (int32_t) cmInfo.cnu_index;
    CopyMacAddr(index->cmUserMac, cmInfo.cnu_mac);

#ifdef SingleKeysIndex
    snmp_set_var_value(indexData, index->cmUserMac, 6);
#else
    snmp_set_var_value(indexData, &index->cmStausIndex, sizeof(int32_t));
    snmp_set_var_value(indexData->next_variable, index->cmUserMac, 6);
#endif

    dbgstrm << "index information: " << *index << endl;

    *loopContext = (void *) index;
    *dataContext = (void *) index;

    return indexData;
}

void SnmpGetCmUserMacAddr(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
        entry->cnu_mac, 6);
}

void SnmpGetCmUserIpAddrType(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    /* mib's ipv4 value is differ with IpAddrType of CmcOamCm.h of ControllerSource.
     * Refer to CmcOamCm.c of OamApi of CmcControllerSource for detail.
     * We support Ipv4 only.
     */
    assert(entry->ip_addr_type == 2);
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 1);
}

void SnmpGetCmUserIpAddr(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    //RunDong's cm do not hold this word,but Thomson'cm.
    snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
        entry->ip_addr, entry->ip_addr_len);
}

void SnmpGetCmConnectState(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
        entry->state);
}

void SnmpGetCmOnlineTime(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_TIMETICKS,
        entry->lr_timestamp * 100);
}

void SnmpGetCmDocsisVersion(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, entry->docsis_ver);
}

void SnmpGetCmTimeOffSet(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
        entry->upstream[0].time_offset);
}

void SnmpGetCmMicroReflect(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
        entry->upstream[0].micro_reflect);
}

void SnmpGetCmPrimaryUsChannel(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
        entry->primary_us_id);
}

void SnmpGetCmPrimaryDsChannel(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
        entry->primary_ds_id);
}

void SnmpGetCmUsSNR(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
        entry->upstream[0].snr);
}

void SnmpGetCmUsPower(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER,
        entry->upstream[0].power_level);
}

void SnmpGetCmDsSNR(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 0);
}

void SnmpGetCmDsPower(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 0);
}

void SnmpGetCmNumberOfCPEs(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 0);
}

void SnmpGetCmConfigFile(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    char const* CmConfigFileName = "cmc_modem.cfg";
    snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
        CmConfigFileName, strlen(CmConfigFileName));
}

void SnmpCmResetNow(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 2);
}

void SnmpGetCmDeleteNow(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 2);
}

void SnmpGetCmUsBandWidth(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 0);
}

void SnmpGetCmDsBandWidth(netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *request, CMCOAM_CnuStatusT *entry)
{
    snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, 0);
}

int GsAnCmcCmUniGet(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    netsnmp_request_info *request;
    netsnmp_table_request_info *tableInfo;
    CmcCmUniEntryIndex* index;
    BASE_ErrCode errCode;

    dbgstrm << "Start." << endl;
    for (request = requests; request; request = request->next)
    {
        index = (CmcCmUniEntryIndex*) netsnmp_extract_iterator_context(request);
        if (index == NULL)
        {
            dbgstrm << "index is NULL" << endl;
            netsnmp_set_request_error(reqInfo, request, SNMP_NOSUCHINSTANCE);
            continue;
        }
        dbgstrm << "index information: " << *index << endl;

        CMCOAM_CnuStatusT cmInfo;
        MacAddressT mac;
        CopyMacAddr(mac.addr, index->cmUserMac);
        errCode = FindCMInfoMgt(mac, &cmInfo);
        if (BASE_OK != errCode)
        {
            netsnmp_set_request_error(reqInfo, request, SNMP_NOSUCHINSTANCE);
            continue;
        }

        tableInfo = netsnmp_extract_table_info(request);

        map<int, SnmpGetHandler>::iterator iter;
        iter = snmpGetValueHandler.find(tableInfo->colnum);
        if (iter != snmpGetValueHandler.end())
        {
            iter->second(reqInfo, requests, &cmInfo);
        }
        else
        {
            dbgstrm << "Error, wrong table column number";
            netsnmp_set_request_error(reqInfo, request, SNMP_NOSUCHOBJECT);
        }
    }

    return SNMP_ERR_NOERROR;
}

int GsAnCmcCmUniSetReserve1(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    U32 ret;
    netsnmp_request_info *request;
    netsnmp_table_request_info *tableInfo;

    dbgstrm << "Start." << endl;

    for (request = requests; request; request = request->next)
    {
        tableInfo = netsnmp_extract_table_info(request);
        switch (tableInfo->colnum)
        {
        case ColCmResetNow:
            /* or possibly 'netsnmp_check_vb_type_and_size' */
            ret = netsnmp_check_vb_int_range(request->requestvb, 1, 1);
            if (ret != SNMP_ERR_NOERROR)
            {
                netsnmp_set_request_error(reqInfo, request, ret);
                return SNMP_ERR_NOERROR;
            }
            break;

        case ColCmDeleteNow:
            /* or possibly 'netsnmp_check_vb_type_and_size' */
            ret = netsnmp_check_vb_int_range(request->requestvb, 1, 1);
            if (ret != SNMP_ERR_NOERROR)
            {
                netsnmp_set_request_error(reqInfo, request, ret);
                return SNMP_ERR_NOERROR;
            }
            break;

        default:
            netsnmp_set_request_error(reqInfo, request, SNMP_ERR_NOTWRITABLE);
            return SNMP_ERR_NOERROR;
        }
    }
    return SNMP_ERR_NOERROR;
}

int GsAnCmcCmUniSetReserve2(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int GsAnCmcCmUniSetFree(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int GsAnCmcCmUniSetAction(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    netsnmp_request_info *request;
    netsnmp_table_request_info *tableInfo;

    BASE_ErrCode errCode;
    CmcCmUniEntryIndex* index;
    MacAddressT mac;

    dbgstrm << "Start." << endl;

    for (request = requests; request; request = request->next)
    {
        index = (CmcCmUniEntryIndex*) netsnmp_extract_iterator_context(request);
        if (index == NULL)
        {
            dbgstrm << "index is NULL" << endl;
            netsnmp_set_request_error(reqInfo, request, SNMP_NOSUCHINSTANCE);
            break;
        }
        dbgstrm << "index information: " << *index << endl;

        CopyMacAddr(mac.addr, index->cmUserMac);
        tableInfo = netsnmp_extract_table_info(request);
        switch (tableInfo->colnum)
        {
        case ColCmResetNow:
            errCode = CMCOAM_Cm_ClearCableModem(mac, cliGetCmc());
            if (BASE_OK != errCode)
            {
                netsnmp_set_request_error(reqInfo, request, SNMP_ERR_GENERR);
                break;
            }
            break;

        case ColCmDeleteNow:
            errCode = CMCOAM_Cm_ClearCableModem(mac, cliGetCmc());
            if (BASE_OK != errCode)
            {
                netsnmp_set_request_error(reqInfo, request, SNMP_ERR_GENERR);
                break;
            }
            break;

        default:
            netsnmp_set_request_error(reqInfo, request, SNMP_ERR_NOTWRITABLE);
            return SNMP_ERR_NOERROR;
        }
    }
    return SNMP_ERR_NOERROR;
}

int GsAnCmcCmUniSetUndo(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int GsAnCmcCmUniSetCommit(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

/** handles requests for the gsCableDocsServiceFlowCfgTable table */
int GsAnCmcCmUniTableHandler(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests)
{
    map<int, SnmpOperationHandler>::iterator iter;
    iter = snmpHandler.find(reqInfo->mode);
    assert(iter != snmpHandler.end());
    iter->second(handler, regInfo, reqInfo, requests);

    return SNMP_ERR_NOERROR;
}

CxxEndNameSpace /* namespace GsAnCmcCmUniTalbe */
CxxEndNameSpace /* namespace Snmp */

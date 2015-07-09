/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 17695 2009-07-21 12:22:18Z dts12 $
 */
#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "Debug.h"
#include "ChannelMgt.h"
#include "docsIfDownstreamChannelTable.h"

using namespace std;

static CMCOAM_DsChannelCfgT dsChannelCfg[BRCM_CMC_MAX_DS_CH];

#define BcmModulationQam64   0
#define BcmModulationQam256  1
#define BcmModulationQam1024 2
#define StdModulationUnknown 1
#define StdModulationOther   2
#define StdModulationQam64   3
#define StdModulationQam256  4
uint8_t modulatoinBcmToStdMap[] =
{
    StdModulationQam64, StdModulationQam256, StdModulationOther
};
uint8_t modulatoinStdToBcmMap[] =
{
    BcmModulationQam64, BcmModulationQam64, BcmModulationQam1024,
    BcmModulationQam64, BcmModulationQam256
};

#define BcmAnnexA  0
#define BcmAnnexB  1
#define StdAnnexUnknown  1
#define StdAnnexOther    2
#define StdAnnexA        3
#define StdAnnexB        4
uint8_t annexBcmToStdMap[] = {StdAnnexA, StdAnnexA};
uint8_t annexStdToBcmMap[] = {BcmAnnexA, BcmAnnexA, BcmAnnexA, BcmAnnexA,BcmAnnexB};

typedef int(*SnmpOperationHandler)(netsnmp_mib_handler*,
                                   netsnmp_handler_registration*,
                                   netsnmp_agent_request_info*,
                                   netsnmp_request_info*);

static int DocsIfDownChannelGet(netsnmp_mib_handler*, netsnmp_handler_registration*,
                                netsnmp_agent_request_info*, netsnmp_request_info*);
static int DocsIfDownChannelSetReserve1(netsnmp_mib_handler*,
                                        netsnmp_handler_registration*,
                                        netsnmp_agent_request_info*,
                                        netsnmp_request_info*);
static int DocsIfDownChannelSetReserve2(netsnmp_mib_handler*,
                                        netsnmp_handler_registration*,
                                        netsnmp_agent_request_info*,
                                        netsnmp_request_info*);
static int DocsIfDownChannelSetFree(netsnmp_mib_handler*,
                                    netsnmp_handler_registration*,
                                    netsnmp_agent_request_info*,
                                    netsnmp_request_info*);
static int DocsIfDownChannelSetAction(netsnmp_mib_handler*,
                                      netsnmp_handler_registration*,
                                      netsnmp_agent_request_info*,
                                      netsnmp_request_info*);
static int DocsIfDownChannelSetUndo(netsnmp_mib_handler*,
                                    netsnmp_handler_registration*,
                                    netsnmp_agent_request_info*,
                                    netsnmp_request_info*);
static int DocsIfDownChannelSetCommit(netsnmp_mib_handler*,
                                      netsnmp_handler_registration*,
                                      netsnmp_agent_request_info*,
                                      netsnmp_request_info*);

typedef void(*SnmpSetValueToPktHandler)(netsnmp_agent_request_info*,
                                        netsnmp_request_info*,
                                        CMCOAM_DsChannelCfgT*);

static void SnmpSetValueToReqestId(netsnmp_agent_request_info *reqInfo,
                                   netsnmp_request_info       *request,
                                   CMCOAM_DsChannelCfgT       *entry);
static void SnmpSetValueToReqestFrequency(netsnmp_agent_request_info *reqInfo,
                                          netsnmp_request_info       *request,
                                          CMCOAM_DsChannelCfgT       *entry);
static void SnmpSetValueToReqestWidth(netsnmp_agent_request_info *reqInfo,
                                      netsnmp_request_info       *request,
                                      CMCOAM_DsChannelCfgT       *entry);
static void SnmpSetValueToReqestModulation(netsnmp_agent_request_info *reqInfo,
                                           netsnmp_request_info       *request,
                                           CMCOAM_DsChannelCfgT       *entry);
static void SnmpSetValueToReqestInterleave(netsnmp_agent_request_info *reqInfo,
                                           netsnmp_request_info       *request,
                                           CMCOAM_DsChannelCfgT       *entry);
static void SnmpSetValueToReqestPower(netsnmp_agent_request_info *reqInfo,
                                      netsnmp_request_info       *request,
                                      CMCOAM_DsChannelCfgT       *entry);
static void SnmpSetValueToReqestAnnex(netsnmp_agent_request_info *reqInfo,
                                      netsnmp_request_info       *request,
                                      CMCOAM_DsChannelCfgT       *entry);
static void SnmpSetValueToReqestStorageType(netsnmp_agent_request_info *reqInfo,
                                            netsnmp_request_info       *request,
                                            CMCOAM_DsChannelCfgT       *entry);

static map<int, SnmpOperationHandler> snmpHandler;
static map<int, SnmpSetValueToPktHandler> snmpSetValueHandler;

/** Initializes the docsIfDownstreamChannelTable module */
void InitDocsIfDownChannelTable(void)
{
  /* here we initialize all the tables we're planning on supporting */
    const oid    tableOid[] = {1,3,6,1,2,1,10,127,1,1,1};
    const size_t oidLen     = OID_LENGTH(tableOid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;

    reg = netsnmp_create_handler_registration("docsIfDownstreamChannelTable",
                                              DocsIfDownChannelHandler,
                                              tableOid, oidLen,
                                              HANDLER_CAN_RWRITE);

    netsnmp_table_registration_info *tableInfo;
    tableInfo = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(tableInfo,
                                     ASN_INTEGER,  /* index: ifIndex */
                                     0);
    tableInfo->min_column = ColumnDocsIfDownChannelId;
    tableInfo->max_column = ColumnDocsIfDownChannelStorageType;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = DocsIfDownChannelGetFirst;
    iinfo->get_next_data_point  = DocsIfDownChannelGetNext;
    iinfo->table_reginfo        = tableInfo;

    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialize the contents of the table here */
    snmpHandler.insert(make_pair(MODE_GET, DocsIfDownChannelGet));
    snmpHandler.insert(make_pair(MODE_SET_RESERVE1, DocsIfDownChannelSetReserve1));
    snmpHandler.insert(make_pair(MODE_SET_RESERVE2, DocsIfDownChannelSetReserve2));
    snmpHandler.insert(make_pair(MODE_SET_FREE, DocsIfDownChannelSetFree));
    snmpHandler.insert(make_pair(MODE_SET_ACTION, DocsIfDownChannelSetAction));
    snmpHandler.insert(make_pair(MODE_SET_UNDO, DocsIfDownChannelSetUndo));
    snmpHandler.insert(make_pair(MODE_SET_COMMIT, DocsIfDownChannelSetCommit));

    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelId, SnmpSetValueToReqestId));
    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelFrequency, SnmpSetValueToReqestFrequency));
    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelWidth, SnmpSetValueToReqestWidth));
    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelModulation, SnmpSetValueToReqestModulation));
    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelInterleave, SnmpSetValueToReqestInterleave));
    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelPower, SnmpSetValueToReqestPower));
    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelAnnex, SnmpSetValueToReqestAnnex));
    snmpSetValueHandler.insert(make_pair(ColumnDocsIfDownChannelStorageType, SnmpSetValueToReqestStorageType));
}

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list* DocsIfDownChannelGetFirst(void **loopContext,
                                                 void **dataContext,
                                                 netsnmp_variable_list *indexData,
                                                 netsnmp_iterator_info *data)
{
    dbgstrm << "Start." << endl;
    CMCOAM_DsChannelCfgT cfg, *i = dsChannelCfg;
    BASE_ErrCode ret;

    for (ret = MgtChannelGetFirstDownstreamSettings(&cfg);
        ret == BASE_OK;
        ret = MgtChannelGetNextDownstreamSettings(&cfg, &cfg))
    {
        /* Broadcom and Standard has different enumeration value, we adjust
         * enumeration from Broadcom to Standard here.
         */
        assert(cfg.modulatoin < 3 && cfg.annex < 2);
        cfg.modulatoin = modulatoinBcmToStdMap[cfg.modulatoin];
        cfg.annex = annexBcmToStdMap[cfg.annex];
        cfg.interleaver = 1;

        memcpy(i++, &cfg, sizeof(CMCOAM_DsChannelCfgT));
    }

    if (i == dsChannelCfg)
    {
        dbgstrm << "no entry." << endl;
        *dataContext = NULL;
        *loopContext = NULL;
        return NULL;
    }

    dbgstrm << "channel id = " << (int)dsChannelCfg[0].ds_ch_id << endl;
    *dataContext = dsChannelCfg;
    *loopContext = dsChannelCfg;
    snmp_set_var_value(indexData, &dsChannelCfg[0].ds_ch_id, sizeof(uint8_t));

    return indexData;
}

/* The GetNext() function is called by net-snmp library to iterate all table index,
 * terminate loop by return NULL.
 */
netsnmp_variable_list* DocsIfDownChannelGetNext(void **loopContext,
                                                void **dataContext,
                                                netsnmp_variable_list *indexData,
                                                netsnmp_iterator_info *data)
{
    dbgstrm << "Start." << endl;
    CMCOAM_DsChannelCfgT *cfg = (CMCOAM_DsChannelCfgT *)*loopContext + 1;
    if (cfg < dsChannelCfg + BRCM_CMC_MAX_DS_CH)
    {
        dbgstrm << "channel id = " << (int)cfg->ds_ch_id << endl;
        *dataContext = cfg;
        *loopContext = cfg;
        snmp_set_var_value(indexData, &cfg->ds_ch_id, sizeof(uint8_t));
        return indexData;
    }

    dbgstrm << "no more entry." << endl;
    *loopContext = NULL;
    *dataContext = NULL;
    return NULL;
}

void SnmpSetValueToReqestId(netsnmp_agent_request_info *reqInfo,
                            netsnmp_request_info       *request,
                            CMCOAM_DsChannelCfgT       *entry)
{
    int32_t value = entry->ds_ch_id;
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

void SnmpSetValueToReqestFrequency(netsnmp_agent_request_info *reqInfo,
                                   netsnmp_request_info       *request,
                                   CMCOAM_DsChannelCfgT       *entry)
{
    int32_t value = entry->freq;
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

void SnmpSetValueToReqestWidth(netsnmp_agent_request_info *reqInfo,
                               netsnmp_request_info       *request,
                               CMCOAM_DsChannelCfgT       *entry)
{
    int32_t value = 6000000;  /* "North America" standard */
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

void SnmpSetValueToReqestModulation(netsnmp_agent_request_info *reqInfo,
                                    netsnmp_request_info       *request,
                                    CMCOAM_DsChannelCfgT       *entry)
{
    int32_t value = entry->modulatoin;
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

void SnmpSetValueToReqestInterleave(netsnmp_agent_request_info *reqInfo,
                                    netsnmp_request_info       *request,
                                    CMCOAM_DsChannelCfgT       *entry)
{
    int32_t value = entry->interleaver;
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

void SnmpSetValueToReqestPower(netsnmp_agent_request_info *reqInfo,
                               netsnmp_request_info       *request,
                               CMCOAM_DsChannelCfgT       *entry)
{
    int32_t value = entry->power_level;
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

void SnmpSetValueToReqestAnnex(netsnmp_agent_request_info *reqInfo,
                               netsnmp_request_info       *request,
                               CMCOAM_DsChannelCfgT       *entry)
{
    int32_t value = entry->annex;
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

void SnmpSetValueToReqestStorageType(netsnmp_agent_request_info *reqInfo,
                                     netsnmp_request_info       *request,
                                     CMCOAM_DsChannelCfgT       *entry)
{
    /* description: The storage type for this conceptual row. Entries with this
     * object set to permanent(4) do not require write operations for read-write
     * objects.
     */
    int32_t value = 0;
    snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                             &value, sizeof(int32_t));
}

int DocsIfDownChannelGet(netsnmp_mib_handler               *handler,
                         netsnmp_handler_registration      *regInfo,
                         netsnmp_agent_request_info        *reqInfo,
                         netsnmp_request_info              *requests)
{
    netsnmp_request_info       *request;
    netsnmp_table_request_info *tableInfo;
    CMCOAM_DsChannelCfgT* entry;

    dbgstrm << "Start." << endl;
    for (request=requests; request; request=request->next)
    {
        entry = (CMCOAM_DsChannelCfgT*)netsnmp_extract_iterator_context(request);
        if (entry == NULL)
        {
            dbgstrm << "entry is NULL" << endl;
            netsnmp_set_request_error(reqInfo, request, SNMP_NOSUCHINSTANCE);
            continue;
        }
        tableInfo  = netsnmp_extract_table_info(request);

        map<int, SnmpSetValueToPktHandler>::iterator iter;
        iter = snmpSetValueHandler.find(tableInfo->colnum);
        if(iter != snmpSetValueHandler.end())
        {
            iter->second(reqInfo, requests, entry);
        }
        else
        {
            dbgstrm << "Error, wrong table column number";
            netsnmp_set_request_error(reqInfo, request, SNMP_NOSUCHOBJECT);
        }
    }

    dbgstrm << "End." << endl;
    return SNMP_ERR_NOERROR;
}

int DocsIfDownChannelSetReserve1(netsnmp_mib_handler               *handler,
                                 netsnmp_handler_registration      *regInfo,
                                 netsnmp_agent_request_info        *reqInfo,
                                 netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    netsnmp_request_info       *request;
    netsnmp_table_request_info *tableInfo;
    CMCOAM_DsChannelCfgT* entry;

    dbgstrm << "Start." << endl;
    for (request=requests; request; request=request->next)
    {
        entry = (CMCOAM_DsChannelCfgT*)netsnmp_extract_iterator_context(request);
        tableInfo = netsnmp_extract_table_info(request);
        if (entry == NULL)
        {
            errstrm << "entry is NULL. " << endl;
            netsnmp_set_request_error(reqInfo, request, SNMP_NOSUCHINSTANCE);
            break;
        }

        int32_t value;
        switch (tableInfo->colnum)
        {
        case ColumnDocsIfDownChannelFrequency:
            break;

        case ColumnDocsIfDownChannelWidth:
            break;

        case ColumnDocsIfDownChannelModulation:
            value = *request->requestvb->val.integer;
            if (value < StdModulationQam64 || value > StdModulationQam256)
            {
                netsnmp_set_request_error(reqInfo, request, SNMP_ERR_BADVALUE);
            }
            break;

        case ColumnDocsIfDownChannelInterleave:
            break;

        case ColumnDocsIfDownChannelPower:
            value = *request->requestvb->val.integer;
            MgtChannelSetDownstreamSettingsPowerLevel(entry->ds_ch_id, value);
            break;

        case ColumnDocsIfDownChannelAnnex:
            value = *request->requestvb->val.integer;
            if (value < StdAnnexA || value > StdAnnexB)
            {
                netsnmp_set_request_error(reqInfo, request, SNMP_ERR_BADVALUE);
            }
            break;

        case ColumnDocsIfDownChannelStorageType:
        default:
            break;
        }
    }
    return SNMP_ERR_NOERROR;
}

/* MODE_SET_RESERVE2:
 *    When create new entry, this function allocate resource for new entry.
 *    When modify or query entry,  do nothing.
 */
int DocsIfDownChannelSetReserve2(netsnmp_mib_handler               *handler,
                                 netsnmp_handler_registration      *regInfo,
                                 netsnmp_agent_request_info        *reqInfo,
                                 netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int DocsIfDownChannelSetFree(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *regInfo,
                             netsnmp_agent_request_info        *reqInfo,
                             netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

/*
 * When modify table,  the program flow is:
 *      SetReserve1()
 *      SetReserve2()
 *      SetAction()
 *      SetCommit()
 */
int DocsIfDownChannelSetCommit(netsnmp_mib_handler               *handler,
                               netsnmp_handler_registration      *regInfo,
                               netsnmp_agent_request_info        *reqInfo,
                               netsnmp_request_info              *requests)
{
    netsnmp_request_info       *request;
    netsnmp_table_request_info *tableInfo;
    CMCOAM_DsChannelCfgT* entry;

    dbgstrm << "Start." << endl;
    for (request=requests; request; request=request->next)
    {
        entry = (CMCOAM_DsChannelCfgT*)netsnmp_extract_iterator_context(request);
        tableInfo = netsnmp_extract_table_info(request);
        if (entry == NULL)
        {
            errstrm << "entry is NULL. " << endl;
            break;
        }

        int32_t value;
        switch (tableInfo->colnum)
        {
        case ColumnDocsIfDownChannelFrequency:
            value = *request->requestvb->val.integer;
            MgtChannelSetDownstreamSettingsFreq(entry->ds_ch_id, value);
            break;

        case ColumnDocsIfDownChannelWidth:
            break;

        case ColumnDocsIfDownChannelModulation:
            value = *request->requestvb->val.integer;
            value = modulatoinStdToBcmMap[value];
            MgtChannelSetDownstreamSettingsModulation(entry->ds_ch_id, value);
            break;

        case ColumnDocsIfDownChannelInterleave:
            break;

        case ColumnDocsIfDownChannelPower:
            value = *request->requestvb->val.integer;
            MgtChannelSetDownstreamSettingsPowerLevel(entry->ds_ch_id, value);
            break;

        case ColumnDocsIfDownChannelAnnex:
            value = *request->requestvb->val.integer;
            value = annexStdToBcmMap[value];
            MgtChannelSetDownstreamSettingsAnnexMode(entry->ds_ch_id, value);
            break;

        case ColumnDocsIfDownChannelStorageType:
        default:
            break;
        }
    }
    return SNMP_ERR_NOERROR;
}

int DocsIfDownChannelSetAction(netsnmp_mib_handler               *handler,
                               netsnmp_handler_registration      *regInfo,
                               netsnmp_agent_request_info        *reqInfo,
                               netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int DocsIfDownChannelSetUndo(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *regInfo,
                             netsnmp_agent_request_info        *reqInfo,
                             netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

/** handles requests for the netSnmpIETFWGTable table */
int DocsIfDownChannelHandler(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *regInfo,
                             netsnmp_agent_request_info        *reqInfo,
                             netsnmp_request_info              *requests)
{
    map<int, SnmpOperationHandler>::iterator iter;
    dbgstrm << "Start." << endl;

    iter = snmpHandler.find(reqInfo->mode);
    assert(iter != snmpHandler.end());
    iter->second(handler, regInfo, reqInfo, requests);

    dbgstrm << "End." << endl;
    return SNMP_ERR_NOERROR;
}
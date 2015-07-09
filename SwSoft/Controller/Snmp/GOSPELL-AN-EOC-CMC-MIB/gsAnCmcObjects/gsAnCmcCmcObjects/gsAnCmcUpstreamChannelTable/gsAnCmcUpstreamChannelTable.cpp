/*
 * Note: this file originally auto-generated by mib2c using
 *  $
 */
#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "Debug.h"
#include "ChannelMgt.h"

#include "gsAnCmcUpstreamChannelTable.h"
using namespace std;

static CMCOAM_UsChannelCfgT usChannelCfg[BRCM_CMC_MAX_US_CH];

/** Initializes the gsAnCmcUpstreamChannelTable module */
void InitGsAnCmcUsChannelTable(void)
{
    const oid CmcUsChannelTableOid[] = {1,3,6,1,4,1,22764,1015,9026,2,2,3};
    const size_t CmcUsChannelTableOidLen   = OID_LENGTH(CmcUsChannelTableOid);

    netsnmp_iterator_info *iinfo;
    netsnmp_handler_registration *reg;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("gsAnCmcUpstreamChannelTable:init", "initializing table gsAnCmcUpstreamChannelTable\n"));

    reg = netsnmp_create_handler_registration(
              "gsAnCmcUpstreamChannelTable", CmcUsChannelTableHandler,
              CmcUsChannelTableOid, CmcUsChannelTableOidLen,
              HANDLER_CAN_RWRITE
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_INTEGER,  /* index: gsAnCmcUpChannelId */
                           0);
    table_info->min_column = ColCmcUpChanErrRate;//ColCmcUpChanId
    table_info->max_column = ColCmcUpChanInputPower;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = CmcUsChannelTableGetFirst;
    iinfo->get_next_data_point = CmcUsChannelTableGetNext;
    iinfo->table_reginfo = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
CmcUsChannelTableGetFirst(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    dbgstrm << "Start." << endl;
    
    CMCOAM_UsChannelCfgT cfg, *i = usChannelCfg;

    BASE_ErrCode ret;
    for (ret = MgtChannelGetFirstUpstreamSettings(&cfg);
         ret == BASE_OK;
         ret = MgtChannelGetNextUpstreamSettings(&cfg, &cfg))
    {
        memcpy(i++, &cfg, sizeof(CMCOAM_UsChannelCfgT));
    }

    *my_loop_context = usChannelCfg;
    return CmcUsChannelTableGetNext(my_loop_context, my_data_context, put_index_data,  mydata);
    
}

netsnmp_variable_list *
CmcUsChannelTableGetNext(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    dbgstrm << "Start." << endl;
    
    CMCOAM_UsChannelCfgT *cfg = (CMCOAM_UsChannelCfgT *)*my_loop_context;
    if (cfg < usChannelCfg + BRCM_CMC_MAX_US_CH)
    {
        snmp_set_var_value(put_index_data, &cfg->us_ch_id, sizeof(uint8_t));
        *my_data_context = cfg++;
        *my_loop_context = cfg;
        return put_index_data;
    }

    *my_loop_context = NULL;
    *my_data_context = NULL;
    return NULL;  
}

typedef int(*CmcUsChanHandlerP)(netsnmp_mib_handler           *handler,
                                      netsnmp_handler_registration  *reginfo,
                                      netsnmp_agent_request_info    *reqinfo,
                                      netsnmp_request_info          *requests);

struct CmcUsChanTableHandlerDispath
{
    int mode;
    CmcUsChanHandlerP handler;
};

int CmcUsChanTableGet(netsnmp_mib_handler               *handler,
                                                    netsnmp_handler_registration      *reginfo,
                                                    netsnmp_agent_request_info        *reqinfo,
                                                    netsnmp_request_info              *requests)
{
    netsnmp_request_info *request;
    netsnmp_table_request_info *tableInfo;

    BASE_ErrCode errCode;
    CMCOAM_UsChannelCfgT *entry;
    char *UsChanName[4] = {"UsChan1","UsChan2","UsChan3","UsChan4"};

    dbgstrm << "Start." << endl;
    
    for (request=requests; request; request=request->next)
    {
        dbgstrm << "...." << endl;
        
        //This entry we get, it will changed each time
        entry = (CMCOAM_UsChannelCfgT*)netsnmp_extract_iterator_context(request);
        if (entry == NULL)
        {
            dbgstrm << "entry is NULL" << endl;
            netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHINSTANCE);
            continue;
        }
        dbgstrm << "index=" << entry->us_ch_id << endl;

        tableInfo = netsnmp_extract_table_info(request);
        switch (tableInfo->colnum)
        {            
            case ColCmcUpChanId:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanId << endl;
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, entry->us_ch_id);
            }
            break;            
            
            case ColCmcUpChanErrRate:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanErrRate << endl;
                int32_t ErrRate = 0; 
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, ErrRate);
            }
            break;
            
            case ColCmcUpChanSnr:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanSnr << endl;
                int32_t Snr = 0; 
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, Snr);
            }
            break;

            case ColCmcUpChanName:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanName << endl;
                snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR, 
                                UsChanName[entry->us_ch_id - 1], 8);
            }
            break;

            case ColCmcUpChanEnable:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanEnable << endl;
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, entry->enabled);
            }
            break;
            
            case ColCmcUpChanActiveCmCount:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanActiveCmCount << endl;
                int32_t ActiveCmCount = 0;
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, ActiveCmCount);
            }
            break;
            
            case ColCmcUpChanRegisterCmCount:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanRegisterCmCount << endl;
                int32_t RegisterCmCount = 0;
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, RegisterCmCount);
            }
            break;

            case ColCmcUpChanUnregisterCmCount:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanUnregisterCmCount << endl;
                int32_t UnregisterCmCount = 0;
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, UnregisterCmCount);
            }
            break;
            
            case ColCmcUpChanInputPower:
            {                
                dbgstrm << "Colnum = " << ColCmcUpChanInputPower << endl;
                U32 InputPower;
                errCode = MgtChannelGetUpstreamSettingsPowerLevel(entry->us_ch_id, &InputPower);
                if (errCode != BASE_OK)
                {
                    netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHINSTANCE);
                }                
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, InputPower);
            }
            break;                       

            default:
                netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHOBJECT);
            break;            
        }          
    }
    return SNMP_ERR_NOERROR;
}


int CmcUsChanTableSetReserve1(netsnmp_mib_handler      *handler,
                                                                         netsnmp_handler_registration      *reginfo,
                                                                         netsnmp_agent_request_info        *reqinfo,
                                                                         netsnmp_request_info              *requests)
{
    U32 ret;
    netsnmp_request_info *request;
    netsnmp_table_request_info *tableInfo;
   
    dbgstrm << "Start." << endl;
    
    for (request=requests ; request;  request=request->next)
    {
        tableInfo = netsnmp_extract_table_info(request);
        switch (tableInfo->colnum)
        {            
            case ColCmcUpChanEnable:
                /* or possibly 'netsnmp_check_vb_type_and_size' */
                ret = netsnmp_check_vb_int( request->requestvb ); 
                if ( ret != SNMP_ERR_NOERROR ) 
                {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
            break;
                
            case ColCmcUpChanInputPower:
                /* or possibly 'netsnmp_check_vb_type_and_size' */
                ret = netsnmp_check_vb_int( request->requestvb ); 
                if ( ret != SNMP_ERR_NOERROR ) 
                {
                    netsnmp_set_request_error( reqinfo, request, ret );
                    return SNMP_ERR_NOERROR;
                }
            break;
                
            default:
                netsnmp_set_request_error(reqinfo, request, SNMP_ERR_NOTWRITABLE);
            return SNMP_ERR_NOERROR;
        }
    }
    return SNMP_ERR_NOERROR;
}

int CmcUsChanTableSetReserve2(netsnmp_mib_handler      *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}
int CmcUsChanTableSetUndo(netsnmp_mib_handler               *handler,
                            netsnmp_handler_registration      *reginfo,
                            netsnmp_agent_request_info        *reqinfo,
                            netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int CmcUsChanTableSetFree(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int CmcUsChanTableSetAction(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    dbgstrm << "Start." << endl;
    return SNMP_ERR_NOERROR;
}

int CmcUsChanTableSetCommit(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    int ret;
    CMCOAM_UsChannelCfgT *entry;
    netsnmp_request_info *request;
    netsnmp_table_request_info *tableInfo;
       
    dbgstrm << "Start." << endl;
    
    for (request=requests ; request;  request=request->next)
    {
        entry = (CMCOAM_UsChannelCfgT*)netsnmp_extract_iterator_context(request);

        if ( NULL == entry )
        {
            dbgstrm << "entry is NULL" << endl;
            netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHINSTANCE);
            break;
        }
        
        tableInfo = netsnmp_extract_table_info(request);
        switch (tableInfo->colnum)
        {            
            case ColCmcUpChanEnable:
                ret = MgtChannelSetUpstreamSettingsEnabled(entry->us_ch_id, *request->requestvb->val.integer);
                if (ret != BASE_OK)
                {
                    netsnmp_set_request_error(reqinfo, request, SNMP_ERR_COMMITFAILED);
                }       
                break;

            case ColCmcUpChanInputPower:
                ret = MgtChannelSetUpstreamSettingsPowerLevel(entry->us_ch_id, 
                                                    *request->requestvb->val.integer);
                if (ret != BASE_OK)
                {
                    netsnmp_set_request_error(reqinfo, request, SNMP_ERR_COMMITFAILED);
                }       
                break;
        }
    }
    return SNMP_ERR_NOERROR;
}



/** handles requests for the gsAnCmcUpstreamChannelTable table */
int
CmcUsChannelTableHandler(
                netsnmp_mib_handler               *handler,
                netsnmp_handler_registration      *reginfo,
                netsnmp_agent_request_info        *reqinfo,
                netsnmp_request_info              *requests) 
{
    dbgstrm << "Start." << endl;
    DEBUGMSGTL(("CmcUpstreamChannelTable:handler", "Processing request (%d)\n", reqinfo->mode));
    
    static struct CmcUsChanTableHandlerDispath dispatch[7] =
    {
        /* Read-support (also covers GetNext requests) */
        {MODE_GET, CmcUsChanTableGet},
        /* Write-support */
        {MODE_SET_RESERVE1, CmcUsChanTableSetReserve1},
        {MODE_SET_RESERVE2, CmcUsChanTableSetReserve2},
        {MODE_SET_FREE, CmcUsChanTableSetFree},
        {MODE_SET_ACTION, CmcUsChanTableSetAction},
        {MODE_SET_UNDO, CmcUsChanTableSetUndo},
        {MODE_SET_COMMIT, CmcUsChanTableSetCommit},
    };
    size_t i;
    for (i = 0; i < sizeof(dispatch) /sizeof(struct CmcUsChanTableHandlerDispath); ++i)
    {
        if (reqinfo->mode == dispatch[i].mode)
        {
            int ret;
            ret = (*dispatch[i].handler)(handler, reginfo, reqinfo, requests);
            if (ret != SNMP_ERR_NOERROR)
            {
                printf("CmcUsChannelTableHandler> %d operation failed, ret = %d\n",
                       dispatch[i].mode, ret);
                return ret;
            }
            break;
        }
    }
    return SNMP_ERR_NOERROR;
}


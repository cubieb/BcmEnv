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
#include "VendorToVlanMgt.h"

#include "gsAnCmcVendorToVlanTable.h"
using namespace std;

#define VlanNum 2048
VendorToVlanEntryT VendorToVlan[VlanNum]; 

/** Initializes the gsAnCmcVendorToVlanTable module */
void InitGsAnCmcVendorToVlanTable(void)
{
    const oid Oid[] = {1,3,6,1,4,1,22764,1015,9026,2,2,8,2};
    const size_t OidLen = OID_LENGTH(Oid);

    netsnmp_iterator_info *iinfo;
    netsnmp_handler_registration *reg;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("gsAnCmcVendorToVlanTable:init", "gsAnCmcVendorToVlanTable\n"));

    reg = netsnmp_create_handler_registration(
              "gsAnCmcVendorToVlanTable", 
              CmcVendorToVlanTableHandler,
              Oid, OidLen,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_UNSIGNED,  /* index: VendorItemIndex */
                           0);
    table_info->min_column = ColVendorToVlanCmMac;
    table_info->max_column = ColVendorToVlanVendorID;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = CmcVendorToVlanTableGetFirst;
    iinfo->get_next_data_point = CmcVendorToVlanTableGetNext;
    iinfo->table_reginfo = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}

/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
CmcVendorToVlanTableGetFirst(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    dbgstrm << "Start." << endl;

    VendorToVlanEntryT VendorToVlanEntry;
    VendorToVlanEntryT *VendorToVlanP = VendorToVlan;

    int j = 0;
    BASE_ErrCode ret;
    for (ret = GetFirstVendorToVlan(&VendorToVlanEntry);
            ret == BASE_OK && j < VlanNum;
            ret = GetNextVendorToVlan(VendorToVlanEntry.index, &VendorToVlanEntry))
    {
        memcpy(&VendorToVlanP, &VendorToVlanEntry, sizeof(VendorToVlanEntryT));
        ++VendorToVlanP;
        ++j;
    }

    //for test
    VendorToVlanP = VendorToVlan;
    VendorToVlanP->index = 1;
    char MacTemp[6] = {0,15,2,14,3,13};
    memcpy(VendorToVlanP->cmMac, MacTemp, 6);
    VendorToVlanP->index = 1;    
    VendorToVlanP->cmState = Online;    
    VendorToVlanP->vid = 1;    
    char *VerdorIdStr = "2049";
    memcpy(VendorToVlanP->vendorId, VerdorIdStr, strlen(VerdorIdStr));
    VendorToVlanP->vendorId[strlen(VerdorIdStr)] = '\0';
    
    *my_loop_context = VendorToVlan;
    return CmcVendorToVlanTableGetNext(my_loop_context, my_data_context, 
                                put_index_data,  mydata);    
}

netsnmp_variable_list *
CmcVendorToVlanTableGetNext(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    dbgstrm << "Start." << endl;
    
    VendorToVlanEntryT *VendorToVlanP = (VendorToVlanEntryT *)*my_loop_context;
    if (NULL == VendorToVlanP)
    {
        dbgstrm << "No more VendorToVlan." << endl;
        *my_loop_context = NULL;
        *my_data_context = NULL;
        return NULL;
    }
    
    if (VendorToVlanP < VendorToVlan + VlanNum)
    {
        if ( 0 == VendorToVlanP->index)
        {
            *my_loop_context = NULL;
            *my_data_context = NULL;
            return NULL;            
        }
        snmp_set_var_value(put_index_data, &VendorToVlanP->index, sizeof(int32_t));
        *my_data_context = VendorToVlanP++;
        *my_loop_context = VendorToVlanP;
        return put_index_data;
    }

    *my_loop_context = NULL;
    *my_data_context = NULL;
    return NULL;
}


int CmcVendorToVlanTableGet(netsnmp_mib_handler               *handler,
                                                    netsnmp_handler_registration      *reginfo,
                                                    netsnmp_agent_request_info        *reqinfo,
                                                    netsnmp_request_info              *requests)
{   
    netsnmp_request_info *request;
    netsnmp_table_request_info *tableInfo;
    
    dbgstrm << "Start." << endl; 

    VendorToVlanEntryT *entry;

    /*
     * Read-support (also covers GetNext requests)
     */
    for (request = requests; request; request = request->next) 
    {
        entry = (VendorToVlanEntryT*)netsnmp_extract_iterator_context(request);      
        if (NULL == entry)
        {
            dbgstrm << "entry is NULL" << endl;
            netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHINSTANCE);
            continue;
        }

        dbgstrm << "index=" << entry->index << endl;
        tableInfo = netsnmp_extract_table_info(request);
        switch (tableInfo->colnum) 
        {       
            case ColVendorToVlanCmMac:
                    snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                      entry->cmMac, 6 ); 
            break;

            case ColVendorToVlanCmState:
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, entry->cmState);
            break;

            case ColVendorToVlanVlanID:
                snmp_set_var_typed_integer(request->requestvb, ASN_INTEGER, entry->vid);
            break;            
            
            case ColVendorToVlanVendorID:
            {
                dbgstrm << "strlen = " << strlen(entry->vendorId) << endl;
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                  entry->vendorId, strlen(entry->vendorId) ); 
            } 
            break;           
            
            default:
                netsnmp_set_request_error(reqinfo, request, SNMP_NOSUCHOBJECT);
            break;
        }   
    }
    return SNMP_ERR_NOERROR;
}

int CmcVendorToVlanTableSetReserve1(netsnmp_mib_handler      *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    return SNMP_ERR_NOERROR;    
}

int CmcVendorToVlanTableSetReserve2(netsnmp_mib_handler      *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    return SNMP_ERR_NOERROR;
}


int CmcVendorToVlanTableSetFree(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    return SNMP_ERR_NOERROR;
}

int CmcVendorToVlanTableSetUndo(netsnmp_mib_handler               *handler,
                            netsnmp_handler_registration      *reginfo,
                            netsnmp_agent_request_info        *reqinfo,
                            netsnmp_request_info              *requests)
{
    return SNMP_ERR_NOERROR;
}
int CmcVendorToVlanTableSetAction(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    return SNMP_ERR_NOERROR;
}
int CmcVendorToVlanTableSetCommit(netsnmp_mib_handler               *handler,
                             netsnmp_handler_registration      *reginfo,
                             netsnmp_agent_request_info        *reqinfo,
                             netsnmp_request_info              *requests)
{
    return SNMP_ERR_NOERROR;
}

typedef int(*CmcVendorToVlanTableHandlerP)(netsnmp_mib_handler           *handler,
                                      netsnmp_handler_registration  *reginfo,
                                      netsnmp_agent_request_info    *reqinfo,
                                      netsnmp_request_info          *requests);

struct CmcVendorToVlanTableHandlerDispath
{
    int mode;
    CmcVendorToVlanTableHandlerP handler;
};


/** handles requests for the gsAnCmcCdtTransSVidTable table */
int
CmcVendorToVlanTableHandler(
                        netsnmp_mib_handler               *handler,
                        netsnmp_handler_registration      *reginfo,
                        netsnmp_agent_request_info        *reqinfo,
                        netsnmp_request_info              *requests) 
{
    dbgstrm << "Start." << endl;
    DEBUGMSGTL(("CmcVendorToVlanTableHandler:", "Processing request (%d)\n", reqinfo->mode));  
    static struct CmcVendorToVlanTableHandlerDispath dispatch[7] =
    {
        /* Read-support (also covers GetNext requests) */
        {MODE_GET, CmcVendorToVlanTableGet},
        /* Write-support */
        {MODE_SET_RESERVE1, CmcVendorToVlanTableSetReserve1},
        {MODE_SET_RESERVE2, CmcVendorToVlanTableSetReserve2},
        {MODE_SET_FREE, CmcVendorToVlanTableSetFree},
        {MODE_SET_ACTION, CmcVendorToVlanTableSetAction},
        {MODE_SET_UNDO, CmcVendorToVlanTableSetUndo},
        {MODE_SET_COMMIT, CmcVendorToVlanTableSetCommit},
    };
    size_t i;
    for (i = 0; i < sizeof(dispatch) /sizeof(struct CmcVendorToVlanTableHandlerDispath); ++i)
    {
        if (reqinfo->mode == dispatch[i].mode)
        {
            int ret;
            ret = (*dispatch[i].handler)(handler, reginfo, reqinfo, requests);
            if (ret != SNMP_ERR_NOERROR)
            {
                printf("CmcVendorToVlanTableHandler> %d operation failed, ret = %d\n",
                       dispatch[i].mode, ret);
                return ret;
            }
            break;
        }
    }
    return SNMP_ERR_NOERROR;
}



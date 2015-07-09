/*
 * Note: this file originally auto-generated by mib2c using
 *        $
 */
 
#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "Debug.h"
#include "VendorToVlanMgt.h"

#include "gsAnCmcVendorToVlanObjects.h"
using namespace std;

/** Initializes the gsAnCmcVendorToVlanObjects module */
void InitGsAnCmcVendorToVlanObjects(void)
{
    const oid Oid[] = { 1,3,6,1,4,1,22764,1015,9026,2,2,8,1 };

  DEBUGMSGTL(("gsAnCmcVendorToVlanObjects", "Initializing\n"));

    netsnmp_register_scalar(
        netsnmp_create_handler_registration(
                               "gsAnCmcVendorToVlanStatus", 
                               HandleCmcVendorToVlanStatus,
                               Oid, OID_LENGTH(Oid),
                               HANDLER_CAN_RWRITE
        ));
}

int
HandleCmcVendorToVlanStatus(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    int ret;
    int32_t state;
    uint32_t value;
    
    switch(reqinfo->mode) {

        case MODE_GET:
            state = GetVenderToVlanState();
            snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,
                                     &state, sizeof(int32_t));
            break;

        case MODE_SET_RESERVE1:
            ret = netsnmp_check_vb_type(requests->requestvb, ASN_UNSIGNED);
            if ( ret != SNMP_ERR_NOERROR ) {
                netsnmp_set_request_error(reqinfo, requests, ret );
            }
            break;

        case MODE_SET_RESERVE2:
            if (0) {
                netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            }
            break;

        case MODE_SET_FREE:
            break;

        case MODE_SET_ACTION:
            value = *requests->requestvb->val.integer;
            ret = SetVenderToVlanState(value);
            if (BASE_OK != ret) {
                netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_GENERR);
            }
            break;

        case MODE_SET_COMMIT:
            if (0) {                
                netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            }
            break;

        case MODE_SET_UNDO:
            if (0) {                
                netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
            }
            break;

        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_gsAnCmcVendorToVlanStatus\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

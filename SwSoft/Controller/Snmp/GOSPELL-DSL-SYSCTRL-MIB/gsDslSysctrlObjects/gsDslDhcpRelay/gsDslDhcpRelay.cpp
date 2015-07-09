/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 17337 2009-01-01 14:28:29Z magfr $
 */
#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "DhcpRelayMgt.h"
#include "gsDslDhcpRelay.h"

using namespace std;

CxxBeginNameSpace(Snmp)
CxxBeginNameSpace(GsDslDhcpRelay)

/** Initializes the gsDslDhcpRelay module */
void InitGsDslDhcpRelay(void)
{
    const oid gsDslDhcpServerAddress[] = { 1,3,6,1,4,1,22764,7900,3,1,100,1 };

    netsnmp_handler_registration* handler;
    handler = netsnmp_create_handler_registration("gsDslDhcpServerAddress",
        HandleDhcpServerAddress, gsDslDhcpServerAddress,
        OID_LENGTH(gsDslDhcpServerAddress), HANDLER_CAN_RWRITE);

    netsnmp_register_scalar(handler);
}

int HandleDhcpServerAddress(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    int ret;
    
    struct in_addr dhcpIp;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        dhcpIp = GetDhcpServerIp();
        snmp_set_var_typed_value(requests->requestvb, ASN_IPADDRESS,
            (u_char *)&dhcpIp.s_addr,
            sizeof(uint32_t));
        break;

        /*
         * SET REQUEST
         *
         * multiple states in the transaction.  See:
         * http://www.net-snmp.org/tutorial-5/toolkit/mib_module/set-actions.jpg
         */
    case MODE_SET_RESERVE1:
        /* or you could use netsnmp_check_vb_type_and_size instead */
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_IPADDRESS);
        if (ret != SNMP_ERR_NOERROR)
        {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        /* malloc "undo" storage buffer */
        break;

    case MODE_SET_FREE:
        /* free resources allocated in RESERVE1 and/or
               RESERVE2.  Something failed somewhere, and the states
               below won't be called. */
        break;

    case MODE_SET_ACTION:
        /* perform the value change here */
        dhcpIp.s_addr = *requests->requestvb->val.integer;
        SetDhcpServerIp(dhcpIp);
        break;

    case MODE_SET_COMMIT:
        /* delete temporary storage */
        break;

    case MODE_SET_UNDO:
        /* UNDO and return to previous value for the object */
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

CxxEndNameSpace /* GsDslDhcpRelay */
CxxEndNameSpace /* Snmp */

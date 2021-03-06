/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 17337 2009-01-01 14:28:29Z magfr $
 */

#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "Debug.h"
#include "SystemInfoMgt.h"
#include "interfaces.h"

using namespace std;
CxxBeginNameSpace(Snmp)
CxxBeginNameSpace(Interfaces)

void InitInterfaces(void)
{
    const oid ifNumberOid[] = {1, 3, 6, 1, 2, 1, 2, 1};
    netsnmp_handler_registration* handler;

    handler = netsnmp_create_handler_registration("ifNumber", HandleIfNumber,
        ifNumberOid, OID_LENGTH(ifNumberOid), HANDLER_CAN_RONLY);
    netsnmp_register_scalar(handler);
}

int HandleIfNumber(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info   *reqinfo,
    netsnmp_request_info         *requests)
{
    int32_t ifNumber = 1;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            (u_char *) &ifNumber, sizeof(int32_t));
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in %s\n", reqinfo->mode, __func__);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

CxxEndNameSpace /* Interfaces */
CxxEndNameSpace /* Snmp */

/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 17337 2009-01-01 14:28:29Z magfr $
 */
#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "CliMain.h"
#include "gsDslCommon.h"

using namespace std;

/** Initializes the gsDslCommon module */
void InitGsDslCommon(void)
{
//    const oid gsDslCliPromptOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,1 };
//    const oid gsDslLineTestRootModeOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,2 };
//    const oid gsDslEPMConfTypeOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,3 };
    const oid gsDslSaveToNvmOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,4 };
    const oid gsDslEraseNvmOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,5 };
//    const oid gsDslSwapCoreOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,6 };
//    const oid gsDslBorrowBoardSetOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,7 };
//    const oid gsDslBorrowBoardGetOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,8 };
//    const oid gsDslSysDateTimeOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,9 };
//    const oid gsDslTimeZoneOffsetOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,10 };
//    const oid gsDslIsHalfTimeZoneOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,11 };
    const oid gsDslSystemRebootOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,12 };
//    const oid gsDslSystemPortListOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,13 };
//    const oid gsDslAutoSaveToNvmIntervalOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,14 };
//    const oid gsDslTimingSaveModifiedToNvmIntervalOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,15 };
//    const oid gsDslTimingSaveToNvmIntervalOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,16 };
//    const oid gsDslUserSideMtuOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,17 };
//    const oid gsDslSysMaxPortNumPerSlotOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,18 };
//    const oid gsDslSysOutbandPortUsageOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,19 };
//    const oid gsDslSysDebugUserEnableOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,20 };
//    const oid gsDslSerialPortBaudRateOid[] = { 1,3,6,1,4,1,22764,7900,3,1,4,21 };

    netsnmp_handler_registration* handler;

//    handler = netsnmp_create_handler_registration("gsDslCliPrompt",
//        HandleGsDslCliPrompt, gsDslCliPromptOid,
//        OID_LENGTH(gsDslCliPromptOid), HANDLER_CAN_RWRITE);
//
//    handler = netsnmp_create_handler_registration("gsDslLineTestRootMode",
//        HandleGsDslLineTestRootMode, gsDslLineTestRootModeOid,
//        OID_LENGTH(gsDslLineTestRootModeOid), HANDLER_CAN_RWRITE);
//
//    handler = netsnmp_create_handler_registration("gsDslEPMConfType",
//        HandleGsDslEPMConfType, gsDslEPMConfTypeOid,
//        OID_LENGTH(gsDslEPMConfTypeOid), HANDLER_CAN_RWRITE);

    handler = netsnmp_create_handler_registration("gsDslSaveToNvm",
        HandleGsDslSaveToNvm, gsDslSaveToNvmOid,
        OID_LENGTH(gsDslSaveToNvmOid), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslEraseNvm",
        HandleGsDslEraseNvm, gsDslEraseNvmOid,
        OID_LENGTH(gsDslEraseNvmOid), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

//    handler = netsnmp_create_handler_registration("gsDslSwapCore",
//        HandleGsDslSwapCore, gsDslSwapCoreOid,
//        OID_LENGTH(gsDslSwapCoreOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslBorrowBoardSet",
//        HandleGsDslBorrowBoardSet, gsDslBorrowBoardSetOid,
//        OID_LENGTH(gsDslBorrowBoardSetOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslBorrowBoardGet",
//        HandleGsDslBorrowBoardGet, gsDslBorrowBoardGetOid,
//        OID_LENGTH(gsDslBorrowBoardGetOid), HANDLER_CAN_RONLY);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslSysDateTime",
//        HandleGsDslSysDateTime, gsDslSysDateTimeOid,
//        OID_LENGTH(gsDslSysDateTimeOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslTimeZoneOffset",
//        HandleGsDslTimeZoneOffset, gsDslTimeZoneOffsetOid,
//        OID_LENGTH(gsDslTimeZoneOffsetOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslIsHalfTimeZone",
//        HandleGsDslIsHalfTimeZone, gsDslIsHalfTimeZoneOid,
//        OID_LENGTH(gsDslIsHalfTimeZoneOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);

    handler = netsnmp_create_handler_registration("gsDslSystemReboot",
        HandleGsDslSystemReboot, gsDslSystemRebootOid,
        OID_LENGTH(gsDslSystemRebootOid), HANDLER_CAN_RWRITE);
    netsnmp_register_scalar(handler);

//    handler = netsnmp_create_handler_registration("gsDslSystemPortList",
//        HandleGsDslSystemPortList, gsDslSystemPortListOid,
//        OID_LENGTH(gsDslSystemPortListOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslAutoSaveToNvmInterval",
//        HandleGsDslAutoSaveToNvmInterval, gsDslAutoSaveToNvmIntervalOid,
//        OID_LENGTH(gsDslAutoSaveToNvmIntervalOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslTimingSaveModifiedToNvmInterval",
//        HandleGsDslTimingSaveModifiedToNvmInterval, gsDslTimingSaveModifiedToNvmIntervalOid,
//        OID_LENGTH(gsDslTimingSaveModifiedToNvmIntervalOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslTimingSaveToNvmInterval",
//        HandleGsDslTimingSaveToNvmInterval, gsDslTimingSaveToNvmIntervalOid,
//        OID_LENGTH(gsDslTimingSaveToNvmIntervalOid),HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslUserSideMtu",
//        HandleGsDslUserSideMtu, gsDslUserSideMtuOid,
//        OID_LENGTH(gsDslUserSideMtuOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslSysMaxPortNumPerSlot",
//        HandleGsDslSysMaxPortNumPerSlot, gsDslSysMaxPortNumPerSlotOid,
//        OID_LENGTH(gsDslSysMaxPortNumPerSlotOid), HANDLER_CAN_RONLY);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslSysOutbandPortUsage",
//        HandleGsDslSysOutbandPortUsage, gsDslSysOutbandPortUsageOid,
//        OID_LENGTH(gsDslSysOutbandPortUsageOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslSysDebugUserEnable",
//        HandleGsDslSysDebugUserEnable, gsDslSysDebugUserEnableOid,
//        OID_LENGTH(gsDslSysDebugUserEnableOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
//
//    handler = netsnmp_create_handler_registration("gsDslSerialPortBaudRate",
//        HandleGsDslSerialPortBaudRate, gsDslSerialPortBaudRateOid,
//        OID_LENGTH(gsDslSerialPortBaudRateOid), HANDLER_CAN_RWRITE);
//    netsnmp_register_scalar(handler);
}

int HandleGsDslCliPrompt(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
      return SNMP_ERR_NOERROR;
}

int HandleGsDslLineTestRootMode(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslEPMConfType(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslSaveToNvm(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int32_t value;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        value = 0;
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            &value, sizeof(int32_t));
        break;

    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        /* or you could use netsnmp_check_vb_type_and_size instead */
        if (*requests->requestvb->val.integer != 0)
        {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
        }
        break;

    case MODE_SET_FREE:
        /* free resources allocated in RESERVE1 and/or RESERVE2.
         * Something failed somewhere, and the states below won't be called.
         */
        break;

    case MODE_SET_ACTION:
        /* perform the value change here */
        if (SaveToCliConfig() != BASE_OK)
        {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_GENERR);
        }

        break;

    case MODE_SET_COMMIT:
        /* delete temporary storage */
        break;

    case MODE_SET_UNDO:
        /* UNDO and return to previous value for the object */
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_gsDslSystemReboot\n",
            reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleGsDslEraseNvm(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int32_t value;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        value = 0;
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            &value, sizeof(int32_t));
        break;

    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        /* or you could use netsnmp_check_vb_type_and_size instead */
        if (*requests->requestvb->val.integer != 0)
        {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_WRONGVALUE);
        }
        break;

    case MODE_SET_FREE:
        /* free resources allocated in RESERVE1 and/or RESERVE2.
         * Something failed somewhere, and the states below won't be called.
         */
        break;

    case MODE_SET_ACTION:
        /* perform the value change here */
        RestoreDefaultConfig();
        break;

    case MODE_SET_COMMIT:
        /* delete temporary storage */
        break;

    case MODE_SET_UNDO:
        /* UNDO and return to previous value for the object */
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_gsDslSystemReboot\n",
            reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int HandleGsDslSwapCore(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslBorrowBoardSet(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslBorrowBoardGet(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslSysDateTime(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslTimeZoneOffset(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslIsHalfTimeZone(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslSystemReboot(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    int ret;
    int32_t value;
    switch(reqinfo->mode)
    {
    case MODE_GET:
        value = 0;
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
            &value, sizeof(int32_t));
        break;

    case MODE_SET_RESERVE1:
    case MODE_SET_RESERVE2:
        /* or you could use netsnmp_check_vb_type_and_size instead */
        ret = netsnmp_check_vb_int_range(requests->requestvb, 1, 2);
        if ( ret != SNMP_ERR_NOERROR )
        {
            netsnmp_set_request_error(reqinfo, requests, ret );
        }
        break;

    case MODE_SET_FREE:
        /* free resources allocated in RESERVE1 and/or RESERVE2.
         * Something failed somewhere, and the states below won't be called.
         */
        break;

    case MODE_SET_ACTION:
        /* perform the value change here */
        StopCli();
        break;

    case MODE_SET_COMMIT:
        /* delete temporary storage */
        break;

    case MODE_SET_UNDO:
        /* UNDO and return to previous value for the object */
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_gsDslSystemReboot\n",
            reqinfo->mode);
        return SNMP_ERR_GENERR;
    }
    return SNMP_ERR_NOERROR;
}

int HandleGsDslSystemPortList(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslAutoSaveToNvmInterval(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslTimingSaveModifiedToNvmInterval(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslTimingSaveToNvmInterval(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslUserSideMtu(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslSysMaxPortNumPerSlot(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslSysOutbandPortUsage(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}

int HandleGsDslSysDebugUserEnable(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}
int HandleGsDslSerialPortBaudRate(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_agent_request_info *reqinfo,
    netsnmp_request_info *requests)
{
    return SNMP_ERR_NOERROR;
}
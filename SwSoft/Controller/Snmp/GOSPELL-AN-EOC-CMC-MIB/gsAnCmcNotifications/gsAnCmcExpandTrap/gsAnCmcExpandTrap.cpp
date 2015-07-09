#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/mib_modules.h>

#include "Debug.h"
#include "CnuStatusMgt.h"
#include "CmcGlobalConfigMgt.h"
#include "SystemInfoMgt.h"

#include "gsAnCmcExpandTrap.h"

using namespace std;

CxxBeginNameSpace(Snmp)
CxxBeginNameSpace(GsAnCmcExpandTrap)

int HandleGsAnExtCmcUpstreamLowSnrAlm(int channel, int snr)
{
    /*
     * define the OID for the notification we're going to send
     * gsAnCmcExpandTrap::gsAnExtCmcCmExceedAlm
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 1015, 9026, 10, 1, 5};
    size_t notificationOidLen = OID_LENGTH(notificationOid);

    /*
     * In the notification, we have to assign our notification OID to
     * the snmpTrapOID.0 object. Here is it's definition.
     */
    oid    snmpTrapOid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};
    size_t snmpTrapOidLen = OID_LENGTH(snmpTrapOid);

    /*
     * here is where we store the variables to be sent in the trap
     */
    netsnmp_variable_list *vars = NULL;

    /*
     * add in the trap definition object
     */
    snmp_varlist_add_variable(&vars,
        snmpTrapOid, snmpTrapOidLen,
        ASN_OBJECT_ID,
        notificationOid,
        notificationOidLen * sizeof(oid));

    /*
     * send the trap out.  This will send it to all registered
     * receivers (see the "SETTING UP TRAP AND/OR INFORM DESTINATIONS"
     * section of the snmpd.conf manual page.
     */
    send_v2trap(vars);

    /*
     * free the created notification variable list
     */
    snmp_free_varbind(vars);

    return 0;
}

int HandleGsAnExtCmcUpstreamLowSnrClr(int channel, int snr)
{
    /*
     * define the OID for the notification we're going to send
     * gsAnCmcExpandTrap::gsAnExtCmcCmExceedAlm
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 1015, 9026, 10, 1, 6};
    size_t notificationOidLen = OID_LENGTH(notificationOid);

    /*
     * In the notification, we have to assign our notification OID to
     * the snmpTrapOID.0 object. Here is it's definition.
     */
    oid    snmpTrapOid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};
    size_t snmpTrapOidLen = OID_LENGTH(snmpTrapOid);

    /*
     * here is where we store the variables to be sent in the trap
     */
    netsnmp_variable_list *vars = NULL;

    /*
     * add in the trap definition object
     */
    snmp_varlist_add_variable(&vars,
        snmpTrapOid, snmpTrapOidLen,
        ASN_OBJECT_ID,
        notificationOid,
        notificationOidLen * sizeof(oid));

    /*
     * send the trap out.  This will send it to all registered
     * receivers (see the "SETTING UP TRAP AND/OR INFORM DESTINATIONS"
     * section of the snmpd.conf manual page.
     */
    send_v2trap(vars);

    /*
     * free the created notification variable list
     */
    snmp_free_varbind(vars);

    return 0;
}

int HandleGsAnExtCmcCmExceedAlarm(int cmcCmNumber)
{
    /*
     * define the OID for the notification we're going to send
     * gsAnCmcExpandTrap::gsAnExtCmcCmExceedAlm
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 1015, 9026, 10, 1, 7};
    size_t notificationOidLen = OID_LENGTH(notificationOid);

    /*
     * In the notification, we have to assign our notification OID to
     * the snmpTrapOID.0 object. Here is it's definition.
     */
    oid    snmpTrapOid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};
    size_t snmpTrapOidLen = OID_LENGTH(snmpTrapOid);

    /*
     * here is where we store the variables to be sent in the trap
     */
    netsnmp_variable_list *vars = NULL;

    /*
     * add in the trap definition object
     */
    snmp_varlist_add_variable(&vars,
        snmpTrapOid, snmpTrapOidLen,
        ASN_OBJECT_ID,
        notificationOid,
        notificationOidLen * sizeof(oid));

    /*
     * send the trap out.  This will send it to all registered
     * receivers (see the "SETTING UP TRAP AND/OR INFORM DESTINATIONS"
     * section of the snmpd.conf manual page.
     */
    send_v2trap(vars);

    /*
     * free the created notification variable list
     */
    snmp_free_varbind(vars);

    return 0;
}

int HandleGsAnExtCmcCmExceedClr(int cmcCmNumber)
{
    /*
     * define the OID for the notification we're going to send
     * gsAnCmcExpandTrap::gsAnExtCmcCmExceedAlm
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 1015, 9026, 10, 1, 8};
    size_t notificationOidLen = OID_LENGTH(notificationOid);

    /*
     * In the notification, we have to assign our notification OID to
     * the snmpTrapOID.0 object. Here is it's definition.
     */
    oid    snmpTrapOid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};
    size_t snmpTrapOidLen = OID_LENGTH(snmpTrapOid);

    /*
     * here is where we store the variables to be sent in the trap
     */
    netsnmp_variable_list *vars = NULL;

    /*
     * add in the trap definition object
     */
    snmp_varlist_add_variable(&vars,
        snmpTrapOid, snmpTrapOidLen,
        ASN_OBJECT_ID,
        notificationOid,
        notificationOidLen * sizeof(oid));

    /*
     * send the trap out.  This will send it to all registered
     * receivers (see the "SETTING UP TRAP AND/OR INFORM DESTINATIONS"
     * section of the snmpd.conf manual page.
     */
    send_v2trap(vars);

    /*
     * free the created notification variable list
     */
    snmp_free_varbind(vars);

    return 0;
}

int HandleGsAnExtCmcUpstreamLowSnrEvent(UsSignalQualityAlarmT* snr)
{
    uint_t snrThreshold;
    int i;
    for (i = 0; i < 4; ++i)
    {
        GetUsSigMgt(i + 1, &snrThreshold);
        if (snr->us_sig[i].snr > snrThreshold)
        {
            HandleGsAnExtCmcUpstreamLowSnrAlm(i, snr->us_sig[i].snr);
        }
        else
        {
            HandleGsAnExtCmcUpstreamLowSnrClr(i, snr->us_sig[i].snr);
        }
    }
    return 0;
}

int HandleGsAnExtCmcCmEvent(JudgeMaxCmOnlineNumberInfoT* cmcCmInfo)
{
    int threshold;
    GetMaxCnuOnlieMgt(&threshold);
    if (cmcCmInfo->cm_online_number >= threshold)
    {
        HandleGsAnExtCmcCmExceedAlarm(cmcCmInfo->cm_online_number);
    }
    else
    {
        HandleGsAnExtCmcCmExceedClr(cmcCmInfo->cm_online_number);
    }
    return 0;
}

void InitGsAnCmcExpandTrap(void)
{
    RegisterUsSigCallbackMgt(HandleGsAnExtCmcUpstreamLowSnrEvent);
    RegisterJudgeMaxCmOnlineNumberCallbackMgt(HandleGsAnExtCmcCmEvent);
}

CxxEndNameSpace /* GsAnCmcExpandTrap */
CxxEndNameSpace /* Snmp */

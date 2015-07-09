#include "SystemInclude.h"
#include "BcmController.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/mib_modules.h>

#include "Debug.h"
#include "SystemInfoMgt.h"
#include "gsDslSysctrlTrapObjects.h"

using namespace std;

CxxBeginNameSpace(Snmp)
CxxBeginNameSpace(GsDslSysctrlTrapObjects)

int HandleGsDslCpuLoadAlarm(CpuUsingInfoT*)
{
    /*
     * define the OID for the notification we're going to send
     * gsDslSysctrlTrapObjects::gsDslCpuLoadAlarm
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 7900, 3, 2, 4};
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

int HandleGsDslCpuLoadAlarmCleard(CpuUsingInfoT*)
{
    /*
     * define the OID for the notification we're going to send
     * gsDslSysctrlTrapObjects::gsDslCpuLoadAlarm
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 7900, 3, 2, 5};
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

int HandleGsDslMemoryOverLoad(RamUsingInfoT*)
{
    /*
     * define the OID for the notification we're going to send
     * gsDslSysctrlTrapObjects::gsDslMemoryOverLoad
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 7900, 3, 2, 6};
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

int HandleGsDslMemoryAlarmCleard(RamUsingInfoT*)
{
    /*
     * define the OID for the notification we're going to send
     * gsDslSysctrlTrapObjects::gsDslMemoryOverLoad
     */
    oid    notificationOid[] = {1, 3, 6, 1, 4, 1, 22764, 7900, 3, 2, 7};
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

int HandleGsDslCpuLoadEvent(CpuUsingInfoT* cpuLoadInfo)
{
    float threshold;
    GetMaxCpuUsingMgt(&threshold);
    if (cpuLoadInfo->cpu_using >= threshold)
    {
        HandleGsDslCpuLoadAlarm(cpuLoadInfo);
    }
    else
    {
        HandleGsDslCpuLoadAlarmCleard(cpuLoadInfo);
    }
    return 0;
}

int HandleGsDslMemoryUseageEvent(RamUsingInfoT* memoryUsageInfo)
{
    unsigned long threshold;
    GetMaxRamUsingMgt(&threshold);
    if (memoryUsageInfo->ram_using >= threshold)
    {
        HandleGsDslMemoryOverLoad(memoryUsageInfo);
    }
    else
    {
        HandleGsDslMemoryAlarmCleard(memoryUsageInfo);
    }
    return 0;
}


void InitGsDslSysctrlTrapObjects(void)
{
    RegisterCpuUsingCallbackMgt(HandleGsDslCpuLoadEvent);
    RegisterRamUsingCallbackMgt(HandleGsDslMemoryUseageEvent);
}

CxxEndNameSpace /* GsDslSysctrlTrapObjects */
CxxEndNameSpace /* Snmp */

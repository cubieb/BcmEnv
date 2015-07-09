//****************************************************************************
//
//  Copyright (c) 2011,2012  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of: 
//          Broadcom Corporation
//          16251 Laguna Canyon Road
//          Irvine, California 92618
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
//****************************************************************************
//

/* --- system includes ---*/
#include "SystemInclude.h"
#include <signal.h>
#include <pthread.h>
#include <syslog.h>

/* --- net-snmp includes ---*/
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/mib_modules.h>

#include "system/system.h"
#include "interfaces/interfaces.h"
#include "interfaces/ifTable/ifTable.h"
#include "gsDslSysctrlObjects/gsDslAddress/gsDslAddress.h"
#include "gsDslSysctrlObjects/gsDslCommon/gsDslCommon.h"
#include "gsDslSysctrlObjects/gsDslFtp/gsDslFtp.h"
#include "gsDslSysctrlObjects/gsDslDhcpRelay/gsDslDhcpRelay.h"
#include "gsDslSysctrlObjects/gsDslSyslog/gsDslSyslogHostTable/gsDslSyslogHostTable.h"
#include "gsDslSysctrlObjects/gsDslTrapHostTable//gsDslTrapHostTable.h"
#include "gsDslSysctrlObjects/gsDslBoardTable/gsDslBoardTable.h"
#include "gsDslSysctrlTrapObjects/gsDslSysctrlTrapObjects.h"
#include "docsIfDownstreamChannelTable/docsIfDownstreamChannelTable.h"
#include "docsIfUpstreamChannelTable/docsIfUpstreamChannelTable.h"
#include "gsAnCmcNotifications/gsAnCmcExpandTrap/gsAnCmcExpandTrap.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcCdtTransSVidTable/gsAnCmcCdtTransSVidTable.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcUpstreamChannelTable/gsAnCmcUpstreamChannelTable.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcDownstreamChannelTable/gsAnCmcDownstreamChannelTable.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcChannelStatisticsTable/gsAnCmcChannelStatisticsTable.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcPortStatisticsTable/gsAnCmcPortStatisticsTable.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcVendorToVlanObjects/gsAnCmcVendorToVlanObjects.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcVendorToVlanObjects/gsAnCmcVendorToVlanTable/gsAnCmcVendorToVlanTable.h"
#include "gsAnCmcObjects/gsAnCmcCmcObjects/gsAnCmcSystem/gsAnCmcSystem.h"
#include "gsAnCmcObjects/gsAnCmcCmObjects/gsAnCmcCmCpeTable/gsAnCmcCmCpeTable.h"
#include "gsAnCmcObjects/gsAnCmcCmObjects/gsAnCmcCmStatistics/gsAnCmcCmStatistics.h"
#include "gsAnCmcObjects/gsAnCmcCmObjects/gsAnCmcCmUniTable/gsAnCmcCmUniTable.h"
#include "gsAnCmcObjects/gsAnCmcCmObjects/gsAnCmcCmTrafficTable/gsAnCmcCmTrafficTable.h"
#include "gsAnCmcObjects/gsAnCmcGlobalObject/gsAnCmcGlobalObject.h"
#include "gsAnCmcObjects/gsCableDocsLoadBalObjects/gsCableDocsLoadBalGrpCfgTable/gsCableDocsLoadBalGrpCfgTable.h"
#include "gsAnCmcObjects/gsCableDocsLoadBalObjects/gsCableDocsLoadBalMacRangeCfgTable/gsCableDocsLoadBalMacRangeCfgTable.h"
#include "gsAnCmcObjects/gsCableDocsLoadBalObjects/gsCableDocsLoadBalExcludeMacCfgTable/gsCableDocsLoadBalExcludeMacCfgTable.h"
#include "gsAnCmcObjects/gsCableDocsLoadBalObjects/gsCableDocsLoadBalSystem/gsCableDocsLoadBalSystem.h"
#include "gsAnCmcObjects/gsCableDocsServiceFlowObjects/gsCableDocsServiceFlowCfgTable/gsCableDocsServiceFlowCfgTable.h"
#include "gsAnCmcObjects/gsCableDocsServiceFlowObjects/gsCableDocsDsQosCfgTable/gsCableDocsDsQosCfgTable.h"

#include "gsAnCmcObjects/gsCableDocsSpectrumObjects/gsCableDocsUsSpectrumSystem/gsCableDocsUsSpectrumSystem.h"

#include "SnmpAgent.h"

using namespace Snmp;

/* Local Functions.*/
static void *RequestHandlerThreadProc(void *ptr)
{
    snmp_log(LOG_INFO,"Snmp Request Handler Thread is running!\n");

    while(true)
    {
        /* if you use select(), see snmp_select_info() in snmp_api(3) */
        /*     --- OR ---  */
        agent_check_and_process(1); /* 0 == don't block */
    }
    return NULL;
}

/* API Functions.*/
void CreateSnmpAgent(const char *pName,
                     const char *pConfig,
                     const char *pMibs,
                     int syslog,
                     int subagent)
{
    if((NULL==pName) || (NULL==pConfig) || (NULL==pMibs))
        return;

    if (syslog)
        snmp_enable_calllog();
    else
        snmp_enable_stderrlog();

    /* we're an agentx subagent? */
    if (subagent)
    {
        /* make us a agentx client. */
        netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);
    }

    /* sepecify the SNMP angent configuration file. */
    netsnmp_ds_set_string(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OPTIONALCONFIG, pConfig);

    /* set the MIB file directorys */
    setenv("MIBDIRS", pMibs, 1);

    /* Initialize tcpip, if necessary */
    SOCK_STARTUP;

    /* Initialize the agent library */
    init_agent(pName);

    /* Initialize the Common MIB modules */
    init_mib_modules();

    /*Initialize SNMP modules */
    InitSystem();
    Interfaces::InitInterfaces();
    Interfaces::InitIfTable();
    InitGsDslAddress();
    InitGsDslCommon();
    GsDslFtp::InitGsDslFtp();
    GsDslDhcpRelay::InitGsDslDhcpRelay();
    GsDslSysctrlTrapObjects::InitGsDslSysctrlTrapObjects();
    gsDslSyslogHostTable::InitGsDslSyslogHostTable();
    GsDslTrapHostTable::InitGsDslTrapHostTable();
    GsDslBoardTable::InitGDslBoardTable();
    InitDocsIfDownChannelTable();
    InitDocsIfUpChannelTable();
    InitDocsLoadBalGrpCfgTable();
    InitDocsLoadBalMacRangeCfgTable();
    InitDocsLoadBalExcludeMacCfgTable();
    InitGsCableDocsLoadBalSystem();
    InitGsCableDocsServiceFlowCfgTable();
    GsCableDocsDsQosCfgTable::InitTable();
    GsAnCmcExpandTrap::InitGsAnCmcExpandTrap();
    InitGsAnCmcCmCpeTable();
    InitGsAnCmcCmStatistics();
    GsAnCmcCmUniTalbe::InitGsAnCmcCmUniTable();
    InitGsAnCmcCmTrafficTable();
    InitGsAnCmcSystem();
    InitGsAnCmcCdtTransSVidTable();
    InitGsAnCmcUsChannelTable();
    InitGsAnCmcDownstreamChannelTable();
    InitGsAnCmcChannelStatisticsTable();
    InitGsAnCmcPortStatisticsTable();
    InitGsAnCmcVendorToVlanObjects();
    InitGsAnCmcVendorToVlanTable();
    GsAnCmcGlobalObject::InitGsAnCmcGlobalObject();
    InitGsCableDocsUsSpectrumSystem();

    /* Snmpd config file. */
    init_snmp(pName);

    /* If we're going to be a snmp master agent, initial the ports */
    if (!subagent)
    {
        init_master_agent();  /* open the port to listen on (defaults to udp:161) */
        //printf("--------------------------");

        pthread_t threadContxt;
        pthread_create(&threadContxt, NULL, RequestHandlerThreadProc, NULL);
    }

    return;
}

void DestorySnmpAgent(const char *pName, int subagent)
{
    if(NULL==pName)
        return;

    snmp_shutdown(pName);
    SOCK_CLEANUP;

    return;
}





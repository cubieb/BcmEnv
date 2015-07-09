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
#ifndef _SnmpAgent_h_
#define _SnmpAgent_h_

#define SNMP_MASTER_AGENT_NAME "controller-snmp-agent"

void CreateSnmpAgent(const char *pName, const char *pConfig,
                     const char *pMibs,
                     int syslog,
                     int subagent);
void DestorySnmpAgent(const char *pName, int subagent);

#endif /* _SnmpAgent_h_ */




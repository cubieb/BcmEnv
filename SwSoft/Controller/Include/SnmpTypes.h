/* --- system includes ---*/
#ifndef _SnmpTypes_h_
#define _SnmpTypes_h_

CxxBeginNameSpace(Snmp)

typedef int (*SnmpOperationHandler)(netsnmp_mib_handler&,
    netsnmp_handler_registration&,
    netsnmp_agent_request_info&,
    netsnmp_request_info&);

typedef void (*SnmpGetHandler)(netsnmp_agent_request_info&,
    netsnmp_request_info&, void*);

typedef void (*SnmpSetReserve2Handler)(netsnmp_agent_request_info&,
    netsnmp_request_info&, void* index);

CxxEndNameSpace /* Snmp */

#endif /* _SnmpTypes_h_ */



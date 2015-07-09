#ifndef _gsDslFtp_h_
#define _gsDslFtp_h_

CxxBeginNameSpace(Snmp)
CxxBeginNameSpace(GsDslFtp)

void InitGsDslFtp(void);
Netsnmp_Node_Handler HandleManagedObjectType;
Netsnmp_Node_Handler HandleAddress;
Netsnmp_Node_Handler HandleUserName;
Netsnmp_Node_Handler HandleUserPwd;
Netsnmp_Node_Handler HandleFilePath;
Netsnmp_Node_Handler HandleFileName;
Netsnmp_Node_Handler HandleLoadType;
Netsnmp_Node_Handler HandleDownloadStatus;
Netsnmp_Node_Handler HandleAddressType;
Netsnmp_Node_Handler HandleIpv6Address;
Netsnmp_Node_Handler HandleProtocolType;
Netsnmp_Node_Handler HandleManagedObjectSubType;

CxxEndNameSpace /* Snmp */
CxxEndNameSpace /* GsDslFtp */

#endif

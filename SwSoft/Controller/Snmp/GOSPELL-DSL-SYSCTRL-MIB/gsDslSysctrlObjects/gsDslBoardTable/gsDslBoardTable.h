/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 17695 2009-07-21 12:22:18Z dts12 $
 */
#ifndef _gsDslBoardTable_h_
#define _gsDslBoardTable_h_

namespace Snmp
{
namespace GsDslBoardTable
{

#define MaxDescrSize 32
#define MaxHardVersionSize 32
#define MaxSoftVersionSize 32
#define MaxBootwareVersionSize 32
#define MaxFwCreateTimeSize 14

/* function declarations */
void InitGDslBoardTable(void);

int SnmpTableHandler(netsnmp_mib_handler *handler,
    netsnmp_handler_registration *regInfo,
    netsnmp_agent_request_info *reqInfo,
    netsnmp_request_info *requests);

Netsnmp_First_Data_Point  GetFirst;
Netsnmp_Next_Data_Point   GetNext;

/* column number definitions for table gsDslBoardTable */
#define ColGsDslBoardRackNo     1
#define ColGsDslBoardShelfNo        2
#define ColGsDslBoardSlotNo     3
#define ColGsDslBoardType       4
#define ColGsDslBoardDescr      5
#define ColGsDslBoardPortNum        6
#define ColGsDslBoardPortType       7
#define ColGsDslBoardAdminStatus        8
#define ColGsDslBoardOperStatus     9
#define ColGsDslBoardHardVersion        10
#define ColGsDslBoardSoftVersion        11
#define ColGsDslBoardSoftLastChange     12
#define ColGsDslBoardReset      13
#define ColGsDslBoardCpuLoad        14
#define ColGsDslBoardCpuLoadThreshhold      15
#define ColGsDslBoardMemUsage       16
#define ColGsDslBoardMemUsageThreshhold     17
#define ColGsDslBoardActPortNum     18
#define ColGsDslBoardHardVerStatus      19
#define ColGsDslBoardSoftVerCfg     20
#define ColGsDslBoardStandbyStatus      21
#define ColGsDslBoardMemTotalSize       22
#define ColGsDslBoardWorkMode       23
#define ColGsDslBoardBootWareVersion        24
#define ColGsDslBoardServiceStoppedReason       25
#define ColGsDslBoardSlaveCpuSwVersion      51
#define ColGsDslBoardSlaveCpuBootVersion        52
#define ColGsDslBoardCpuTemperature     53
#define ColGsDslBoardFwCreateTime       54

} /* namespace GsDslBoardTable*/
} /* namespace Snmp*/

#endif /* _gsDslBoardTable_h_ */

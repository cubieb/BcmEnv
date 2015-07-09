#ifndef _StatisticsMgt_h_
#define _StatisticsMgt_h_

typedef unsigned long long uint64;

typedef struct {
	/*** RFC 1213 ***/
	uint64	snmpIfInOctets;
	uint64	snmpIfInUcastPkts;
 	uint64	snmpIfInNUcastPkts;
	uint64	snmpIfInDiscards;
	uint64	snmpIfInErrors;
	uint64	snmpIfInUnknownProtos;
	uint64	snmpIfOutOctets;
	uint64	snmpIfOutUcastPkts;
	uint64	snmpIfOutNUcastPkts;
	uint64	snmpIfOutDiscards;
	uint64	snmpIfOutErrors;
	uint64	snmpIfOutQLen;
	uint64	snmpIpInReceives;
	uint64	snmpIpInHdrErrors;
	uint64	snmpIpForwDatagrams;
	uint64	snmpIpInDiscards;
} __attribute__ ((packed))  WAN_PORT_STAT_CFG;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BASE_ErrCode MgtGetWanPortStat(WAN_PORT_STAT_CFG * wan_port_stat_entry);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


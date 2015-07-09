#ifndef _SnmpCfgMgt_h_
#define _SnmpCfgMgt_h_

#define MaxHostCommunitySize  64

typedef struct
{
    uint32_t trapHostIpAddr;
    int32_t  trapHostSnmpVer;
    char     trapHostCommunity[MaxHostCommunitySize];
    int32_t  trapHostIsGosNmsSever; /* bool */
    int32_t  trapHostEventType;
    int32_t  trapHostMinEventLevel;
}TrapCfgEntry;

void InitSnmpCfg();

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

bool GetFirstTrapHost(TrapCfgEntry* entry);
bool GetNextTrapHost(uint32_t curIpAddr, TrapCfgEntry* next);
bool FindTrapHost(uint32_t ip, TrapCfgEntry* entry);

bool InsertTrapHost(TrapCfgEntry* entry);
void DeleteTrapHost(uint32_t hostIp);
bool UpdateTrapHost(TrapCfgEntry* entry);

bool SaveSnmpCfg();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

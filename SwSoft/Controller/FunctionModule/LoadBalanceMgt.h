#ifndef _LoadBalanceMgt_h_
#define _LoadBalanceMgt_h_

/*0=Disabled, 1=Dynamic (run-time statistics) default, 2=Static (minimum reserved rate)*/
#define DefaultMethod                1
#define DefaultDataCollectPeriod     60
#define DefaultWeightedAveragePeriod 2
#define DefaultOverLoadThreshold     60
#define DefaultDiffThreshold         10
#define DefaultMaxCMMove             32
#define DefaultMinCMHoldTime         240
/*1=Enabled (default), 2=Disabled. */
#define DefaultRangOverrideMode      1
#define DefaultDCCInitATDMAMode      1
#define DefaultDCCInitSCDMAMode      1
#define DefaultDBCInitATDMAMode      1
#define DefaultDBCInitSCDMAMode      1

#define LdBlcMethodDisabled 0
#define LdBlcMethodDynamic  1
#define LdBlcMethodStatic   2

#define RangOverrideModeEnable 1
#define RangOverrideModeDisabled 2

void InitLoadBalance();
void DeinitLoadBalance();

struct LoadBalGrpCfg
{
    int32_t id;
    int32_t type;
    uint8_t dsChList[64];
    uint8_t dsChNumber;
    uint8_t usChList[64];
    uint8_t usChNumber;
    CMCOAM_CnuMacRangeT  macRange[BRCM_CMC_MAX_MAC_RANG];
    uint8_t              macRangeNum;
};
typedef struct LoadBalGrpCfg LoadBalGrpCfgT;

#ifdef __cplusplus
std::ostream& operator << (std::ostream& os, LoadBalGrpCfg const& grpCfg);
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MacRangeStartIndex 1   //for MIB
BASE_ErrCode GetFirstLoadBalanceGrp(LoadBalGrpCfgT*, CMCOAM_CmcAddrT*);
BASE_ErrCode GetNextLoadBalanceGrp(int32_t, LoadBalGrpCfgT*, CMCOAM_CmcAddrT*);
BASE_ErrCode FindLoadBalanceGrp(int32_t, LoadBalGrpCfgT*, CMCOAM_CmcAddrT*);

BASE_ErrCode SetFullLoadBalancingGrp(LoadBalGrpCfgT* grpCfg,
                                     CMCOAM_CmcAddrT* cmcMac);

BASE_ErrCode CreateLoadBalanceGrp(uint8_t groupId,
                                  CMCOAM_LoadBlcMethodT type,
                                  CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode RemoveLoadBalanceGrp(uint8_t groupId, CMCOAM_CmcAddrT* cmcMac);

BASE_ErrCode AddDsChannelToLoadBalanceGrp(uint8_t groupId,
                                          uint8_t* channelList, uint8_t channelNumber,
                                          CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode AddUsChannelToLoadBalanceGrp(uint8_t groupId,
                                          uint8_t* channelList, uint8_t channelNumber,
                                          CMCOAM_CmcAddrT* cmcMac);

BASE_ErrCode RemoveDsChannelFromLoadBalanceGrp(uint8_t groupId,
                                               uint8_t* channelList, uint8_t channelNumber,
                                               CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode RemoveUsChannelFromLoadBalanceGrp(uint8_t groupId,
                                               uint8_t* channelList, uint8_t channelNumber,
                                               CMCOAM_CmcAddrT* cmcMac);

BASE_ErrCode AddCnuRangeToLoadBalanceGrp(uint8_t groupId,
                                         MacAddressT* startMac,
                                         MacAddressT* endMac,
                                         CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode RemoveCnuRangeFromLoadBalanceGrp(uint8_t groupId,
                                              MacAddressT* startMac,
                                              MacAddressT* endMac,
                                              CMCOAM_CmcAddrT* cmcMac);

BASE_ErrCode AddCnuExcludeRange(MacAddressT* startMac,
                                 MacAddressT* endMac,
                                 CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode RemoveCnuExcludeRange(MacAddressT* startMac,
                                    MacAddressT* endMac,
                                    CMCOAM_CmcAddrT* cmcMac);

#define CnuExecludeMacRangeStartIndex 1
struct CnuExecludeMacRangeCfg
{
    int32_t index;
    CMCOAM_CnuMacRangeT macRange;
};
typedef struct CnuExecludeMacRangeCfg CnuExecludeMacRangeCfgT;

BASE_ErrCode GetFirstCnuExcludeMacRange(CnuExecludeMacRangeCfgT* range,
                                        CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode GetNextCnuExcludeMacRange(int32_t index,
                                       CnuExecludeMacRangeCfgT* range,
                                       CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode FindCnuExcludeMacRange(int32_t index,
                                    CnuExecludeMacRangeCfgT* range,
                                    CMCOAM_CmcAddrT* cmcMac);

BASE_ErrCode UpdateCnuExcludeMacRange(CnuExecludeMacRangeCfgT* range,
                                      CMCOAM_CmcAddrT* cmcMac);

size_t GetCnuMacExcludeRangeSize(CMCOAM_CmcAddrT* cmcMac);

BASE_ErrCode SaveLoadBalanceConfig();
void ClearLoadBalanceConfig();

void UnitTestLoadBalanceMgtShow();

BASE_ErrCode GetDynamicLoadBalanceConfig( CMCOAM_GetLdBlcDynCfgRspT * pdynLdBlcCfg );
BASE_ErrCode SetDynamicLoadBalanceMethod( U8 *pmethod );
BASE_ErrCode SetDynamicLoadBalancePeriod( U32 *pPeriod );
BASE_ErrCode SetDynamicLoadWeightedAveragePeriod( U32 *pWAPeriod );
BASE_ErrCode SetDynamicLoadOverloadThreshold( U32 * pOverloadThreshold );
BASE_ErrCode SetDynamicLoadDiffThreshold( U32 * pDiffThreshold );
BASE_ErrCode SetDynamicLoadMaxMove( U32 * pMaxMove );
BASE_ErrCode SetDynamicLoadMinHoldTime( U32 * pMinHoldTime );
BASE_ErrCode SetDynamicLoadRangeOverrideMode( U8 * pRangeOverrideMode );
BASE_ErrCode SetDynamicLoadAtdmaDccInitTechMode( U8 * pAtdmaDccInitTech );
BASE_ErrCode SetDynamicLoadScdmaDccInitTechMode( U8 * pScdmaDccInitTech );
BASE_ErrCode SetDynamicLoadAtdmaDbcInitTechMode( U8 * pAtdmaDbcInitTech );
BASE_ErrCode SetDynamicLoadScdmaDbcInitTechMode( U8 * pScdmaDbcInitTech );

#ifdef __cplusplus
}
#endif /* __cplusplus */

extern pthread_mutex_t mgtMutex;

#endif

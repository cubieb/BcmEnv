#include "SystemInclude.h"
#include "BcmController.h"

#include "Debug.h"
#include "Util.h"
#include "AutoLock.h"
#include "SaveCfgFile.h"
#include "LoadBalanceDb.h"
#include "LoadBalanceMgt.h"

using namespace std;

ostream& operator << (ostream& os, LoadBalGrpCfg const& grpCfg)
{
    typedef pair<int, string> TypeIdNamePair;
    map<int, string> typeMap;
    typeMap.insert(TypeIdNamePair(STATIC_LD_BLC, "Static"));
    typeMap.insert(TypeIdNamePair(DYNAMIC_LD_BLC, "Dynamic"));

    os << "===Load Balance Group Configuration===" << endl;
    os << "id       :" << grpCfg.id << endl;
    os << "type     :" << typeMap.find(grpCfg.type)->second << endl;
    os << "dsChList :";
    uint8_t i;
    for (i = 0; i < grpCfg.dsChNumber; ++i)
    {
        if (i != 0)
            os << ", ";
        os << hex << (int)grpCfg.dsChList[i];
    }
    os << endl;

    os << "usChList :";
    for (i = 0; i < grpCfg.usChNumber; ++i)
    {
        if (i != 0)
            os << ", ";
        os << hex << (int)grpCfg.usChList[i];
    }
    os << dec << endl;

    os << "macRange :" << endl;;
    for (i = 0; i < grpCfg.macRangeNum; ++i)
    {
        os << "    " << MacToString(grpCfg.macRange[i].start)
           << "    " << MacToString(grpCfg.macRange[i].end) << endl;
    }

    return os;
}


void InitLoadBalance()
{
    AutoLock autoLock(mgtMutex);
    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    /*0=Disabled, 1=Dynamic (run-time statistics) default, 2=Static (minimum reserved rate)*/
    dynLdBlcCfg.method = DefaultMethod ;
    dynLdBlcCfg.period = DefaultDataCollectPeriod ;
    dynLdBlcCfg.wa_period = DefaultWeightedAveragePeriod ;
    dynLdBlcCfg.overload_threshold = DefaultOverLoadThreshold ;
    dynLdBlcCfg.diff_threshold = DefaultDiffThreshold ;
    dynLdBlcCfg.max_move = DefaultMaxCMMove ;
    dynLdBlcCfg.min_hold_time = DefaultMinCMHoldTime ;
    /*1=Enabled (default), 2=Disabled. */
    dynLdBlcCfg.rang_override_mode = DefaultRangOverrideMode ;
    dynLdBlcCfg.atdma_dcc_init_tech = DefaultDCCInitATDMAMode ;
    dynLdBlcCfg.scdma_dcc_init_tech = DefaultDCCInitSCDMAMode ;
    dynLdBlcCfg.atdma_dbc_init_tech = DefaultDBCInitATDMAMode ;
    dynLdBlcCfg.scdma_dbc_init_tech = DefaultDBCInitSCDMAMode ;
    
    DBSetDynamicLoadBalance( &dynLdBlcCfg );
   
    return ;
}

static void ClearCmcIdLoadBalanceConfig(CMCOAM_CmcAddrT* cmcId);
static void ClearDynamicLoadBalanceCfg();
void DeinitLoadBalance()
{
    CMCOAM_CmcAddrT* cmcId;
    AutoLock autoLock(mgtMutex);

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc;
    for (cmc = cmcs.Begin(); cmc != cmcs.End(); ++cmc)
    {
        cmcId = (CMCOAM_CmcAddrT*)cmc->GetCmcMac();
        ClearCmcIdLoadBalanceConfig(cmcId);
    }

    cmcs.Clear();
    ClearDynamicLoadBalanceCfg();
}

static void CopyGrpToGrpCfg(LoadBalGrpCfgT& to, LoadBalanceGroup& from)
{
    memset(&to, 0, sizeof(LoadBalGrpCfgT));
    to.id = from.GetGroupId();
    to.type = from.GetType();
    from.GetDsChannel(to.dsChList, sizeof(to.dsChList), &to.dsChNumber);
    from.GetUsChannel(to.usChList, sizeof(to.usChList), &to.usChNumber);

    LoadBalanceGroup::CnuMacRangeIterator iter;
    to.macRangeNum = from.GetCnuMacRangeSize();
    size_t i;
    for (iter = from.CnuMacRangeBegin(), i = 0;
         iter != from.CnuMacRangeEnd() && i < BRCM_CMC_MAX_MAC_RANG;
         ++iter, ++i)
    {
        memcpy(to.macRange[i].start, iter->start, 6);
        memcpy(to.macRange[i].end, iter->end, 6);
    }
}

BASE_ErrCode GetFirstLoadBalanceGrp(LoadBalGrpCfgT* grpCfg, CMCOAM_CmcAddrT* cmcMac)
{
    assert(grpCfg != NULL);
    AutoLock autoLock(mgtMutex);

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        return BASE_ERROR;
    }

    LoadBalanceCmc::Iterator group = cmc->Begin();
    if (group == cmc->End())
    {
        return BASE_ERROR;
    }

    CopyGrpToGrpCfg(*grpCfg, group->second);

    return BASE_OK;
}

BASE_ErrCode GetNextLoadBalanceGrp(int32_t id, LoadBalGrpCfgT* next,
                                   CMCOAM_CmcAddrT* cmcMac)
{
    assert(next != NULL);
    AutoLock autoLock(mgtMutex);

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        return BASE_ERROR;
    }

    LoadBalanceCmc::Iterator group = cmc->Find(id);
    if (group == cmc->End())
    {
        return BASE_ERROR;
    }
    if (++group == cmc->End())
    {
        return BASE_ERROR;
    }

    CopyGrpToGrpCfg(*next, group->second);
    return BASE_OK;
}

BASE_ErrCode FindLoadBalanceGrp(int32_t id, LoadBalGrpCfgT* grpCfg,
                                CMCOAM_CmcAddrT* cmcMac)
{
    assert(grpCfg != NULL);
    AutoLock autoLock(mgtMutex);

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        return BASE_ERROR;
    }

    LoadBalanceCmc::Iterator group = cmc->Find(id);
    if (group == cmc->End())
    {
        return BASE_ERROR;
    }

    CopyGrpToGrpCfg(*grpCfg, group->second);

    return BASE_OK;
}

BASE_ErrCode SetFullLoadBalancingGrp(LoadBalGrpCfgT* grpCfg,
                                     CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    dbgstrm << "Start, " << *grpCfg << endl;

    errCode = CMCOAM_Group_SetFullLoadBalancingGrpSettings(grpCfg->id,
                                                           (CMCOAM_LoadBlcMethodT)grpCfg->type,
                                                           grpCfg->dsChList,
                                                           grpCfg->dsChNumber,
                                                           grpCfg->usChList,
                                                           grpCfg->usChNumber,
                                                           grpCfg->macRange,
                                                           grpCfg->macRangeNum,
                                                           cmcMac);
    if (errCode != BASE_OK)
    {
        errstrm << "CMCOAM_Group_SetFullLoadBalancingGrpSettings() Failed." << endl;
        return BASE_ERROR;
    }

    LoadBalanceGroup* grp = FindLoadBalanceGroup(grpCfg->id, (uint8_t*)cmcMac);
    if (grp == NULL)
    {
        CreateLoadBalanceGroup(grpCfg->id, (CMCOAM_LoadBlcMethodT)grpCfg->type,
                               grpCfg->dsChList,
                               grpCfg->dsChNumber,
                               grpCfg->usChList,
                               grpCfg->usChNumber,
                               grpCfg->macRange,
                               grpCfg->macRangeNum,
                               (uint8_t*)cmcMac);
        return BASE_OK;
    }

    grp->UpdateType((CMCOAM_LoadBlcMethodT)grpCfg->type);
    grp->UpdateChannel(grpCfg->dsChList, grpCfg->dsChNumber,
                       grpCfg->usChList, grpCfg->usChNumber);
    grp->UpdateCnuMacRange(grpCfg->macRange, grpCfg->macRangeNum);
    return errCode;
}

BASE_ErrCode CreateLoadBalanceGrp(uint8_t groupId, CMCOAM_LoadBlcMethodT type,
                                  CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;

    dbgstrm << DbgExpandVar(groupId, int) << ", " << DbgExpandVar(type, int)
        << "cmcMac = " << CmcAddrToString((uint8_t*)cmcMac) << endl;

    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_CreateDestroyLoadBalancingGrp(groupId, type,
                                                         LD_BLC_CREATE,
                                                         cmcMac);

    if (errCode != BASE_OK)
        return BASE_ERROR;

    CreateLoadBalanceGroup(groupId, type,
                           NULL, 0, NULL, 0, NULL, 0,
                           (uint8_t*)cmcMac);

    return errCode;
}

BASE_ErrCode RemoveLoadBalanceGrp(uint8_t groupId, CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;

    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_CreateDestroyLoadBalancingGrp(groupId,
                                                         STATIC_LD_BLC,
                                                         LD_BLC_DESTROY,
                                                         cmcMac);
    if (errCode != BASE_OK)
    {
        dbgstrm << "CMCOAM_Group_CreateDestroyLoadBalancingGrp() failed." << endl;
        return BASE_ERROR;
    }

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        return BASE_ERROR;
    }

    cmc->Erase(groupId);
    return BASE_OK;
}

BASE_ErrCode AddDsChannelToLoadBalanceGrp(uint8_t groupId,
                                          uint8_t* channelList, uint8_t channelNumber,
                                          CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_AddRemoveDsUsToFromLoadBalancingGrp(groupId, channelList,
                                               channelNumber,
                                               LD_BLC_CREATE,
                                               SetAddRemoveDsToFromLoadBlcGrpRequest,
                                               cmcMac);

    if (errCode != BASE_OK)
        return BASE_ERROR;

    LoadBalanceGroup* group = FindLoadBalanceGroup(groupId, (uint8_t*)cmcMac);
    /* CMCOAM never return error, even if group has not been created before.
     * So we must check to see if the group's existence,  or else program
     * will crash.
     */
    if (group == NULL)
    {
        return BASE_ERROR;
    }

    for (uint8_t i = 0; i < channelNumber; ++i)
    {
        group->AddDsChannel(channelList[i]);
    }

    return errCode;
}

BASE_ErrCode AddUsChannelToLoadBalanceGrp(uint8_t groupId,
                                          uint8_t* channelList, uint8_t channelNumber,
                                          CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_AddRemoveDsUsToFromLoadBalancingGrp(groupId, channelList,
                                                   channelNumber,
                                                   LD_BLC_CREATE,
                                                   SetAddRemoveUsToFromLoadBlcGrpRequest,
                                                   cmcMac);
    if (errCode != BASE_OK)
        return BASE_ERROR;

    LoadBalanceGroup* group = FindLoadBalanceGroup(groupId, (uint8_t*)cmcMac);
    /* CMCOAM never return error, even if group has not been created before.
     * So we must check to see if the group's existence,  or else program
     * will crash.
     */
    if (group == NULL)
    {
        return BASE_ERROR;
    }

    for (uint8_t i = 0; i < channelNumber; ++i)
    {
        group->AddUsChannel(channelList[i]);
    }

    return errCode;
}

BASE_ErrCode RemoveDsChannelFromLoadBalanceGrp(uint8_t groupId,
                                               uint8_t* channelList,
                                               uint8_t channelNumber,
                                               CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_AddRemoveDsUsToFromLoadBalancingGrp(groupId, channelList,
                                                   channelNumber,
                                                   LD_BLC_DESTROY,
                                                   SetAddRemoveDsToFromLoadBlcGrpRequest,
                                                   cmcMac);
    if (errCode != BASE_OK)
        return BASE_ERROR;

    LoadBalanceGroup* group = FindLoadBalanceGroup(groupId, (uint8_t*)cmcMac);
    /* CMCOAM never return error, even if group has not been created before.
     * So we must check to see if the group's existence,  or else program
     * will crash.
     */
    if (group == NULL)
    {
        return BASE_ERROR;
    }

    for (uint8_t i = 0; i < channelNumber; ++i)
    {
        group->RemoveDsChannel(channelList[i]);
    }

    return errCode;
}

BASE_ErrCode RemoveUsChannelFromLoadBalanceGrp(uint8_t groupId,
                                               uint8_t* channelList,
                                               uint8_t channelNumber,
                                               CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_AddRemoveDsUsToFromLoadBalancingGrp(groupId, channelList,
                                               channelNumber,
                                               LD_BLC_DESTROY,
                                               SetAddRemoveUsToFromLoadBlcGrpRequest,
                                               cmcMac);
    if (errCode != BASE_OK)
        return BASE_ERROR;

    LoadBalanceGroup* group = FindLoadBalanceGroup(groupId, (uint8_t*)cmcMac);
    /* CMCOAM never return error, even if group has not been created before.
     * So we must check to see if the group's existence,  or else program
     * will crash.
     */
    if (group == NULL)
    {
        return BASE_ERROR;
    }

    for (uint8_t i = 0; i < channelNumber; ++i)
    {
        group->RemoveUsChannel(channelList[i]);
    }

    return errCode;
}

BASE_ErrCode AddCnuRangeToLoadBalanceGrp(uint8_t groupId,
                                    MacAddressT* startMac,
                                    MacAddressT* endMac,
                                    CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;

    dbgstrm << DbgExpandVar(groupId, int)
        << ", strarMac = " << MacToString(startMac->addr)
        << ", endMac = " << MacToString(endMac->addr)
        << ", cmcMac = " << CmcAddrToString((uint8_t*)cmcMac) << endl;

    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_AddRemoveCnuToFromLoadBalancingGrp(groupId, LD_BLC_CREATE,
                                                              startMac, endMac,
                                                              cmcMac);

    if (errCode != BASE_OK)
    {
        dbgstrm << "CMCOAM_Group_AddRemoveCnuToFromLoadBalancingGrp() failed." << endl;
        return BASE_ERROR;
    }

    LoadBalanceGroup* group = FindLoadBalanceGroup(groupId, (uint8_t*)cmcMac);
    /* CMCOAM never return error, even if group has not been created before.
     * So we must check to see if the group's existence,  or else program
     * will crash.
     */
    if (group == NULL)
    {
        dbgstrm << "cant get group information." << endl;
        return BASE_ERROR;
    }

    CMCOAM_CnuMacRangeT macRange;
    CopyMacAddr(macRange.start, startMac->addr);
    CopyMacAddr(macRange.end, endMac->addr);
    if (!group->IsCnuMacRangeExisted(macRange))
    {
        group->AddCnuMacRange(macRange);
    }

    /* Remove range from other group */
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cliGetCmc());
    if (cmc == cmcs.End())
    {
        errstrm << "cant get cmc information." << endl;
        return BASE_ERROR;
    }
    LoadBalanceCmc::Iterator grpIter;
    for (grpIter = cmc->Begin(); grpIter != cmc->End(); ++grpIter)
    {
        if (group->GetId() == grpIter->second.GetId())
        {
            continue;
        }
        if (grpIter->second.IsCnuMacRangeExisted(macRange))
        {
            grpIter->second.RemoveCnuMacRange(macRange);
        }
    }

    return BASE_OK;
}

BASE_ErrCode RemoveCnuRangeFromLoadBalanceGrp(uint8_t groupId,
                                         MacAddressT* startMac,
                                         MacAddressT* endMac,
                                         CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;

    dbgstrm << DbgExpandVar(groupId, int)
        << ", strarMac = " << MacToString(startMac->addr)
        << ", endMac = " << MacToString(endMac->addr)
        << ", cmcMac = " << CmcAddrToString((uint8_t*)cmcMac) << endl;

    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Group_AddRemoveCnuToFromLoadBalancingGrp(groupId, LD_BLC_DESTROY,
                                                              startMac, endMac,
                                                              cmcMac);
    if (errCode != BASE_OK)
    {
        dbgstrm << "CMCOAM_Group_AddRemoveCnuToFromLoadBalancingGrp() failed." << endl;
        return BASE_ERROR;
    }

    LoadBalanceGroup* group = FindLoadBalanceGroup(groupId, (uint8_t*)cmcMac);
    /* CMCOAM never return error, even if group has not been created before.
     * So we must check to see if the group's existence,  or else program
     * will crash.
     */
    if (group == NULL)
    {
        dbgstrm << "cant get group information." << endl;
        return BASE_ERROR;
    }

    CMCOAM_CnuMacRangeT macRange;
    CopyMacAddr(macRange.start, startMac->addr);
    CopyMacAddr(macRange.end, endMac->addr);
    group->RemoveCnuMacRange(macRange);

    return errCode;
}

static BASE_ErrCode AddCnuExcludeRangeByAddr(MacAddressT* startMac,
                                             MacAddressT* endMac,
                                             CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;

    dbgstrm << "strarMac = " << MacToString(startMac->addr)
        << ", endMac = " << MacToString(endMac->addr)
        << ", cmcMac = " << CmcAddrToString((uint8_t*)cmcMac) << endl;

    errCode = CMCOAM_Group_SetExcludeCnusFromLoadBalancingGrp(LD_BLC_CREATE,
                                                              startMac, endMac,
                                                              cmcMac);

    if (errCode != BASE_OK)
    {
        dbgstrm << "CMCOAM_Group_SetExcludeCnusFromLoadBalancingGrp() failed." << endl;
        return BASE_ERROR;
    }

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        dbgstrm << "cant get cmc information, create new one." << endl;
        cmcs.PushBack((uint8_t *)cmcMac);
        cmc = cmcs.Find((uint8_t*)cmcMac);
    }

    CMCOAM_CnuMacRangeT macRange;
    CopyMacAddr(macRange.start, startMac->addr);
    CopyMacAddr(macRange.end, endMac->addr);

    cmc->AddCnuMacExcludeRange(macRange);

    return errCode;
}

BASE_ErrCode AddCnuExcludeRange(MacAddressT* startMac,
                                 MacAddressT* endMac,
                                 CMCOAM_CmcAddrT* cmcMac)
{
    AutoLock autoLock(mgtMutex);
    return AddCnuExcludeRangeByAddr(startMac, endMac, cmcMac);
}

static BASE_ErrCode RemoveCnuExcludeRangeByAddr(MacAddressT* startMac,
                                                MacAddressT* endMac,
                                                CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;

    dbgstrm << "strarMac = " << MacToString(startMac->addr)
            << ", endMac = " << MacToString(endMac->addr)
            << ", cmcMac = " << CmcAddrToString((uint8_t*)cmcMac) << endl;

    errCode = CMCOAM_Group_SetExcludeCnusFromLoadBalancingGrp(LD_BLC_DESTROY,
                                                              startMac, endMac,
                                                              cmcMac);

    if (errCode != BASE_OK)
    {
        dbgstrm << "CMCOAM_Group_SetExcludeCnusFromLoadBalancingGrp() failed." << endl;
        return BASE_ERROR;
    }

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        dbgstrm << "cant get cmc information." << endl;
        return BASE_ERROR;
    }

    CMCOAM_CnuMacRangeT macRange;
    CopyMacAddr(macRange.start, startMac->addr);
    CopyMacAddr(macRange.end, endMac->addr);

    cmc->RemoveCnuMacExcludeRange(macRange);

    return errCode;
}

BASE_ErrCode RemoveCnuExcludeRange(MacAddressT* startMac,
                                    MacAddressT* endMac,
                                    CMCOAM_CmcAddrT* cmcMac)
{
    AutoLock autoLock(mgtMutex);
    return RemoveCnuExcludeRangeByAddr(startMac, endMac, cmcMac);
}

BASE_ErrCode GetFirstCnuExcludeMacRange(CnuExecludeMacRangeCfgT* range,
                                        CMCOAM_CmcAddrT* cmcMac)
{
    AutoLock autoLock(mgtMutex);

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        dbgstrm << "cant get cmc information, cmcMac = "
                << MacToString((uint8_t*)cmcMac)
                << endl;
        return BASE_ERROR;
    }

    LoadBalanceCmc::CnuRangeItorator iter = cmc->CnuMacExcludeRangeBegin();
    if (iter == cmc->CnuMacExcludeRangeEnd())
    {
        dbgstrm << "No exclude MAC configuration." << endl;
        return BASE_ERROR;
    }

    range->index = CnuExecludeMacRangeStartIndex;
    memcpy(range->macRange.start, iter->start, 6);
    memcpy(range->macRange.end, iter->end, 6);
    return BASE_OK;
}

static BASE_ErrCode FindCnuExcludeMacRangeByIndex(int32_t index,
                                                  CnuExecludeMacRangeCfgT* range,
                                                  CMCOAM_CmcAddrT* cmcMac)
{
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        /* We don't have any configuration about LoadBalance. */
        dbgstrm << "cant get cmc information, cmcMac = "
                << MacToString((uint8_t*)cmcMac)
                << endl;
        return BASE_ERROR;
    }

    if ((size_t)index - CnuExecludeMacRangeStartIndex + 1 > cmc->GetCnuMacExcludeRangeSize())
    {
        return BASE_ERROR;
    }
    LoadBalanceCmc::CnuRangeItorator iter;
    iter = cmc->FindMacExcludeRange(index - CnuExecludeMacRangeStartIndex);

    range->index = index;
    memcpy(range->macRange.start, iter->start, 6);
    memcpy(range->macRange.end, iter->end, 6);
    return BASE_OK;
}

BASE_ErrCode GetNextCnuExcludeMacRange(int32_t index,
                                        CnuExecludeMacRangeCfgT* range,
                                        CMCOAM_CmcAddrT* cmcMac)
{
    AutoLock autoLock(mgtMutex);

    return FindCnuExcludeMacRangeByIndex(index + 1, range, cmcMac);
}

BASE_ErrCode FindCnuExcludeMacRange(int32_t index,
                                    CnuExecludeMacRangeCfgT* range,
                                    CMCOAM_CmcAddrT* cmcMac)
{
    AutoLock autoLock(mgtMutex);

    return FindCnuExcludeMacRangeByIndex(index, range, cmcMac);
}

BASE_ErrCode UpdateCnuExcludeMacRange(CnuExecludeMacRangeCfgT* range,
                                      CMCOAM_CmcAddrT* cmcMac)
{
    CnuExecludeMacRangeCfgT old;
    AutoLock autoLock(mgtMutex);

    BASE_ErrCode errCode = FindCnuExcludeMacRangeByIndex(range->index, &old, cmcMac);
    if (errCode != BASE_OK)
    {
        errstrm << "Can't find original exclude MAC range." << endl;
    }

    MacAddressT start, end;
    memcpy(start.addr, old.macRange.start, 6);
    memcpy(end.addr, old.macRange.end, 6);
    RemoveCnuExcludeRangeByAddr(&start, &end, cmcMac);
    if (errCode != BASE_OK)
    {
        errstrm << "Can't remove original exclude MAC range." << endl;
        return errCode;
    }

    memcpy(start.addr, range->macRange.start, 6);
    memcpy(end.addr, range->macRange.end, 6);
    errCode = AddCnuExcludeRangeByAddr(&start, &end, cmcMac);

    return errCode;
}

size_t GetCnuMacExcludeRangeSize(CMCOAM_CmcAddrT* cmcMac)
{
    AutoLock autoLock(mgtMutex);
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        /* We don't have any configuration about LoadBalance. */
        dbgstrm << "cant get cmc information, cmcMac = "
                << MacToString((uint8_t*)cmcMac)
                << endl;
        return 0;
    }

    return cmc->GetCnuMacExcludeRangeSize();
}

static BASE_ErrCode SaveLoadBalanceGroupConfig()
{
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cliGetCmc());
    if (cmc == cmcs.End())
    {
        /* We don't have any configuration about LoadBalance. */
        dbgstrm << "cant get cmc information." << endl;
        return BASE_OK;
    }

    LoadBalanceCmc::Iterator grpIter;
    uint8_t dsChannelList[BRCM_CMC_MAX_DS_CH];
    uint8_t dsChannelListNum;
    uint8_t usChannelList[BRCM_CMC_MAX_US_CH];
    uint8_t usChannelListNum;

    ostringstream os;
    for (grpIter = cmc->Begin(); grpIter != cmc->End(); ++grpIter)
    {
        LoadBalanceGroup& group = grpIter->second;

        os << "cable load-balance group " << (int)group.GetId();
        if (group.GetType() == STATIC_LD_BLC)
            os << " static" << endl;
        else
            os << " dynamic" << endl;

        uint8_t i;
        group.GetDsChannel(dsChannelList, sizeof(dsChannelList), &dsChannelListNum);
        for (i = 0; i < dsChannelListNum; ++i)
        {
            os << "cable load-balance group " << (int)group.GetId()
                << " add downstream " << (int)dsChannelList[i]
                << endl;
        }

        group.GetUsChannel(usChannelList, sizeof(usChannelList), &usChannelListNum);
        for (i = 0; i < usChannelListNum; ++i)
        {
            os << "cable load-balance group " << (int)group.GetId()
                << " add upstream " << (int)usChannelList[i]
                << endl;
        }

        LoadBalanceGroup::CnuMacRangeIterator cnuRange;
        for (cnuRange = group.CnuMacRangeBegin();
            cnuRange != group.CnuMacRangeEnd();
            ++cnuRange)
        {
            os << "cable load-balance group " << (int)group.GetId()
                << " add modem " << MacToString(cnuRange->start)
                << " " << MacToString(cnuRange->end)
                << endl;
        }
    }

    LoadBalanceCmc::CnuRangeItorator cnuExcludeRange;
    for (cnuExcludeRange = cmc->CnuMacExcludeRangeBegin();
         cnuExcludeRange != cmc->CnuMacExcludeRangeEnd();
         ++cnuExcludeRange)
    {
        os << "cable load-balance group exclude modem "
            << MacToString(cnuExcludeRange->start)
            << " " << MacToString(cnuExcludeRange->end)
            << endl;
    }
    
    SaveCfg(os.str().c_str(), InsertToEnd);

    return BASE_OK;
}

static BASE_ErrCode SaveLoadBalanceDynamicConfig()
{
    AutoLock autoLock(mgtMutex);
    ostringstream os;
    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    
    DBGetDynamicLoadBalance( &dynLdBlcCfg );
    
    if( LdBlcMethodDisabled == dynLdBlcCfg.method )
        os << "cable load-balance dynamic method disable "
             << endl;
    else if( LdBlcMethodDynamic == dynLdBlcCfg.method )
        os << "cable load-balance dynamic method dynamic "
             << endl;
    else if( LdBlcMethodStatic == dynLdBlcCfg.method )
        os << "cable load-balance dynamic method static "
             << endl;

    if ( DefaultDataCollectPeriod != dynLdBlcCfg.period )
        os << "cable load-balance dynamic period "<< (int)dynLdBlcCfg.period 
             << endl;

    if ( DefaultWeightedAveragePeriod != dynLdBlcCfg.wa_period )
        os << "cable load-balance dynamic weighted-average-period "
           << (int)dynLdBlcCfg.wa_period << endl;

    if ( DefaultOverLoadThreshold != dynLdBlcCfg.overload_threshold )
        os << "cable load-balance dynamic overload-threshold-percent "
           << (int)dynLdBlcCfg.overload_threshold << endl;

    if ( DefaultDiffThreshold != dynLdBlcCfg.diff_threshold )
        os << "cable load-balance dynamic difference-threshold-percent "
           << (int)dynLdBlcCfg.diff_threshold << endl;

    if ( DefaultMaxCMMove != dynLdBlcCfg.max_move )
        os << "cable load-balance dynamic max-moves "
           << (int)dynLdBlcCfg.max_move << endl;

    if ( DefaultMinCMHoldTime != dynLdBlcCfg.min_hold_time )
        os << "cable load-balance dynamic min-hold-time "
           << (int)dynLdBlcCfg.min_hold_time << endl;

    if ( RangOverrideModeEnable == dynLdBlcCfg.rang_override_mode)
        os << "cable load-balance dynamic ranging-override-mode enable " << endl;
    else if ( RangOverrideModeDisabled == dynLdBlcCfg.rang_override_mode)
        os << "cable load-balance dynamic ranging-override-mode disable " << endl;

    if ( DefaultDCCInitATDMAMode != dynLdBlcCfg.atdma_dcc_init_tech )
        os << "cable load-balance dynamic atdma-dcc-init-tech "
           << (int)dynLdBlcCfg.atdma_dcc_init_tech << endl;

    if ( DefaultDCCInitSCDMAMode != dynLdBlcCfg.scdma_dcc_init_tech )
        os << "cable load-balance dynamic scdma-dcc-init-tech "
           << (int)dynLdBlcCfg.scdma_dcc_init_tech << endl;

    if ( DefaultDBCInitATDMAMode != dynLdBlcCfg.atdma_dbc_init_tech )
        os << "cable load-balance dynamic atdma-dbc-init-tech "
           << (int)dynLdBlcCfg.atdma_dbc_init_tech << endl;

    if ( DefaultDBCInitSCDMAMode != dynLdBlcCfg.scdma_dbc_init_tech )
        os << "cable load-balance dynamic scdma-dbc-init-tech "
           << (int)dynLdBlcCfg.scdma_dbc_init_tech << endl;

    SaveCfg(os.str().c_str(), InsertToEnd);
    return BASE_OK;
    
}

BASE_ErrCode SaveLoadBalanceConfig()
{
    SaveLoadBalanceGroupConfig();
    SaveLoadBalanceDynamicConfig();
    
    return BASE_OK;
}

void ClearCmcIdLoadBalanceConfig(CMCOAM_CmcAddrT* cmcId)
{
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcId);

    LoadBalanceCmc::Iterator grpIter;
    uint8_t dsChannelList[BRCM_CMC_MAX_DS_CH];
    uint8_t dsChannelListNum;
    uint8_t usChannelList[BRCM_CMC_MAX_US_CH];
    uint8_t usChannelListNum;

    BASE_ErrCode errCode;
    for (grpIter = cmc->Begin(); grpIter != cmc->End(); ++grpIter)
    {
        LoadBalanceGroup& group = grpIter->second;

        group.GetDsChannel(dsChannelList, sizeof(dsChannelList), &dsChannelListNum);
        if (dsChannelListNum != 0)
        {
            errCode = CMCOAM_Group_AddRemoveDsUsToFromLoadBalancingGrp(group.GetId(),
                           dsChannelList, dsChannelListNum, LD_BLC_DESTROY,
                           SetAddRemoveDsToFromLoadBlcGrpRequest, cmcId);
            if (errCode != BASE_OK)
            {
                dbgstrm << "Cannot remove ds channel." << (int)group.GetId()
                    << ", dsChannelListNum = " << (int)dsChannelListNum
                    << "[" << ArrayToString(dsChannelList, dsChannelListNum) << "]"
                    << endl;
            }
        }

        group.GetUsChannel(usChannelList, sizeof(usChannelList), &usChannelListNum);
        if (usChannelListNum != 0)
        {
            errCode = CMCOAM_Group_AddRemoveDsUsToFromLoadBalancingGrp(group.GetId(),
                           usChannelList, usChannelListNum, LD_BLC_DESTROY,
                           SetAddRemoveUsToFromLoadBlcGrpRequest, cmcId);
            if (errCode != BASE_OK)
            {
                dbgstrm << "Cannot remove us channel. groupId = " << (int)group.GetId()
                    << ", usChannelListNum = " << (int)usChannelListNum
                    << "[" << ArrayToString(usChannelList, usChannelListNum) << "]"
                    << endl;
            }
        }

        LoadBalanceGroup::CnuMacRangeIterator cnuRange;
        for (cnuRange = group.CnuMacRangeBegin();
            cnuRange != group.CnuMacRangeEnd();
            ++cnuRange)
        {
            MacAddressT start, end;
            CopyMacAddr(start.addr, cnuRange->start);
            CopyMacAddr(end.addr, cnuRange->end);
            errCode = CMCOAM_Group_AddRemoveCnuToFromLoadBalancingGrp(group.GetId(),
                                                                      LD_BLC_DESTROY,
                                                                      &start, &end,
                                                                      cmcId);
            if (errCode != BASE_OK)
            {
                dbgstrm << "Cannot remove cnu from group." << endl;
            }
        }

        errCode = CMCOAM_Group_CreateDestroyLoadBalancingGrp(group.GetId(),
                                                             group.GetType(),
                                                             LD_BLC_DESTROY,
                                                             cmcId);
        if (errCode != BASE_OK)
        {
            dbgstrm << "Cannot remove group." << endl;
        }
    }

    LoadBalanceCmc::CnuRangeItorator cnuExcludeRange;
    for (cnuExcludeRange = cmc->CnuMacExcludeRangeBegin();
         cnuExcludeRange != cmc->CnuMacExcludeRangeEnd();
         ++cnuExcludeRange)
    {
        MacAddressT start, end;
        CopyMacAddr(start.addr, cnuExcludeRange->start);
        CopyMacAddr(end.addr, cnuExcludeRange->end);
        errCode = CMCOAM_Group_SetExcludeCnusFromLoadBalancingGrp(LD_BLC_DESTROY,
                                                        &start, &end,
                                                        cmcId);
        if (errCode != BASE_OK)
        {
            dbgstrm << "Cannot remove cnu mac excluded range." << endl;
        }
    }
}

void ClearLoadBalanceConfig()
{
    CMCOAM_CmcAddrT* cmcId;
    AutoLock autoLock(mgtMutex);

    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc;
    for (cmc = cmcs.Begin(); cmc != cmcs.End(); ++cmc)
    {
        cmcId = (CMCOAM_CmcAddrT*)cmc->GetCmcMac();
        ClearCmcIdLoadBalanceConfig(cmcId);
    }

    cmcs.Clear();
}

void UnitTestLoadBalanceMgtShow()
{
    UnitTestLoadBalanceDbShow();
}


//edit by qingsong

BASE_ErrCode GetDynamicLoadBalanceConfig( CMCOAM_GetLdBlcDynCfgRspT * pdynLdBlcCfg )
{
    AutoLock autoLock(mgtMutex);

    if(CMCOAM_Group_GetLoadBalancingDynConfig( pdynLdBlcCfg, cliGetCmc() ) != BASE_OK)
    	return BASE_ERROR;

    DBSetDynamicLoadBalance( pdynLdBlcCfg );
    return BASE_OK;
}

BASE_ErrCode 
SetDynamicLoadBalanceMethod( U8 *pmethod )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig( &dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.method = *pmethod;
    errCode = CMCOAM_Group_SetLoadBalancingDynConfig( &dynLdBlcCfg, cliGetCmc() );

    if ( BASE_OK == errCode )
    {
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
        dbgstrm << "DBSetDynamicLoadBalance-method ok " <<endl;
    }
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;
}

BASE_ErrCode SetDynamicLoadBalancePeriod( U32 *pPeriod )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.period = *pPeriod;
    errCode = CMCOAM_Group_SetLoadBalancingDynConfig( &dynLdBlcCfg, cliGetCmc() );

    if ( BASE_OK == errCode )
    {
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    }
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;
 
}

BASE_ErrCode SetDynamicLoadWeightedAveragePeriod( U32 *pWAPeriod )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    /*Number of periods for calculating the weighted average load. 
    Each period gets twice the weight of the previous period. 
    Valid values are 1, 2, or 4. Applies to Balancing Method 1 only. (0=no change)*/
    if( LdBlcMethodDynamic != dynLdBlcCfg.method )
    {
    	BASE_ERR("Weighted Average Periods Applies to \"Dynamic\" Load Balance Method Only!");
    	return BASE_ERROR;
    }
    if((*pWAPeriod != 1) && (*pWAPeriod != 2) && (*pWAPeriod != 4))
    {
    	BASE_ERR("Only Value 1,2,or 4 are valid!");
    	return BASE_ERROR_PARAM;
    }

    dynLdBlcCfg.wa_period = *pWAPeriod;
    errCode = CMCOAM_Group_SetLoadBalancingDynConfig( &dynLdBlcCfg, cliGetCmc() );

    if ( BASE_OK == errCode )       
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;
    
 }

BASE_ErrCode SetDynamicLoadOverloadThreshold( U32 * pOverloadThreshold )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    /*A channel is considered overloaded when the load is at least this percent of capacity. 
    Below this, no attempt is made to balance its load. 
    Applies to Balancing Method 1 only. (0=no change)*/
    if( LdBlcMethodDynamic != dynLdBlcCfg.method)
    {
    	BASE_ERR("Overload Threshold Percent Applies to \"Dynamic\" Load Balance Method Only!");
    	return BASE_ERROR;
    }

    if( *pOverloadThreshold > 100 )
    {
    	BASE_ERR("Invalid Input Value, should not greater than 100\n");
    	return BASE_ERROR_PARAM;
    }

    dynLdBlcCfg.overload_threshold = *pOverloadThreshold;
    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	

    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }


BASE_ErrCode SetDynamicLoadDiffThreshold( U32 * pDiffThreshold )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

     /* For channels at or above the overload threshold,
        only consider less loaded alternate channels where the load
        difference is at least this percent. Applies to Balancing
        Method 1 only. (0=no change)
      */
    if( LdBlcMethodDynamic != dynLdBlcCfg.method)
    {
    	BASE_ERR("Difference Threshold Percent Applies to \"Dynamic\" Load Balance Method Only!");
    	return BASE_ERROR;
    }

     if( *pDiffThreshold > 100 )
    {
    	BASE_ERR("Difference threshold should not greater than 100 \n");
    	return BASE_ERROR_PARAM;
    }

    if( *pDiffThreshold > dynLdBlcCfg.overload_threshold)
    {
    	BASE_ERR("Difference threshold should not greater than overload threshold(%u%%)\n", dynLdBlcCfg.overload_threshold);
    	return BASE_ERROR_PARAM;
    }	

    dynLdBlcCfg.diff_threshold = *pDiffThreshold;   
    
    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }

BASE_ErrCode SetDynamicLoadMaxMove( U32 * pMaxMove )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.max_move = *pMaxMove;

    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }
    
BASE_ErrCode SetDynamicLoadMinHoldTime( U32 * pMinHoldTime )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.min_hold_time = *pMinHoldTime;

    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }

BASE_ErrCode SetDynamicLoadRangeOverrideMode( U8 * pRangeOverrideMode )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.rang_override_mode = *pRangeOverrideMode;

    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }

BASE_ErrCode SetDynamicLoadAtdmaDccInitTechMode( U8 * pAtdmaDccInitTech )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if(CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.atdma_dcc_init_tech = *pAtdmaDccInitTech;

    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }

BASE_ErrCode SetDynamicLoadScdmaDccInitTechMode( U8 * pScdmaDccInitTech )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if (CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.scdma_dcc_init_tech = *pScdmaDccInitTech;

    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }

BASE_ErrCode SetDynamicLoadAtdmaDbcInitTechMode( U8 * pAtdmaDbcInitTech )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if (CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.atdma_dbc_init_tech = *pAtdmaDbcInitTech;

    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }

BASE_ErrCode SetDynamicLoadScdmaDbcInitTechMode( U8 * pScdmaDbcInitTech )
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);

    CMCOAM_GetLdBlcDynCfgRspT dynLdBlcCfg;
    memset(&dynLdBlcCfg, 0, sizeof(dynLdBlcCfg));

    if (CMCOAM_Group_GetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc()) != BASE_OK)
    	return BASE_ERROR;

    dynLdBlcCfg.scdma_dbc_init_tech = *pScdmaDbcInitTech;

    errCode = CMCOAM_Group_SetLoadBalancingDynConfig(&dynLdBlcCfg, cliGetCmc());	
    if ( BASE_OK == errCode )
        DBSetDynamicLoadBalance( &dynLdBlcCfg );
    else 
    {
        dbgstrm << "CMCOAM_Group_SetLoadBalancingDynConfig failed!" <<endl;
        return BASE_ERROR; 
    }
    return BASE_OK;    
 }


void ClearDynamicLoadBalanceCfg()
{
    dbgstrm << "DelInitDynamicLoadBalanceConfig testing " <<endl;

    CMCOAM_GetLdBlcDynCfgRspT  dynLdBlcCfg;
    memset( &dynLdBlcCfg , 0 , sizeof(dynLdBlcCfg) );
     
    DBGetDynamicLoadBalance( &dynLdBlcCfg );
    
    if ( DefaultMethod != dynLdBlcCfg.method )
    {
        dynLdBlcCfg.method = DefaultMethod;
        SetDynamicLoadBalanceMethod( &dynLdBlcCfg.method );
    } 
    
    if ( DefaultDataCollectPeriod != dynLdBlcCfg.period )
    {
        dynLdBlcCfg.period = DefaultDataCollectPeriod;
        SetDynamicLoadBalancePeriod( &dynLdBlcCfg.period );
    } 

    if ( DefaultWeightedAveragePeriod != dynLdBlcCfg.wa_period )
    {
        dynLdBlcCfg.wa_period = DefaultWeightedAveragePeriod;
        U32 waPeriod = (U32)dynLdBlcCfg.wa_period;
        SetDynamicLoadWeightedAveragePeriod( & waPeriod );
    }

    if ( DefaultOverLoadThreshold != dynLdBlcCfg.overload_threshold )
    {
        dynLdBlcCfg.overload_threshold = DefaultOverLoadThreshold;
        U32 overload = (U32)dynLdBlcCfg.overload_threshold;
        SetDynamicLoadOverloadThreshold( &overload  );
    }

    if ( DefaultDiffThreshold != dynLdBlcCfg.diff_threshold )
    {
        dynLdBlcCfg.diff_threshold = DefaultDiffThreshold;
        U32 diff = (U32)dynLdBlcCfg.diff_threshold;
        SetDynamicLoadDiffThreshold( &diff  );
    }

    if ( DefaultMaxCMMove != dynLdBlcCfg.max_move )
    {
        dynLdBlcCfg.max_move = DefaultMaxCMMove;
        SetDynamicLoadMaxMove( &dynLdBlcCfg.max_move  );
    }

    if ( DefaultMinCMHoldTime != dynLdBlcCfg.min_hold_time )
    {
        dynLdBlcCfg.min_hold_time = DefaultMinCMHoldTime;
        SetDynamicLoadMinHoldTime( &dynLdBlcCfg.min_hold_time  );
    }

    if ( DefaultRangOverrideMode != dynLdBlcCfg.rang_override_mode )
    {
        dynLdBlcCfg.rang_override_mode = DefaultRangOverrideMode;
        SetDynamicLoadRangeOverrideMode( &dynLdBlcCfg.rang_override_mode  );
    }

    if ( DefaultDCCInitATDMAMode != dynLdBlcCfg.atdma_dcc_init_tech )
    {
        dynLdBlcCfg.atdma_dcc_init_tech = DefaultDCCInitATDMAMode;
        SetDynamicLoadAtdmaDccInitTechMode( &dynLdBlcCfg.atdma_dcc_init_tech  );
    }

    if ( DefaultDCCInitSCDMAMode != dynLdBlcCfg.scdma_dcc_init_tech )
    {
        dynLdBlcCfg.scdma_dcc_init_tech = DefaultDCCInitSCDMAMode;
        SetDynamicLoadScdmaDccInitTechMode( &dynLdBlcCfg.scdma_dcc_init_tech  );
    }

    if ( DefaultDBCInitATDMAMode != dynLdBlcCfg.atdma_dbc_init_tech )
    {
        dynLdBlcCfg.atdma_dbc_init_tech = DefaultDBCInitATDMAMode;
        SetDynamicLoadAtdmaDbcInitTechMode( &dynLdBlcCfg.atdma_dbc_init_tech  );
    }

    if ( DefaultDBCInitSCDMAMode != dynLdBlcCfg.scdma_dbc_init_tech )
    {
        dynLdBlcCfg.scdma_dbc_init_tech = DefaultDBCInitSCDMAMode;
        SetDynamicLoadScdmaDbcInitTechMode( &dynLdBlcCfg.scdma_dbc_init_tech  );
    }
}

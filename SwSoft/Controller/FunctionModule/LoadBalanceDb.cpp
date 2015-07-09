#include "SystemInclude.h"
#include "BcmController.h"

#include "Debug.h"
#include "Util.h"
#include "LoadBalanceDb.h"

using namespace std;
CMCOAM_GetLdBlcDynCfgRspT  LdBlcDynCfg;

//LoadBalanceGroup::LoadBalanceGroup(uint8_t theGroupId) : groupId(theGroupId)
// {
//    type = STATIC_LD_BLC;
//    dsChannelListNum = 0;
//    usChannelListNum = 0;
//    macRangeNum = 0;
// }
LoadBalanceGroup::LoadBalanceGroup(uint8_t theGroupId, CMCOAM_LoadBlcMethodT type,
                                   uint8_t* dsChannelList, uint8_t dsChannelListNum,
                                   uint8_t* usChannelList, uint8_t usChannelListNum,
                                   CMCOAM_CnuMacRangeT* macRange,
                                   uint8_t macRangeNum) : groupId(theGroupId)
 {
    UpdateType(type);
    UpdateChannel(dsChannelList, dsChannelListNum, usChannelList, usChannelListNum);
    UpdateCnuMacRange(macRange, macRangeNum);
 }

LoadBalanceGroup::LoadBalanceGroup(LoadBalanceGroup const& group) : groupId (group.groupId)
{
    UpdateType(group.type);
    UpdateChannel(group.dsChannelList, group.dsChannelListNum,
                  group.usChannelList, group.usChannelListNum);
    UpdateCnuMacRange(group.macRanges);
}

LoadBalanceGroup::~LoadBalanceGroup()
{
}

void LoadBalanceGroup::UpdateType(CMCOAM_LoadBlcMethodT newType)
{
    type = newType;
}

void LoadBalanceGroup::UpdateChannel(uint8_t const* dsChList, uint8_t dsChNumber,
                                     uint8_t const* usChList, uint8_t usChNumber)
{
    UpdateDsChannel(dsChList, dsChNumber);
    UpdateUsChannel(usChList, usChNumber);
}

void LoadBalanceGroup::UpdateDsChannel(uint8_t const* channelList, uint8_t channelNumber)
{
    assert(dsChannelList != channelList);

    memcpy(dsChannelList, channelList, sizeof(uint8_t) * channelNumber);
    dsChannelListNum = channelNumber;
}

void LoadBalanceGroup::UpdateUsChannel(uint8_t const* channelList, uint8_t channelNumber)
{
    assert(usChannelList != channelList);

    memcpy(usChannelList, channelList, sizeof(uint8_t) * channelNumber);
    usChannelListNum = channelNumber;
}

void LoadBalanceGroup::AddDsChannel(uint8_t channelId)
{
    uint8_t* iter = find(dsChannelList, dsChannelList + dsChannelListNum, channelId);

    if (iter == dsChannelList + dsChannelListNum && dsChannelListNum < BRCM_CMC_MAX_DS_CH)
    {
        dsChannelList[dsChannelListNum] = channelId;
        ++dsChannelListNum;
    }
}

void LoadBalanceGroup::RemoveDsChannel(uint8_t channelId)
{
    uint8_t* iter = remove(dsChannelList, dsChannelList + dsChannelListNum, channelId);
    if (iter != dsChannelList + dsChannelListNum)
    {
        --dsChannelListNum;
    }
}

void LoadBalanceGroup::AddUsChannel(uint8_t channelId)
{
    uint8_t* iter = find(usChannelList, usChannelList + usChannelListNum, channelId);

    if (iter == usChannelList + usChannelListNum && usChannelListNum < BRCM_CMC_MAX_DS_CH)
    {
        usChannelList[usChannelListNum] = channelId;
        ++usChannelListNum;
    }
}

void LoadBalanceGroup::RemoveUsChannel(uint8_t channelId)
{
    uint8_t* iter = remove(usChannelList, usChannelList + usChannelListNum, channelId);
    if (iter != usChannelList + usChannelListNum)
    {
        --usChannelListNum;
    }
}

void LoadBalanceGroup::GetDsChannel(uint8_t* channelList, size_t length,
                                    uint8_t* channelNumber)
{
    size_t len;
    len = length < dsChannelListNum ? length : dsChannelListNum;
    memcpy(channelList, dsChannelList, sizeof(uint8_t) * len);
    *channelNumber = dsChannelListNum;
}

void LoadBalanceGroup::GetUsChannel(uint8_t* channelList, size_t length,
                                    uint8_t* channelNumber)
{
    size_t len;
    len = length < usChannelListNum ? length : usChannelListNum;
    memcpy(channelList, usChannelList, sizeof(uint8_t) * len);
    *channelNumber = usChannelListNum;
}

void LoadBalanceGroup::UpdateCnuMacRange(CMCOAM_CnuMacRangeT* macRange, uint8_t macRangeNum)
{
    macRanges.clear();

    uint8_t i;
    for (i = 0; i < macRangeNum; ++i)
    {
        macRanges.push_back(*(macRange+i));
    }
}

void LoadBalanceGroup::UpdateCnuMacRange(list<CMCOAM_CnuMacRangeT> cnuMacRanges)
{
    macRanges = cnuMacRanges;
}

bool LoadBalanceGroup::IsCnuMacRangeExisted(CMCOAM_CnuMacRangeT const& range)
{
    CnuMacRangeIterator iter;
    iter = find_if(macRanges.begin(), macRanges.end(), bind2nd(CmpMacRange(),range));

    return (iter != macRanges.end());
}

void LoadBalanceGroup::AddCnuMacRange(CMCOAM_CnuMacRangeT const& range)
{
    macRanges.push_back(range);
}

void LoadBalanceGroup::RemoveCnuMacRange(CMCOAM_CnuMacRangeT const& range)
{
    macRanges.remove_if(bind2nd(CmpMacRange(),range));
}

int LoadBalanceGroup::Compare(LoadBalanceGroup const& right) const
{
    return groupId - right.groupId;
}

uint8_t LoadBalanceGroup::GetGroupId() const
{
    return groupId;
}

void LoadBalanceGroup::Put(ostream& os) const
{
    os << setw(8) << (int)groupId
        << setw(8) << (type == STATIC_LD_BLC ? "static" : "dynamic")
        << setw(16) << ArrayToString(dsChannelList, dsChannelListNum)
        << setw(16) << ArrayToString(usChannelList, usChannelListNum)
        << setw(10) << macRanges.size();
    os  << endl;

    list<CMCOAM_CnuMacRangeT>::const_iterator iter;
    for (iter = macRanges.begin(); iter != macRanges.end(); ++iter)
    {
        os << setw(39) << MacToString(iter->start)
            << ", " << MacToString(iter->end)
            << endl;
    }
};

void LoadBalanceGroup::PutTitle(ostream& os)
{
    os << setw(8) << "Grp ID"
        << setw(8) << "Type"
        << setw(16) << "Ds_Channel"
        << setw(16) << "Us_Channel"
        << setw(10) << "Mac_Num" << endl;
}

/**********************************************************************/
LoadBalanceCmc::LoadBalanceCmc(uint8_t const* theCmcMac)
{
    CopyCmcAddr(cmcMac, theCmcMac);
}

LoadBalanceCmc::~LoadBalanceCmc()
{
}

LoadBalanceCmc::InsertRet LoadBalanceCmc::Insert(LoadBalanceGroup const& group)
{
    return groups.insert(IdGroupPair(group.GetGroupId(), group));
}

LoadBalanceCmc::InsertRet
LoadBalanceCmc::Insert(uint8_t groupId, CMCOAM_LoadBlcMethodT type,
                          uint8_t* dsChannelList, uint8_t dsChannelListNum,
                          uint8_t* usChannelList, uint8_t usChannelListNum,
                          CMCOAM_CnuMacRangeT* macRange,
                          uint8_t macRangeNum)
{
    LoadBalanceGroup group(groupId, type, dsChannelList, dsChannelListNum,
                           usChannelList, usChannelListNum,
                           macRange, macRangeNum);

    return Insert(group);
}

LoadBalanceCmc::CnuRangeItorator LoadBalanceCmc::CnuMacExcludeRangeBegin()
{
    return macExcludeRanges.begin();
}

LoadBalanceCmc::CnuRangeItorator LoadBalanceCmc::CnuMacExcludeRangeEnd()
{
    return macExcludeRanges.end();
}

LoadBalanceCmc::CnuRangeItorator LoadBalanceCmc::FindMacExcludeRange(size_t index)
{
    if (index < macExcludeRanges.size())
    {
        return macExcludeRanges.begin() + index;
    }

    return macExcludeRanges.end();
}

size_t LoadBalanceCmc::GetCnuMacExcludeRangeSize()
{
    return macExcludeRanges.size();
}

void LoadBalanceCmc::UpdateCnuMacExcludeRange(CMCOAM_CnuMacRangeT* macRange,
                                               uint8_t macRangeNum)
{
    macExcludeRanges.clear();
    uint8_t i;
    for (i = 0; i < macRangeNum; ++i)
    {
        macExcludeRanges.push_back(*(macRange+i));
    }
}

//void LoadBalanceCmc::UpdateCnuMacExcludeRange(vector<CMCOAM_CnuMacRangeT> cnuMacRanges)
//{
//    macExcludeRanges = cnuMacRanges;
//}

void LoadBalanceCmc::AddCnuMacExcludeRange(CMCOAM_CnuMacRangeT const& range)
{
    macExcludeRanges.push_back(range);
}

void LoadBalanceCmc::RemoveCnuMacExcludeRange(CMCOAM_CnuMacRangeT const& range)
{
    CnuRangeItorator iter;
    iter = remove_if(macExcludeRanges.begin(), macExcludeRanges.end(),
                     bind2nd(CmpMacRange(),range));
    macExcludeRanges.erase(iter, macExcludeRanges.end());
}

LoadBalanceCmc::Iterator LoadBalanceCmc::Begin()
{
    return groups.begin();
}

LoadBalanceCmc::Iterator LoadBalanceCmc::End()
{
    return groups.end();
}

LoadBalanceCmc::Iterator LoadBalanceCmc::Find(uint8_t groupId)
{
    return groups.find(groupId);
}

size_t LoadBalanceCmc::Erase(uint8_t groupId)
{
    return groups.erase(groupId);
}

int LoadBalanceCmc::Compare(LoadBalanceCmc const& right) const
{
    return CompareCmcAddr(cmcMac, right.cmcMac);
}

void LoadBalanceCmc::Put(ostream& os) const
{
    os << "Cmc addr is:" << CmcAddrToString(cmcMac) << endl;
    os << "CnuMac exclude ranges:" << endl;

    vector<CMCOAM_CnuMacRangeT>::const_iterator iter;
    for (iter = macExcludeRanges.begin(); iter != macExcludeRanges.end(); ++iter)
    {
        os << string("  ") << MacToString(iter->start)
            << ", " << MacToString(iter->end) << endl;
    }

    cout << "LoadBalanceGroup list:" << endl;
    LoadBalanceGroup::PutTitle(cout);
    map<uint8_t, LoadBalanceGroup>::const_iterator group;
    for (group = groups.begin(); group != groups.end(); ++group)
    {
        os << group->second;
    }
    os << endl;

};

/**********************************************************************/
LoadBalanceCmcs::LoadBalanceCmcs()
{
}

LoadBalanceCmcs& LoadBalanceCmcs::GetInstance()
{
    static LoadBalanceCmcs instance;
    return instance;
}

void LoadBalanceCmcs::PushBack(LoadBalanceCmc const& groups)
{
    return cmcList.push_back(groups);
}

LoadBalanceCmcs::Iterator LoadBalanceCmcs::Begin()
{
    return cmcList.begin();
}

LoadBalanceCmcs::Iterator LoadBalanceCmcs::End()
{
    return cmcList.end();
}

LoadBalanceCmcs::Iterator LoadBalanceCmcs::Find(uint8_t const* cmcMac)
{
    Iterator iter;
    iter = find(cmcList.begin(), cmcList.end(), LoadBalanceCmc(cmcMac));

    return iter;
}

/**********************************************************************/
/**********************************************************************/
LoadBalanceGroup* FindLoadBalanceGroup(uint8_t groupId, uint8_t const* cmcMac)
{
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);
    if (cmc == cmcs.End())
    {
        return NULL;
    }

    LoadBalanceCmc::Iterator group = cmc->Find(groupId);
    if (group == cmc->End())
    {
        return NULL;
    }

    return &(group->second);
}

LoadBalanceGroup* CreateLoadBalanceGroup(uint8_t groupId, CMCOAM_LoadBlcMethodT type,
                       uint8_t* dsChannelList, uint8_t dsChannelListNum,
                       uint8_t* usChannelList, uint8_t usChannelListNum,
                       CMCOAM_CnuMacRangeT* macRange, uint8_t macRangeNum,
                       uint8_t* cmcMac)
{
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc = cmcs.Find((uint8_t*)cmcMac);

    if (cmc == cmcs.End())
    {
        LoadBalanceCmc loadBalanceCmc((uint8_t *)cmcMac);
        cmcs.PushBack(loadBalanceCmc);
        cmc = cmcs.Find((uint8_t*)cmcMac);
    }

    cmc->Insert(groupId, type, dsChannelList, dsChannelListNum,
                usChannelList, usChannelListNum,
                macRange, macRangeNum);

    LoadBalanceCmc::Iterator group = cmc->Find(groupId);
    return &(group->second);
}

static void PrintAllLoadBalanceGroup()
{
    LoadBalanceCmcs& cmcs = LoadBalanceCmcs::GetInstance();
    LoadBalanceCmcs::Iterator cmc;
    for (cmc = cmcs.Begin(); cmc != cmcs.End(); ++cmc)
    {
        cout << *cmc;
    }
}

void UnitTestLoadBalanceDbShow()
{
    PrintAllLoadBalanceGroup();
}

BASE_ErrCode 
DBSetDynamicLoadBalance( CMCOAM_GetLdBlcDynCfgRspT * dynLdBlcCfg)
{
    memcpy( &LdBlcDynCfg, dynLdBlcCfg, sizeof(CMCOAM_GetLdBlcDynCfgRspT) );

    return BASE_OK;
}

BASE_ErrCode 
DBGetDynamicLoadBalance( CMCOAM_GetLdBlcDynCfgRspT * dynLdBlcCfg )
{
    memcpy( dynLdBlcCfg, &LdBlcDynCfg, sizeof(CMCOAM_GetLdBlcDynCfgRspT) );

    return BASE_OK;
}




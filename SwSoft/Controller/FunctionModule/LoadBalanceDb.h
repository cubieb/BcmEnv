#ifndef _LoadBalanceDb_h_
#define _LoadBalanceDb_h_

struct CmpMacRange : public std::binary_function<CMCOAM_CnuMacRangeT, CMCOAM_CnuMacRangeT, bool>
{
    bool operator()(const first_argument_type& left, const second_argument_type& right) const
    {
        if (memcmp(left.start, right.start, 6) != 0)
            return false;
        if (memcmp(left.end, right.end, 6) != 0)
            return false;

        return true;
    }
};

class LoadBalanceGroup
{
public:
    typedef std::list<CMCOAM_CnuMacRangeT>::iterator CnuMacRangeIterator;

//    LoadBalanceGroup(uint8_t theGroupId);
    LoadBalanceGroup(uint8_t groupId, CMCOAM_LoadBlcMethodT type,
                     uint8_t* dsChannelList, uint8_t dsChannelListNum,
                     uint8_t* usChannelList, uint8_t usChannelListNum,
                     CMCOAM_CnuMacRangeT* macRange,
                     uint8_t macRangeNum);
    LoadBalanceGroup(LoadBalanceGroup const& group);
    ~LoadBalanceGroup();

    uint8_t GetId() { return groupId; }

    CMCOAM_LoadBlcMethodT GetType() { return type; }
    void UpdateType(CMCOAM_LoadBlcMethodT newType);

    void UpdateChannel(uint8_t const* dsChList, uint8_t dsChNumber,
                       uint8_t const* usChList, uint8_t usChNumber);
    void UpdateDsChannel(uint8_t const* channelList, uint8_t channelNumber);
    void UpdateUsChannel(uint8_t const* channelList, uint8_t channelNumber);
    void AddDsChannel(uint8_t channelId);
    void RemoveDsChannel(uint8_t channelId);
    void AddUsChannel(uint8_t channelId);
    void RemoveUsChannel(uint8_t channelId);
    void GetDsChannel(uint8_t* channelList, size_t length, uint8_t* channelNumber);
    void GetUsChannel(uint8_t* channelList, size_t length, uint8_t* channelNumber);

    CnuMacRangeIterator CnuMacRangeBegin() { return macRanges.begin();}
    CnuMacRangeIterator CnuMacRangeEnd() { return macRanges.end();}
    size_t GetCnuMacRangeSize() { return macRanges.size(); }
    void UpdateCnuMacRange(CMCOAM_CnuMacRangeT* macRange, uint8_t macRangeNum);
    void UpdateCnuMacRange(std::list<CMCOAM_CnuMacRangeT> cnuMacRanges);
    bool IsCnuMacRangeExisted(CMCOAM_CnuMacRangeT const& range);
    void AddCnuMacRange(CMCOAM_CnuMacRangeT const& range);
    void RemoveCnuMacRange(CMCOAM_CnuMacRangeT const& range);

    int Compare(LoadBalanceGroup const& right) const;
    uint8_t GetGroupId() const;
    void Put(std::ostream& os) const;
    static void PutTitle(std::ostream& os);

private:
    uint8_t groupId;
    CMCOAM_LoadBlcMethodT type;
    uint8_t dsChannelList[BRCM_CMC_MAX_DS_CH];
    uint8_t dsChannelListNum;
    uint8_t usChannelList[BRCM_CMC_MAX_US_CH];
    uint8_t usChannelListNum;
    std::list<CMCOAM_CnuMacRangeT> macRanges; //max node BRCM_CMC_MAX_MAC_RANG.
};

inline bool operator == (LoadBalanceGroup const& left, LoadBalanceGroup const& right)
{
    return left.Compare(right) == 0;
}
inline bool operator <= (LoadBalanceGroup const& left, LoadBalanceGroup const& right)
{
    return left.Compare(right) <= 0;
}
inline bool operator >= (LoadBalanceGroup const& left, LoadBalanceGroup const& right)
{
    return left.Compare(right) >= 0;
}
inline bool operator <  (LoadBalanceGroup const& left, LoadBalanceGroup const& right)
{
    return left.Compare(right) < 0;
}
inline bool operator > (LoadBalanceGroup const& left, LoadBalanceGroup const& right)
{
    return left.Compare(right) > 0;
}

inline std::ostream& operator << (std::ostream& os, LoadBalanceGroup const& group)
{
    group.Put(os);
    return os;
}

class LoadBalanceCmc
{
public:
    typedef std::map<uint8_t, LoadBalanceGroup>::iterator Iterator;
    typedef std::vector<CMCOAM_CnuMacRangeT>::iterator CnuRangeItorator;
    typedef std::pair<uint8_t, LoadBalanceGroup> IdGroupPair;
    typedef std::pair<Iterator, bool> InsertRet;

    LoadBalanceCmc(uint8_t const* theCmcMac);
    ~LoadBalanceCmc();
    InsertRet Insert(LoadBalanceGroup const& group);
    InsertRet Insert(uint8_t groupId, CMCOAM_LoadBlcMethodT type,
                     uint8_t* dsChannelList, uint8_t dsChannelListNum,
                     uint8_t* usChannelList, uint8_t usChannelListNum,
                     CMCOAM_CnuMacRangeT* macRange,
                     uint8_t macRangeNum);

    CnuRangeItorator CnuMacExcludeRangeBegin();
    CnuRangeItorator CnuMacExcludeRangeEnd();
    CnuRangeItorator FindMacExcludeRange(size_t);
    size_t GetCnuMacExcludeRangeSize();
    void UpdateCnuMacExcludeRange(CMCOAM_CnuMacRangeT* macRange, uint8_t macRangeNum);
    //void UpdateCnuMacExcludeRange(vector<CMCOAM_CnuMacRangeT> cnuMacRanges);
    void AddCnuMacExcludeRange(CMCOAM_CnuMacRangeT const& range);
    void RemoveCnuMacExcludeRange(CMCOAM_CnuMacRangeT const& range);

    Iterator Begin();
    Iterator End();
    Iterator Find(uint8_t groupId);
    size_t Erase(uint8_t groupId);
    int Compare(LoadBalanceCmc const& right) const;

    uint8_t* GetCmcMac() { return cmcMac; }
    void Put(std::ostream& os) const;

private:
    uint8_t cmcMac[CmcAddrLength];
    std::map<uint8_t, LoadBalanceGroup> groups;
    std::vector<CMCOAM_CnuMacRangeT> macExcludeRanges; /* shared configuration by all groups */
};

inline bool operator == (LoadBalanceCmc const& left, LoadBalanceCmc const& right)
{
    return left.Compare(right) == 0;
}
inline bool operator <= (LoadBalanceCmc const& left, LoadBalanceCmc const& right)
{
    return left.Compare(right) <= 0;
}
inline bool operator >= (LoadBalanceCmc const& left, LoadBalanceCmc const& right)
{
    return left.Compare(right) >= 0;
}
inline bool operator <  (LoadBalanceCmc const& left, LoadBalanceCmc const& right)
{
    return left.Compare(right) < 0;
}
inline bool operator > (LoadBalanceCmc const& left, LoadBalanceCmc const& right)
{
    return left.Compare(right) > 0;
}

inline std::ostream& operator << (std::ostream& os, LoadBalanceCmc const& cmc)
{
    cmc.Put(os);
    return os;
}

class LoadBalanceCmcs
{
public:
    typedef std::list<LoadBalanceCmc>::iterator Iterator;

    static LoadBalanceCmcs& GetInstance();
    void PushBack(LoadBalanceCmc const& groups);
    Iterator Begin();
    Iterator End();
    Iterator Find(uint8_t const* cmcMac);
    void Clear() { cmcList.clear(); }

private:
    std::list<LoadBalanceCmc> cmcList;
    LoadBalanceCmcs();
};

/**********************************************************************/
/**********************************************************************/
LoadBalanceGroup* FindLoadBalanceGroup(uint8_t groupId, uint8_t const* cmcMac);

LoadBalanceGroup* CreateLoadBalanceGroup(uint8_t groupId, CMCOAM_LoadBlcMethodT type,
                       uint8_t* dsChannelList, uint8_t dsChannelListNum,
                       uint8_t* usChannelList, uint8_t usChannelListNum,
                       CMCOAM_CnuMacRangeT* macRange, uint8_t macRangeNum,
                       uint8_t* cmcMac);

void UnitTestLoadBalanceDbShow();

BASE_ErrCode DBSetDynamicLoadBalance( CMCOAM_GetLdBlcDynCfgRspT * dynLdBlcCfg);
BASE_ErrCode DBGetDynamicLoadBalance( CMCOAM_GetLdBlcDynCfgRspT * dynLdBlcCfg );

#endif


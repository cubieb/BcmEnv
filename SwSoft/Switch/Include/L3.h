#ifndef _L3_h_
#define _L3_h_

#include "Util.h"

/*
class L3Rx : public Rx
{
public:
    static L3Rx& GetInstance();
    bool Check(int unit, bcm_pkt_t *pkt);

private:
    L3Rx() {}
};
*/

bcm_rx_t L3ArpRxCallback(int unit, bcm_pkt_t* pkt);

struct RouteInfor
{
    bcm_if_t intf;
    bcm_ip_t ip;   /* key */
    uint32_t ttl;  /* time to life in seconds. */

    RouteInfor() : intf(0), ip (0), ttl(0) {}
    RouteInfor(bcm_if_t theIntf, bcm_ip_t theIp, uint32_t theTtl)
        : intf(theIntf), ip (theIp), ttl(theTtl) {}
};

inline bool operator == (RouteInfor const& left, RouteInfor const& right)
{
    return (left.ip == right.ip);
}

inline std::ostream& operator << (std::ostream& s, RouteInfor const& object)
{
    s << "intf = " << object.intf
        << ", ip = " << InetToString(object.ip)
        << ", ttl = " << object.ttl;
    return s;
}

class ArpPacket;
class L3HostRoute
{
public:
    static L3HostRoute& GetInstance();
    void ProcessRxArp(bcm_vlan_t vid, bcm_port_t port, ArpPacket& pkt);
    void OnSecondTick();

private:
    L3HostRoute() : TimeToLiveSeconds(3600)
    {}

    const uint32_t TimeToLiveSeconds;
    std::list<RouteInfor> routes;
};

#endif /* _L2_h_ */

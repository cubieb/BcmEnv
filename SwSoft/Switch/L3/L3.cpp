#include "SystemInclude.h"

#include "BcmSdk.h"
#include "Debug.h"
#include "Util.h"
#include "DriverRoute.h"
#include "DriverRx.h"
#include "L2.h"
#include "L3.h"

using namespace std;
/*
L3Rx& L3Rx::GetInstance()
{
    static L3Rx l3Rx;
    return l3Rx;
}

bool L3Rx::Check(int unit, bcm_pkt_t *pkt)
{
    return true;
}
*/

bcm_rx_t L3ArpRxCallback(int unit, bcm_pkt_t* pkt)
{
    L3HostRoute& route = L3HostRoute::GetInstance();

    ArpPacket arpPkt(GetEthNextHeader(pkt));
    route.ProcessRxArp(pkt->vlan, pkt->rx_port, arpPkt);

    return BCM_RX_HANDLED;
}

L3HostRoute& L3HostRoute::GetInstance()
{
    static L3HostRoute route;
    return route;
}

void L3HostRoute::ProcessRxArp(bcm_vlan_t vid, bcm_port_t port, ArpPacket& pkt)
{
    int unit = 0;

//    dbgstrm << "Process incoming arp packet, "
//            << "vid = " << vid << ", port = " << port
//            << ", Sender Ip = " << InetToString(pkt.GetSenderIp())
//            << ", Sender Mac = " << MacToString(pkt.GetSenderMac())
//            << endl;
    if (pkt.GetSenderIp() == 0)
    {
        /* Dhcp client may send arp packet to check if a IP is used
         * by others, at this condition the sender IP is 0 */
        return;
    }

    list<RouteInfor>::iterator iter;
    iter = find(routes.begin(),
                routes.end(),
                RouteInfor(0, pkt.GetSenderIp(), 0));
    if (iter != routes.end())
    {
        //dbgstrm << "Update timer." << (*iter) << endl;
        iter->ttl = TimeToLiveSeconds;
        return;
    }

    bcm_l3_intf_t intf;
    bcm_l3_intf_t_init(&intf);
    intf.l3a_vid = vid;
    if (bcm_l3_intf_find_vlan(unit, &intf) != BCM_E_NONE)
    {
        errstrm << "can not find interface id. " << endl;
        return ;
    }

    HostRoute hostRoute;
    bool rv;
    rv = hostRoute.Add(intf.l3a_intf_id, port,
                       pkt.GetSenderIp(), pkt.GetSenderMac());
    if (!rv)
    {
        errstrm << "Can't add route, intf_id = " << intf.l3a_intf_id << endl;
    }

    routes.push_back(RouteInfor(intf.l3a_intf_id, pkt.GetSenderIp(), TimeToLiveSeconds));
    return ;
}

void L3HostRoute::OnSecondTick()
{
    list<RouteInfor>::iterator iter;
    for (iter = routes.begin(); iter != routes.end(); ++iter)
    {
        iter->ttl--;
        if (iter->ttl == 0)
        {
            HostRoute hostRoute;
            hostRoute.Remove(iter->ip);
            iter = routes.erase(iter);
        }
    }
}



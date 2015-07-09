#include "SystemInclude.h"
#include "BcmController.h"

#include <net/if_arp.h>
#include <string.h>
#include "Debug.h"
#include "Util.h"
#include "DhcpRelay.h"

#include "CnuStatusMgt.h"
#include "CnuHostStatusMgt.h"

using namespace std;

TerminalTypeNamePair trTypeName[] =
{
    {TerminalTypeCm,   "docsis"},
    {TerminalTypeMta,  "pktc"},
    {TerminalTypeStb,  "stb"},
    {TerminalTypeHost, "MSFT 5.0"},
};

VlanCmInfo::VlanCmInfo(VlanId vid, struct in_addr cmIp, const uint8_t* cmMac)
{
    this->vid = vid;
    this->cmIp.s_addr = cmIp.s_addr;
    memcpy(this->cmMac, cmMac, 6);
}

VlanId VlanCmInfo::GetVid()
{
    return vid;
}

uint32_t VlanCmInfo::GetCmIp()
{
    return cmIp.s_addr;
}

const uint8_t* VlanCmInfo::GetCmMac()
{
    return cmMac;
}

void VlanCmInfo::Put(ostream& os) const
{
    os << "Vid = " << (int)vid
        << ", CmIp = " << InetToString(cmIp.s_addr)
        << ", Mac = " << MacToString(cmMac);
    os << endl;
}

void DhcpRelay::ClearServerIp()
{
    servers.clear();
}

void DhcpRelay::AddServerIp(struct in_addr server)
{
    servers.push_back(server);
}

bool DhcpRelay::CreateSocket()
 {
     socketFd = socket(PF_INET, SOCK_DGRAM, 0);
     if (socketFd == -1)
     {
         errstrm << "DhcpRelay::CreateSocket()> can not create socket." << endl;
         return false;
     }

     int socketOpt = 1;
     int mtuOpt = IP_PMTUDISC_DONT;
     if (setsockopt(socketFd, SOL_IP, IP_PKTINFO, &socketOpt, sizeof(socketOpt)) != 0)
     {
         errstrm << "can not setsockopt." << endl;
         goto ErrHandle;
     }
     if (setsockopt(socketFd, SOL_SOCKET, SO_BROADCAST, &socketOpt, sizeof(socketOpt)) != 0)
     {
         errstrm << "can not setsockopt." << endl;
         goto ErrHandle;
     }
     if (setsockopt(socketFd, SOL_IP, IP_MTU_DISCOVER, &mtuOpt, sizeof(mtuOpt)) != 0)
     {
         errstrm << "can not setsockopt." << endl;
         goto ErrHandle;
     }

     return true;

ErrHandle:
     CloseSocket();
     return false;
 }

 void DhcpRelay::CloseSocket()
 {
     if (socketFd != -1)
     {
         close(socketFd);
         socketFd = -1;
     }
 }

bool DhcpRelay::BindSocket()
{
    struct   sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(DhcpServerPort);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socketFd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) != 0)
    {
        errstrm << "cannot bind DHCP server socket." << std::endl;
        return false;
    }

    return true;
}

ssize_t DhcpRelay::ReceiveMsg(struct msghdr& msg)
{
    ssize_t msgSize;
    while ((msgSize = recvmsg(socketFd, &msg, 0)) == -1 && errno == EINTR);

    if ((msg.msg_flags & MSG_TRUNC) || msgSize < (ssize_t)(sizeof(DhcpPkt)))
    {
        errstrm << "received a bad packet." << endl;
        return -1;
    }
    if (msg.msg_controllen < sizeof(struct cmsghdr))
    {
        errstrm << "system error, can get message control information." << endl;
        return -1;
    }
    msg.msg_iov->iov_len = msgSize;

    return msgSize;
}

/*
 * Refer to ip (7) and RFC 3542 for more information about in_pktinfo.
IP_PKTINFO (since Linux 2.2)
Pass an IP_PKTINFO ancillary message that contains a  pktinfo  structure  that  supplies  some
information  about  the  incoming packet.  This only works for datagram oriented sockets.  The
argument is a flag that tells the socket whether the IP_PKTINFO message should  be  passed  or
not.   The  message  itself  can only be sent/retrieved as control message with a packet using
recvmsg(2) or sendmsg(2).

  struct in_pktinfo {
      unsigned int   ipi_ifindex;  // Interface index
      struct in_addr ipi_spec_dst; // Local address
      struct in_addr ipi_addr;     // Header Destination address
  };

ipi_ifindex is the unique index of the interface the packet was received on.  ipi_spec_dst  is
the  local address of the packet and ipi_addr is the destination address in the packet header.
If IP_PKTINFO is passed to sendmsg(2) and ipi_spec_dst is not zero, then it  is  used  as  the
local  source address for the routing table lookup and for setting up IP source route options.
When ipi_ifindex is not zero, the primary local address of  the  interface  specified  by  the
index overwrites ipi_spec_dst for the routing table lookup.
 */
bool DhcpRelay::GetIngressInterace(struct msghdr& msg, Interface& intf)
{
    struct cmsghdr    *cmptr;
    intf.index = 0;
    for (cmptr = CMSG_FIRSTHDR(&msg); cmptr != NULL; cmptr = CMSG_NXTHDR(&msg, cmptr))
    {
        if (cmptr->cmsg_level == SOL_IP && cmptr->cmsg_type == IP_PKTINFO)
        {
            intf.index = ((struct in_pktinfo *) CMSG_DATA(cmptr))->ipi_ifindex;
            break;
        }
    }

    if (intf.index == 0)
    {
        errstrm << "can not get interface information. " << endl;
        return false;
    }

    /* 1,  ioctl can get Interface Name by index.
     * 2,  ioctl can only get address by Interface Name,
     *     and because of union {index, address}, if we need both
     *     address information and interface name, we must call ioctl
     *     twice.
     */
    struct ifreq ifr;
    ifr.ifr_ifindex = intf.index;
    if (ioctl(socketFd, SIOCGIFNAME, &ifr) == -1)
    {
        errstrm << "cant ioctl(SIOCGIFNAME)"
            << ", ifr.ifr_ifindex = " << ifr.ifr_ifindex
            << endl;
        return false;
    }
    intf.name = ifr.ifr_name;

    ifr.ifr_addr.sa_family = AF_INET;
    if (ioctl(socketFd, SIOCGIFADDR, &ifr) == -1)
    {
        errstrm << "can not ioctl. " << strerror(errno)
            << ", ifr.ifr_ifindex = " << ifr.ifr_name
            << endl;
        return false;
    }
    intf.addr = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr;

    return true;
}

VlanId DhcpRelay::GetVlanId(Interface& intf)
{
    VlanId vlan;
    vlan = (VlanId)atoi(intf.name.c_str() + strlen("vlan"));

    return vlan;
}

uint8_t DhcpRelay::GetDhcpOption(const DhcpPktWithOpts* packet,
                                 uint8_t code,
                                 char* value,
                                 uint8_t valueLen)
{
    assert(packet != NULL && value != NULL);

    const uint8_t* options = packet->options;
    //traverse options and find the pointer of end option code(255)
    uint8_t endCode = 0xff;
    uint8_t len;
    while ((options < (uint8_t*)(packet + 1)) && ( *options != endCode))
    {
        if (*options == code)
            break;

        len = *(options + 1);         //just the length of option information
        options = options + 2 + len;
    }

    if (*options == code)
    {
        len = *(options + 1);
        memcpy(value, options+2, len);
        return len;
    }

    return 0;
}

TerminalType DhcpRelay::GetTerminalType(const DhcpPktWithOpts* packet)
{
    char option60[256] = {0};
    TerminalType trType = DefaultTerminalType;

    /* option 60 is included only in Discover, Request packet.  Not included.
     * in Offer Ack packet.
     */
    if (GetDhcpOption(packet, 60, option60, sizeof(option60)) != 0)
    {
        for (size_t i = 0; i < sizeof(trTypeName) / sizeof(TerminalTypeNamePair); ++i)
        {
            if (strncmp(trTypeName[i].name, option60, strlen(trTypeName[i].name)) == 0)
            {
                trType = trTypeName[i].type;
                dbgstrm << "Terminal type is " << trTypeName[i].name << endl;
                break;
            }
        }
    }
    else
    {
        VlanId vid = GetVlanId(packet);
        list<VlanCmInfo>::iterator iter;
        for (iter = vlanCmlist.begin(); iter != vlanCmlist.end(); ++iter)
        {
            if (vid == iter->GetVid()
                && memcmp(iter->GetCmMac(), packet->dhcpPkt.chaddr, 6) == 0)
            {
                trType = TerminalTypeCm;
            }
        }
    }

    return trType;
}

VlanId DhcpRelay::GetVlanId(const DhcpPktWithOpts* packet)
{
    VlanId vid;

    Option82 option82;
    option82.code = 82;
    option82.len = GetDhcpOption(packet, 82, (char*)&option82 + 2, sizeof(Option82) - 2);
    assert(option82.len != 0);

    char vlan[8] = {0};
    memcpy(vlan, option82.agentCircuitId, option82.sub1Len);
    vid = (VlanId)atoi(vlan);

    return vid;
}

void DhcpRelay::AddOption82(struct msghdr& msg)
{
    dbgstrm << "Start" << endl;

    Interface interface;
    uint8_t endCode = 0xff;
    bool rv = GetIngressInterace(msg, interface);
    assert(rv);

    DhcpPktWithOpts* packet = (DhcpPktWithOpts*)packetBuf;
    uint8_t* options = packet->options;

    //traverse options and find the pointer of end option code(255)
    uint8_t len;
    while ((options < (uint8_t*)(packet + 1)) && ( *options != endCode))
    {
        len = *(options + 1);         //just the length of option information
        options = options + 2 + len;
    }
    assert(options + sizeof(endCode) + sizeof(Option82) < (uint8_t*)(packet + 1));

    ostringstream strVlan;
    strVlan << GetVlanId(interface);
    Option82* option82 = (Option82*)options;
    option82->code = 82;
    option82->len = sizeof(struct Option82) - 2; /* code and len are not included */

    /* sub option 1 "Agent Circuit ID */
    option82->sub1Code = 1;
    option82->sub1Len = 4;
    /* in order to simplify the codes, we define option82->vlan as char[4],
     * that require the vlan id must is in the range of 1000~4094.
     * maybe new function specification will voilate this limitation, that
     * will cause assert() failed,  at that time, we modify codes.
     */
    assert(strVlan.str().length() == 4);
    strcpy(option82->agentCircuitId, strVlan.str().c_str());

    /* sub option 1 "Agent Remote ID */
    option82->sub2code = 2;
    option82->sub2Len = 4;

    ostringstream strCmcId;
    uint32_t ip = packet->dhcpPkt.giaddr.s_addr;
    strCmcId << setw(4) << setfill('0') << (int)((ip / (256 * 256)) % 256);
    strcpy(option82->agentRemoteId, strCmcId.str().c_str());

    /* end code 0xff. */
    options = options + sizeof(struct Option82);
    *options = endCode;

    msg.msg_iov->iov_len += sizeof(struct Option82);
}

void DhcpRelay::ProcessRequest(struct msghdr& msg)
{
    std::list<string>::iterator iter;
    Interface interface;

    if (!GetIngressInterace(msg, interface))
    {
        return;
    }

    VlanId vid = GetVlanId(interface);
    if (vid < 2049 || vid > 2299)
    {
        return;
    }

    /* We don't forward packet which are come from "forwarding to" network. */
    iter = find(broadcastIntferces.begin(),
                broadcastIntferces.end(),
                interface.name);
    if (iter != broadcastIntferces.end( ))
        return;

    /* check if it came from an allowed interface */
    iter = find(excludeIntferces.begin(),
                excludeIntferces.end(),
                interface.name);
    if (iter != excludeIntferces.end( ))
        return;

    DhcpPkt* packet = (DhcpPkt*)packetBuf;
    /* already previous relay information. */
    if (packet->giaddr.s_addr != 0)
    {
        /* this packet is routed before, check if by us, to stop on loops. */
        std::list<Interface>::iterator iter;
        for (iter = interfaces.begin(); iter != interfaces.end(); ++iter)
        {
            if (iter->addr.s_addr == packet->giaddr.s_addr)
                return;
        }
    }
    else
    {
        /* plug in our address */
        packet->giaddr = interface.addr;
    }

    /* add option-82 */
    AddOption82(msg);

    /* send to all configured servers. */
    std::list<struct in_addr>::iterator srv;
    for (srv = servers.begin(); srv != servers.end(); ++srv)
    {
        struct sockaddr_in saddr;
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = srv->s_addr;
        saddr.sin_port = htons(DhcpServerPort);

        int ret;
        ret = sendto(socketFd, packet, msg.msg_iov->iov_len, 0,
                     (struct sockaddr *)&saddr, sizeof(saddr));
        if (ret == -1)
        {
            errstrm << "can not sendto, " << strerror(errno) << endl;
        }
    }

    /* build address->interface index table for returning answers */
    std::list<Interface>::iterator intf;
    for (intf =interfaces.begin(); intf != interfaces.end(); ++intf)
    {
        if (intf->addr.s_addr == interface.addr.s_addr)
        {
            break;
        }
    }

    /* not there, add a new entry */
    if (intf == interfaces.end())
    {
        dbgstrm << "add new interface"
            << ", addr = " << string(inet_ntoa(interface.addr))
            << ", index= " << interface.index << endl;
        interfaces.push_back(interface);
    }

    DhcpPktWithOpts* pktWithOpt = (DhcpPktWithOpts*)packetBuf;
    UpdateCpeInfoTable(pktWithOpt);
}

/* dsVid:  downstream vid */
void DhcpRelay::UpdateCpeInfoTable(const DhcpPktWithOpts* packet)
{
    uint8_t len;

    char type;
    len = GetDhcpOption(packet, 53, &type, sizeof(uint8_t));
    assert(len == 1);

    VlanCmInfo vlanCm(GetVlanId(packet), packet->dhcpPkt.yiaddr, packet->dhcpPkt.chaddr);
    /* trType is included only in Discover, Request packet. */
    int trType = GetTerminalType(packet);
    dbgstrm << "type=" << (int)type << ", trType = " << trType << endl;

    list<VlanCmInfo>::iterator iter;
    if (type == DhcpDiscover)
    {
        if (trType == TerminalTypeCm)
        {
            vlanCmlist.remove(vlanCm);
            vlanCmlist.push_back(vlanCm);
        }
    }
    else if (type == DhcpAck)
    {
        switch (trType)
        {
        case TerminalTypeCm:
            /* Client is a CM. */
            vlanCmlist.remove(vlanCm);
            vlanCmlist.push_back(vlanCm);

            /*Updata mac to ip map*/
            MacAddressT        temp_mac;
            memcpy(temp_mac.addr,packet->dhcpPkt.chaddr, 6);
            AddMacToIpMapMgt(temp_mac, packet->dhcpPkt.yiaddr.s_addr);
            //DumpCnuMacToIpMapMgt();	

            dbgstrm << "Create vlanCm, " << vlanCm << endl;
            break;
        case TerminalTypeMta:
        case TerminalTypeStb:
        case TerminalTypeHost:
        case TerminalTypeWiFi:
        case TerminalTypeIsTb:
            iter = std::find(vlanCmlist.begin(), vlanCmlist.end(), vlanCm);
            if (iter != vlanCmlist.end())
            {
                MacAddressT        mac;
                memcpy(mac.addr, iter->GetCmMac(), 6);

                CMCOAM_CmHostInfoT host;
                host.ipv4_addr = packet->dhcpPkt.yiaddr.s_addr;
                memcpy(host.host_mac.addr, packet->dhcpPkt.chaddr, 6);
                host.cpe_type = (int32_t)trType;
                Update_Cm_HostMgt(mac, host);
                dbgstrm << "Create host, vid = " << (int)iter->GetVid()
                    << ", ip = " << InetToString(host.ipv4_addr)
                    << ", mac = " << MacToString(host.host_mac.addr)
                    << endl;
            }
            break;
        }
    }
    else if (type == DhcpRelease)
    {
        switch (trType)
        {
        case TerminalTypeCm:
            /* Clint is a CM. */
            vlanCmlist.remove(vlanCm);

             /*Remove mac to ip map*/
            MacAddressT        temp_mac;
            memcpy(temp_mac.addr,packet->dhcpPkt.chaddr, 6);
            DeleteMacToIpMapMgt(temp_mac);	           
            
            break;
        case TerminalTypeMta:
        case TerminalTypeStb:
        case TerminalTypeHost:
        case TerminalTypeWiFi:
        case TerminalTypeIsTb:
            iter = std::find(vlanCmlist.begin(), vlanCmlist.end(), vlanCm);
            if (iter != vlanCmlist.end())
            {
                MacAddressT        mac;
                memcpy(mac.addr, iter->GetCmMac(), 6);

                CMCOAM_CmHostInfoT host;
                host.ipv4_addr = packet->dhcpPkt.yiaddr.s_addr;
                memcpy(host.host_mac.addr, packet->dhcpPkt.chaddr, 6);
                host.cpe_type = (int32_t)trType;
                Del_Cm_HostMgt(mac, host);
                dbgstrm << "Remove host, vid = " << (int)iter->GetVid()
                    << ", ip = " << InetToString(host.ipv4_addr)
                    << ", mac = " << MacToString(host.host_mac.addr)
                    << endl;
            }
            break;
        }
    }
}

void DhcpRelay::ProecessReply(struct msghdr& msg)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(DhcpClientPort);

    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_name = &saddr;
    msg.msg_namelen = sizeof(saddr);

    DhcpPkt* packet = (DhcpPkt*)packetBuf;
    /* look up interface index in cache */
    std::list<Interface>::iterator intf;
    for (intf =interfaces.begin(); intf != interfaces.end(); ++intf)
    {
        if (intf->addr.s_addr == packet->giaddr.s_addr)
        {
            break;
        }
    }
    if (intf == interfaces.end())
    {
        errstrm << "can't find relay interface." << endl;
        return;
    }

    if (packet->ciaddr.s_addr != 0)
    {
        saddr.sin_addr = packet->ciaddr;
        dbgstrm << "send reply packet to " << string(inet_ntoa(saddr.sin_addr)) << endl;
    }
    else
    {
        if (ntohs(packet->flags) & BoradcastFlag)
        {
            /* broadcast bit is set,  send packet to 255.255.255.255 */
            msg.msg_control = control;
            msg.msg_controllen = sizeof(control);
            saddr.sin_addr.s_addr = INADDR_BROADCAST;
            dbgstrm << "send reply packet to " << string(inet_ntoa(saddr.sin_addr)) << endl;

            struct cmsghdr    *cmptr;
            struct in_pktinfo *pktInterfaceInfo;
            cmptr =  CMSG_FIRSTHDR(&msg);
            pktInterfaceInfo = (struct in_pktinfo *)CMSG_DATA(cmptr);
            pktInterfaceInfo->ipi_ifindex = intf->index;
            pktInterfaceInfo->ipi_spec_dst.s_addr = 0;
            msg.msg_controllen = cmptr->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));
            cmptr->cmsg_level = SOL_IP;
            cmptr->cmsg_type = IP_PKTINFO;
        }
        else
        {
            /* client not configured and cannot reply to ARP.
               Insert arp entry direct.*/
            saddr.sin_addr = packet->yiaddr;
            struct arpreq req;
            *((struct sockaddr_in *)&req.arp_pa) = saddr;
            req.arp_ha.sa_family = packet->htype;
            memcpy(req.arp_ha.sa_data, packet->chaddr, packet->hlen);
            strncpy(req.arp_dev, intf->name.c_str(), IF_NAMESIZE);
            req.arp_flags = ATF_COM;
            if (ioctl(socketFd, SIOCSARP, &req) == -1)
            {
                dbgstrm << "cant ioctl, iterface name = " << intf->name << endl;
            }

            dbgstrm << "send reply packet to " << string(inet_ntoa(saddr.sin_addr)) << endl;
        }
    }

    while (sendmsg(socketFd, &msg, 0) == -1 && errno == EINTR);

    DhcpPktWithOpts* pktWithOpt = (DhcpPktWithOpts*)packetBuf;
    UpdateCpeInfoTable(pktWithOpt);
}

bool DhcpRelay::Start()
{
    /* you must configure DHCP server address before. */
    assert(servers.size() != 0);

    if (!CreateSocket())
    {
        return false;
    }

    if (!BindSocket())
    {
        CloseSocket();
        return false;
    }

    struct   sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(DhcpServerPort);
    saddr.sin_addr.s_addr = INADDR_ANY;

    struct msghdr  msg;
    struct iovec   iov;
    while (true)
    {
        msg.msg_control = control;
        msg.msg_controllen = sizeof(control);
        msg.msg_name = &saddr;
        msg.msg_namelen = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_iov->iov_base = packetBuf;
        msg.msg_iov->iov_len  = bufSize;

        dbgstrm << "Ready to receive packets." << endl;
        if (ReceiveMsg(msg) == -1)
        {
            continue;
        }

        DhcpPkt* packet = (DhcpPkt*)packetBuf;
        
        /* last ditch loop squashing. */
        if ((packet->hops++) > 20)
        {
            dbgstrm << "Drop packet because of hops." << endl;
            continue;
        }

        if (packet->hlen > DhcpChaddrMax)
        {
            dbgstrm << "Drop packet because of hlen." << endl;
            continue;
        }

        if (packet->op == BootpRequest)
        {
            dbgstrm << "Received a request packet." << endl;
            ProcessRequest(msg);
        }
        else if (packet->op == BootpReply)
        {
            dbgstrm << "Received a reply packet." << endl;
            ProecessReply(msg);
        }
    }

    return true;
};

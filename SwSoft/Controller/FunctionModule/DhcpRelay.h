#ifndef _DhcpRelay_h_
#define _DhcpRelay_h_

#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::binary_function;
using std::find_if;
using std::bind2nd;
using std::cout;
using std::cerr;
using std::endl;

#define DhcpChaddrMax    16
#define DhcpSnameLen     64
#define DhcpFileLen      128
#define DhcpOptionLen    312

#define DhcpServerPort   67
#define DhcpClientPort   68

#define BootpRequest     1
#define BootpReply       2

#define BoradcastFlag 0x8000

typedef enum
{
    DhcpDiscover = 0x01,
    DhcpOffer	 = 0x02,
    DhcpRequest  = 0x03,
    DhcpDecline  = 0x04,
    DhcpAck      = 0x05,
    DhcpNak      = 0x06,
    DhcpRelease  = 0x07,
    DhcpInform   = 0x08,
}DhcpMsgType;

struct DhcpPkt
{
    uint8_t  op, htype, hlen, hops;
    uint32_t xid;
    uint16_t secs, flags;
    struct in_addr ciaddr, yiaddr, siaddr, giaddr;
    uint8_t chaddr[DhcpChaddrMax], sname[DhcpSnameLen], file[DhcpFileLen];
};

struct DhcpPktWithOpts
{
    DhcpPkt  dhcpPkt;
    uint32_t magicCookie;
    uint8_t  options[DhcpOptionLen];
};

struct Interface
{
    int     index;
    std::string  name;
    struct  in_addr addr;
};

typedef uint16_t VlanId;

class VlanCmInfo
{
private:
    VlanId vid;
    struct in_addr cmIp;
    uint8_t cmMac[6];

public:
    VlanCmInfo(VlanId vid, struct in_addr cmIp, const uint8_t* cmMac);
    VlanId GetVid();
    uint32_t GetCmIp();
    const uint8_t* GetCmMac();
    void Put(std::ostream& os) const;

    int Compare(VlanCmInfo const& right) const
    {
        return vid - right.vid;
    }
};

inline bool operator == (VlanCmInfo const& left, VlanCmInfo const& right)
{
    return left.Compare(right) == 0;
}

inline std::ostream& operator << (std::ostream& os, VlanCmInfo const& vlanCm)
{
    vlanCm.Put(os);
    return os;
}

enum TerminalType
{
    TerminalTypeCm = 0,

    /* CPE terminal type */
    TerminalTypeMta  = 1,
    TerminalTypeStb  = 2,
    TerminalTypeHost = 3,
    TerminalTypeWiFi = 4,
    TerminalTypeIsTb = 5,
};

#define DefaultTerminalType TerminalTypeHost

struct TerminalTypeNamePair
{
    TerminalType type;
    const char*  name;
};

struct Option82
{
    uint8_t  code;
    uint8_t  len;
    uint8_t  sub1Code;
    uint8_t  sub1Len;
    char     agentCircuitId[4];
    uint8_t  sub2code;
    uint8_t  sub2Len;
    char     agentRemoteId[4];
} __attribute__ ((packed));

class DhcpRelay
{
public:
    typedef std::list<struct in_addr>::iterator ServerIterator;

    ~DhcpRelay()
    {
        delete[] packetBuf;
    }

    bool Start();
    void Stop();

    static DhcpRelay& GetInstance()
    {
        static DhcpRelay instance;
        return instance;
    }
    void ClearServerIp();
    void AddServerIp(struct in_addr server);
    ServerIterator DhcpServerBegin() { return servers.begin();}
    ServerIterator DhcpServerEnd() { return servers.end();}

private:
    std::list<struct in_addr> servers;
    std::list<std::string>  broadcastIntferces;
    std::list<std::string>  excludeIntferces;
    std::list<Interface>      interfaces;
    std::list<VlanCmInfo>     vlanCmlist;
    uint8_t* packetBuf;
    size_t   bufSize;
    int      socketFd;
    uint8_t control[CMSG_SPACE(sizeof(struct in_pktinfo))];

    DhcpRelay() : socketFd(-1)
    {
        /* MUT: 1500 without eth header.  1514 if eth header included. */
        bufSize = 1500 - 20; //20 is the size of IP header.
        packetBuf = new uint8_t[bufSize];
    }

    bool CreateSocket();
    void CloseSocket();
    bool BindSocket();
    ssize_t ReceiveMsg(struct msghdr&);
    bool GetIngressInterace(struct msghdr& msg, Interface& intf);
    VlanId GetVlanId(Interface& intf);
    uint8_t GetDhcpOption(const DhcpPktWithOpts* packet,
                          uint8_t code,
                          char* value,
                          uint8_t valueLen);
    TerminalType GetTerminalType(const DhcpPktWithOpts* packet);
    VlanId GetVlanId(const DhcpPktWithOpts* packet);
    void AddOption82(struct msghdr& msg);
    void ProcessRequest(struct msghdr& msg);
    void UpdateCpeInfoTable(const DhcpPktWithOpts* packet);
    void ProecessReply(struct msghdr& msg);
};



#endif

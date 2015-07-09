#include "SystemInclude.h"
#include <arpa/inet.h>
#include <netinet/in.h>

#include <Util.h>
using namespace std;

string InetToString(uint32_t ip)
{
    struct in_addr addr;
    addr.s_addr = ip;
    return string(inet_ntoa(addr));
}

string MacToString(uint8_t const* mac)
{
    ostringstream os;
    for (unsigned int i = 0; i < 6; ++i)
    {
        if (i != 0)
            os << ":";
        os << setw(2) << setfill('0') << hex << (int)(mac[i]);
    }

    return os.str();
}

string ArrayToString(uint8_t const* array, uint8_t len)
{
    ostringstream os;
    for (uint8_t i = 0; i < len; ++i)
    {
        if(i != 0)
            os << ",";

        os << (int)array[i];
    }

    return os.str();
}

uint16_t GetUint16(uint8_t const* data)
{
    uint16_t ret = ntohs(*(uint16_t *)(data));
    return ret;
}

uint32_t GetUint32(uint8_t const* data)
{
    uint32_t ret = ntohl(*(uint32_t *)(data));
    return ret;
}

uint8_t* CopyMacAddr(uint8_t* dst, uint8_t const* src)
{
    memcpy(dst, src, 6);
    return dst;
}

int CompareCmcAddr(uint8_t const* left, uint8_t const* right)
{
    return memcmp(left, right, CmcAddrLength);
}

void* CopyCmcAddr(uint8_t* to, uint8_t const* from)
{
    return memcpy(to, from, CmcAddrLength);
}

string CmcAddrToString(uint8_t const* addr)
{
#if 1
    uint32_t ip = GetUint32(addr);
    return InetToString(ip);
#else
    return MacToString(addr);
#endif
}

namespace std
{
    ostream& comma(ostream& os)
    {
        os << ", ";
        return (os);
    }
}

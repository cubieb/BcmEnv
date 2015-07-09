#ifndef _Util_h_
#define _Util_h_

#define CmcAddrLength 20

std::string InetToString(uint32_t ip);
std::string MacToString(uint8_t const* mac);
std::string ArrayToString(uint8_t const* array, uint8_t len);
uint16_t GetUint16(uint8_t const* data);
uint32_t GetUint32(uint8_t const* data);
uint8_t* CopyMacAddr(uint8_t* dst, uint8_t const* src);
int CompareCmcAddr(uint8_t const* left, uint8_t const* right);
void* CopyCmcAddr(uint8_t* to, uint8_t const* from);
std::string CmcAddrToString(uint8_t const* addr);

namespace std
{
    ostream& comma(ostream& os);
}
#endif

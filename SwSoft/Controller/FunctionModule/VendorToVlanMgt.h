#ifndef _VendorToVlanMgt_h_
#define _VendorToVlanMgt_h_

/*
    gsAnCmcVendorToVlanObjects
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define VendorToVlanStateDisable 1
#define VendorToVlanStateEnable  2

int32_t GetVenderToVlanState();
BASE_ErrCode SetVenderToVlanState(int32_t state);

#define MaxVendorIdLength 32
struct VendorToVlanEntry
{
    int32_t index;
    uint8_t cmMac[6];
    int32_t cmState;
    int32_t vid;
    char    vendorId[MaxVendorIdLength];
};
typedef struct VendorToVlanEntry VendorToVlanEntryT;

BASE_ErrCode GetFirstVendorToVlan(VendorToVlanEntryT* entry);
BASE_ErrCode GetNextVendorToVlan(int32_t index, VendorToVlanEntryT* entry);
BASE_ErrCode FindVendorToVlan(int32_t index, VendorToVlanEntryT* entry);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //_VendorToVlanMgt_h_

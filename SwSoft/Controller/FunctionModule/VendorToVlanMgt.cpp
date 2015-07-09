#include "SystemInclude.h"
#include "BcmController.h"

#include "Debug.h"
#include "Util.h"
#include "AutoLock.h"
#include "SaveCfgFile.h"
#include "VendorToVlanMgt.h"

using namespace std;

int32_t GetVenderToVlanState()
{
    return VendorToVlanStateDisable;
}

BASE_ErrCode SetVenderToVlanState(int32_t state)
{
    if (state == VendorToVlanStateDisable)
        return BASE_OK;

    return BASE_ERROR;
}

BASE_ErrCode GetFirstVendorToVlan(VendorToVlanEntryT* entry)
{
    return BASE_ERROR;
}

BASE_ErrCode GetNextVendorToVlan(int32_t index, VendorToVlanEntryT* entry)
{
    return BASE_ERROR;
}

BASE_ErrCode FindVendorToVlan(int32_t index, VendorToVlanEntryT* entry)
{
    return BASE_ERROR;
}

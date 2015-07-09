#include "SystemInclude.h"
#include "BcmController.h"

#include "Debug.h"
#include "Util.h"
#include "AutoLock.h"
#include "SaveCfgFile.h"
#include "CdtCosTransSvlanMgt.h"

using namespace std;

BASE_ErrCode GetFirstCdtTransSvid(CdtTransSvidT* entry)
{
    entry->cdtCos = 0;
    entry->svlanCos = 0;
    return BASE_OK;
}

BASE_ErrCode GetNextCdtTransSvid(int32_t cos, CdtTransSvidT* entry)
{
    return BASE_ERROR;
}

BASE_ErrCode FindCdtTransSvid(int32_t cos, CdtTransSvidT* entry)
{
    if (cos == 0)
    {
        entry->cdtCos = 0;
        entry->svlanCos = 0;
        return BASE_OK;
    }
    return BASE_ERROR;
}

BASE_ErrCode CreateCdtTransSvid(CdtTransSvidT* entry)
{
    return BASE_ERROR;
}

BASE_ErrCode RemoveCdtTransSvid(int32_t cos)
{
    return BASE_ERROR;
}

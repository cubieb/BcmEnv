#include "SystemInclude.h"
#include "BcmController.h"

#include "Debug.h"
#include "Util.h"
#include "AutoLock.h"
#include "CmcGlobalConfigMgt.h"

using namespace std;
int upstreamErrorRateThreshold;
int upstreamSnrThreshold;
int ddosAttackType = DdosAttackTypeArp;

void GetUpstreamErrorRateThreshold(int* threshold)
{
    *threshold = upstreamErrorRateThreshold;
}

bool SetUpstreamErrorRateThreshold(int threshold)
{
    upstreamErrorRateThreshold = threshold;
    return true;
}

void GetUpstreamSnrThreshold(int* threshold)
{
    *threshold = upstreamSnrThreshold;
}

bool SetUpstreamSnrThreshold(int threshold)
{
    upstreamSnrThreshold = threshold;
    return BASE_ERROR;
}

void GetDdosAttackType(int* type)
{
    *type = ddosAttackType;
}

bool ResetAllCm()
{
    return true;
}

bool DeleteAllCm()
{
    return true;
}

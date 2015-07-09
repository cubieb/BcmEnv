#ifndef _CmcGlobalConfigMgt_h_
#define _CmcGlobalConfigMgt_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void GetUpstreamErrorRateThreshold(int* threshold);
bool SetUpstreamErrorRateThreshold(int threshold);

void GetUpstreamSnrThreshold(int* threshold);
bool SetUpstreamSnrThreshold(int threshold);

#define DdosAttackTypeArp  0
#define DdosAttackTypeIgmp 1
#define DdosAttackTypeIcmp 2
#define DdosAttackTypeDhcp 3
void GetDdosAttackType(int* type);

bool ResetAllCm();
bool DeleteAllCm();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //_CmcGlobalConfigMgt_h_

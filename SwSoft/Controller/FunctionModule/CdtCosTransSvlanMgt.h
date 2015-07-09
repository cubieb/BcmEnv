#ifndef _CdtCosTransSvlanMgt_h_
#define _CdtCosTransSvlanMgt_h_

/*
    gsAnCmcCdtTransSVidTable
    gsCableDocsUsSpectrumChartTable
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct CdtTransSvid
{
    int32_t cdtCos;
    int32_t svlanCos;
};
typedef struct CdtTransSvid CdtTransSvidT;

BASE_ErrCode GetFirstCdtTransSvid(CdtTransSvidT* entry);
BASE_ErrCode GetNextCdtTransSvid(CdtTransSvidT* entry);
BASE_ErrCode FindCdtTransSvid(int32_t cos, CdtTransSvidT* entry);

BASE_ErrCode CreateCdtTransSvid(CdtTransSvidT* entry);
BASE_ErrCode RemoveCdtTransSvid(int32_t cos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //_CdtCosTransSvlanMgt_h_

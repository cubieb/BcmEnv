#ifndef _DsQosConfigMgt_h_
#define _DsQosConfigMgt_h_

typedef struct {
    unsigned int ip_address;
    unsigned int bandwidth;
} __attribute__ ((packed))  DS_QOS_CFG;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BASE_ErrCode MgtAddDsQosARule(U32 ip_address, U32 bandwidth);
BASE_ErrCode MgtDeleteDsQosARule(U32 ip_address);
BASE_ErrCode MgtShowDsQosARule();
BASE_ErrCode MgtSaveDsQosARule();
BASE_ErrCode SaveDsQosConfig();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


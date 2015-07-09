#ifndef _DhcpRelayMgt_h_
#define _DhcpRelayMgt_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void InitDhcpRelay();
void SetDhcpServerIp(struct in_addr server);
struct in_addr GetDhcpServerIp();
void DeinitDhcpRelay();

void StartDhcpRealy();
BASE_ErrCode SaveDhcpRelayConfig();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* _DhcpRelayMgt_h_ */

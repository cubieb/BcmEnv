#ifndef _CmMgt_h_
#define _CmMgt_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BASE_ErrCode ClearCableModem(MacAddressT mac, CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode SetCnuDownstream(MacAddressT mac, uint8_t* chanList, uint8_t chanNum,
                              CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode SetCnuUpstream(MacAddressT mac, uint8_t* chanList, uint8_t chanNum,
                            CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode ClearCableModemHistory(CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode SendCmCtlMute(MacAddressT mac, uint8_t usChannelId, U32 muteInterval,
                           CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode SendCmCtlReinit(MacAddressT mac, CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode SendCmCtlDisableForwarding(MacAddressT mac, uint8_t forwarding,
                                        CMCOAM_CmcAddrT* cmcMac);
BASE_ErrCode GetCmcCmMmmCnt(MacAddressT * macList,
                            uint8_t macCount, uint8_t cleanFlag,
                            CMCOAM_GetCmcCmMmmCntRspT *pRsp,
                            CMCOAM_CmcAddrT* cmcMac);
#ifdef __cplusplus
}
#endif /* __cplusplus */

extern pthread_mutex_t mgtMutex;


#endif //_CmMgt_h_

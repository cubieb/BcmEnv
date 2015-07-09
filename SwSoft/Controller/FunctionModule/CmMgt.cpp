#include "SystemInclude.h"
#include "BcmController.h"

#include "AutoLock.h"
#include "CmMgt.h"

using namespace std;

BASE_ErrCode ClearCableModem(MacAddressT mac, CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_ClearCableModem(mac, cmcMac);

    return errCode;
}

BASE_ErrCode SetCnuDownstream(MacAddressT mac, uint8_t* chanList, uint8_t chanNum,
                              CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_SetCnuDownstream(mac, chanList, chanNum, cmcMac);

    return errCode;
}

BASE_ErrCode SetCnuUpstream(MacAddressT mac, uint8_t* chanList, uint8_t chanNum,
                    CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_SetCnuUpstream(mac, chanList, chanNum, cmcMac);

    return errCode;
}

BASE_ErrCode ClearCableModemHistory(CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_ClearCableModemHistory(cmcMac);

    return errCode;
}

BASE_ErrCode SendCmCtlMute(MacAddressT mac, uint8_t usChannelId, U32 muteInterval,
                           CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_SendCmCtlMute(mac, usChannelId, muteInterval, cmcMac);

    return errCode;
}

BASE_ErrCode SendCmCtlReinit(MacAddressT mac, CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_SendCmCtlReinit(mac, cmcMac);

    return errCode;
}

BASE_ErrCode SendCmCtlDisableForwarding(MacAddressT mac, uint8_t forwarding,
                                        CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_SendCmCtlDisableForwarding(mac, forwarding, cmcMac);

    return errCode;
}

BASE_ErrCode GetCmcCmMmmCnt(MacAddressT* macList,
                            uint8_t macCount, uint8_t cleanFlag,
                            CMCOAM_GetCmcCmMmmCntRspT *pRsp,
                            CMCOAM_CmcAddrT* cmcMac)
{
    BASE_ErrCode errCode;
    AutoLock autoLock(mgtMutex);
    errCode = CMCOAM_Cm_GetCmcCmMmmCnt(macList, macCount, cleanFlag, pRsp, cmcMac);

    return errCode;
}

#include "SystemInclude.h"

#include "BcmSdk.h"
#include "Debug.h"
#include "Util.h"
#include "DriverRx.h"
#include "L2.h"

using namespace std;
ArpPacket::ArpPacket(uint8_t* data)
{
    hwType = GetUint16(data + ArpHwTypeOffset);
    ptType = GetUint16(data + ArpPtTypeOffset);
    hwSize = data[ArpHwSizeOffset];
    ptSize = data[ArpPtSizeOffset];
    opCode = GetUint16(data + ArpOpCodeOffset);
    memcpy(senderHwAddr, data + ArpSndHwAddrOffset, sizeof(senderHwAddr));
    senderIpAddr = GetUint32(data +ArpSndIpAddrOffset);
    memcpy(targetHwAddr, data + ArpTgtHwAddrOffset, sizeof(targetHwAddr));
    targetIpAddr = GetUint32(data + ArpTgtIpAddrOffset);
}

bcm_ip_t ArpPacket::GetSenderIp()
{
    return senderIpAddr;
}

uint8_t* ArpPacket::GetSenderMac()
{
    return senderHwAddr;
}

ArpRx& ArpRx::GetInstance()
{
    static ArpRx arpRx;
    return arpRx;
}

bool ArpRx::Check(int unit, bcm_pkt_t *pkt)
{
    EthNextHeaderType type;
    int offset = PrefacePadding + EthNextHeaderOffset;
    if ((pkt->rx_untagged & BCM_PKT_OUTER_UNTAGGED) != BCM_PKT_OUTER_UNTAGGED
        && (pkt->rx_untagged & BCM_PKT_INNER_UNTAGGED) != BCM_PKT_INNER_UNTAGGED)
    {
        offset = offset + EthTagSize;
    }

    type = (EthNextHeaderType)GetUint16(pkt->pkt_data->data + offset);

//    if (pkt->vlan == 2049)
//    {
//        dbgstrm << "Received a "
//            << hex << type << dec << " packet" << endl;
//        PrintPacket(pkt);
//    }

    return (type == EthArp);
}

bcm_rx_t L2ArpRxCallback(int unit, bcm_pkt_t* pkt)
{
    ArpRx& arpRx = ArpRx::GetInstance();
    return arpRx.ProcessRxPacket(unit, pkt);
}


#include "SystemInclude.h"
#include "BcmSdk.h"
#include <netinet/in.h>

#include "Debug.h"
#include "Util.h"
#include "DriverRx.h"

using namespace std;

void PrintPacket(bcm_pkt_t *pkt)
{
    bool outerTagged = true;
    bool innerTagged = true;

    if ((pkt->rx_untagged & BCM_PKT_OUTER_UNTAGGED) == BCM_PKT_OUTER_UNTAGGED)
        outerTagged = false;

    if ((pkt->rx_untagged & BCM_PKT_INNER_UNTAGGED) == BCM_PKT_INNER_UNTAGGED)
        innerTagged = false;

    int len = 0, idx = 0;
    uint8_t* location = NULL;
    bcm_pkt_byte_index(pkt, (int)12, &len, &idx, &location);

    dbgstrm << "outer tagged = " << (outerTagged ? "yes" : "no")
            << ", inner tagged = " << (innerTagged ? "yes" : "no")
            << ", vlan id = " << pkt->vlan << endl << "    "
            << ", rx_l3_intf = " << pkt->rx_l3_intf
            << ", inner vlan = " << pkt->inner_vlan
            << ", flags = " << (pkt->flags & (BCM_PKT_F_HGHDR | BCM_PKT_F_SLTAG | BCM_PKT_F_NO_VTAG))
            << ", pkt len = " << pkt->pkt_len << endl << "    "
            << ", rx_outer_tag_action = " << pkt->rx_outer_tag_action
            << ", rx_inner_tag_action = " << pkt->rx_inner_tag_action
            << endl;

    uint8_t* data = pkt->pkt_data->data;

    uint16_t i;
    for (i = 0; i < pkt->pkt_len; ++i)
    {
        if (i % 16 == 0)
        {
            if (i != 0)
                dbgcstrm << endl;
        }
        else
        {
            if (i % 8 == 0)
                dbgcstrm << "    ";
        }

        dbgcstrm << hex << setfill('0') << setw(2) << (int)data[i] << " ";
    }

    dbgcstrm << dec << endl;
}

uint8_t* GetEthNextHeader(bcm_pkt_t *pkt)
{
    int offset = PrefacePadding + EthHeaderSize;
    if ((pkt->rx_untagged & BCM_PKT_OUTER_UNTAGGED) != BCM_PKT_OUTER_UNTAGGED
        && (pkt->rx_untagged & BCM_PKT_INNER_UNTAGGED) != BCM_PKT_INNER_UNTAGGED)
    {
        offset = offset + EthTagSize;
    }

    return pkt->pkt_data->data + offset;
}

bcm_rx_t ProcessRxPacket(int unit, bcm_pkt_t *pkt, void*)
{
    DriverRx& rx = DriverRx::GetInstance();

    return rx.ProcessRxPacket(unit, pkt);
}

void Rx::Regiser(RxCallback callback)
{
    list<RxCallback>::iterator iter;
    iter = find(callbacks.begin(), callbacks.end(), callback);

    if (iter != callbacks.end())
    {
        errstrm << "duplicated registration for " << endl;
    }

    callbacks.push_back(callback);
}

bcm_rx_t Rx::ProcessRxPacket(int unit, bcm_pkt_t* pkt)
{
    if (!Check(unit, pkt))
        return BCM_RX_NOT_HANDLED;

    list<RxCallback>::iterator iter;
    for (iter = callbacks.begin(); iter != callbacks.end(); ++iter)
    {
        if ((*iter)(unit, pkt) == BCM_RX_HANDLED)
            return BCM_RX_HANDLED;
    }

    return BCM_RX_NOT_HANDLED;
}

DriverRx& DriverRx::GetInstance()
{
    static DriverRx rx;
    return rx;
}

bool DriverRx::Check(int , bcm_pkt_t*)
{
    return true;
}

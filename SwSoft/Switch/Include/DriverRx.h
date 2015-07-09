#ifndef _DriverRx_h_
#define _DriverRx_h_

#if defined(__cplusplus)
extern "C"
{
#endif  //#if defined(__cplusplus)

bcm_rx_t ProcessRxPacket(int unit, bcm_pkt_t *pkt, void *cookie);

#if defined(__cplusplus)
}
#endif  //#if defined(__cplusplus)

enum EthNextHeaderType
{
    EthArp = 0x0806,
    EthIp  = 0x0800
};

/* Currently, there will allways 4 bytes padding in front of the packet. */
#define PrefacePadding      4

/* Packet field offset,  if there is no padding, no vlan tag information. */
#define EthNextHeaderOffset 12
#define EthNextHeaderSize   2
#define EthHeaderSize       14
#define EthTagSize          4

void PrintPacket(bcm_pkt_t *pkt);
uint8_t* GetEthNextHeader(bcm_pkt_t *pkt);

typedef bcm_rx_t (*RxCallback)(int, bcm_pkt_t*);
using std::list;

class Rx
{
protected:
    Rx() {};
    virtual ~Rx() {};

public:
    virtual void Regiser(RxCallback callback);
    virtual bcm_rx_t ProcessRxPacket(int unit, bcm_pkt_t *pkt);
    virtual bool Check(int unit, bcm_pkt_t *pkt) = 0;

private:
    list<RxCallback> callbacks;
};

class DriverRx : public Rx
{
public:
    static DriverRx& GetInstance();
    bool Check(int unit, bcm_pkt_t *pkt);

private:
    DriverRx() {}
};

#endif

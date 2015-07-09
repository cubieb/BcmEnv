#ifndef _L2_h_
#define _L2_h_

#define ArpHwTypeOffset       0
#define ArpPtTypeOffset       2
#define ArpHwSizeOffset       4
#define ArpPtSizeOffset       5
#define ArpOpCodeOffset       6
#define ArpSndHwAddrOffset    8
#define ArpSndIpAddrOffset    14
#define ArpTgtHwAddrOffset    18
#define ArpTgtIpAddrOffset    24
#define ArpPaddingOffset      28

class ArpPacket
{
public:
    ArpPacket(uint8_t*);

    bcm_ip_t GetSenderIp();
    uint8_t* GetSenderMac();

private:
    /* ARP packet */
    uint16_t hwType;           /* hardware type (must be ARPHRD_ETHER) */
    uint16_t ptType;           /* protocol type (must be ETH_P_IP) */
    uint8_t  hwSize;           /* hardware address length (must be 6) */
    uint8_t  ptSize;           /* protocol address length (must be 4) */
    uint16_t opCode;           /* ARP opcode */
    uint8_t  senderHwAddr[6];  /* sender's hardware address */
    uint32_t senderIpAddr;  /* sender's IP address */
    uint8_t  targetHwAddr[6];  /* target's hardware address */
    uint32_t targetIpAddr;  /* target's IP address */
    uint8_t  pad[18];          /* pad for min. Ethernet payload (60 bytes) */
};

class ArpRx : public Rx
{
public:
    static ArpRx& GetInstance();
    bool Check(int unit, bcm_pkt_t *pkt);

private:
    ArpRx() {}
};

bcm_rx_t L2ArpRxCallback(int unit, bcm_pkt_t* pkt);

#endif /* _L3_h_ */

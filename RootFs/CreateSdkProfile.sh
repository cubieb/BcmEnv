#!/bin/bash 
# Program: 
#       This program creates rc.soc profile. 
# Version:
#	0.1 .
# History: 
# 	no.
# Author:
#	Liu Hao
# Example:
#   ./CreateSdkProfile.sh 5 20.0.0.5 00:0C:29:3B:53:C7 > /var/lib/tftp/rc.soc

if [ $# != 3 ]
then
    echo "Wrong parameter."
    echo "Usage: CreateSdkProfile.sh {cmcId:1~100} {wanIp} {defaultRouterMac}"
    exit 1
fi
if [ $1 -lt 1 -o $1 -gt 100 ]
then
    echo "Wrong parameter."
    echo "Usage: CreateSdkProfile.sh {cmcId:1~100} {wanIp} {defaultRouterMac}"
    exit 1
fi 
cmcId=$1
wanIp=$2
maxVlanNumber=125
defaultRouterMac=$3
mac0to3=02:10:18:ff
mac04hex=$(printf "%x" $cmcId)

echo "init all"
echo "l3 init"
echo ""

echo "rxm init"
echo "rxm start"
echo ""

echo "#use ge2 as normal port."
echo "#phy ge2 0x1c 0xfcec"
echo "#use ge2 to connect cmc upstream port."
echo "phy ge2 0x1c 0xfc0b"
echo ""

echo "#congiurate vlan."
echo "#vlan 1, used for wan port."
echo "vlan remove 1 PortBitMap=ge,hg"
echo "vlan add 1 PortBitMap=ge0 UntagBitMap=ge0"
echo "pvlan set ge0 1"
echo ""

echo "#vlan 2, used to connnect cmc upstream port."
echo "vlan create 2 PortBitMap=cpu,ge2 UntagBitMap=ge2"
echo "pvlan set ge2 2"
echo ""

echo "#configrate packet to cpu."
echo "switchcontrol L3EgressMode=1"
echo "switchcontrol DhcpPktToCpu=1"
echo "switchcontrol ArpRequestToCpu=1"
echo "switchcontrol ArpReplyToCpu=1"
echo ""

echo "################ vlan 1 interface ################"
echo "#create L3 interface"
echo "l3 intf add vlan=1 mac=$mac0to3:$mac04hex:02 intf=1"
echo ""

echo "#create knet interface. ID=1"
echo "knet netif create vlan=1 RCPU=no ifname=vlan1"
echo "knet filter create desttype=netif destid=1 striptag=yes vlan=1 reason=nhop Mirror=yes"
echo "knet filter create desttype=netif destid=1 striptag=yes vlan=1 reason=protocol Mirror=yes"
echo ""

echo "################# create default route ################"
echo "#we assume the default route is connected to ge0 and the route's mac is 00:01:02:03:04:05."
echo "#At the real environment the \"mac\" must be changed to the real router's mac address."
echo "l3 egress add mac=${defaultRouterMac} intf=1 port=ge0 copytocpu=false"
echo "l3 defip add ip=0.0.0.0 mask=0.0.0.0 intf=\$egr_object_id port=ge0"
echo ""

echo "################ route to vlan 1 ################"
echo "l3 egress add intf=1 copytocpu=true"
echo "l3 l3table add ip=$wanIp intf=\$egr_object_id"
echo ""

echo "################ vlan 2 interface ################"
echo "#create L3 interface"
echo "l3 intf add vlan=2 mac=$mac0to3:$mac04hex:03 intf=2"
echo ""

echo "#create knet interface. ID=2"
echo "knet netif create vlan=2 RCPU=no ifname=vlan2"
echo "knet filter create desttype=netif destid=2 striptag=yes vlan=2 reason=nhop Mirror=yes"
echo "knet filter create desttype=netif destid=2 striptag=yes vlan=2 reason=protocol Mirror=yes"
echo ""

echo "################ route to vlan 2 ################"
echo "l3 egress add intf=2 copytocpu=true"
echo "l3 l3table add ip=192.168.2.254 intf=\$egr_object_id"
echo ""

echo "################ vlan 2049~2059 interface ################"
echo "#vlan 2049~$((2048+maxVlanNumber)), used by CPE,CM to connection internet."
mac5dec=4
intf=3
net=1
for vid in $(seq 2049 $((2048+maxVlanNumber)))
do
    echo "vlan create $vid PortBitMap=cpu,ge1,ge2"
    mac5hex=$(printf "%x" $mac5dec)
    echo "l3 intf add vlan=$vid mac=$mac0to3:$mac04hex:$mac5hex intf=$intf"
    echo "knet netif create vlan=$vid RCPU=no ifname=vlan$vid"
    if [ $maxVlanNumber -le 125 ]
    then
    	echo "knet filter create desttype=netif destid=$intf striptag=yes vlan=$vid reason=nhop Mirror=yes"
    fi
    echo "knet filter create desttype=netif destid=$intf striptag=yes vlan=$vid reason=protocol Mirror=yes"
    echo "l3 egress add intf=$intf copytocpu=true"
    echo "l3 l3table add ip=192.$cmcId.$net.1 intf=\$egr_object_id"
    
    ((mac5dec++, intf++, net++))
    echo ""
done

#echo "knet filter create desttype=netif striptag=yes reason=nhop Mirror=yes"
#echo "knet filter create desttype=netif striptag=yes reason=protocol Mirror=yes"

echo "################ config tpid 0x88a8 for port ge1,ge2 ################"
echo "dtag tpid ge1,ge2 0x88a8"
echo ""

exit 0

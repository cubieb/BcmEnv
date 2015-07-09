#!/bin/bash 
# Usage:
#   Create-dhcpd.conf.sh {maxCmcNumber:1~100} {maxVlanNumber:1~250} {tftpServer}
# Example:
#   Create-dhcpd.conf.sh 100 250 20.0.0.3

if [ $# != 3 ]
then
    echo "Wrong parameter."
    echo "Usage: Create-dhcpd.conf.sh {maxCmcNumber:1~100} {maxVlanNumber:1~250} {tftpServer}"
    exit 1
fi
if [ $1 -lt 1 -o $1 -gt 100 ]
then
    echo "Wrong parameter."
    echo "Usage: Crreate-dhcpd.conf.sh {maxCmcNumber:1~100} {maxVlanNumber:1~250} {tftpServer}"
    exit 1
fi
if [ $2 -lt 1 -o $2 -gt 250 ]
then
    echo "Wrong parameter."
    echo "Usage: Crreate-dhcpd.conf.sh {maxCmcNumber:1~100} {maxVlanNumber:1~250} {tftpServer}"
    exit 1
fi

maxCmcNumber=$1
minVlan=2049
maxVlan=$(expr 2048 + $2)
tftpServer=$3

echo '
    #########################################################
    #   dhcod.conf structuration:                           #
    #       global parameters                               #
    #       class CmcXVlanX ........                        #
    #       server subnet: subnet 20.0.0.0 xxxx             #
    #       vlans subnet: subnet 192.168.10.0 xxxx ....     #
    #########################################################

    ###############################################
    # global parameters
    ###############################################

    #ddns-update-style none;
    ddns-updates off;
    deny client-updates;
    one-lease-per-client false;
    allow bootp;
    option T150 code 150 = string;
    ddns-update-style interim;
    log-facility local7;
    default-lease-time              360000;
    max-lease-time                  372000;
    #stash-agent-options true;
'
echo '
    #
    # DHCP Server Configuration file.
    #   see /usr/share/doc/dhcp*/dhcpd.conf.sample
    #   see "man 5 dhcpd.conf"
    # See "cat /var/log/messages" for debuging dhcpd.conf 
    # 
'

echo '
    #################################################
    # class CmcXVlanX... declare
    #################################################
    #can not have two "match if ()" in each class
'

for cmcId in $(seq 1 ${maxCmcNumber})
do
    strCmcId=$(printf "%04d" $cmcId)
    for vlan in $(seq ${minVlan} ${maxVlan})
    do
        echo ""
        echo "class \"Cmc${cmcId}Vlan${vlan}\" {"
        echo "    match if" 
        echo "    ("
        echo "        (option agent.circuit-id = \"${vlan}\") and (option agent.remote-id = \"${strCmcId}\")"
        echo "    );"
        echo "}"
    done
done

echo '
    ##############################################
    # server subnet: "subnet 20.0.0.0 xxxx"
    ##############################################
    # must define a same subnet with the server ,else it will be a error
    # This is cmc device management subnet ip address,if the device communicates 
    # with others through dhcp that "subnet 20.0.0.0" must be defined.
    subnet 20.0.0.0 netmask 255.255.255.0 {
    } 
'

echo '
    ################################################
    # vlans subnet: "subnet 192.168.10.0 xxxx" ....
    ################################################
'

for cmcId in $(seq 1 ${maxCmcNumber})
do
  for subnet in $(seq 1 $((maxVlan-minVlan+1)))
  do
    vlan=$(expr $subnet + 2048)
    echo "
    subnet 192.${cmcId}.${subnet}.0 netmask 255.255.255.0 {
      option routers                  192.${cmcId}.${subnet}.1;
      option broadcast-address        192.${cmcId}.${subnet}.255;
      option subnet-mask              255.255.255.0;

      pool {
        allow members of            \"Cmc${cmcId}Vlan${vlan}\";
        range                       192.${cmcId}.${subnet}.50 192.${cmcId}.${subnet}.100;
        next-server                 ${tftpServer};
        filename                    \"cmc_modem.cfg\";
      }
    }
    "
  done
done

exit 0

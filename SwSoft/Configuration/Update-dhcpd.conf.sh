#!/bin/bash 
# Usage:
#   Update-dhcpd.conf.sh 

sudo bash -c "./Create-dhcpd.conf.sh 100 250 20.0.0.3 > /etc/dhcp/dhcpd.conf"

exit 0

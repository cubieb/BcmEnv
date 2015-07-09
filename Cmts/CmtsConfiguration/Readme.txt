You can customize the system configuration in 4 ways:
1) The dhcp server will use a custom dhcpd.conf file if one is found at /data/cmts/dhcpd.conf
2) The CMTS will use a custom cmtsconfig.xml file if one is found at /data/cmts/cmtsconfig.xml
3) Any *.bin or *.cfg files in /cmts will be copied to the /var/tftpboot directory for tftp downloading
4) Any *.bin or *.cfg files in /data/cmts will be copied to the /var/tftpboot directory for tftp downloading
   (Note: Any files in the flash file system at /data/cmts/ will overwrite the standard ones in /cmts/, so be careful!)
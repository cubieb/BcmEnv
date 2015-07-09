WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

#sudo mount -o loop mips-2007-01-21.iso /media
#sudo umount /media
CrossToolsSrc = ~/ToolsSource

.PHONY: all 
all: Eldk BcmElfTools BcmCmtsToos

Eldk:
	# Note: /opt/eldk/usr/bin should be added into PATH evn variable.
	if [ -d /opt/eldk ]; \
	then \
	    echo "/opt/eldk existed befor!"; exit 1; \
	fi
	tar -xvf $(CrossToolsSrc)/mips-2007-01-21.tar.gz
	sudo mips-2007-01-21/install -d /opt/eldk
	$(RM) -r mips-2007-01-21

BcmElfTools:
	# install tool chains for broadcom cfe.
	# Note: /opt/i686-pc-linux-gnu/bin should be added into PATH evn variable.
	if [ -d /opt/i686-pc-linux-gnu ]; \
	then \
	    echo "/opt/i686-pc-linux-gnu existed befor!"; exit 1; \
	fi
	tar -jxvf $(CrossToolsSrc)/broadcom_2006a_410.i686-pc-linux-gnu.sb1-elf.tar.bz2
	sudo $(MV) ./opt/specifix/broadcom_2006a_410/i686-pc-linux-gnu /opt
	$(RM) -r opt

BcmCmtsToos:
	# intsall tool chains for Broadcom Cmts.
	cd /; sudo tar -xvf $(CrossToolsSrc)/uclibc-crosstools-gcc-4.2.3-4.tar.bz2
	
	
	
	
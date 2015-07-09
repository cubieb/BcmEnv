WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

BusyboxDir=$(TftpRoot)/Install
#LibList  = ld.so.1 libcrypt.so.1 libdl.so.2 libm.so libresolv.so libstdc++.so libthread_db.so
#LibList += libcrypto.so.0.9.7f libc.so.6 libgcc_s.so.1 libm.so.6 librt.so libstdc++.so.6 
#LibList += libutil.so libdl.so libm-2.3.5.so libpthread.so.0 librt.so.1 
#LibList += libstdc++.so.6.0.4
#LibListBuild = $(addprefix /opt/eldk/mips_4KC/lib/,$(LibList))

#LibExt  = libcrypto.so.5 libz.so.1
#LibExtBuild = $(addprefix /opt/eldk/mips_4KC/usr/lib/,$(LibExt))

#MakeFsCmd=mkfs.jffs2 --big-endian --pad=0x500000 -n -e 0x20000 -r FsRoot -o $(TftpRoot)/RootFs.img
MakeFsCmd=mkfs.jffs2 --big-endian -n -e 0x20000 -r FsRoot -o $(TftpRoot)/RootFs.img

.PHONY: all debug_ver release_ver step1 step2 clean dos2unix
all: debug_ver

debug_ver: dos2unix step1
	mv FsRoot/etc/profile.debug FsRoot/etc/profile
	#Create image
	$(MakeFsCmd)
	
release_ver: dos2unix step1 step2
	mv FsRoot/etc/profile.release FsRoot/etc/profile
	#Create image
	$(MakeFsCmd)

CmcId ?= 5
WanIp ?= 20.0.0.5
DefaultRouterMac ?= 80:38:bc:16:c4:f3

step1:
	$(RM) -r FsRoot
	
	#Create File System root folder
	mkdir FsRoot/{,bin,dev/{,pts},etc,home,lib,mnt,opt,proc,sys,tmp,var/{,log,run},root}
	mkdir FsRoot/lib/modules

	$(CP) -r InitFile/* FsRoot
	./CreateSdkProfile.sh $(CmcId) $(WanIp) $(DefaultRouterMac) > FsRoot/etc/rc.soc
	$(CP) FsRoot/etc/rc.soc $(TftpRoot)/rc.soc

	#Copy Busybox to File System Root.
	$(CP) -r $(BusyboxDir)/* FsRoot
	#To clear the warning:"Using fallback suid method"
	sudo touch FsRoot/etc/busybox.conf
	sudo touch FsRoot/.init_enable_core

	#Create devices
	sudo ./MakeDevices.sh
	#Copy broadcom SDK modules.
	$(CP) $(WorkspaceRoot)/Sdk/Sdk.Install/*.ko FsRoot/lib/modules
	
step2:
	$(CP) $(TftpRoot)/daemon.exe FsRoot/sbin
	$(CP) $(TftpRoot)/switch.exe FsRoot/sbin
	$(CP) $(TftpRoot)/controller.exe FsRoot/sbin

clean:
	$(RM) -r FsRoot 
	$(RM) $(TftpRoot)/RootFs.img

Scripts=InitFile/etc/init.d/* 
dos2unix:
	dos2unix $(Scripts)

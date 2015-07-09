WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

AllOjects=linux-kernel-bde.ko,linux-user-bde.ko,linux-uk-proxy.ko,linux-bcm-knet.ko,bcm.user

KernalDir = $(WorkspaceRoot)/Os/linux-2.6.27.18
KernalFlagDir = /opt/eldk/usr/lib/gcc/mips-linux/4.0.0/include
SdkRoot = $(WorkspaceRoot)/Sdk/sdk-all-6.2.2
SharedLib=0

Env = BUILD_PLATFORM=ELDK KERN_VER=2.6.27.18 ELDK_VERSION=4.1 \
	KERNDIR=$(KernalDir) KFLAG_INCLD=$(KernalFlagDir) \
	LINUX_MAKE_SHARED_LIB=$(SharedLib)


.PHONY: all configure install variable clean
all: 
	$(MAKE) -C $(SdkRoot)/systems/linux/user/keystone-2_6 $(Env)

configure:
	tar -xvf sdk-all-6.2.2.tar.gz
	$(CP) patches/Make.local sdk-all-6.2.2/make

	#Patch from LiuHao for shared libary.	
	if [ "$(SharedLib)" = "1" ]; \
	then \
		$(CP) patches/shared_lib.c $(SdkRoot)/src/shared;  \
		cd $(SdkRoot); \
		patch -p1 < ../patches/001.etcgmac.c.patch; \
		patch -p1 < ../patches/002.wcmod_main.c.patch; \
	fi
	#xmem.c.patch, to create a tmp.txt file to indicate diag_shell 
	#has finished its initialization. 
	cd $(SdkRoot); patch -p1 < ../patches/003.xmem.c.patch

install:
	$(RM) -r Sdk.Install
	mkdir Sdk.Install
	$(CP) $(SdkRoot)/systems/linux/user/keystone-2_6/{$(AllOjects)} Sdk.Install
	#$(Strip) Sdk.Install/*.ko
	$(CP) Sdk.Install/bcm.user $(TftpRoot)
	$(Strip) $(TftpRoot)/bcm.user

variable:
	$(MAKE) -C $(SdkRoot)/systems/linux/user/keystone-2_6 variable VAR=$(VAR)

clean:
	$(RM) -r $(SdkRoot)
	$(RM) -r Sdk.Install


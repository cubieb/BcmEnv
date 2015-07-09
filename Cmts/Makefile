#note,  thie Makefile has not completed yet......

PATH:=/opt/toolchains/uclibc-crosstools-gcc-4.2.3-4/usr/bin:$(PATH)

.PHONY: all configure menuconfig hosttools install clean distclean

CmcSourceDir =$(CURDIR)/Src
CmcBuildDir  =$(CmcSourceDir)/cmts/CmtsApp/build/Linux/BCM3380
CmcConfigDir =$(CmcSourceDir)/src/bcm93380
TargetDir    =$(CmcSourceDir)/src/bcm93380/targets/93382CMC

all: 
	#$(MAKE) -C $(CmcBuildDir)
	cd Src; ./build_linux_fs.sh

PatchFiles = $(shell ls patches/*.patch 2> /dev/null)	
configure:
	mkdir -p Src; cd Src; tar -zxvf ../cmc_src_cmc4_3_0beta3.tgz
	for patch in $(PatchFiles); do\
		echo "=== Applying patches from $$patch";\
		patch -p0 < $$patch || exit 1;\
	done
	
hosttools:
	cd Src/src/bcm93380/hostTools; make fakeroot
	cd Src/src/bcm93380/hostTools; make mksquashfs

menuconfig:
	$(MAKE) -C $(CmcConfigDir) menuconfig
	
clean:
	$(MAKE) -C $(CmcBuildDir) clean
	
install:
	cp -p $(TargetDir)/bcm93382CMC_fs_kernel_cm $(TftpRoot)

distclean:
	rm -f $(TftpRoot)/bcm93382CMC_fs_kernel_cm
	rm -rf Src
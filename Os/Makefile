WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

LinuxBuildDir=linux-2.6.27.18
.PHONY: all configure patch added build install saveconfig getconfig menuconfig clean

all: build 
	@echo Done!

decompress:
	tar -xvf linux-2.6.27.18.tar.gz 
	tar -xvf soc-0.9.0.tar.gz
	$(CD) soc-0.9.0; tar -xvf keystone-OPENSOURCE-0.9.0.tar

# apply the patch
Ver = 2.6.27.18
SocRoot = $(CURDIR)/soc-0.9.0
SocMakeDir = $(SocRoot)/systems/linux/keystone/kernel
LinuxPatchSource = $(SocRoot)/systems/linux/keystone/kernel/patches/linux-$(Ver)
LinuxPatchConfig = $(SocRoot)/systems/linux/keystone/kernel/configs/linux-$(Ver)
LinuxBuildDir = $(CURDIR)/linux-$(Ver)
PatchFiles = $(shell ls $(LinuxPatchSource)/*.patch 2> /dev/null)

patch: decompress
	@echo "PATCHING LINUX KERNEL ..."
	cd $(LinuxBuildDir); \
	for patch in $(PatchFiles); do\
		echo "=== Applying patches from $$patch";\
		patch -p1 < $$patch || exit 1;\
	done
	@echo "... KERNEL PATCHED"

RsyncCommand = rsync -vrbuWptCL --suffix=.backup --backup --exclude=CVS
added: patch
	$(RsyncCommand) $(SocMakeDir)/added/ $(LinuxBuildDir)/
	$(RsyncCommand) $(SocRoot)/systems/shared/bcmsi/src/ $(LinuxBuildDir)/arch/mips/brcm/bcmsi/
	$(RsyncCommand) $(SocRoot)/systems/shared/bcmsi/include/ $(LinuxBuildDir)/include/asm-mips/bcmsi/
	$(RsyncCommand) $(SocRoot)/systems/shared/gmac/ $(LinuxBuildDir)/drivers/net/gmac/

	#modify linux-2.6.27.18/drivers/net/gmac/etc.c, orelse switch manage port wont work.
	@echo "Patching for Bcm5300 ..."
	$(CD) $(LinuxBuildDir); patch -p1 < ../patches/001.etc.c.patch || exit 1;

	#copy file and apply patch for read only file system "squashfs")
	#cp -R squashfs linux-2.6.27.18/fs
	#cd $(LinuxBuildDir); patch -p1 < ../patches/003.magic.h.patch || exit 1;
	#cd $(LinuxBuildDir); patch -p1 < ../patches/004.fs.Kconfig.patch || exit 1;
	#cd $(LinuxBuildDir); patch -p1 < ../patches/005.fs.Makefile.patch || exit 1;
	#cd $(LinuxBuildDir); patch -p1 < ../patches/100.export.c.patch || exit 1;
	#modify linux-2.6.27.18/drivers/mtd/maps/bcm953000-flash.c to 匹配linux分区和cfe分区
	#cd $(LinuxBuildDir); patch -p1 < ../patches/002.bcm953000-flash.c.patch || exit 1;
	
	#apply patch if Flash version is 1.5.   
	#Flash 1.3 ID: S29GL512P10TFIR1
	#Flash 1.5 ID: S29GL512S10TFI01
	cd $(LinuxBuildDir); patch -p1 < ../patches/006.cfi_cmdset_0002.c.patch || exit 1;
	@echo "Bcm5300 ... patched"

configure: added
	$(RsyncCommand) $(LinuxPatchConfig)/ $(LinuxBuildDir)/arch/mips/configs/
	cd $(LinuxBuildDir); make ARCH=mips bcm5300x_defconfig
	$(CP) save.config $(LinuxBuildDir)/.config

build: 
	$(MAKE) -C $(LinuxBuildDir) ARCH=mips CROSS_COMPILE=mips_4KC- all

install:
	$(CP) $(LinuxBuildDir)/vmlinux $(TftpRoot)
	$(Strip) $(TftpRoot)/vmlinux

saveconfig:
	$(CP) $(LinuxBuildDir)/.config save.config

getconfig:
	$(CP) save.config $(LinuxBuildDir)/.config

menuconfig:
	#at least, we should config "support jffs2", "CONFIG_CMDLINE".
	# and clear USB support, SCSI support.
	cd $(LinuxBuildDir); make ARCH=mips menuconfig

clean:
	$(RM) -r  soc-0.9.0 linux-$(Ver)
	$(RM) $(TftpRoot)/vmlinux

	
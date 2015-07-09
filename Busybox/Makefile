WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

Version=1.22.1

ifeq ($(Version), 1.0)
	#can't specify compiler by "make CROSS_COMPILE=xxxx" for busybox1.0. In order
	#to use other compiler than mips_4KC-gcc, change configuration by "make menuconfig"
	FullName = busybox
else
	FullName = busybox-$(Version)
	Env = CROSS_COMPILE=$(CrossPrefix) CONFIG_PREFIX=$(TftpRoot)/Install
endif
SourceDir=$(FullName)
SourceTar=$(FullName).tar.bz2
PatchDir   := $(CURDIR)/patches
PatchFiles := $(wildcard $(PatchDir)/busybox-$(Version)-*.patch)
OldCfgFile=save.$(Version).config

.PHONY: all configure patch build menuconfig getconfig install
all: build
	@

configure: 
	$(RM) -r $(SourceDir)
	tar -xvf $(SourceTar)
	if [ -f $(OldCfgFile) ]; \
	then \
		$(CP) $(OldCfgFile) $(SourceDir)/.config; \
	else \
		$(MAKE) -C $(SourceDir) allnoconfig; \
	fi

	@echo "Paching Busybox ..."
	cd $(SourceDir); \
	for patch in $(PatchFiles); do\
		echo "=== Applying patches from $$patch";\
		patch -p1 < $$patch || exit 1;\
	done
	@echo "... Busybox pathed"
	
build:
	$(MAKE) -C $(SourceDir) $(Env)

menuconfig:
	cd $(SourceDir); make menuconfig

saveconfig:
	$(CP) $(SourceDir)/.config $(OldCfgFile)

getconfig:
	$(CP) $(OldCfgFile) $(SourceDir)/.config
    
install:
	$(RM) -r $(TftpRoot)/Install; mkdir $(TftpRoot)/Install
	$(MAKE) -C $(SourceDir) install $(Env)
	$(Strip) $(TftpRoot)/Install/bin/busybox
	

clean:
	$(RM) -r $(SourceDir) $(TftpRoot)/Install
	


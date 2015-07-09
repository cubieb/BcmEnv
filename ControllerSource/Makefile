WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

BuildDir = $(CURDIR)/CmcController/build
BuildEnv = CMCOAM_MSGMODE=TCP CMCOAM_DMMMODE=CDOCSIS CROSS=$(CrossPrefix)

.PHONY: all liboamsdk configure install clean

all: liboamsdk
	$(MAKE) -C $(BuildDir) all $(BuildEnv) CC=$(CC)

liboamsdk: 
	#Note,  if cross tools is mips_4KC-, 
	#modify build/makefile_linux_lib not to use -fstack-protector-all gcc option.
	$(MAKE) -C $(BuildDir) -f makefile_linux_lib $(BuildEnv)

PatchFiles = $(shell ls patches/*.patch 2> /dev/null)
configure:
	tar -xvf CmcController-1.1.05.tar.gz
	for patch in $(PatchFiles); do\
		echo "=== Applying patches from $$patch";\
		patch -p0 < $$patch || exit 1;\
	done

install: 
	$(CP) $(BuildDir)/cmcCli_linuxuser $(TftpRoot)
	$(CD) $(BuildDir); $(CP) libBcmOamSdk.a $(CommlibDir)
	$(CD) CmcController/opensource/cli_parser-0.5/build/unix/lib; $(CP) libcparser.a $(CommlibDir)
	
clean:
	$(CD) $(BuildDir); $(MAKE) all-clean
	$(RM) $(CommlibDir)/*




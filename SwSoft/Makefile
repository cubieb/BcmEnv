WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

#CppFlags += -Dxxxx
#export CppFlags

IncludePath += -I$(WorkspaceRoot)/SwSoft/Include
export IncludePath

BuildDir	= $(WorkspaceRoot)/SwSoft/Build
BuildLibDir = $(BuildDir)/Lib
LibPath	+= -L$(BuildLibDir)
export LibPath

#LdFlags += -lxxx
#export LdFlags

SharedLibVer = 0
export SharedLibVer

IncDirs = Include
ConfigDirs = Configuration
ifeq ($(strip $(CrossCompile)),yes)
    LibDirs = Common
    ExeDirs = Switch Controller MyEcho Daemon
    SubDirs = $(LibDirs) $(ExeDirs)
else
    LibDirs	= Common
    ExeDirs = Controller MyEcho Daemon
    SubDirs = $(LibDirs) $(ExeDirs)
endif    

all: $(SubDirs)
$(ExeDirs): $(LibDirs)

update: 	
	for i in $(shell find $(SubDirs) $(IncDirs) $(ConfigDirs) -type d); do \
		$(MAKE) --makefile=$(WorkspaceRoot)/Make/Make.UpdateCodes -C $$i; \
	done

install: 
	$(Strip) $(BuildDir)/Switch/switch.exe
	$(CP) -p $(BuildDir)/Switch/switch.exe $(TftpRoot)
#	$(Strip) $(BuildDir)/Controller/controller.exe
	$(CP) -p $(BuildDir)/Controller/controller.exe $(TftpRoot)
	$(Strip) $(BuildDir)/Daemon/daemon.exe
	$(CP) -p $(BuildDir)/Daemon/daemon.exe $(TftpRoot)
	$(CP) -p $(BuildDir)/MyEcho/myecho.exe $(TftpRoot)

.PHONY: all update install

include $(WorkspaceRoot)/Make/Make.SubDirs


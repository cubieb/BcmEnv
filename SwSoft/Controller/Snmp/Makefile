WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller/Snmp/SNMPv2-MIB
IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller/Snmp/GOSPELL-DSL-SYSCTRL-MIB
IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller/Snmp/DOCS-IF-MIB
IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller/Snmp/GOSPELL-AN-EOC-CMC-MIB
export IncludePath

CXX 	= $(C++)

ifeq ($(SharedLibVer), 1)
    LibFile	=lib$(notdir $(CURDIR)).so
else
    LibFile	=lib$(notdir $(CURDIR)).a
    include $(WorkspaceRoot)/Make/Make.StaticLib
endif

SubDirs = SNMPv2-MIB GOSPELL-DSL-SYSCTRL-MIB DOCS-IF-MIB GOSPELL-AN-EOC-CMC-MIB
all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs

WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

CXX 	= $(C++)

ifeq ($(SharedLibVer), 1)
    LibFile	=lib$(notdir $(CURDIR)).so
else
    LibFile	=lib$(notdir $(CURDIR)).a
    include $(WorkspaceRoot)/Make/Make.StaticLib
endif

SubDirs = gsAnCmcVendorToVlanTable
all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs

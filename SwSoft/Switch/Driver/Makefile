WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

CXX 	= $(C++)

CustomizeTarget = $(BuildDir)/SocDialog.o
CustomizeSource = SocDialog.c
CustomizeCmd    = $(CC) -c $< -o $@ $(CppFlags) $(IncludePath)

ifeq ($(SharedLibVer), 1)
    LibFile	=lib$(notdir $(CURDIR)).so
else
    LibFile	=lib$(notdir $(CURDIR)).a
    include $(WorkspaceRoot)/Make/Make.StaticLib
endif


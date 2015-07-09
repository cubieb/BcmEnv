WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

ControllerSrcDir= $(WorkspaceRoot)/ControllerSource/CmcController
IncludePath += -I$(ControllerSrcDir)/base/include
IncludePath += -I$(ControllerSrcDir)/BcmController/include
IncludePath += -I$(ControllerSrcDir)/OamApi/include
IncludePath += -I$(ControllerSrcDir)/CmcCli/include

IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller

CXX 	= $(CC)

CustomizeTarget = cparser_tree.c
CustomizeSource = CliList.cli cmc.cli cmc.config.cli
CustomizeCmd    = ./mk_parser.py CliList.cli

ifeq ($(SharedLibVer), 1)
    LibFile	=lib$(notdir $(CURDIR)).so
else
    LibFile	=lib$(notdir $(CURDIR)).a
    include $(WorkspaceRoot)/Make/Make.StaticLib
endif

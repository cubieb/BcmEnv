WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

SubDirs = gsAnCmcCmcObjects gsAnCmcCmObjects gsAnCmcGlobalObject \
          gsCableDocsLoadBalObjects gsCableDocsServiceFlowObjects \
          gsCableDocsSpectrumObjects
          

all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs


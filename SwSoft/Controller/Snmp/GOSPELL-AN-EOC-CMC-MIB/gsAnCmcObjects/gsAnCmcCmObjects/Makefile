WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

SubDirs = gsAnCmcCmCpeTable gsAnCmcCmStatistics gsAnCmcCmUniTable \
                gsAnCmcCmTrafficTable

all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs


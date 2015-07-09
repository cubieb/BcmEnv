WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

SubDirs = gsCableDocsLoadBalGrpCfgTable gsCableDocsLoadBalMacRangeCfgTable \
	gsCableDocsLoadBalExcludeMacCfgTable gsCableDocsLoadBalSystem \
	gsCableDocsLoadBalMacRangeCfgTable

all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs


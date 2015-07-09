WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

SubDirs = docsIfUpstreamChannelTable docsIfDownstreamChannelTable

all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs


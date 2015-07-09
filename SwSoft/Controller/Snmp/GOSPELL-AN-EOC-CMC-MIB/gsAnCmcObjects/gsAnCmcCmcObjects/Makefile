WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

SubDirs = gsAnCmcSystem gsAnCmcUpstreamChannelTable \
          gsAnCmcDownstreamChannelTable gsAnCmcCdtTransSVidTable \
          gsAnCmcCdtTransSVidTable gsAnCmcChannelStatisticsTable \
          gsAnCmcPortStatisticsTable gsAnCmcVendorToVlanObjects \

all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs


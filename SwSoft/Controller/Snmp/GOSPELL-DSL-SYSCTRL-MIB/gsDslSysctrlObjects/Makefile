WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

SubDirs = gsDslAddress gsDslBoardTable gsDslCommon gsDslDhcpRelay \
          gsDslFtp gsDslSyslog gsDslTrapHostTable

all: $(SubDirs)
include $(WorkspaceRoot)/Make/Make.SubDirs


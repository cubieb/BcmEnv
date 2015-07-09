WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

CppFlags += -DNETWORKINGDEV_TCP -DDMM_CONTROLLER_CDOCSIS_MODE
CppFlags += -Wall -Werror 
export CppFlags

IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller/Include
IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller/Include/cli_parser-0.5
IncludePath += -I$(WorkspaceRoot)/SwSoft/Controller/FunctionModule

ControllerSrcDir= $(WorkspaceRoot)/ControllerSource/CmcController
IncludePath += -I$(ControllerSrcDir)/base/include
IncludePath += -I$(ControllerSrcDir)/BcmController/include
IncludePath += -I$(ControllerSrcDir)/OamApi/include
IncludePath += -I$(ControllerSrcDir)/CmcCli/include

SnmpBuildDirs = $(WorkspaceRoot)/NetSnmp/net-snmp-5.7.1
IncludePath += -I$(SnmpBuildDirs)/include
IncludePath += -I$(WorkspaceRoot)/SwSoft/Snmp
export IncludePath

#LibPath	+= -L$(XXXX)
#export LibPath

CommLibsBuild = -L$(CommlibDir) -Wl,-Bstatic -lBcmOamSdk -lcparser
ifeq ($(strip $(CrossCompile)),yes)
    SnmpLibsBuild = -L$(SnmpBuildDirs)/agent/.libs -L$(SnmpBuildDirs)/snmplib/.libs \
    				-Wl,-Bstatic -lnetsnmpmibs -lnetsnmpagent -lnetsnmp 
else
	SnmpLibsBuild = -lnetsnmpmibs -lnetsnmpagent -lnetsnmp
endif

LdFlags += -lCli -lSnmp
LdFlags += -lsystem -linterfaces -lifTable
LdFlags += -lgsDslAddress -lgsDslCommon -lgsDslFtp -lgsDslSyslogHostTable -lgsDslTrapHostTable 
LdFlags += -lgsDslBoardTable -lgsDslDhcpRelay -lgsDslSysctrlTrapObjects
LdFlags += -ldocsIfDownstreamChannelTable -ldocsIfUpstreamChannelTable
LdFlags += -lgsAnCmcExpandTrap
LdFlags += -lgsCableDocsLoadBalGrpCfgTable -lgsCableDocsLoadBalMacRangeCfgTable
LdFlags += -lgsCableDocsLoadBalExcludeMacCfgTable -lgsCableDocsServiceFlowCfgTable
LdFlags += -lgsCableDocsLoadBalSystem -lgsAnCmcCmCpeTable -lgsAnCmcCmStatistics
LdFlags += -lgsAnCmcCmUniTable -lgsAnCmcCmTrafficTable -lgsAnCmcSystem
LdFlags += -lgsAnCmcUpstreamChannelTable -lgsAnCmcDownstreamChannelTable
LdFlags += -lgsCableDocsUsSpectrumSystem -lgsAnCmcCdtTransSVidTable
LdFlags += -lgsAnCmcChannelStatisticsTable -lgsAnCmcPortStatisticsTable 
LdFlags += -lgsAnCmcVendorToVlanObjects -lgsAnCmcVendorToVlanTable
LdFlags += -lgsAnCmcGlobalObject -lgsCableDocsDsQosCfgTable
LdFlags += -lFunctionModule -lCommon

LdFlags += $(CommLibsBuild) $(SnmpLibsBuild) 
LdFlags += -Wl,-Bdynamic -lstdc++ -lnsl -lpthread -lm -lrt -lcrypto 
ifeq ($(strip $(CrossCompile)),no)
	LdFlags += -Wall $(shell perl -MExtUtils::Embed -e ldopts)
endif 

CXX 	= $(C++)
ExeFile=controller.exe

SubDirs = FunctionModule Cli Snmp

all: $(ExeFile) $(SubDirs)
$(ExeFile): $(SubDirs)

include $(WorkspaceRoot)/Make/Make.Exec
include $(WorkspaceRoot)/Make/Make.SubDirs

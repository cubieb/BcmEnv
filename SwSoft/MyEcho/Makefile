WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

#CppFlags += -Dxxxx
#export CppFlags

IncludePath += -I$(SwSoftRoot)/Include 
SnmpBuildDirs = $(WorkspaceRoot)/NetSnmp/net-snmp-5.7.1
IncludePath += -I$(SnmpBuildDirs)/include
IncludePath += -I$(WorkspaceRoot)/SwSoft/Snmp
IncludePath += -I$(WorkspaceRoot)/Os/linux-2.6.27.18/include
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

LdFlags += $(CommLibsBuild) $(SnmpLibsBuild) 
LdFlags += -Wl,-Bdynamic -lstdc++ -lnsl -lpthread -lm -lrt -lcrypto 
ifeq ($(strip $(CrossCompile)),no)
	LdFlags += -Wall $(shell perl -MExtUtils::Embed -e ldopts)
endif 

CXX 	= $(C++)

ExeFile=myecho.exe
include $(WorkspaceRoot)/Make/Make.Exec

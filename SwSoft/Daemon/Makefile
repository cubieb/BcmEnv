WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

#CppFlags += -Dxxxx
#export CppFlags

IncludePath += -I$(SwSoftRoot)/Include 
export IncludePath

#LibPath	+= -L$(XXXX)
#export LibPath

LdFlags += -lCommon
LdFlags += -lstdc++ -static -lnsl -lm -lrt

CXX = $(C++)

ExeFile=daemon.exe
include $(WorkspaceRoot)/Make/Make.Exec

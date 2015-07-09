WorkspaceRoot=$(shell FindRootPath.sh $(CURDIR))
include $(WorkspaceRoot)/Make/Make.Configuration

CppFlags += -O2 -D_REENTRANT -DLINUX -DNO_BCM_5690_A0 -DNO_BCM_5670_A0 -DNO_BCM_5673_A0 -DNO_BCM_5674_A0 -DNO_BCM_5665_A0 -DNO_BCM_5665_B0 -DNO_BCM_5650_C0 -DNO_BCM_5695_A0 -DNO_BCM_56601_A0 -DNO_BCM_56601_B0 -DNO_BCM_56601_C0 -DNO_BCM_56602_A0 -DNO_BCM_56602_B0 -DNO_BCM_56602_C0 -DSYS_BE_PIO=0 -DSYS_BE_PACKET=1 -DSYS_BE_OTHER=0 -DBE_HOST=1 -DKEYSTONE -DBCM_PLATFORM_STRING="KEYSTONE" -DBCM_ALL_CHIPS -DINCLUDE_BCMX -DINCLUDE_BCMX_DIAG -DINCLUDE_BFD -DINCLUDE_BHH -DINCLUDE_CES -DINCLUDE_CHASSIS -DINCLUDE_CINT -DINCLUDE_CUSTOMER -DINCLUDE_EDITLINE -DINCLUDE_FIELD -DINCLUDE_I2C -DINCLUDE_KNET -DINCLUDE_L3 -DINCLUDE_MEM_SCAN -DINCLUDE_PTP -DINCLUDE_RCPU -DINCLUDE_TEST -DINCLUDE_MSTP -DEBBUS_BIT16 -DBCM_EA_SUPPORT -DBCM_ROBO_SUPPORT -DBCM_RPC_SUPPORT -DBCM_ESW_SUPPORT -DBCM_TK371X_SUPPORT -DBCM_SHADOW_SUPPORT -DINCLUDE_LIB_CPUDB -DINCLUDE_LIB_CPUTRANS -DINCLUDE_LIB_DISCOVER -DINCLUDE_LIB_STKTASK -DDISCOVER_APP_DATA_BOARDID -DINCLUDE_LIB_CINT -DCINT_CONFIG_INCLUDE_SDK_SAL=1 -DCINT_CONFIG_INCLUDE_PARSER=1 -DCINT_CONFIG_INCLUDE_CINT_LOAD=0 -DVENDOR_CALHOUN -DVENDOR_GAMMA -DVENDOR_BROADCOM -DINCLUDE_PHY_522X -DINCLUDE_PHY_54XX -DINCLUDE_PHY_5464 -DINCLUDE_PHY_5421S -DINCLUDE_PHY_5482 -DINCLUDE_PHY_54616 -DINCLUDE_PHY_54680 -DINCLUDE_PHY_54680E -DINCLUDE_PHY_52681E -DINCLUDE_PHY_54880E -DINCLUDE_PHY_54682 -DINCLUDE_PHY_54684 -DINCLUDE_PHY_54640 -DINCLUDE_PHY_54640E -DINCLUDE_PHY_54880 -DINCLUDE_PHY_SERDES -DINCLUDE_PHY_SIMUL -DINCLUDE_PHY_8703 -DINCLUDE_PHY_8705 -DINCLUDE_PHY_8706 -DINCLUDE_PHY_8072 -DINCLUDE_PHY_8040 -DINCLUDE_PHY_8481 -DINCLUDE_PHY_8750 -DINCLUDE_PHY_8729 -DINCLUDE_PHY_84740 -DINCLUDE_PHY_84756 -DINCLUDE_PHY_54380 -DINCLUDE_PHY_542XX -DINCLUDE_PHY_84334 -DINCLUDE_PHY_84728 -DINCLUDE_PHY_84749 -DINCLUDE_PHY_84328 -DINCLUDE_LONGREACH -DBCM_CES_SDK -DAG_GCC -DAG_STAM -DAG_PC_PROG -DBE_HOST=1 -D__BSD_SOURCE -DUNIX -Wall -Werror -fno-strict-aliasing
CppFlags += -DBROADCOM_DEBUG -g -DLINUX_SAL_DMA_ALLOC_OVERRIDE
export CppFlags

IncludePath += -I$(WorkspaceRoot)/SwSoft/Switch/Include
####Start, Bcm SDK include files.
SdkRoot = $(WorkspaceRoot)/Sdk/sdk-all-6.2.2
IncludePath += -I$(SdkRoot)/include 
IncludePath += -I$(SdkRoot)/include/soc/robo 
IncludePath += -I$(SdkRoot)/include/bcm_int/robo 
IncludePath += -I$(SdkRoot)/src/soc/robo 
IncludePath += -I$(SdkRoot)/include/soc/ea 
IncludePath += -I$(SdkRoot)/include/bcm_int/ea 
IncludePath += -I$(SdkRoot)/systems  
IncludePath += -I$(SdkRoot)/include/soc/ces/nemo_driver 
IncludePath += -I$(SdkRoot)/include/soc/ces/include_bcm 
IncludePath += -I$(SdkRoot)/include/soc/ces/include_int 
IncludePath += -I$(SdkRoot)/include/soc/ces/include_ext 
IncludePath += -I$(SdkRoot)/include/soc/ces/clsbuilder 
IncludePath += -I$(SdkRoot)/systems/linux/kernel/modules/include 
IncludePath += -I$(SdkRoot)/systems/bde/linux/include
IncludePath += -I$(SdkRoot)/systems/bde/linux/include
####End, Bcm SDK include files.
export IncludePath

#LibPath	+= -L$(XXXX)
#export LibPath

BcmLibs = version.o platform_defines.o
BcmLibs += libdiag.a libdiag_esw.a libdiag_bcmx.a libtest.a libdiscover.a libstktask.a \
	libacl.a libdiagcint.a libcint.a libdiagapi.a libsal_appl_editline.a libuser.a \
	libsal_appl.a libsal_appl_plat.a liblubde.a \
	libbcmx.a libboard_mgr.a libboard_dvr.a libboard_stk.a libboard_spt.a \
	libbcm.a libbcm_rpc.a libbcm_compat.a libbcm_esw.a \
	libcputrans.a libcpudb.a libsoccommon.a libptp.a libfirebolt.a libtitan.a \
	libtrident.a libhumv.a libbradley.a libherc.a libscorpion.a libtriumph.a \
	libtrx.a libtriumph2.a libenduro.a libkatana.a libflexctr.a \
	libbcm_esw.a libsoc_esw.a libsoc.a libsoc_phy.a libsoc_wcmod.a \
	libsoc_tscmod.a libsoc_mcm.a libbcm_regex.a libxgs5.a libsoc_fcmap.a libbcm_common.a \
	libsoc_shared.a libshared.a libsoc_i2c.a libsal_core.a libsal_core_plat.a \
	libcustomer.a libsoc_nemo.a libsoc_clsbuilder.a libsoc_sal.a libacl.a
BcmLibsBuild = $(addprefix $(SdkRoot)/build/unix-user/keystone-2_6/,$(BcmLibs)) 
SwLibsBuild = $(WorkspaceRoot)/SwSoft/Build/Switch/Driver/SocDialog.o

LdFlags  = -lQos -lL2 -lL3 -lDriver -lStatistics -lCommon 
LdFlags += -Wl,--start-group $(SwLibsBuild)  $(BcmLibsBuild) -Wl,--end-group
LdFlags += -lstdc++ -static -lnsl -lpthread -lm -lrt
export LdFlags

CXX 	= $(C++)
ExeFile=switch.exe

SubDirs = Driver L2 L3 Qos Statistics

all: $(ExeFile) $(SubDirs)
$(ExeFile): $(SubDirs)

include $(WorkspaceRoot)/Make/Make.Exec
include $(WorkspaceRoot)/Make/Make.SubDirs


#include "SystemInclude.h"
#include "BcmController.h"

#include "Debug.h"
#include "ChannelMgt.h"
#include "LoadBalanceMgt.h"
#include "DhcpRelayMgt.h"
#include "UpdateMgt.h"
#include "CliMain.h"
#include "DsQosConfigMgt.h"
#include "CnuStatusMgt.h"
#include "SystemInfoMgt.h"
#include "SnmpCfgMgt.h"
#include "SystemInfoMgt.h"
#include "ControllerInit.h"

using namespace std;
ControllerInit::ControllerInit()
{
    oamHandle = NULL;
}

ControllerInit::~ControllerInit()
{
    if (oamHandle != NULL)
    {
        CMCCTRL_Destroy();
        CMCOAM_Platform_Destroy(reinterpret_cast<CMCOAM_Handler *>(oamHandle));
    }
}

bool ControllerInit::InitStep1()
{
    /*Initialize the BRCM OAM platform*/
    oamHandle = CMCOAM_Platform_Init(const_cast<char *>("vlan2"),
                                     const_cast<char *>("OAM"));
    if(NULL == oamHandle)
    {
        cerr << "Create OAM Platform Failed!" << endl;
        return false;
    }

    dbgstrm << "CMCCTRL_Init!\n" << endl;
    CMCCTRL_Init();

    InitCli();
    return true;
}

bool ControllerInit::InitStep2()
{
    InitUpdate();
    InitDhcpRelay();
    InitLoadBalance();

    /**call back function**/
    initUpdateCmOnlineNumberHandlerMgt();
    initUpdateCmIpHandlerMgt();
    initUpdateCmOnlineStatusHandlerMgt();
    
    InitSnmpCfg();

    /*Initialize Mgt channel module*/
    if(MgtChannelInitialize() != 0)
    {
        cerr << "Initialize Mgt channel fail!" << endl;
    }

    return true;
}

bool ControllerInit::InitStep3()
{
    ReadCliConfig();

    StartDhcpRealy();
    
    //edit by leonli
    StartSystemInfoCherk();

    return true;
}

void ControllerInit::DeinitStep3()
{
    DeinitUpdate();
    DeinitDhcpRelay();
    DeinitLoadBalance();

     /*Initialize Mgt channel module*/
    if(MgtChannelDeInit() != 0)
         cerr << "DeInitialize channel fail !" << endl;

    uint_t level = GetRebootLevel();
    if (level >= RebootLevelRebootCmc)
    {
        cout << "Reboot Cmc Board." << endl;
        char* rebootCmd = "shell reboot";
        CMCOAM_System_SetRemoteCmd(rebootCmd, strlen(rebootCmd), cliGetCmc());
        //if(CMCCTRL_UnRegisterCmc(cliGetCmc()) != 0)
        //    cerr << "CMCCTRL_UnRegisterCmc fail !" << endl;
        //else
        //    cout << "UnRegister Cmc Success!" <<endl;
    }
}

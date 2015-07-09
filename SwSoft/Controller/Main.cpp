#include "SystemInclude.h"
#include <pthread.h>
#include <syslog.h>
#include <sys/reboot.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "InitTask.h"
#include "ControllerInit.h"
#include "Debug.h"
#include "SnmpAgent.h"
#include "CliMain.h"
#include "SystemInfoMgt.h"
#include "LinuxCmd.h"

using namespace std;
ControllerInit ControllerInit::controllerInitInstant;
pthread_mutex_t mgtMutex = PTHREAD_MUTEX_INITIALIZER;

static void CtrlCHander(int signal)
{
    StopCli();
}

/*
 *
 */
int main()
{
    struct sigaction sa;
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &CtrlCHander;
    sigaction (SIGINT, &sa, NULL);

    InitTask<ModuleInit>& initTask = InitTask<ModuleInit>::GetInstance();
    initTask.Init();

    CreateSnmpAgent(SNMP_MASTER_AGENT_NAME,
                    "/etc/snmp/snmpd.conf",
                    "/usr/share/snmp/mibs",
                    0, 0);
    StartCli();

    initTask.Deinit();

    uint_t level = GetRebootLevel();
    if (level >= RebootLevelRebootSw)
    {
        reboot(RB_AUTOBOOT);
    }

    return 0;
}


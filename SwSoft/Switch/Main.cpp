#include "SystemInclude.h"

#include "BcmSdk.h"
#include <soc/phyctrl.h>
#include <soc/phy.h>

#include "Debug.h"
#include "InitTask.h"
#include "DriverInit.h"
#include "QosInit.h"
#include "L2Init.h"
#include "L3Init.h"
#include "L3.h"

L2Init L2Init::l2InitInstant;
L3Init L3Init::l3InitInstant;
DriverInit DriverInit::driverInitInstant;
QosInit QosInit::qosInitInstant;

int main()
{
    chdir("/etc");

    InitTask<ModuleInit>& initTask = InitTask<ModuleInit>::GetInstance();
    initTask.Init();

    //L3HostRoute& l3HostRoute = L3HostRoute::GetInstance();
    while (true)
    {
        //l3HostRoute.OnSecondTick();
        sleep(1);
    }
    return 0;
	
}


#include "SystemInclude.h"

#include "BcmSdk.h"
#include <syslog.h>

#include "Util.h"
#include "Debug.h"
#include "DriverRx.h"
#include "L2.h"
#include "L2Init.h"

using namespace std;
L2Init::L2Init()
{
    dbgstrm << "Do nothing." << endl;
}

L2Init::~L2Init()
{
}

bool L2Init::InitStep1()
{
    dbgstrm << "Do nothing." << endl;
    return true;
}

bool L2Init::InitStep2()
{
    dbgstrm << "Do nothing." << endl;
    return true;
}

bool L2Init::InitStep3()
{
    DriverRx& rx = DriverRx::GetInstance();

    dbgstrm << "register arp rx callback function." << endl;
    rx.Regiser(L2ArpRxCallback);

    return true;
}


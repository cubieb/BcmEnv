#include "SystemInclude.h"

#include "BcmSdk.h"

#include "Util.h"
#include "Debug.h"
#include "DriverRx.h"
#include "L2.h"
#include "L3.h"
#include "L3Init.h"

using namespace std;
L3Init::L3Init()
{
    dbgstrm << "Do nothing." << endl;
}

L3Init::~L3Init()
{
}

bool L3Init::InitStep1()
{
    dbgstrm << "Do nothing." << endl;
    return true;
}

bool L3Init::InitStep2()
{
    return true;
}

bool L3Init::InitStep3()
{
    ArpRx& arpRx = ArpRx::GetInstance();
    arpRx.Regiser(L3ArpRxCallback);
    return true;
}


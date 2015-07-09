#include "SystemInclude.h"
#include "Debug.h"

#include "DebugMgt.h"

using namespace std;

void SetDebugStateOn(const char* funcName)
{
    DebugFlag flag;
    flag.SetState(funcName, true);
}

void SetDebugStateOff(const char* funcName)
{
    DebugFlag flag;
    flag.SetState(funcName, false);
}



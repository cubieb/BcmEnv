#include "SystemInclude.h"

#include "Debug.h"
#include "InitTask.h"
#include "ModuleInit.h"

using namespace std;

ModuleInit::ModuleInit()
{
    dbgstrm << "Start." << endl;
    InitTask<ModuleInit>& initTask = InitTask<ModuleInit>::GetInstance();

    initTask.Register(*this);
}

string ModuleInit::GetClassName()
{
    return typeid(*this).name();
}


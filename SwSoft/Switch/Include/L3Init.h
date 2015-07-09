#ifndef _L3Init_h_
#define _L3Init_h_

#include "ModuleInit.h"
class L3Init : public ModuleInit
{
public:
    bool InitStep1();
    bool InitStep2();
    bool InitStep3();

    static L3Init l3InitInstant;
private:
    L3Init();
    ~L3Init();
};

#endif

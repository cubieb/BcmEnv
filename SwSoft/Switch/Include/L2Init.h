#ifndef _L2Init_h_
#define _L2Init_h_

#include "ModuleInit.h"
class L2Init : public ModuleInit
{
public:
    bool InitStep1();
    bool InitStep2();
    bool InitStep3();

    static L2Init l2InitInstant;
private:
    L2Init();
    ~L2Init();
};

#endif

#ifndef _DriverInit_h_
#define _DriverInit_h_

#include "ModuleInit.h"
class DriverInit : public ModuleInit
{
public:
    bool InitStep1();
    bool InitStep2();
    bool InitStep3();

    static DriverInit driverInitInstant;
private:
    DriverInit();
    ~DriverInit();
};

extern "C" void knet_kcom_config(void);
extern "C" void diag_shell(void);

#endif

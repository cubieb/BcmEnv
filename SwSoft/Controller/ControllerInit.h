#ifndef _DriverInit_h_
#define _DriverInit_h_

#include "ModuleInit.h"
#include "BcmController.h"

class ControllerInit : public ModuleInit
{
public:
    bool InitStep1();
    bool InitStep2();
    bool InitStep3();

    void DeinitStep3();

    static ControllerInit controllerInitInstant;
private:
    ControllerInit();
    ~ControllerInit();
    CMCOAM_Handler* oamHandle;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

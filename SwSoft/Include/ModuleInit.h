#ifndef _ModuleInit_h_
#define _ModuleInit_h_

/* ModuleInit,  all modules' base class which need to be initialize */
class ModuleInit
{
public:
    ModuleInit();
    virtual ~ModuleInit() {}

    virtual bool InitStep1() { return true; }
    virtual bool InitStep2() { return true; }
    virtual bool InitStep3() { return true; }

    virtual void DeinitStep3() {}
    virtual void DeinitStep2() {}
    virtual void DeinitStep1() {}

    virtual std::string GetClassName();
};

#endif

#ifndef _QosInit_h_
#define _QosInit_h_

#include "ModuleInit.h"
class QosInit : public ModuleInit
{
public:
    bool InitStep1();
    bool InitStep2();
    bool InitStep3();

    static QosInit qosInitInstant;
private:
    QosInit();
    ~QosInit();
};

#endif  //_QosInit_h_

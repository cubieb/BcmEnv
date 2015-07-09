#ifndef _InitTask_h_
#define _InitTask_h_

/*******************************************/
/* InitTask, provide three API (InitStep1, InitStep2, InitStep3) called
* main() function, so all registered module could be initialized step
* by step.
*/
template<class M>
class InitTask
{
public:
    typedef bool (M::* DoInitStep)();
    typedef void (M::* DoDeinitStep)();

    void Register(M& module)
    { 
        std::cout << "InitTask::Register> Start." << std::endl;
        moduleList.push_back(&module);
    }

    void Init()
    {
        InitStepN(&M::InitStep1);
        InitStepN(&M::InitStep2);
        InitStepN(&M::InitStep3);
    }

    void Deinit()
    {
        DeinitStepN(&M::DeinitStep3);
        DeinitStepN(&M::DeinitStep2);
        DeinitStepN(&M::DeinitStep1);
    }

    static InitTask& GetInstance()
    {
        static InitTask<M> instance;
        return instance;
    }

private:
    InitTask()
    {
        std::cout << "InitTask::InitTask()> Start." << std::endl;
    }

    void InitStepN(DoInitStep step)
    {
        typename std::list<M*>::iterator iter;
        for (iter = moduleList.begin(); iter != moduleList.end(); ++iter)
        {
            std::string className = (*iter)->GetClassName();

            if (!((*iter)->*step)())
            {
                std::cerr << "Failed to initialize " << className << std::endl;
            }
        }
    }

    void DeinitStepN(DoDeinitStep step)
    {
        typename std::list<M*>::iterator iter;
        for (iter = moduleList.begin(); iter != moduleList.end(); ++iter)
        {
            std::string className = (*iter)->GetClassName();

            ((*iter)->*step)();
        }
    }

    std::list<M*> moduleList;
};

#endif

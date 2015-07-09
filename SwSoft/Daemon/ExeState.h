#ifndef _ExeState_h_
#define _ExeState_h_

class ExeState
{
public:
    ExeState(const char* pidFile);
    bool IsRunning();

private:
    std::string file;

    int LockFile(int fd);
};

#endif

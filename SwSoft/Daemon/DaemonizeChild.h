#ifndef _DaemonizeChild_h_
#define _DaemonizeChild_h_

class DaemonizeChild
{
public:
    DaemonizeChild(const char* child, const char* wait);
    virtual ~DaemonizeChild();

    void Daemonize();
    int  GetPtyFd();

private:
    int ptyFd;
    std::string ptyName;
    std::string ttyName;
    std::string cldName;
    std::string waitMsg;

    bool OpenPtyFd();
    void WaitChildMsg();
};

#endif

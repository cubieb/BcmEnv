#include "SystemInclude.h"

#include "Debug.h"
#include "DaemonizeChild.h"
using namespace std;

DaemonizeChild::DaemonizeChild(const char* child, const char* wait)
    : ptyName("/dev/ptypX"),
      ttyName(""),
      cldName(child),
      waitMsg(wait)
{
    bool rv = OpenPtyFd();
    assert(rv);
}

DaemonizeChild::~DaemonizeChild()
{
    close(ptyFd);
}

/* eclipse can't resolve prototype for basename() */
char *basename(char const* path);

void DaemonizeChild::Daemonize()
{
    int mask = umask(0);

    pid_t pid = fork();
    assert(pid >= 0);

    if (pid != 0)
    {
        umask(mask);
        WaitChildMsg();
        return;
    }

    /* Become a session leader to lose controlling TTY. */
    setsid();

    /* Ensure future opens won't allocate controlling TTYs.
       SIGHUP, hang-up signal, 系统对SIGHUP信号的默认处理是终止收到该信号的进程。
                     所以若程序中没有捕捉该信号，当收到该信号时，进程就会退出. 该signal产生于以下情况:
       1 终端关闭时.该信号被发送到 session首进程以及作为job提交的进程(即用&符号提交的进程).
       2 session首进程退出时, 该信号被发送到该session中的前台进程组中的每一个进程
    */
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        syslog(LOG_USER, "can't ignore SIGHUP.");
        exit(1);
    }

    pid = fork();
    assert(pid >= 0);
    if (pid != 0)
    {
        exit(0);
    }
    
    /* Change the current working directory to the / directory. */
    chdir("/");

    /* Close all open file descriptors */
    int i;
    for (i = 0; i < 3; i++)
        close(i);

    /* In child, open the child's side of the tty.  */
    if (open(ttyName.c_str(), O_RDWR /*| O_NOCTTY*/) < 0)
    {
        syslog(LOG_USER, "Could not open tty");
        exit(1);
    }
    dup(0);
    dup(0);

    /* We should not set terminal attribute here,  because the child exe
     * will re-configure its terminal attribute.
     */
//    struct termios old_term;
//    struct termios new_term;
//    tcgetattr(STDIN_FILENO, &old_term);
//    new_term = old_term;
//    new_term.c_lflag &= ~(ICANON | ECHO | ECHONL);
//
//    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    const char* child = cldName.c_str();
    execl(child, basename(child), NULL);

    assert(false);
}

int  DaemonizeChild::GetPtyFd()
{
    return ptyFd;
}

bool DaemonizeChild::OpenPtyFd()
{
    struct stat stb;
    /* maybe, there is a bug in telnetd of busybox. we encounter error
     * when telnet.  In order to work around, we start out ptyName at
     * /dev/ptyp8
     */
    for (int i = 8; i < 16; ++i)
    {
        ptyName[9] = (i < 10 ? i + '0' : i - 10 + 'a');

        if (stat(ptyName.c_str(), &stb) < 0)
            continue;

        if ((ptyFd = open(ptyName.c_str(), O_RDWR | O_NOCTTY)) >= 0)
        {
            ttyName = ptyName;
            ttyName[5] = 't';
            return true;
        }
    }

    return false;
}

void DaemonizeChild::WaitChildMsg()
{
    dbgstrm << "start to wait child message." << endl;
    /* this is parent process. wait for bcm.user child initialization
     * then exit.  then user will configure interface address from
     * shell,  that should depends on the bcm.user's initialization.
     * if we can read the "BCM.0>" from bcm.user output, that means
     * the initialization has completed.
     */
    string msg(waitMsg.length(), ' ');
    assert(msg.length() > 1);

    char ch;
    while (read(ptyFd, &ch, 1) > 0)
    {
        cout << ch << flush;
        int i;
        for (i = 0; i < msg.length() - 1; ++i)
        {
            msg[i] =  msg[i+1];
        }
        msg[i] = ch;

        if (msg == waitMsg)
            break;
    }
}

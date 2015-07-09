#include "SystemInclude.h"

#include "Debug.h"
#include "ExeState.h"
#include "DaemonizeChild.h"

const char* switchExeName = "switch.exe";
const char* switchMsg  = "Switch Init Done!";
const char* controllerExeName = "controller.exe";
const char* controllerMsg  = "Controller#";
using namespace std;

void MakeChildReadPty(int ptyFd)
{
    pid_t pid = fork();
    if (pid != 0)
        return;

    /* I am child process, read ptyFd and print it out.*/
    char ch;
    /* we will read the same string as the DaemonizeChild::WaitCldMsg(),
     * because the child makes a copy of FD from parent, and the FD's
     * buffer has not been cleared. To avoid this, we seek to the end of
     * the file.
     */
    lseek(ptyFd, 0, SEEK_END);

    while (read(ptyFd, &ch, 1) > 0)
    {
        cout << ch << flush;
    }
}

static bool running = true;

static void CtrlCHander (int signal)
{
    running = false;
}

bool IsExecableFile(const char *name)
{
    struct stat state;
    if (access(name, X_OK) == 0)
    {
        if (stat(name, &state) == 0)
        {
            return S_ISREG(state.st_mode);
        }
    }

    return false;
}

/* just like linux which command. */
string Which(const char* exeFile)
{
    string path("/sbin:/usr/sbin:/bin:/usr/bin:/mnt:");

    string exePath;
    string::size_type begin, end;
    for (begin = 0, end = path.find_first_of(":", begin);
        end != string::npos;
        begin = end+1, end = path.find_first_of(":", begin))
    {
        exePath = path.substr(begin, end - begin);
        exePath.append("/");
        exePath.append(exeFile);
        if (IsExecableFile(exePath.c_str()))
        {
            return exePath;
        }
    }
    exePath.assign("");

    return exePath;
}

int main(int argc, char *argv[])
{
    struct termios old_term;
    struct termios new_term;

    struct sigaction sa;
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &CtrlCHander;
    sigaction (SIGINT, &sa, NULL);

    string switchPath = Which(switchExeName);
    assert(switchPath.length() > strlen(switchExeName));
    DaemonizeChild switchExe(switchPath.c_str(), switchMsg);
    switchExe.Daemonize();

    string controllerPath = Which(controllerExeName);
    assert(controllerPath.length() > strlen(controllerExeName));
    DaemonizeChild controllerExe(controllerPath.c_str(), controllerMsg);
    controllerExe.Daemonize();

    /* To support "Backspace",  we have to suppress (ICANON | ECHO),
     * and echo user input by ourself.
     * Caution:  stop echo user input when child process doing this.
     * for now,  bcm.user and controller.exe will echo user input.
     */
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO | ECHONL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    MakeChildReadPty(switchExe.GetPtyFd());
    MakeChildReadPty(controllerExe.GetPtyFd());

    int  ptyFd[2];
    ptyFd[0] = switchExe.GetPtyFd();
    ptyFd[1] = controllerExe.GetPtyFd();

    char ch;
    int index = 1;
    while((ch = getchar()) != EOF && running)
    {
        if (ch == '+')
        {
            index = index == 0 ? 1 : 0;
            continue;
        }
        write(ptyFd[index], &ch, 1);
    }

    return 0;
}

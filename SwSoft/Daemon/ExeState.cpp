#include "SystemInclude.h"

#include "ExeState.h"
using namespace std;

ExeState::ExeState(const char* pidFile)
    : file(pidFile)
{
}

int ExeState::LockFile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return(fcntl(fd, F_SETLK, &fl));
}

bool ExeState::IsRunning()
{
    int fd;
    fd = open(file.c_str(), O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    assert(fd >= 0);

    if (LockFile(fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            return true;
        }
        else
        {
            assert(false);
        }
    }
    ftruncate(fd, 0);

    char  buf[16];
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf)+1);
    return false;
}


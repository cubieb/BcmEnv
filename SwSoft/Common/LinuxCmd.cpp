#include "SystemInclude.h"

#include "LinuxCmd.h"

int  ExeLinuxCommand(const char* cmd, const char* para0)
{
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0)
    {
        execl(cmd, para0, (char*)NULL);
        return 0;
    }
    else
    {
        /* wait for child process. */
        int status;
        if (waitpid(pid, &status, 0) == pid)
        {
            return status;
        }
        return -1;
    }
}

int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1)
{
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0)
    {
        execl(cmd, para0, para1, (char*)NULL);
        return 0;
    }
    else
    {
        /* wait for child process. */
        int status;
        if (waitpid(pid, &status, 0) == pid)
        {
            return status;
        }
        return -1;
    }
}

int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2)
{
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0)
    {
        execl(cmd, para0, para1, para2, (char*)NULL);
        return 0;
    }
    else
    {
        /* wait for child process. */
        int status;
        if (waitpid(pid, &status, 0) == pid)
        {
            return status;
        }
        return -1;
    }
}

int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2,
                     const char* para3, const char* para4)
{
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0)
    {
        execl(cmd, para0, para1, para2, para3, para4, (char*)NULL);
        return 0;
    }
    else
    {
        /* wait for child process. */
        int status;
        if (waitpid(pid, &status, 0) == pid)
        {
            return status;
        }
        return -1;
    }
}

int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2,
                     const char* para3, const char* para4,
                     const char* para5, const char* para6)
{
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0)
    {
        execl(cmd, para0, para1, para2, para3, para4, para5, para6, (char*)NULL);
        return 0;
    }
    else
    {
        /* wait for child process. */
        int status;
        if (waitpid(pid, &status, 0) == pid)
        {
            return status;
        }
        return -1;
    }
}

int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2,
                     const char* para3, const char* para4,
                     const char* para5, const char* para6,
                     const char* para7)
{
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0)
    {
        execl(cmd, para0, para1, para2, para3, para4,
            para5, para6, para7, (char*)NULL);
        return 0;
    }
    else
    {
        /* wait for child process. */
        int status;
        if (waitpid(pid, &status, 0) == pid)
        {
            return status;
        }
        return -1;
    }
}







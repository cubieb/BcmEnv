#ifndef _LinuxCmd_h_
#define _LinuxCmd_h_

int  ExeLinuxCommand(const char* cmd, const char* para0);
int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1);
int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2);

int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2,
                     const char* para3, const char* para4);
                     
int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2,
                     const char* para3, const char* para4,
                     const char* para5, const char* para6);

int  ExeLinuxCommand(const char* cmd, const char* para0,
                     const char* para1, const char* para2,
                     const char* para3, const char* para4,
                     const char* para5, const char* para6,
                     const char* para7);
#endif

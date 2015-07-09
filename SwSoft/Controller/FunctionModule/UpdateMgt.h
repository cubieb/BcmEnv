#ifndef _UpdateMgt_h_
#define _UpdateMgt_h_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/stat.h> 
#include <sys/reboot.h>
#include <linux/compiler.h>
#include <mtd/mtd-user.h>

#define MaxObjectType   16
#define MaxUserName     64
#define MaxUserPassword 64
#define MaxFileName     64
#define MaxFilePath     64

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BASE_ErrCode UpdateSwitchRootFs(const char* tftpServerIp, const char* fileName);
BASE_ErrCode UpdateControllerConfigure(const char* tftpServerIp, const char* fileName);
BASE_ErrCode UpdateSwitchConfigure(const char* tftpServerIp, const char* fileName);
BASE_ErrCode SaveControllerConfigure(const char* tftpServerIp, const char* fileName);
BASE_ErrCode SaveSwitchConfigure(const char* tftpServerIp, const char* fileName);

#define FileTypeSwitchProfile     1
#define FileTypeControllerProfile 2
#define FileTypeRootFs            3

void InitUpdate();
void DeinitUpdate();

BASE_ErrCode UpdateSwitchConfigFromFtp(const char* ip, const char* user,
    const char* password, const char* file);
BASE_ErrCode UpdateControllerConfigFromFtp(const char* ip, const char* user,
    const char* password, const char* file);
BASE_ErrCode UpdateRootFsFromFtp(const char* ip, const char* user,
    const char* password, const char* file);

BASE_ErrCode UpdateFromFtp(const char* ip, const char* user, const char* password,
    const char* file, int32_t type);
void AsynUpdateFromFtp(const char* ip, const char* user, const char* password,
    const char* file, int32_t type);
int GetUpdateStatus();

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif


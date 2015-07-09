#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"
#include "LinuxCmd.h"
#include "AutoLock.h"
#include "UpdateMgt.h"

using namespace std;

#define READ_SIZE 1024
extern pthread_mutex_t mgtMutex;

ssize_t  safe_read(int fd, void *buf, size_t count)
{
	ssize_t n;

	do {
		n = read(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}

ssize_t  full_read(int fd, void *buf, size_t len, size_t time_len)
{
	ssize_t cc;
	ssize_t total;

	total = 0;

	while (len) {
		cc = safe_read(fd, buf, time_len);

		if (cc < 0) {
			if (total) {
				/* we already have some! */
				/* user can do another read to know the error code */
				return total;
			}
			return cc; /* read() returns -1 on failure. */
		}
		if (cc == 0)
			break;

              //cout << "read " << total << " bytes " << endl;
        
		buf = ((char *)buf) + cc;
		total += cc;
		len -= cc;
	}

	return total;
}

ssize_t safe_write(int fd, const void *buf, size_t count)
{
	ssize_t n;

	do {
		n = write(fd, buf, count);
	} while (n < 0 && errno == EINTR);

	return n;
}

ssize_t  full_write(int fd, const void *buf, size_t len)
{
	ssize_t cc;
	ssize_t total;

	total = 0;

	while (len) {
		cc = safe_write(fd, buf, len);

		if (cc < 0) {
			if (total) {
				/* we already wrote some! */
				/* user can do another write to know the error code */
				return total;
			}
			return cc;  /* write() returns -1 on failure. */
		}

		total += cc;
		buf = ((const char *)buf) + cc;
		len -= cc;
	}

	return total;
}

/*
 * Refer to busybox 1.22.1 function "flash_eraseall_main()";
 */
bool EraseFlash(int flashFd)
{
    int ret;

    //Get MTD information.
    struct mtd_info_user mtdInfo;
    ret = ioctl(flashFd, MEMGETINFO, &mtdInfo);
    if(ret < 0)
    {
        errstrm << "Error: can't get MTD device info from MTD device." << endl;
        return false;
    }
    dbgstrm << "type=" << mtdInfo.type << ", flags=" << mtdInfo.flags
        << ", size=" << mtdInfo.size << ", erasesize=" << mtdInfo.erasesize
        << endl;

    //Erase Flash.
     erase_info_t erase;
     erase.length = mtdInfo.erasesize;
     for(erase.start = 0; erase.start < mtdInfo.size; erase.start += mtdInfo.erasesize)
     {
         cout << "\rErasing " << (unsigned int)mtdInfo.erasesize / 1024 << "Kibyte "
             << "@ " << erase.start
             << ", " << (erase.start * 100) / mtdInfo.size << " % complete."
             << flush;

         ret = ioctl(flashFd, MEMERASE, &erase);
         if(ret < 0)
         {
             dbgstrm << "Erase failed for offset 0x" << hex << erase.start << endl;
             return false;
         }
     }
     cout << "Succeeded to erase flash." << endl;

    return true;
}

void UpdateRootFsFromImage(const char* image)
{
    int flashFd, imageFd;

    //Open MTD device.
    flashFd = open("/dev/mtd3", O_RDWR);
    if (flashFd < 0)
    {
        dbgstrm << "Error: Can't open /dev/mtd3." << endl;
        return;
    }
   
    imageFd = open(image, O_RDONLY, 0666);
    if(imageFd <0)
    {
        cout << "Open image file failed" << endl;
        goto OpenImageError;
    }

    //Get image size.
    size_t fileSize;
    struct stat status;
    if(stat(image, &status) < 0)
    {  
        dbgstrm << "Failed to get file status." << endl;
        goto StatImageError;
    }
    fileSize = (size_t)status.st_size;

    //Allocate buffer.
    char* imageBuffer;
    imageBuffer = (char*)malloc(fileSize);
    if(imageBuffer == NULL)
    {
        dbgstrm <<"Failed to allocate buffer." << endl;
        goto StatImageError;
    }

    //Read image
    ssize_t bytesReaden;
    bytesReaden = full_read(imageFd, imageBuffer, fileSize, READ_SIZE);
    if (bytesReaden < 0)
    {
        dbgstrm << "Failed to read the image." << endl;
        goto ReadImageError;;
    }

    if (lseek(flashFd, 0, SEEK_SET) < 0)
    {
        dbgstrm << "Failed to rewind after erasing." << endl;
        goto ReadImageError;
    }

    if (!EraseFlash(flashFd))
    {
        dbgstrm << "Failed to erase flash." << endl;
        goto ReadImageError;
    }

    //Write image
    cout << "Writing Image . . . . . " << endl;
    ssize_t bytesWritten = full_write(flashFd, imageBuffer, bytesReaden);
    if (bytesWritten < 0)
    {
        dbgstrm << "Failed to write the image" << endl;
        goto ReadImageError;
    }

    //return 0;
    reboot(RB_AUTOBOOT);
    return;

ReadImageError:
    free(imageBuffer);
StatImageError:
    close(imageFd);
OpenImageError:
    close(flashFd);
}

/*
 * Description:
 */
BASE_ErrCode UpdateSwitchRootFs(const char* tftpServerIp, const char* fileName)
{
    assert(tftpServerIp != NULL && fileName != NULL);
    int  ret = 0;
    const char* localFile = "/var/RootFs.img";

    AutoLock autoLock(mgtMutex);
    cout << "Start to download image file." << endl;
    ret = ExeLinuxCommand("/bin/tftp", "tftp", "-l", localFile,
        "-r", fileName, "-g", tftpServerIp);

    /* check tftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        errstrm << "Tftp downloading failed." << endl;
        return BASE_ERROR;
    }

    cout << "Start to write image to flash." << endl;
    UpdateRootFsFromImage(localFile);

    return BASE_OK;
}

BASE_ErrCode UpdateControllerConfigure(const char* tftpServerIp, const char* fileName)
{
    int  ret = 0;
    const char* localFile = "/etc/cmc_cfg_file.cli";
    assert(tftpServerIp != NULL && fileName != NULL);

    AutoLock autoLock(mgtMutex);
    cout << "Start to download file." << endl;
    ret = ExeLinuxCommand("/bin/tftp", "tftp", "-l", localFile,
        "-r", fileName, "-g", tftpServerIp);

    /* check tftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        errstrm << "Tftp downloading failed." << endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

BASE_ErrCode UpdateSwitchConfigure(const char* tftpServerIp, const char* fileName)
{
    int  ret = 0;
    const char* localFile = "/etc/rc.soc";
    assert(tftpServerIp != NULL && fileName != NULL);

    AutoLock autoLock(mgtMutex);
    cout << "Start to download file." << endl;
    ret = ExeLinuxCommand("/bin/tftp", "tftp", "-l", localFile,
        "-r", fileName, "-g", tftpServerIp);

    /* check tftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        dbgstrm << "Tftp downloading failed." << endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

BASE_ErrCode SaveControllerConfigure(const char* tftpServerIp, const char* fileName)
{
    int  ret = 0;
    const char* localFile = "/etc/cmc_cfg_file.cli";
    assert(tftpServerIp != NULL && fileName != NULL);

    AutoLock autoLock(mgtMutex);
    cout << "Start to upload file." << endl;
    ret = ExeLinuxCommand("/bin/tftp", "tftp", "-l", localFile,
        "-r", fileName, "-p", tftpServerIp);

    /* check tftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        dbgstrm << "Tftp downloading failed." << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

BASE_ErrCode SaveSwitchConfigure(const char* tftpServerIp, const char* fileName)
{
    int  ret = 0;
    const char* localFile = "/etc/rc.soc";
    assert(tftpServerIp != NULL && fileName != NULL);

    AutoLock autoLock(mgtMutex);
    cout << "Start to upload file." << endl;
    ret = ExeLinuxCommand("/bin/tftp", "tftp", "-l", localFile,
        "-r", fileName, "-p", tftpServerIp);

    /* check tftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        dbgstrm << "Tftp downloading failed." << endl;
        return BASE_ERROR;
    }
    return BASE_OK;
}

/*******************************ftp*******************************/
typedef BASE_ErrCode (*UpdateFromFtpHandler)(const char* ip,
    const char* user, const char* password, const char* file);

typedef map<BASE_ErrCode, UpdateFromFtpHandler> MapTypeToOpt;
static MapTypeToOpt::value_type typeOptPair[] =
{
    make_pair(FileTypeSwitchProfile, UpdateSwitchConfigFromFtp),
    make_pair(FileTypeControllerProfile, UpdateControllerConfigFromFtp),
    make_pair(FileTypeRootFs, UpdateRootFsFromFtp)
};

static MapTypeToOpt handlerMap(typeOptPair,
    typeOptPair + sizeof(typeOptPair) / sizeof(MapTypeToOpt::value_type));

enum UpdateStaus
{
    NotStarted = 1,
    InProgress,
    Success,
    Failed
};

enum UpdateStaus updateStaus = NotStarted;

void InitUpdate()
{
}

void DeinitUpdate()
{
}

static BASE_ErrCode CheckFileFirstLine(const char* file, const char* firstLine)
{
    ifstream inFile(file);
    string line;
    BASE_ErrCode ret = BASE_ERROR;
    if (getline(inFile, line))
    {
        if (line == firstLine)
        {
            ret = BASE_OK;
        }
        else
        {
            dbgstrm << "Wrong file type." << endl;
        }
    }
    else
    {
        dbgstrm << "Can't read first line of file." << endl;
    }

    return ret;
}

BASE_ErrCode UpdateSwitchConfigFromFtp(const char* ip, const char* user,
    const char* password, const char* file)
{
    const char* tmpFile = "/etc/tmp.soc";
    const char* localFile = "/etc/rc.soc";
    assert(user != NULL && password != NULL && file != NULL);

    updateStaus = InProgress;

    cout << "Start to download file." << endl;
    BASE_ErrCode ret = ExeLinuxCommand("/bin/ftpget", "ftpget", "-u", user,
        "-p", password, ip, tmpFile, file);

    /* check ftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        errstrm << "ftp downloading failed." << endl;
        updateStaus = Failed;
        return BASE_ERROR;
    }

    /* check file content. */
    if (CheckFileFirstLine(tmpFile, "init all") != BASE_OK)
    {
        errstrm << "CheckFileFirstLine() failed." << endl;
        updateStaus = Failed;
        return BASE_ERROR;
    }
    remove(localFile);
    rename(tmpFile, localFile);
    updateStaus = Success;

    return BASE_OK;
}

BASE_ErrCode UpdateControllerConfigFromFtp(const char* ip, const char* user,
    const char* password, const char* file)
{
    const char* tmpFile = "/etc/tmp.cli";
    const char* localFile = "/etc/cmc_cfg_file.cli";
    assert(user != NULL && password != NULL && file != NULL);
    cout << "Start to download file." << endl;
    BASE_ErrCode ret = ExeLinuxCommand("/bin/ftpget", "ftpget", "-u", user,
        "-p", password, ip, tmpFile, file);

    /* check ftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        errstrm << "ftp downloading failed." << endl;
        updateStaus = Failed;
        return BASE_ERROR;
    }

    /* check file content. */
    if (CheckFileFirstLine(tmpFile, "configure terminal") != BASE_OK)
    {
        errstrm << "CheckFileFirstLine() failed." << endl;
        updateStaus = Failed;
        return BASE_ERROR;
    }
    remove(localFile);
    rename(tmpFile, localFile);
    updateStaus = Success;

    return BASE_OK;
}

BASE_ErrCode UpdateRootFsFromFtp(const char* ip, const char* user,
    const char* password, const char* file)
{
    const char* tmpFile = "/var/RootFs.img";
    assert(user != NULL && password != NULL && file != NULL);
    cout << "Start to download file." << endl;
    BASE_ErrCode ret = ExeLinuxCommand("/bin/ftpget", "ftpget", "-u", user,
        "-p", password, ip, tmpFile, file);

    /* check ftp exit value */
    if(ret < 0 || !WIFEXITED(ret) || WEXITSTATUS(ret) != 0)
    {
        errstrm << "CheckFileFirstLine() failed." << endl;
        updateStaus = Failed;
        return BASE_ERROR;
    }
    cout << "Start to write image to flash." << endl;
    UpdateRootFsFromImage(tmpFile);

    return BASE_OK;
}

BASE_ErrCode UpdateFromFtp(const char* ip, const char* user, const char* password,
    const char* file, int32_t type)
{
    dbgstrm << "Start, ip=" << ip
        << ", user=" << user << ", password=" << password
        << ", file=" << file << ", type=" << type << endl;

    AutoLock autoLock(mgtMutex);
    updateStaus = InProgress;

    MapTypeToOpt::iterator iter;
    iter = handlerMap.find(type);
    if(iter != handlerMap.end())
    {
        return iter->second(ip, user, password, file);
    }

    updateStaus = Failed;
    return BASE_ERROR;
}

struct UpdatePara
{
    char ip[INET_ADDRSTRLEN];
    char user[MaxUserName];
    char password[MaxUserPassword];
    char file[MaxFileName];
    int32_t type;
};

static void *UpdateThreadProc(void *ptr)
{
    auto_ptr<UpdatePara> para(static_cast<UpdatePara*>(ptr));

    AutoLock autoLock(mgtMutex);
    if (updateStaus == InProgress)
        return NULL;

    updateStaus = InProgress;
    MapTypeToOpt::iterator iter;
    iter = handlerMap.find(para->type);
    if(iter != handlerMap.end())
    {
        iter->second(para->ip, para->user, para->password, para->file);
        return NULL;
    }

    updateStaus = Failed;
    return NULL;
}

void AsynUpdateFromFtp(const char* ip, const char* user, const char* password,
    const char* file, int32_t type)
{
    UpdatePara* para = new UpdatePara;
    pthread_t threadContxt;

    strncpy(para->ip, ip, INET_ADDRSTRLEN);
    strncpy(para->user, user, MaxUserName);
    strncpy(para->password, password, MaxUserPassword);
    strncpy(para->file, file, MaxFileName);
    para->type = type;

    pthread_create(&threadContxt, NULL, UpdateThreadProc, para);
}


int GetUpdateStatus()
{
    return (int)updateStaus;
}


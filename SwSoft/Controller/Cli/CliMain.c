#include "SystemInclude.h"
#include "BcmController.h"

#include "cparser.h"
#include "cparser_priv.h"
#include "cparser_token.h"
#include "cparser_tree.h"

#include "cli.h"
#include "SaveCfgFile.h"
#include "LoadBalanceMgt.h"
#include "ChannelMgt.h"
#include "DhcpRelayMgt.h"
#include "DsQosConfigMgt.h"
#include "CnuStatusMgt.h"
#include "SystemInfoMgt.h"
#include "CliMain.h"

cparser_t parser;

void InitCli()
{
    memset(&parser, 0, sizeof(parser));

    parser.cfg.root = &cparser_root;
    parser.cfg.ch_complete = '\t';

    parser.cfg.ch_erase = '\b';
    parser.cfg.ch_del = 127;
    parser.cfg.ch_help = '?';
    parser.cfg.flags = 0;
    strcpy(parser.cfg.prompt, "Controller# ");
    parser.cfg.fd = STDOUT_FILENO;
    cparser_io_config(&parser);

    if (CPARSER_OK != cparser_init(&parser.cfg, &parser)) {
        printf("Fail to initialize parser.\n");
        return;
    }
}

void ReadCliConfig()
{
    /* We'd better read configuration file when receives a cmc register message,
     * but,  controller platform support only one can get "BrcmOamSysEventMsg" event
     * by registered call back function.
     * for more detail, refer to CMCOAM_Platform_RegisterMsgHandler()
     * So we have to loop to check cmc register state.
     */
    uint8_t* cmcId;

    int i;
    for (i = 0; i < 180; ++i)
    {
        cmcId = (uint8_t*) cliGetCmc();
        if (cmcId[0] != 0 || cmcId[1] != 0 || cmcId[3] != 0 || cmcId[4] != 0)
        {
            /* Read configuration file, if file exists */
            if (access(ConfigureFileName, F_OK) == 0)
            {
                cparser_load_cmd(&parser, (char *) ConfigureFileName);
            }
            break;
        }
        printf("...");
        fflush(stdout);
        sleep(1);
    }
}

BASE_ErrCode SaveToCliConfig()
{
    SaveCfg("configure terminal", InsertToStart);

    if (SaveDhcpRelayConfig() != BASE_OK)
        return BASE_ERROR;

    if (SaveSyslogConfig() != BASE_OK)
        return BASE_ERROR;

    if (SaveRebootLevelConfig() != BASE_OK)
        return BASE_ERROR;

    if (SaveLoadBalanceConfig() != BASE_OK)
        return BASE_ERROR;

    if (SaveChannelConfig() != BASE_OK)
        return BASE_ERROR;

    if(SaveDsQosConfig() != BASE_OK)
        return BASE_ERROR;

    if(SaveMaxCnuOnlieMgt() != BASE_OK)
        return BASE_ERROR;

    if(SaveMaxCpuUsingMgt() != BASE_OK)
        return BASE_ERROR;

    if(SaveMaxRamUsingMgt() != BASE_OK)
        return BASE_ERROR;

    if(SaveUsSigMgt() != BASE_OK)
        return BASE_ERROR;

    return BASE_OK;
}

int file_lock_set(int fd, int type)
{
    struct flock lock;
    lock.l_whence = SEEK_SET;
    lock.l_start =0;
    lock.l_len = 0;
    while(1) {
        lock.l_type = type;
        if((fcntl(fd, F_SETLK, &lock)) == 0) {
            if(lock.l_type == F_RDLCK)
                printf("read a lock set by %d\n",getpid());
            else if(lock.l_type == F_WRLCK)
                printf("write a lock set by %d\n",getpid());
            else if(lock.l_type == F_UNLCK)
                printf("release lock by %d\n",getpid());
            return 0;
        }

        fcntl(fd, F_GETLK, &lock);
        if(lock.l_type != F_UNLCK) {
            if(lock.l_type == F_RDLCK)
                printf("read a lock already set by %d\n",lock.l_pid);
            else if(lock.l_type == F_WRLCK)
                printf("write lock already set by %d\n",lock.l_pid);
            //return -1;
        }
        //else
        //return 0;
    }
}

int recovery_cli_cfg_default(void)
{
    int defatul_cfg_fd;
    if((defatul_cfg_fd = open("/etc/default_cli_config.cfg" , O_RDONLY , 0666)) < 0) {
        perror("open:");
        goto end1;
    } else
        printf("open default cli config success!\n");


    int now_cfg_fd;
    if((now_cfg_fd = open("/etc/now_cli_config.cfg" , O_WRONLY|O_CREAT|O_TRUNC , 0666)) < 0) {
        perror("open:");
        goto end2;
    } else
        printf("open now cli config success!\n");


    int size = 0;
    unsigned char io_buffer[100];


    lseek(defatul_cfg_fd, 0, SEEK_SET);//move to default cfg file begin

    while(1) {
        size = read(defatul_cfg_fd, io_buffer , 100);

        if(size < 0) {
            perror("read:");
            break;
        } else if(size == 0) {
            break;
        } else
            printf("read from default cfg file success!\n");

        file_lock_set(now_cfg_fd, F_WRLCK);
        if((size = write(now_cfg_fd, io_buffer, size)) < 0) {
            perror("write:");
            break;
        } else
            printf("write now cfg file success!\n");
        file_lock_set(now_cfg_fd, F_UNLCK);

        memset(io_buffer, 0 , 100);
    }

end2:
    close(now_cfg_fd);
end1:
    close(defatul_cfg_fd);

    return 0;
}

void RestoreDefaultConfig()
{
    //read a default cli config and recovery now config+
    recovery_cli_cfg_default();

    StopCli();
}

void StartCli()
{
    cparser_run(&parser);
}

void StopCli()
{
    cparser_quit(&parser);
}



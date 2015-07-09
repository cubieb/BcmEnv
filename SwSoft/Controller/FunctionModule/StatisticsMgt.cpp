#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#include "SystemInclude.h"
#include "BcmController.h"
#include "Debug.h"
#include "AutoLock.h"
#include "IPC_SocketMgt.h"
#include "StatisticsMgt.h"

using namespace std;
extern pthread_mutex_t mgtMutex;

unsigned int stat_connfd, stat_receive_data_length;
unsigned char  stat_revbuf[512];

BASE_ErrCode
MgtGetWanPortStat(WAN_PORT_STAT_CFG * wan_port_stat_entry)
{
    assert(wan_port_stat_entry);
    AutoLock autoLock(mgtMutex);
    int ret;

    //WAN_PORT_STAT_CFG temp_wan_port_stat_entry;
    IPC_HEADER* ipc_header_point;
    //memset((unsigned char*)&temp_wan_port_stat_entry, 0, sizeof(WAN_PORT_STAT_CFG));

    if(connect_qos_cfg_ipc(&stat_connfd)<0) {
        errstrm << "connect fail" << endl;
        return BASE_ERROR;
    }

    ret = send_ipc_packet(stat_connfd, 0x04, NULL, 0, 0);//只发送包头带opcode = 0x04
    if(ret >= 0) {
        ret = recv_ipc_packet(stat_connfd, stat_revbuf, &stat_receive_data_length, 3);//等待3秒
        if(ret >= 0)
            dbgstrm << "send and reponse success" << endl;
    }
    if(ret < 0) {
        errstrm << "send or recev fail" << endl;
        return BASE_ERROR;
    }

    ipc_header_point = (IPC_HEADER*)stat_revbuf;
    if(ipc_header_point->opcode == 0x04)
    {
            //memcpy(&temp_wan_port_stat_entry, revbuf+sizeof(IPC_HEADER), sizeof(WAN_PORT_STAT_CFG));  
            memcpy(wan_port_stat_entry, stat_revbuf+sizeof(IPC_HEADER), sizeof(WAN_PORT_STAT_CFG));  
    }
    else
        return BASE_ERROR;
    
    return BASE_OK;
}


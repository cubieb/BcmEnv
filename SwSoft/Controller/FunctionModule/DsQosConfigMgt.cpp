#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#include "SystemInclude.h"
#include "BcmController.h"
#include <Util.h>
#include "Debug.h"
#include "AutoLock.h"
#include "SaveCfgFile.h"
#include "CnuStatusMgt.h"
#include "IPC_SocketMgt.h"
#include "DsQosConfigMgt.h"

using namespace std;

extern pthread_mutex_t mgtMutex;

unsigned int ds_qos_connfd, ds_qos_receive_data_length;
unsigned char  ds_revbuf[512];

BASE_ErrCode
MgtAddDsQosARule(U32 ip_address, U32 bandwidth)
{
    AutoLock autoLock(mgtMutex);
    int ret;

    DS_QOS_CFG AddADsQosRule_Entry;
    memset((unsigned char*)&AddADsQosRule_Entry, 0, sizeof(DS_QOS_CFG));

    AddADsQosRule_Entry.ip_address = ip_address;
    AddADsQosRule_Entry.bandwidth = bandwidth;

    if(connect_qos_cfg_ipc(&ds_qos_connfd)<0) {
        errstrm << "connect fail" << endl;
        return BASE_ERROR;
    }
    ret = send_ipc_packet(ds_qos_connfd, 0x01, (unsigned char*)&AddADsQosRule_Entry, sizeof(DS_QOS_CFG), 0);//×èÈû·¢ËÍ
    if(ret >= 0) {
        ret = recv_ipc_packet(ds_qos_connfd, ds_revbuf, &ds_qos_receive_data_length, 3);//µÈ´ý3Ãë
        if(ret >= 0)
            dbgstrm << "send and reponse success" << endl;
    }
    if(ret < 0) {
        errstrm << "send or recev fail" << endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

BASE_ErrCode
MgtDeleteDsQosARule(U32 ip_address)
{
    AutoLock autoLock(mgtMutex);
    int ret;

    DS_QOS_CFG AddADsQosRule_Entry;
    memset((unsigned char*)&AddADsQosRule_Entry, 0, sizeof(DS_QOS_CFG));

    AddADsQosRule_Entry.ip_address = ip_address;
    //AddADsQosRule_Entry.bandwidth = bandwidth;

    if(connect_qos_cfg_ipc(&ds_qos_connfd)<0) {
        errstrm << "connect fail" << endl;
        return BASE_ERROR;
    }

    ret = send_ipc_packet(ds_qos_connfd, 0x02, (unsigned char*)&AddADsQosRule_Entry, sizeof(DS_QOS_CFG), 0);//×èÈû·¢ËÍ
    if(ret >= 0) {
        ret = recv_ipc_packet(ds_qos_connfd, ds_revbuf, &ds_qos_receive_data_length, 3);//µÈ´ý3Ãë
        if(ret >= 0)
            dbgstrm << "send and reponse success" << endl;
    }
    if(ret < 0) {
        errstrm << "send or recev fail" << endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

BASE_ErrCode
MgtShowDsQosARule()
{
    AutoLock autoLock(mgtMutex);
    int ret;

    DS_QOS_CFG AddADsQosRule_Entry;
    memset((unsigned char*)&AddADsQosRule_Entry, 0, sizeof(DS_QOS_CFG));

    //AddADsQosRule_Entry.ip_address = ip_address;
    //AddADsQosRule_Entry.bandwidth = bandwidth;

    if(connect_qos_cfg_ipc(&ds_qos_connfd)<0) {
        errstrm << "connect fail" << endl;
        return BASE_ERROR;
    }

    ret = send_ipc_packet(ds_qos_connfd, 0x03, (unsigned char*)&AddADsQosRule_Entry, sizeof(DS_QOS_CFG), 0);//×èÈû·¢ËÍ
    if(ret >= 0) {
        ret = recv_ipc_packet(ds_qos_connfd, ds_revbuf, &ds_qos_receive_data_length, 3);//µÈ´ý3Ãë
        if(ret >= 0)
            dbgstrm << "send and reponse success" << endl;
    }
    if(ret < 0) {
        errstrm << "send or recev fail" << endl;
        return BASE_ERROR;
    }

    return BASE_OK;
}

BASE_ErrCode
MgtSaveDsQosARule()
{
    AutoLock autoLock(mgtMutex);
    ostringstream os;
    string mac_string;
    int ret = 0;
    DS_QoS_InfoT now_ds_qos_info_entry,next_ds_qos_info_entry;
    memset(&now_ds_qos_info_entry, 0, sizeof(DS_QoS_InfoT));
    memset(&next_ds_qos_info_entry, 0, sizeof(DS_QoS_InfoT));

    if(GetFirstDsQosMgt(&now_ds_qos_info_entry) < 0) {
        dbgstrm << "There is no qos setting !" << endl;
        return BASE_ERROR;
    } else {
        mac_string = MacToString(now_ds_qos_info_entry.ds_qos_cnu_mac.addr);
        os << "add ds_qos mac_address " << mac_string
           << " bandwidth_M " << (int)now_ds_qos_info_entry.bandwidth << endl;
    }

    do {
        ret = GetNextDsQosMgt(&now_ds_qos_info_entry, &next_ds_qos_info_entry);
        if(ret >= 0) {
            mac_string = MacToString(next_ds_qos_info_entry.ds_qos_cnu_mac.addr);
            os << "add ds_qos mac_address " << mac_string
               << " bandwidth_M " << (int)next_ds_qos_info_entry.bandwidth << endl;
        }

    } while(ret >= 0);

    if(SaveCfg(os.str().c_str() ,CONTINUE )<0) {
        dbgstrm << "save error"<< endl;
        return BASE_ERROR;
    }
    os.str("");

    return BASE_OK;
}

BASE_ErrCode
SaveDsQosConfig()
{
    MgtSaveDsQosARule();
    return BASE_OK;
}



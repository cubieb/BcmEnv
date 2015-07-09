#include <sys/socket.h>
#include <sys/un.h>
#include "SystemInclude.h"
#include "BcmSdk.h"
#include "Util.h"
#include "Debug.h"
#include "Statistics.h"
#include "Qos.h"
#include "QosInit.h"

using namespace std;

#define MAX_CONNECTION_NUMBER 5
#define RECV_BUFFER_LENGTH 1024

typedef struct {
    unsigned char heardstring[4];//IPC
    unsigned int opcode;//op code
    int packetlength;//include header
    int result;//reponse result
} __attribute__ ((packed))  IPC_HEADER;

typedef struct {
    unsigned int ip_address;
    unsigned int bandwidth;
} __attribute__ ((packed))  DS_QOS_CFG;

typedef struct {
	/*** RFC 1213 ***/
	uint64	snmpIfInOctets;
	uint64	snmpIfInUcastPkts;
 	uint64	snmpIfInNUcastPkts;
	uint64	snmpIfInDiscards;
	uint64	snmpIfInErrors;
	uint64	snmpIfInUnknownProtos;
	uint64	snmpIfOutOctets;
	uint64	snmpIfOutUcastPkts;
	uint64	snmpIfOutNUcastPkts;
	uint64	snmpIfOutDiscards;
	uint64	snmpIfOutErrors;
	uint64	snmpIfOutQLen;
	uint64	snmpIpInReceives;
	uint64	snmpIpInHdrErrors;
	uint64	snmpIpForwDatagrams;
	uint64	snmpIpInDiscards;
} __attribute__ ((packed))  WAN_PORT_STAT_CFG;


void 
printf_buffer(unsigned char *p, unsigned int len, unsigned short width)
{
    unsigned int i=0;
    unsigned char *x = p;

    //printf("\n=========server \n");
    for(i = 0; i < len; i++) {
        if((i != 0) && (i%width == 0)) {
            //printf("\r\n%02x ",x[i]);
            dbgstrm << "\r\n" << hex << x[i];
        } else {
            //printf("%02x ",x[i]);
            dbgstrm << hex << x[i];
        }
    }
    //printf("\r\n");
    dbgstrm << endl;
}

/* * Create a server endpoint of a connection. * Returns fd if all OK, <0 on error. */
int 
unix_socket_create(const char *servername)
{
    int fd;
    struct sockaddr_un un;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        return(-1);
    }
    int len;
    unlink(servername);               /* in case it already exists */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, servername);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(servername);

    if (bind(fd, (struct sockaddr *)&un, len) < 0)
        return -2;
    else
        return fd;

}

int 
unix_socket_listen(int fd)
{
    int rval;

    if (listen(fd, MAX_CONNECTION_NUMBER) < 0) {
        rval =  -3;
    } else {
        return fd;
    }

    int err;
    err = errno;
    close(fd);
    errno = err;
    return rval;
}

int 
unix_socket_accept(int listenfd, uid_t *uidptr)
{
    int clifd, len, rval;
    //time_t staletime;
    struct sockaddr_un un;
    struct stat statbuf;
    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr *)&un, (socklen_t*)&len)) < 0) {
        return(-1);
    }
    /* obtain the client's uid from its calling address */
    len -= offsetof(struct sockaddr_un, sun_path);  /* len of pathname */
    un.sun_path[len] = 0; /* null terminate */
    if (stat(un.sun_path, &statbuf) < 0) {
        rval = -2;
    } else {
        if (S_ISSOCK(statbuf.st_mode) ) {
            if (uidptr != NULL) *uidptr = statbuf.st_uid;    /* return uid of caller */
            unlink(un.sun_path);       /* we're done with pathname now */
            return clifd;
        } else {
            rval = -3;     /* not a socket */
        }
    }
    int err;
    err = errno;
    close(clifd);
    errno = err;
    return(rval);
}

void 
unix_socket_close(int fd)
{
    close(fd);
}

int 
send_ipc_packet(int send_to_fd, int opcode, unsigned char* send_data_buffer, int send_data_length, int time_out)
{
    IPC_HEADER * ipc_header_point = NULL;
    int size;
    unsigned char* pdata = NULL;

    dbgstrm << "malloc buffer length is header: " << sizeof(IPC_HEADER) << " ! data: "<< send_data_length << " !\n";
    pdata=(unsigned char*)malloc(sizeof(IPC_HEADER)+send_data_length);
    if(pdata == NULL) {
        errstrm << "mallloc fail!" << endl;
        return -1;
    }
    memset(pdata,0,sizeof(IPC_HEADER)+send_data_length); //reset to 0

    ipc_header_point = (IPC_HEADER*)pdata;
    ipc_header_point->heardstring[0] = 'I';
    ipc_header_point->heardstring[1] = 'P';
    ipc_header_point->heardstring[2] = 'C';
    ipc_header_point->heardstring[3] = '!';
    ipc_header_point->opcode = opcode;
    ipc_header_point->packetlength = sizeof(IPC_HEADER)+send_data_length;

    if(send_data_buffer != NULL)
        memcpy(pdata+sizeof(IPC_HEADER), send_data_buffer, send_data_length);

    //set time out to receive result
    if(time_out) {
        dbgstrm << "set time out " << time_out << "seconds" << endl;
        struct timeval timeout= {time_out,0};
        int ret=setsockopt(send_to_fd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
        if(ret <0 )
            errstrm << "set timeout error!" << endl;
    }
    dbgstrm << "Start send data to server length is: " << ipc_header_point->packetlength << " !" <<endl;
    printf_buffer((unsigned char *)pdata, ipc_header_point->packetlength, 16);
    size = send(send_to_fd, pdata, ipc_header_point->packetlength, 0);
    if(size>=0) {
        dbgstrm << "Sended data : " << size << "!" << endl;
    } else if(size==-1) {
        if(time_out>0 && errno==EAGAIN)
            errstrm << "Send time out :when sending Data!" << endl;
        else
            errstrm << "Error when sending Data!" << endl;

        free(pdata);
        return -1;
    }
    free(pdata);
    return 0;
}

int 
recv_ipc(int receive_from_fd, unsigned char* receive_data_buffer, unsigned int AllSize)
{
    int nrecv= 0;
    unsigned int nleft = 0;
    unsigned char* ptr;
    
    ptr = receive_data_buffer;
    nleft = AllSize;

    while(nleft > 0) {
        if ((nrecv = recv(receive_from_fd, ptr, nleft, 0)) < 0) {
            errstrm << "recv error!" << endl;
            if (errno == EINTR)
                nrecv = 0;		/* and call read() again */
            else
                return(-1);
        }else if (nrecv == 0)
            break;                      /* EOF */

	nleft -= nrecv;
	ptr   += nrecv;        
    }
    return(AllSize - nleft);		/* return >= 0 */
}

int 
recv_ipc_packet(int receive_from_fd, unsigned char* receive_data_buffer, unsigned int* receive_data_length, int time_out)
{
    IPC_HEADER * ipc_header_point = NULL;
    int size;

    //char revbuf[512];
    //memset(revbuf,0,512); //reset to 0

    //set timeout
    struct timeval timeout= {time_out,0};
    int ret=setsockopt(receive_from_fd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
    if(ret <0 )
        errstrm << "set time out error!" << endl;

    size = recv_ipc(receive_from_fd, receive_data_buffer, sizeof(IPC_HEADER));
    if(size>=0) {
        ipc_header_point = (IPC_HEADER*)receive_data_buffer;
        if( ipc_header_point->heardstring[0] == 'I'&& ipc_header_point->heardstring[1] == 'P' && ipc_header_point->heardstring[2] == 'C' && ipc_header_point->heardstring[3] == '!') {
            //while(size != ipc_header_point->packetlength) {
            if(size != ipc_header_point->packetlength) {                
                //size += recv_ipc(receive_from_fd, receive_data_buffer+size, ipc_header_point->packetlength -sizeof(IPC_HEADER) , sizeof(IPC_HEADER));
                size += recv_ipc(receive_from_fd, receive_data_buffer+size, ipc_header_point->packetlength -sizeof(IPC_HEADER));
            }
        }
    } else if(size==-1&&errno==EAGAIN) {
        errstrm << "timeout Error Client when recieving Data." << endl;
    }

    if(size > 512) {
        errstrm << "Receive data too long!" << endl;
        return -1;
    }

    if(size == ipc_header_point->packetlength ) {
        printf_buffer((unsigned char *)receive_data_buffer, size, 16);
    }
    return 0;
}


int 
QosIpcReceiveStart(void)
{
    int socket_fd;
    int listenfd;
    int connfd = 0;
    int ret;
    unsigned int receive_data_length;
    unsigned char rvbuf[RECV_BUFFER_LENGTH];
    uid_t uid;
    uint64 stat_val;

    IPC_HEADER * ipc_header_point = NULL;
    DS_QOS_CFG *ds_qos_cfg_point = NULL;

    socket_fd = unix_socket_create("foo.sock");

    while(1) {
        listenfd = unix_socket_listen(socket_fd);
        if(listenfd<0) {
            errstrm << "Error when listening..." << endl;
            return -1;
        }

        connfd = unix_socket_accept(listenfd, &uid);
        if(connfd<0) {
            errstrm << "Error when accepting..." << endl;
            return -1;
        }

        ret = recv_ipc_packet(connfd, rvbuf, &receive_data_length, 0);//×èÈû½ÓÊÕ
        if(ret >= 0)
            dbgstrm << "recv bytes success" << endl;
        else {
            errstrm << "recv fail" << endl;
            unix_socket_close(connfd);
            break;
        }

        ipc_header_point = (IPC_HEADER*)rvbuf;
        switch(ipc_header_point->opcode) {
            case 0x01:
                ds_qos_cfg_point = (DS_QOS_CFG*)(rvbuf+sizeof(IPC_HEADER));
                ret = ds_qos_rule_cfg_create(ds_qos_cfg_point->ip_address, 0xffffff00, ds_qos_cfg_point->bandwidth);
                break;

            case 0x02:
                ds_qos_cfg_point = (DS_QOS_CFG*)(rvbuf+sizeof(IPC_HEADER));
                ret = ds_qos_rule_cfg_delete(ds_qos_cfg_point->ip_address);
                break;

            case 0x03:
                ds_qos_rule_cfg_show();
                break;

            case 0x04:
                WAN_PORT_STAT_CFG temp_wan_port_stat_entry;
                memset((unsigned char*)&temp_wan_port_stat_entry, 0, sizeof(WAN_PORT_STAT_CFG));
                int s;
                uint64* temp_wan_port_stat_point = &(temp_wan_port_stat_entry.snmpIfInOctets);

                for(s = 0; s <= snmpIpInDiscards; s++)
                {
                    //cout << "s = " << s << endl;
                    Stat_get(s, &stat_val);
                    //temp_wan_port_stat_entry.snmpIfInOctets = stat_val;
                    *temp_wan_port_stat_point = stat_val;
                    temp_wan_port_stat_point++;
                }

                ret = send_ipc_packet(connfd, ipc_header_point->opcode, (unsigned char*)(&temp_wan_port_stat_entry), sizeof(WAN_PORT_STAT_CFG), 3);//3Ãë·Ç×èÈû·¢ËÍ
                if(ret >= 0)
                    dbgstrm << "Wan Port Reponse success" << endl;
                else {
                    errstrm << "Wan Port Reponse fail!" << endl;
                    return -1;
                }
                
                unix_socket_close(connfd);
                dbgstrm << "Recv process exit" << endl;

                continue;
                
                break;

            default:
                errstrm << "Error opcode!" << endl;
                break;
        }

        IPC_HEADER reponse_entry;
        memcpy(&reponse_entry, ipc_header_point, sizeof(IPC_HEADER));
        reponse_entry.packetlength = sizeof(IPC_HEADER);
        reponse_entry.result = ret;
        ret = send_ipc_packet(connfd, ipc_header_point->opcode, (unsigned char*)(&reponse_entry), 0, 3);//3Ãë·Ç×èÈû·¢ËÍ
        if(ret >= 0)
            dbgstrm << "Reponse success" << endl;
        else {
            errstrm << "reponse fail!" << endl;
            return -1;
        }

        unix_socket_close(connfd);
        dbgstrm << "Recv process exit" << endl;

    }

    dbgstrm << "IPC Server exited." << endl;

    return 0;
}

static void* QosIpcReceiveThread(void*)
{
    QosIpcReceiveStart();
    return NULL;
}

QosInit::QosInit()
{
    dbgstrm << "Do nothing." << endl;
}

QosInit::~QosInit()
{

}

bool QosInit::InitStep1()
{
    dbgstrm << "Do nothing." << endl;
    return true;
}

bool QosInit::InitStep2()
{
    return true;
}

bool QosInit::InitStep3()
{
    pthread_t id;
    int ret;
    ret = pthread_create(&id, NULL, QosIpcReceiveThread, NULL);
    if(ret!=0) {
        errstrm<< "Create pthread error!" <<endl;
        exit (1);
    } else
        dbgstrm << "Create pthread success!" <<endl;
    return true;
}


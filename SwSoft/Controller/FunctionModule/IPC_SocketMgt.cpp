#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#include "SystemInclude.h"
#include "Debug.h"
#include "IPC_SocketMgt.h"

using namespace std;

//print buffer
void 
printf_buffer(unsigned char *p, unsigned int len, unsigned short width)
{
    unsigned int i=0;
    unsigned char *x = p;

    //printf("\n=========client \n");

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

/* Create a client endpoint and connect to a server.   Returns fd if all OK, <0 on error. */
int 
unix_socket_conn(const char *servername)
{
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {  /* create a UNIX domain stream socket */
        return(-1);
    }

    int len, rval;
    struct sockaddr_un un;
    memset(&un, 0, sizeof(un));            /* fill socket address structure with our address */

    un.sun_family = AF_UNIX;
    sprintf(un.sun_path, "scktmp%05d", getpid());
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
    unlink(un.sun_path);               /* in case it already exists */

    if (bind(fd, (struct sockaddr *)&un, len) < 0) {
        rval=  -2;
    } else {
        /* fill socket address structure with server's address */
        memset(&un, 0, sizeof(un));
        un.sun_family = AF_UNIX;
        strcpy(un.sun_path, servername);
        len = offsetof(struct sockaddr_un, sun_path) + strlen(servername);

        if (connect(fd, (struct sockaddr *)&un, len) < 0) {
            rval= -4;
        } else {
            return (fd);
        }
    }
    int err;
    err = errno;
    close(fd);
    errno = err;
    return rval;
}

void 
unix_socket_close(int fd)
{
    close(fd);
}

int 
connect_qos_cfg_ipc(unsigned int* connfd)
{
    *connfd = unix_socket_conn("foo.sock");

    assert(*connfd>=0);

    if(*connfd<0) {
        errstrm << "Error when connecting.." << endl;
        return -1;
    } else
        return 0;
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
        dbgstrm<< "set time out!" << endl;
        struct timeval timeout= {time_out,0};
        int ret=setsockopt(send_to_fd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
        if(ret <0 )
            errstrm << "set time out error!" << endl;
    }
    dbgstrm << "Start send data to server length is: " << ipc_header_point->packetlength << " !" <<endl;
    printf_buffer((unsigned char *)pdata, ipc_header_point->packetlength, 16);
    size = send(send_to_fd, pdata, ipc_header_point->packetlength, 0);
    if(size>=0) {
        dbgstrm << "Sended data : " << size << "!" << endl;
    } else if(size==-1) {
        if(time_out>0 && errno==EAGAIN)
            errstrm << "Send time out :Error when sending Data!" << endl;
        else
            errstrm << "Error when sending Data." << endl;

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


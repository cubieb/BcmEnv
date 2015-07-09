#ifndef _IPC_SocketMgt_h_
#define _IPC_SocketMgt_h_

typedef struct {
    unsigned char heardstring[4];//IPC
    unsigned int opcode;//op code
    int packetlength;//include header
    int result;//reponse result
} __attribute__ ((packed))  IPC_HEADER;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int connect_qos_cfg_ipc(unsigned int* connfd);
int send_ipc_packet(int send_to_fd, int opcode, unsigned char* send_data_buffer, int send_data_length, int time_out);
int recv_ipc_packet(int receive_from_fd, unsigned char* receive_data_buffer, unsigned int* receive_data_length, int time_out);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


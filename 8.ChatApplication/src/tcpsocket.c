#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <log.h>
#include <queue.h>
#include <string.h>
#include <unistd.h>
#include "tcpsocket.h"

typedef struct {
    int fd;
    struct sockaddr_in sa;
} machine_t;


/* static constant variable */
static machine_t host = {0};
static queue_t peerMachines;
static char buffRecv[256];

static int s_createTCPIpv4Socket(void){ 
    return socket(AF_INET, SOCK_STREAM, 0);
}

static void s_createIPv4Address(struct sockaddr_in* address, char* ip, int port) {
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if(strlen(ip) == 0){
        address->sin_addr.s_addr = INADDR_ANY;
    } else {
        inet_pton(AF_INET, ip, &(address->sin_addr.s_addr));
    }
}

void ts_initHost(int port){
    /* init host socket and address */
    int opt = 1; // enable option
    host.fd = s_createTCPIpv4Socket();
    s_createIPv4Address(&host.sa, "", port);

    /* prevent address in used*/
    if(D_ERROR == setsockopt(host.fd, SOL_SOCKET, SO_REUSEADDR,&opt,sizeof(opt))){
         M_HANDLE_ERROR("Error setsockopt()\n");
    }

    if (D_ERROR == bind(host.fd, (const struct sockaddr*)(&host.sa), sizeof (struct sockaddr_in))){
        M_HANDLE_ERROR("Error host bind()\n");
    }

    if (D_ERROR == listen(host.fd, D_MAX_ACCEPT_MACHINE) ) {
        M_HANDLE_ERROR("Error host listen()\n");
    }

    /* init container of peer connection */
    queue_initQueue(&peerMachines);
}

void ts_deinitHost(){
    close(host.fd);
}

int ts_getHostPort(){
    return ntohs(host.sa.sin_port);
}

void ts_getHostIP(char* hostIP){
    char tmp[INET_ADDRSTRLEN] = {0};
    
    inet_ntop(AF_INET, &(host.sa.sin_addr), tmp, INET_ADDRSTRLEN);
    memcpy(hostIP, tmp, INET_ADDRSTRLEN);
}

void ts_acceptClient(){
    machine_t client;
    char peerIP[INET_ADDRSTRLEN];
    socklen_t length;

    length = sizeof(struct sockaddr_in);
    
    while (1){
        client.fd = accept(host.fd, (struct sockaddr*)(&client.sa), &length);
        if(D_ERROR == client.fd) {
            /* TODO: xu ly ko hop le */
            M_HANDLE_ERROR("Error accept()");
        } else {
            inet_ntop(AF_INET, &(client.sa.sin_addr), peerIP, INET_ADDRSTRLEN);
            queue_enqueue(&peerMachines, client.fd, ntohs(client.sa.sin_port), peerIP);
            /* TODO notify a peer try connection to host */

        }

        printf("ip:%s port:%u\n", peerIP, ntohs(client.sa.sin_port));
    }
}

int ts_connectPeer(char* ip, int port){
    int retStatus;
    machine_t peer;

    peer.fd = s_createTCPIpv4Socket();
    s_createIPv4Address(&peer.sa, ip, port);

    retStatus = connect(peer.fd, (struct sockaddr*)(&peer.sa), sizeof (struct sockaddr_in));
    if(D_ERROR == retStatus) {
        /* TODO: xu ly ko hop le */
        close(peer.fd);
    } else {
        /* TODO: thong bao write connect den peer*/

       queue_enqueue(&peerMachines, peer.fd, port, ip);
    }

    return retStatus;
}

int ts_sendMsg(char* msg, int id){
    int retStatus;
    int peerSocketFD;

    retStatus = D_ERROR;

    /* TODO: xu ly check dieu kien ip hop le */
    if(1){
        peerSocketFD = queue_getSocketFd(&peerMachines, id);
        if(D_ERROR != peerSocketFD){
            retStatus = write(peerSocketFD, msg, sizeof(msg));
        } else {
            //
        }
    } else {
        /* TODO: xu ly ip ko hop le */
    }

    return retStatus;
}

int ts_recvMsg(int socketFD, char** msg, char** ip, int* port){
    int retStatus;

    retStatus = D_ERROR;

    memset(buffRecv, 0, sizeof(buffRecv)); 
    /* this function will block until it read from another peer or conection from peer terminate */
    retStatus = read(socketFD, buffRecv, sizeof(buffRecv));
    switch (retStatus){
        case D_ERROR:
        /* TODO: xu ly read error */
        break;

        case D_EOF:
        /* delete peer socket if conection is terminate */  
            queue_deletePeerSocket(&peerMachines, socketFD);
        break;

        default:
        /* TODO: read successfully from peer and send data to GUI */
            queue_getAddr(&peerMachines, socketFD, ip, port);
        break;
    }

    /* Update output value*/
    *msg = buffRecv;
    return retStatus;
}

void ts_destroyAllPeerMachine(){
    queue_destroy(&peerMachines);
}

int ts_getDataPeer(char (*ip)[16], int *port){
    return queue_getDataPeer(&peerMachines, ip, port);
}

int ts_removePeerSocket(int id){
    int socketFD;
    int ret;

    socketFD = queue_getSocketFd(&peerMachines, id);
    if(socketFD == D_ERROR){
        /* not exist socketFD return -1 */
        ret = D_ERROR;
    } else {
        /* socket found and destroy peer's address */
        ret = D_OK;
        queue_deletePeerSocket(&peerMachines, socketFD);
        close(socketFD);
    }

    return ret;
}
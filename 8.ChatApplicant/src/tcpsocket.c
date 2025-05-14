#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <log.h>
#include <queue.h>

#define D_MAX_ACCEPT_MACHINE   (5U)

typedef struct {
    int fd;
    struct sockaddr_in sa;
} machine_t;


/* static constant variable */
static machine_t host = {0};
static queue_t peerMachines;
// static char buffSend[256];
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

void initHost(int port){
    /* init host socket and address */
    host.fd = s_createTCPIpv4Socket();
    s_createIPv4Address(&host.sa, "", port);

    if (D_ERROR == bind(host.fd, (const struct sockaddr*)(&host.sa), sizeof (struct sockaddr_in))){
        M_HANDLE_ERROR("Error host bind()\n");
    }

    if (D_ERROR == listen(host.fd, D_MAX_ACCEPT_MACHINE) ) {
        M_HANDLE_ERROR("Error host listen()\n");
    }

    /* init container of peer connection */
    initQueue(&peerMachines);
}

void ts_acceptClient(){
    machine_t client;
    
    while (1){
        client.fd = accept(host.fd, (struct sockaddr*)(&client.sa), sizeof (struct sockaddr_in));
        if(D_ERROR == client.fd) {
            /* TODO: xu ly ko hop le */
            M_LOG("Error accept()");
        } else {
            queue_enqueue(&peerMachines, client.fd, client.sa.sin_port, client.sa.sin_addr.s_addr);
        }

        printf("ip:%s port:%u\n", client.sa.sin_addr.s_addr, ntohl(client.sa.sin_port));
    }
}

void ts_connectPeer(char* ip, int port){
    int retStatus;
    machine_t peer;

    peer.fd = s_createTCPIpv4Socket();
    s_createIPv4Address(&peer.sa, ip, port);

    retStatus = connect(peer.fd, (struct sockaddr*)(&peer.sa), sizeof (struct sockaddr_in));
    if(D_ERROR == retStatus) {
        /* TODO: xu ly ko hop le */
    } else {
        /* TODO: thong bao write connect den peer*/

       queue_enqueue(&peerMachines, peer.fd, port, ip);
    }
}

int ts_sendMsg(char* msg, char* ip, int port){
    int retStatus;
    int peerSocketFD;

    retStatus = D_ERROR;

    /* TODO: xu ly check dieu kien ip hop le */
    if(1){
        peerSocketFD = queue_getSocketFd(&peerMachines, ip, port);
        retStatus = write(peerSocketFD, msg, sizeof(msg));
    } else {
        /* TODO: xu ly ip ko hop le */
    }

    return retStatus;
}

void ts_recvMsg(){
    int retStatus;
    int numPeer;
    int arrPeerSocket[10];
    char ip[16];
    int port;

    retStatus = D_ERROR;

    /* TODO xu ly cap nhat flag */
    int flagListenAllPeer = 1;
    while(flagListenAllPeer) {
        memset(arrPeerSocket, D_ERROR, sizeof(arrPeerSocket));
        numPeer = queue_getArrSocketFd(&peerMachines, arrPeerSocket, 10);

        if(0 < numPeer){
            for(int i=0; i<numPeer; ++i){
                memset(buffRecv, 0, sizeof(buffRecv)); 
                retStatus = read(arrPeerSocket[i], buffRecv, sizeof(buffRecv));

                switch (retStatus){
                case D_ERROR:
                    /* TODO: xu ly read error */
                    break;

                case D_EOF:
                    /* delete peer socket */  
                    queue_deletePeerSocket(&peerMachines, arrPeerSocket[i]);

                default:
                    /* TODO: read successfully from peer and send data to GUI */
                    queue_getAddr(&peerMachines, arrPeerSocket[i], &ip, &port);
                    gui_drawMsg(buffRecv,strlen(buffRecv), ip, port);
                    break;
                }
            }
        } 
    }

}
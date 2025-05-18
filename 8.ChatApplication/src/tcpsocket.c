#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <log.h>
#include <queue.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "tcpsocket.h"

typedef struct {
    int fd;
    struct sockaddr_in sa;
} machine_t;

typedef struct {
    pthread_t ID;
    int isUsed;
    int fd;
} readThreadID_t;

static int s_createThreadRead(int socketFd);
static void s_destroyThread(int socketFd);
static int s_createTCPIpv4Socket(void);
static void s_createIPv4Address(struct sockaddr_in* address, char* ip, int port);

int ts_dataFD;

/* static constant variable */
static machine_t host = {0};
static volatile queue_t peerMachines;
static char buffRecv[256];
static int numReadThread = 0;
static readThreadID_t tReadID[D_MAX_ACCEPT_MACHINE] = {0};
static cbFunc_t s_ctrlRead = NULL;
static pthread_mutex_t lockContainer;
static pthread_mutex_t lockReadID;


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

void ts_initCb(cbFunc_t ptr){
    s_ctrlRead = ptr;
}

void ts_initHost(int port){
    /* init host socket and address */
    int opt = 1; // enable option
    host.fd = s_createTCPIpv4Socket();
    s_createIPv4Address(&host.sa, "", port);

    /* prevent address in used*/
    if(D_ERROR == setsockopt(host.fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))){
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

        pthread_mutex_lock(&lockReadID);
        if(D_ERROR == client.fd) {
            /* TODO: xu ly ko hop le */
            M_HANDLE_ERROR("Error accept()");

        } else if (numReadThread <= D_MAX_ACCEPT_MACHINE) {
            /* Add an element to the container */
            inet_ntop(AF_INET, &(client.sa.sin_addr), peerIP, INET_ADDRSTRLEN);

            pthread_mutex_lock(&lockContainer);
            queue_enqueue(&peerMachines, client.fd, ntohs(client.sa.sin_port), peerIP);
            pthread_mutex_unlock(&lockContainer);

            /* create thread for each peer connection to read */
            s_createThreadRead(client.fd);
        
            /* TODO notify a peer try connection to host */

        } else {
            /* maximum the number of connecting peer */
            close(client.fd);
        }
        pthread_mutex_unlock(&lockReadID);
    }
}

static int s_createThreadRead(int socketFd){
    int checkValidThread;
    int i;

    i = 0;
    checkValidThread = D_OFF;
    while ((D_OFF == checkValidThread) && 
            (i < D_MAX_ACCEPT_MACHINE)){
        if (D_OFF == tReadID[i].isUsed){
            /*Update value*/
            ++numReadThread;
            checkValidThread = D_ON; // break loop
            tReadID[i].isUsed = D_ON;
            tReadID[i].fd = socketFd;

            /* ts_dataFD is input for cb function s_ctrlRead */
            ts_dataFD = socketFd;

            /* assign a thread to read msg from peer */
            pthread_create(&(tReadID[i].ID), NULL, s_ctrlRead, NULL);

        } else {
            /* check the next thread is occupy or not */
            ++i;
        }
    }


    return checkValidThread;
}


int ts_connectPeer(char* ip, int port){
    int retStatus;
    machine_t peer;

    peer.fd = s_createTCPIpv4Socket();
    s_createIPv4Address(&peer.sa, ip, port);

    retStatus = connect(peer.fd, (struct sockaddr*)(&peer.sa), sizeof (struct sockaddr_in));
    pthread_mutex_lock(&lockReadID);
    if(D_ERROR == retStatus) {
        /* TODO xu loi connect */

    } else if (numReadThread <= D_MAX_ACCEPT_MACHINE) {
        /* TODO: thong bao write connect den peer*/
        pthread_mutex_lock(&lockContainer);
        queue_enqueue(&peerMachines, peer.fd, port, ip);
        pthread_mutex_unlock(&lockContainer);

        /* create thread for each peer connection to read */
        s_createThreadRead(peer.fd);
        
    } else {
        close(peer.fd);
    }
    pthread_mutex_unlock(&lockReadID);

    return retStatus;
}

int ts_sendMsg(char* msg, int id){
    int retStatus;
    int peerSocketFD;

    retStatus = D_ERROR;

    /* TODO: xu ly check dieu kien ip hop le */
    if(1){
        pthread_mutex_lock(&lockContainer);
        peerSocketFD = queue_getSocketFd(&peerMachines, id);
        pthread_mutex_unlock(&lockContainer);
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
            s_destroyThread(socketFD);
            pthread_mutex_lock(&lockContainer);
            queue_deletePeerSocket(&peerMachines, socketFD);
            pthread_mutex_unlock(&lockContainer);
        break;

        default:
        /* TODO: read successfully from peer and send data to GUI */
            pthread_mutex_lock(&lockContainer);
            queue_getAddr(&peerMachines, socketFD, ip, port);
            pthread_mutex_unlock(&lockContainer);
        break;
    }

    /* Update output value*/
    *msg = buffRecv;
    return retStatus;
}

void ts_destroyAllPeerMachine(){
    pthread_mutex_lock(&lockContainer);
    queue_destroy(&peerMachines);
    pthread_mutex_unlock(&lockContainer);
}

int ts_getDataPeer(char (*ip)[16], int *port){
    int ret;
    
    pthread_mutex_lock(&lockContainer);
    ret = queue_getDataPeer(&peerMachines, ip, port);
    pthread_mutex_unlock(&lockContainer);

    return ret;
}

int ts_removePeerSocket(int id){
    int socketFD;
    int ret;

    pthread_mutex_lock(&lockContainer);
    socketFD = queue_getSocketFd(&peerMachines, id);
    if(socketFD == D_ERROR){
        /* not exist socketFD return -1 */
        ret = D_ERROR;
    } else {
        /* socket found and destroy peer's address */
        ret = D_OK;
        s_destroyThread(id);
        queue_deletePeerSocket(&peerMachines, socketFD);
        close(socketFD);
    }
    pthread_mutex_unlock(&lockContainer);

    return ret;
}

static void s_destroyThread(int socketFd){
    pthread_mutex_lock(&lockReadID);
    for(int i=0;i<D_MAX_ACCEPT_MACHINE;++i){
        if(tReadID[i].fd == socketFd){
            pthread_cancel(tReadID[i].ID);
            tReadID[i].isUsed = D_OFF;
            --numReadThread;
            i = D_MAX_ACCEPT_MACHINE; // break loop
        }
    }
    pthread_mutex_unlock(&lockReadID);
}
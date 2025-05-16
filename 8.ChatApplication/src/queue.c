#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"
#include "log.h"


void queue_initQueue(queue_t* container){
    container->head = NULL;
    container->tail = NULL;
}

static node_t* s_createNode(int socketFd, uint16_t port, char* ip) {
    node_t* node = (node_t*)malloc(sizeof(node_t));

    node->next = NULL;
    node->guest.socketFd = socketFd;
    node->guest.port = port;
    memset(node->guest.ip, 0, 16);
    memcpy(node->guest.ip, ip, strlen(ip));

    return node;
}

static int s_checkEmptyQueue(queue_t* container){
    int ret;

    ret = 0;
    if ((NULL == container->head) && 
        (NULL == container->tail)) {
        ret = 1;
    }

    return ret;
}

void queue_enqueue(queue_t* container, int socketFd, uint16_t port, char* ip) {
    node_t* node = s_createNode(socketFd, port, ip);
    int isQueueEmpty = s_checkEmptyQueue(container);

    if (isQueueEmpty) {
        /* check empty container */
        container->head = node;
        container->tail = node;
    } else {
        container->head->next = node;
        container->head = node;
    }
}

void queue_destroy(queue_t* container) {
    node_t* tmp = container->tail;

    while (NULL != tmp)
    {
        close(tmp->guest.socketFd);
        tmp = tmp->next;
        free(container->tail);
        container->tail = tmp;
    }
    
}

static int s_getNode(queue_t* container, char* ip, int port, node_t* node){
    node_t* tmp = NULL;
    int isQueueEmpty;
    int flag;           /* if find the right ip flag = 1 */

    flag = 0;
    isQueueEmpty = s_checkEmptyQueue(container);
    if(isQueueEmpty) {

    } else {
        tmp = container->tail;

        /* Search list by ip */
        while((NULL != tmp) && (flag == 0)){
            if ((strcmp(tmp->guest.ip, ip) == 0) && (tmp->guest.port == port)) {
                flag = 1;
            } else {
                tmp = tmp->next;
            }
        }
    }

    /* return value */
    *node = *tmp;
    return flag;
}

int queue_getSocketFd(queue_t* container, char *ip, int port){
    int flag;
    node_t node;
    int ret;

    flag = s_getNode(container, ip, port, &node);
    if(flag){
        /* socket found */
        ret = node.guest.socketFd;
    } else {
        ret = D_ERROR;
    }
    
    return ret;
}

/* TODO num will be value out put not input*/
int queue_getArrSocketFd(queue_t* container, int* arr, int num){
    int i=0;
    int isQueueEmpty;
    node_t* tmp = NULL;
    int ret;            /* if suceesfull return the number of socketFD else return -1 */

    ret = 0;
    isQueueEmpty = s_checkEmptyQueue(container);
    if( isQueueEmpty ) {
        ret = D_ERROR;
    } else {
        tmp = container->tail;
        while ((num > 0) && (NULL != tmp)) { 
            arr[i] = tmp->guest.socketFd;

            /* Update value */
            tmp = tmp->next;
            --num;
            ++i;
        }

        ret = i;
    }

    return ret;
}

int queue_deletePeerSocket(queue_t* container, int socket){
    node_t* deleteNode = NULL;
    node_t* preNode = NULL;
    int isQueueEmpty;
    int flag;                       /* flag = -1 empty queue, 0 not found, 1 found */

    flag = 0;

    isQueueEmpty = s_checkEmptyQueue(container);
    if(isQueueEmpty) {
        flag=-1;
    } else {
        deleteNode = container->tail;

        /* Search list by socket */
        while((NULL != deleteNode) && (flag == 0)){
            if ((deleteNode->guest.socketFd == socket)) {
                flag = 1;
            } else {
                preNode = deleteNode;
                deleteNode = deleteNode->next;
            }
        }

        /* found socket */
        if(flag == 1){
        /* if not 1st tail node */
            if(NULL != preNode){
                preNode->next = deleteNode->next;
            } else {
                container->tail = deleteNode->next;
            }

            free(deleteNode);
        }

    }

    return flag;
}

int queue_getAddr(queue_t* container, int fd, char** ip, int *port){
    node_t* tmp = NULL;
    int isQueueEmpty;
    int flag;           /* if find the right socket, flag = 1 */

    flag = 0;
    isQueueEmpty = s_checkEmptyQueue(container);
    if(isQueueEmpty) {

    } else {
        tmp = container->tail;

        /* Search list by fd */
        while((NULL != tmp) && (flag == 0)){
            if (tmp->guest.socketFd == fd) {
                flag = 1;
            } else {
                tmp = tmp->next;
            }
        }
    }

    /* return value */
    *port = tmp->guest.port;
    *ip = tmp->guest.ip;
    return flag;
}

int queue_getDataPeer(queue_t* container, char** ip, int* port){
    int i=0;
    int isQueueEmpty;
    node_t* tmp = NULL;
    int ret;            /* if suceesfull return the number of socketFD else return -1 */

    ret = 0;
    isQueueEmpty = s_checkEmptyQueue(container);
    if( isQueueEmpty ) {
        ret = D_ERROR;
    } else {
        tmp = container->tail;
        while ((NULL != tmp)) { 
            port[i] = tmp->guest.port;
            memcpy(ip[i],tmp->guest.ip, 16);

            /* Update value */
            tmp = tmp->next;
            ++i;
        }
        ret = i;
    }

    return ret;
}
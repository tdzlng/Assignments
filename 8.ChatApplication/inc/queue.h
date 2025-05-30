#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

/* PUBLIC TYPEDEF */
typedef struct {
    int socketFd;
    uint16_t port;
    char ip[16]; 
} data_t;

typedef struct node{
    data_t guest;
    struct node* next;
} node_t;

typedef struct {
    node_t* head;
    node_t* tail;
} queue_t;

/* PUBLIC FUNCTION PROTOTYPE */
void queue_initQueue(queue_t* container);
void queue_enqueue(queue_t* container, int socketFd, uint16_t port, char* ip);
int queue_getSocketFd(queue_t* container, int id);
int queue_checkExistAddr(queue_t* container, char* ip, int port);
int queue_deletePeerSocket(queue_t* container, int socket);
int queue_getAddr(queue_t* container, int fd, char** ip, int *port);
void queue_destroy(queue_t* container);
int queue_getDataPeer(queue_t* container, char (*ip)[16], int* port);

#endif
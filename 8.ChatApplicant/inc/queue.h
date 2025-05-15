#include <stdint.h>

typedef struct {
    int socketFd;
    uint16_t port;
    char ip[16]; 
} data_t;

typedef struct list{
    data_t guest;
    struct list* next;
} list_t;

typedef struct {
    list_t* head;
    list_t* tail;
} queue_t;

void queue_initQueue(queue_t* container);
void queue_enqueue(queue_t* container, int socketFd, uint16_t port, char* ip);
int queue_getSocketFd(queue_t* container, char *ip, int port);
int queue_getArrSocketFd(queue_t* container, int* arr, int num);
int queue_deletePeerSocket(queue_t* container, int socket);
int queue_getAddr(queue_t* container, int fd, char** ip, int *port);
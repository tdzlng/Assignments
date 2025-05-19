#ifndef COMMON_H
#define COMMON_H
#include <error.h>
#include <stdio.h>

/* DEFINE CONSTANTS */
#define D_SEARCH_BY_ADD     (-1)
#define D_NO_USED_PORT      (0)
#define D_NO_USED_IP        (NULL)
#define D_ON                (1)
#define D_OFF               (0)
#define D_OK                (1)
#define D_NONE              (0)
#define D_ERROR             (-1)
#define D_EOF               (0)

/* MACRO FUNCTION */
#define M_HANDLE_ERROR(msg) \
    { perror(msg);}
#define M_LOG(msg) \
    { printf("Server Log: %s\n",msg);}

#endif
#include <error.h>
#include <stdio.h>

#define D_ERROR (-1)
#define D_EOF   (0)

// macro function
#define M_HANDLE_ERROR(msg) \
    { perror(msg); exit(EXIT_FAILURE);}
#define M_LOG(msg) \
    { printf("Server Log: %s\n",msg);}
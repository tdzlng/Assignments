#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// define section
#define D_READ (1)
#define D_WRITE (2)
#define D_NONE (3)
#define D_ERROR (-1)

// function prototype
static char* getName(const char* str);
static int getNumByte(const char* str);
static int getMode(const char* str);
static char* getContext(const char* str);

static int writeContext(char* fileName, const char* context, int num);
static int readContext(char* fileName, char** context, int num);

// function definition
char* getName(const char* str)
{
    return str;
}

int getNumByte(const char* str)
{
    int ret = 0;
    while (str != NULL)
    {
        if(( *str >= '0' ) && ( *str <= '9' )){
            ret = 10*ret + (int)(*str - '0');
        } else {
            ret = 0x0FFFFFFF;
        }
        ++str;
    }
    return ret;
}

int getMode(const char* str)
{
    int ret;
    switch (str[0]) {
    case 'r':
        ret = D_READ;
        break;
    case 'w':
        ret = D_WRITE;
        break;

    default:
        ret = D_NONE;
        break;
    }
    return ret;
}

char* getContext(const char* str){
    return str;
}

#define __USE_XOPEN

int writeContext(char* fileName, const char* context, int num) {
    int fd;
    int ret;

    fd = open(fileName, O_APPEND | O_WRONLY, S_IWUSR );

    if(D_ERROR != fd) {
        write(fd, context, num);
    }
    ret = fd;
    close(fileName);
    return ret;
}

int readContext(char* fileName, char** context, int num) {
    int fd;
    int ret;
    fd = open(fileName, O_RDONLY, S_IRUSR );

    if(D_ERROR != fd) {
        read(fd, &context, num);
    }
    ret = fd;
    close(fileName);

    return ret;
}
#undef __USE_XOPEN

// main function
void main(int argc, char **argv){
    char* name = NULL;
    int numByte;
    int mode;
    char* context = NULL;
    int jdgflag;
    int* outputConext = NULL;
    
    if( NULL == name ) {
        name = getName(argv[1]); 
    }

    numByte = getNumByte(argv[2]);

    mode = getMode(argv[3]);

    if( NULL == context) {
        context = getContext(argv[4]);
    }

    jdgflag = D_ERROR;

    switch(mode) {
        case D_READ:
            if (NULL == outputConext)
            {
                jdgflag = readContext(name,&outputConext, numByte);
                printf("Byte read: %s\n", outputConext);
            }
            break;
        case D_WRITE:
            jdgflag = writeContext(name, context, numByte);
            break;
        default:
    }

    printf("\n\nStatus: %d\n",jdgflag);

}


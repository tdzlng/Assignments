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

// static global
static char buff[255];

// function prototype
static int getNumByte(const char* str);
static int getMode(const char* str);

static int writeContext(char* fileName, const char* context, int num);
static int readContext(char* fileName, char** context, int num);

// function definition
int getNumByte(const char* str)
{
    int ret = 0;
    while ((str != NULL) && (*str != 0))
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
    if( NULL != str){
        switch (str[0]) {
            case 'r':
                ret = D_READ;
                break;
            case 'w':
                ret = D_WRITE;
                break;
        
            default:
                ret = D_NONE;
                printf("mode error\n");
                break;
            }
    } else {
        ret = D_ERROR;
    }

    return ret;
}

#define __USE_XOPEN

int writeContext(char* fileName, const char* context, int num) {
    int fd;
    int ret = D_ERROR;

    fd = open(fileName, O_APPEND | O_WRONLY, S_IWUSR );

    if(D_ERROR != fd) {
        write(fd, context, num);
        ret = D_WRITE;
    } else {
        perror("open fail\n");
    }

    close(fd);
    return ret;
}

int readContext(char* fileName, char** context, int num) {
    int fd;
    int ret = D_ERROR;
    
    fd = open(fileName, O_RDONLY, S_IRUSR );

    if(D_ERROR != fd) {
        read(fd, buff, num);
        ret = D_READ;
        *context = buff;
    } else {
        perror("open fail\n");
    }
    close(fd);

    return ret;
}
#undef __USE_XOPEN

// main function
void main(int argc, char *argv[]){
    char* name = NULL;
    int numByte;
    int mode;
    char* context = NULL;
    int jdgflag;
    char* outputContext = NULL;
    
    if (( NULL == name ) && ( argc > 1 )) {
        name = argv[1]; 
    } else {

    }

    if ( argc > 2 ) {
        numByte = getNumByte(argv[2]);
    } else {

    }

    if ( argc > 3 ) {
        mode = getMode(argv[3]);
    }


    if (( NULL == context) && ( argc > 4 )) {
        context = argv[4];
    } else {

    }

    jdgflag = D_ERROR;

    switch(mode) {
        case D_READ:
            if (NULL == outputContext)
            {
                jdgflag = readContext(name,&outputContext, numByte);
                printf("Byte read: %s\n", outputContext);
            }
            break;
        case D_WRITE:
            jdgflag = writeContext(name, context, numByte);
            break;
        default:
            break;
    }

    printf("Status: %d\n",jdgflag);

}


// BT2. Viết một chương trình có só lượng command-line arguments là 3, có dạng như sau:
// $ ./example_program filename num-bytes [r/w] "Hello"
// Trong đó:
// 	1. example_grogram: Tên file thực thi
// 	2. filename: Tên file
// 	3. num-bytes: Số byte muốn read/write
// 	4. [r/w]: r -> Thực hiện đọc từ  filename và in ra màn hình
// 		    w -> Thực hiện ghi vào filename
// 	5. "Hello": Nội dung bất kì muốn read/write vào filename
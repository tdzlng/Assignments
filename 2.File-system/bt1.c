#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define D_ERROR (-1)
void main()
{
    const char* file = "log.txt";
    const char text[]  = "Place,time,temperature\n";
    int fd;
    fd = open("log.txt", O_APPEND |O_WRONLY);

    if(D_ERROR != fd){
        lseek(fd, 0, SEEK_SET);
        int c = write(fd, text, sizeof(text)-1);
        if(D_ERROR == c)
        {
            char *errmsg;
            errmsg = strerror( errno );
            printf("write error:%s\n",errmsg);
        }
        printf("open\n");
    } else {
        printf("error open\n");
    }

    close(fd);
}

/*BT1. Viết một chương trình mở một file bất kì và sử dụng cờ O_APPEND, sau đó thực hiện seek về đầu file 
rồi ghi một vài dữ  liệu vào file đó. Dữ liệu sẽ xuất hiện ở vị trí nào của file và tại sao lại như vậy?*/

/* file luon duoc ghi o cuoi du duoc set pointer */
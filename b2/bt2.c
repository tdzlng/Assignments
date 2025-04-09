// BT2. Sau mỗi lệnh write dưới đây, Cho biết nội dung sẽ được ghi vào file nào, nội dung là gì và giải thích tại sao?
// fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); => user read +write
// fd3 = open(file, O_RDWR);
// write(fd1, "Hello,", 6); => ghi ky tu Hello, vao 1 file moi
// write(fd2, "world", 6);
// lseek(fd2, 0, SEEK_SET);
// write(fd1, "HELLO,", 6); => Hello,HELLO, vao file
// write(fd3, "Gidday", 6); => Hello,HELLO,Gidday


// O_RDWR lenh read + write
// O_CREAT mo 1 file neu ko ton tai thi tao file
// O_TRUNC ghi de

#define __USE_XOPEN

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
    fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if(D_ERROR != fd){
        lseek(fd, 0, SEEK_SET);
        int c = write(fd, "Hello,", 6);
        c = write(fd, "HELLO,", 6);
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
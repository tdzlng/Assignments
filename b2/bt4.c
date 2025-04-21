#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

char* t = "YYYY-mm-dd hh:MM:SS";
// define constant
#define D_FILEPATH  (1)
#define D_DATA      (2)
#define D_ERROR     (-1)
#define D_OK        (0);

// function declaration
static int unixTime2DateTime(const struct stat* buffer, char** output);

// static global variable


// function definition
int unixTime2DateTime(const struct stat* buffer, char** output){
    int ret = D_OK;
    struct tm* info = NULL;

    if (( NULL != buffer) && ( NULL != output)){
        // convert unix time to date time
        info = localtime(&(buffer->st_mtime));
        *output = asctime(info);
    } else {
        ret = D_ERROR;
    }
    return ret;
}

// main function
void main(int argc, char *argv[]){
    char* filepath = "./default.txt";
    char* writeData = "test data";
    char* time = NULL;
    int statusfile;
    int statusWrite;
    int fd;
    struct stat buffer;

    if ( argc>1 ){
        filepath = argv[D_FILEPATH];
    }

    if ( argc>2 ){
        writeData = argv[D_DATA];
    }

    statusWrite = D_ERROR;
    fd = open(filepath, O_WRONLY | O_CREAT);
    if( D_ERROR!= fd){
        statusWrite = write(fd, writeData, 9);

        if (D_ERROR == statusWrite){
            perror("write fail\n");
        }
    } else {
        perror(" open fail\n");
    }
    close(fd);

    if (D_ERROR != statusWrite) {
        statusfile = stat(filepath, &buffer);

        unixTime2DateTime(&buffer, &time);

        printf("File type:%o,\nFile name:%s,\nLast modified: %s\nSize:%ld\n",
            buffer.st_mode, 
            filepath, 
            time, 
            buffer.st_size);

    } else {

    }

}


// BT3. Tạo file bất kì và ghi một đoạn dữ liệu vào file đó. 
// Thực hiện ghi ra màn hình các thông tin: Loại file, tên file, 
// thời gian chỉnh sửa file lần cuối, kích thước. - Sử dụng struct stat
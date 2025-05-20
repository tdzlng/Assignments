#include <stdio.h>
#include <unistd.h> // thu vien de su dung pipe vaf fork
#include <errno.h>
#include <stdlib.h> // define EXIT_FAILURE
#include <string.h>
#include <sys/wait.h>


// define constant
#define D_ERROR          (-1)
#define D_CHILD_PROCESS  (0)
#define D_MAX_BUFFER     (1024)

typedef enum {
    E_PIPE_READ_END = 0,
    E_PIPE_WRITE_END = 1
} E_PIPE_FD;

// define macro function
#define M_HANDLE_ERROR(msg) \
    { perror(msg); exit(EXIT_FAILURE);}
#define M_LOG(msg) \
    { printf("Log: %s\n",msg);}

// global static variable

static char buffer[D_MAX_BUFFER] = {0};
static char data[]="We and our third-party partners use cookies and similar technologies to run the website. Some cookies are strictly necessary. We also use optional cookies to provide a more personalized experience, improve the way our websites work and support our marketing operations. Optional cookies will only be set with your consent. You can manage your cookie preferences through the Manage Preferences button. For more information see ourPrivacy Notice";

// main process
void main(){
    int retRead;
    int totalByte;                                   
    char* ptr = NULL;
    int length;
    char dataLog[20];
    char readBuff[D_MAX_BUFFER] = {0};

    int fd[2];                                              // init file descriptor
    int PipeStatus = pipe(fd);                              // create one way pipe process
    int id = fork();                                        // create child process


    if(D_ERROR == PipeStatus) {
        M_HANDLE_ERROR("Cannot create pipe process\n");
    }

    if (D_ERROR == id){
        M_HANDLE_ERROR("Cannot create child process error\n");
    } else if (D_CHILD_PROCESS ==  id){                     // child process
        close(fd[E_PIPE_WRITE_END]);

        totalByte = 0;
        ptr = readBuff;
        memset(buffer,D_MAX_BUFFER,0);
        int i=0;
        do {
            retRead = read(fd[E_PIPE_READ_END], buffer, 60);
            if(D_ERROR == retRead){
                M_HANDLE_ERROR("Read pipe error\n");
            }

            
            // update status chus y cho nay khi retRead = 0 thi pipe van con chua data
            if(0 < retRead){
                length = strlen(buffer);
                memcpy(ptr,buffer, retRead);

                memset(buffer,D_MAX_BUFFER,0);
                ptr += retRead;
                totalByte += retRead;
            }

        }while(0 < retRead);

        
        sprintf(dataLog,"Total bytes: %d",totalByte);
        M_LOG(dataLog);
        M_LOG(readBuff);

        close(fd[E_PIPE_READ_END]);
        exit(EXIT_SUCCESS);
    } else {                                                // parrent process
        close(fd[E_PIPE_READ_END]);
        length = strlen(data);
            
        if(D_ERROR == write(fd[E_PIPE_WRITE_END], data, length)){
            M_HANDLE_ERROR("Write pipe error\n");
        }
        
        close(fd[E_PIPE_WRITE_END]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }


}

// parent process send data to child
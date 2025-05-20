#include <stdio.h>
#include <unistd.h> // thu vien de su dung pipe vaf fork
#include <errno.h>
#include <stdlib.h> // define EXIT_FAILURE
#include <string.h>


// define constant
#define D_ERROR          (-1)
#define D_CHILD_PROCESS  (0)
#define D_MAX_BUFFER     (100)

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

// main process
void main(){
    int fd[2];                                              // init file descriptor
    int PipeStatus = pipe(fd);                              // create one way pipe process
    int id = fork();                                        // create child process
    char data[] = "Send string to another process";
    char* ptr = NULL;
    int length;

    if(D_ERROR == PipeStatus) {
        M_HANDLE_ERROR("Cannot create pipe process\n");
    }

    if (D_ERROR == id){
        M_HANDLE_ERROR("Cannot create child process error\n");
    } else if (D_CHILD_PROCESS ==  id){                     // child process
        close(fd[E_PIPE_WRITE_END]);

        if(D_ERROR ==  read(fd[E_PIPE_READ_END], buffer, D_MAX_BUFFER)){
            M_HANDLE_ERROR("Read pipe error\n");
        }

        M_LOG(buffer);

        close(fd[E_PIPE_READ_END]);
    } else {                                                // parrent process
        close(fd[E_PIPE_READ_END]);
        length = strlen(data);
        ptr = data;
        sleep(5);
        for(int i=0; i< length; ++i){
            // sleep(1);
            if(D_ERROR == write(fd[E_PIPE_WRITE_END], ptr, 1)){
                M_HANDLE_ERROR("Write pipe error\n");
            }
            ++ptr;
        }
        

        close(fd[E_PIPE_WRITE_END]);
    }


}

// parent process send data to child
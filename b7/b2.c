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

// declaration
void parrent(int* fd);
void child_1(int* fd);
void child_2(int* fd);

// define macro function
#define M_HANDLE_ERROR(msg) \
    { perror(msg); exit(EXIT_FAILURE);}
#define M_LOG(msg) \
    { printf("Log: %s\n",msg);}

// global static variable
static const char text[] = "BT2: Sử dụng nhiều Pipe để giao tiếp giữa nhiều tiến trìnhY";
static char buffer[D_MAX_BUFFER] = {0};
static char someData[] = "   ____ZXCvbn213_____";
static int fd_P_C1[2];                                              // init file descriptor
static int fd_C1_C2[2];                                              // init file descriptor

// definition function

void parrent(int* fd){
    int length = strlen(text);

    if(D_ERROR ==  write(fd[E_PIPE_WRITE_END], text, length)){
        M_HANDLE_ERROR("Write pipe error\n");
    }

}

void child_1(int* fd){
    int PipeStatus = pipe(fd_C1_C2);                             // create one way pipe process
    int id = fork();                                            // create child process
    char *ptr = NULL;
    int length;
    if (D_ERROR == id){
        M_HANDLE_ERROR("Cannot create child 2 process error\n");
    } else if (D_CHILD_PROCESS ==  id){                         // child 2 process
        close(fd_C1_C2[E_PIPE_WRITE_END]);
        child_2(fd_C1_C2);
        close(fd_C1_C2[E_PIPE_READ_END]);
    } else {                                                    // child 1 process
        close(fd_C1_C2[E_PIPE_READ_END]);                        
        if(D_ERROR ==  read(fd[E_PIPE_READ_END], buffer, D_MAX_BUFFER)){
            M_HANDLE_ERROR("Read pipe error\n");
        }
        ptr = buffer;
        length = strlen(buffer);
        ptr+= length;
        memcpy(ptr, someData, strlen(someData));
        length += strlen(someData);
        if(D_ERROR ==  write(fd_C1_C2[E_PIPE_WRITE_END], buffer, strlen(buffer))){
            M_HANDLE_ERROR("Write pipe error\n");
        }        

        close(fd_C1_C2[E_PIPE_WRITE_END]);


    }

}

void child_2(int* fd){
    if(D_ERROR ==  read(fd[E_PIPE_READ_END], buffer, D_MAX_BUFFER)){
        M_HANDLE_ERROR("Read pipe error\n");
    }

    M_LOG(buffer);
}

void main (){

    int PipeStatus = pipe(fd_P_C1);                             // create one way pipe process
    int id = fork();                                            // create child process

    if (D_ERROR == id){
        M_HANDLE_ERROR("Cannot create child 1 process error\n");
    } else if (D_CHILD_PROCESS ==  id){                         // child 1 process
        close(fd_P_C1[E_PIPE_WRITE_END]); 
        child_1(fd_P_C1);
        close(fd_P_C1[E_PIPE_READ_END]);
    } else {                                                    // parrent process 
        close(fd_P_C1[E_PIPE_READ_END]);
        parrent(fd_P_C1);
        close(fd_P_C1[E_PIPE_WRITE_END]);
    }
    
}
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define D_ERROR             (-1)
#define D_CHILD_PROCESS     (0)


static unsigned int cnt = 1;

void handler_siguser1(int sig){
    printf("Received signal from parent %u times\n",cnt++);
    if(5 < cnt){
        exit(EXIT_SUCCESS);
    }
}

void childProcess(){
    // init signal
    signal(SIGUSR1, handler_siguser1);
    while(1);
    //
}

void parrentProcess(int childID){
    
    while (5 >= cnt)
    {
        sleep(1);
        kill(childID, SIGUSR1);
        ++cnt;
    }
    
    wait(NULL);
    exit(EXIT_SUCCESS);
}


void main(){

    int id = fork();

    if(D_ERROR == id){
        perror("Create child process error\n");
        exit(EXIT_FAILURE);
    } else if (D_CHILD_PROCESS == id) {
        childProcess();
    } else {
        parrentProcess(id);
    }

}

// Câu hỏi: Tại sao chúng ta cần sử dụng kill() để gửi tín hiệu trong bài này?
// kill de gui tin hieu sang 1 process khac
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>


#define D_ERROR         (-1)

void handler_sig(int sig);

void handler_sig(int sig){
    if(SIGINT ==  sig){
        printf("SIGINT received\n");
    }
}

void main(){
    int ret;
    char buff[20] = {0};
    struct pollfd pfd;

    pfd.fd = STDIN_FILENO;  
    pfd.events = POLLIN;            //handler_sig
    
    signal(SIGINT, handler_sig);
    signal(SIGTERM, SIG_DFL);
    

    printf("ID:%d isprocess\n",getpid());

    fflush(stdin);
    ret = poll(&pfd,1,20000);

    if(D_ERROR == ret){
        perror("poll error\n");
    }

    if(0 == ret){
        printf("Timeout\n");
    }

    if(pfd.revents & POLLIN){
        fgets(buff,20,stdin);
        printf("stdin is readable:%s\n",buff);
    }
    while(1){
        sleep(1);
        printf("ID:%d isprocess\n",getpid());
        printf("main loop\n");
    }
}

//poll(tro den pollfd, so luong fd, timeout ms)
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void handler_sig(int sig);

void handler_sig(int sig){
    if(SIGTSTP ==  sig){
        printf("SIGTSTP received\n");
    }
}

void main(){
    void* current_handler =  signal(SIGTSTP,SIG_IGN);


    while(1){
        sleep(1);
        printf("main loop\n");
        
        if(current_handler == SIG_DFL){
            printf("SIGTSTP ignored\n");
        }
    }
}
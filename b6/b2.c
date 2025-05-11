#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static unsigned int cnt = 0;

void handle_sigalrm(int sig) {
    alarm(1);
    printf("Timer: %u seconds\n",cnt++);
    if(10 <= cnt){
        cnt = 0;
        exit(EXIT_SUCCESS);
    }
}

void main(){
    signal(SIGALRM, handle_sigalrm);
    alarm(1); // set timer to 1s
    while (1)
    {
        
        printf("main loop\n");
        sleep(1);
    }
    
}

// Câu hỏi: Điều gì xảy ra nếu không gọi lại alarm(1) trong hàm xử lý?
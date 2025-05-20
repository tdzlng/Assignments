#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef D_REGISTER_SIGINT
void handle_sigint( int sig) {
    static unsigned int count = 1;
    printf("SIGINT received\n");
    ++count;
    if(3 < count){
        exit(EXIT_SUCCESS);
    }
}
#endif

void main(){
#ifdef D_REGISTER_SIGINT    
    signal(SIGINT, handle_sigint);
#endif
    while (1)
    {
        printf("main loop\n");
        sleep(1);
    }
    
}

// Câu hỏi: Nếu bỏ qua tín hiệu SIGINT, chuyện gì sẽ xảy ra khi nhấn Ctrl+C?
// SIGINT       P1990      Term    Interrupt from keyboard
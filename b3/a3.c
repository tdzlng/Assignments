#include <stdio.h>
#include <unistd.h>
#include <signal.h> 

void handle_sig1()  { 
    printf("Received a signal SIGUSR1"); 
}

void handle_sig2()  { 
    printf("Received a signal SIGUSR2"); 
} 

void init_sig(){
    signal(SIGUSR1, handle_sig1); // registor signal 10
    signal(SIGUSR2, handle_sig2); // registor signal 12
}

void main()  { 
    int id = fork();
    int pid = getpid();


    if(id > 0) {                 // parent process
        while(1){
            sleep(5);            // set period 5s to send the signal
            kill(id, SIGUSR1);   // send signal to child id
        }
    } else {
        init_sig();
        while (1){               // child process
            printf("child's ID %d loop: ",pid);
            sleep(1);
            printf("\n");
        }
    }

}
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static int cnt = 0;

#define __M_BREAK_AFTER(time) if(cnt==(time)){break;}

void main(int argc, char *argv[])  { 
    int id = fork();
    int pid = getpid();
    
    int status;

    if(id > 0) {                            // parent process
        while (1)
        {
            sleep(1);
            printf("PPID: %d | Parrent %d timelife: %ds\n", getppid(),pid, cnt++);
            __M_BREAK_AFTER(5);
        }
        exit(1);
        wait(&status);
        
    } else {                                // child process
        while(1){
            sleep(1);
            printf("PPID: %d | Child %d timelife: %ds\n", getppid(),pid, cnt++);
            // __M_BREAK_AFTER(5);
            
        }
        exit(2);
    }

}
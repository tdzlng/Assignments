#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/wait.h>

#define D_INVALID_NUM   (0x0fffffff)
#define D_OFF           (0)
#define D_ON            (1)
#define D_NULL_CHAR     (0)

int str2int(const char* str, int* output);

int str2int(const char* str, int* output){
    int ret = 0;
    int flagValid = D_ON;
    *output = 0;
    while ((*str != D_NULL_CHAR) || (flagValid == D_ON))
    {
        if(( *str >= '0') && ( *str <= '9')){
            *output = 10*(*output) + (int)(*str - '0');
            flagValid = D_ON;
        } else {
            flagValid = D_OFF;
        }
        ++str;
    }
    
    return flagValid;
}

void main(int argc, char *argv[])  { 
    int id = fork();
    int pid = getpid();
    int status;
    int childRetVal;

    if(argc > 1 ) {
        str2int(argv[1], &childRetVal);
    } else {
        childRetVal = 0;
    }

    if(id > 0) {                            // parent process
        wait(&status);
        if( WIFEXITED(status) )             // check child terminate normally with macro WIFEXITED
        {
            printf("Parent PID: %d\n", pid);
            printf("Parent process has child's return value: %d\n", WEXITSTATUS(status)); // return exit child status with macro WEXITSTATUS
        }
    } else {                                // child process
        sleep(2);
        printf("Child PID: %d\n", pid);
        exit(childRetVal);
    }

}
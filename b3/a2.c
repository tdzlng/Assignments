#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>


// declaration
static int strcmp(const char* str1,const char* str2);
static int getMode(const char* mode);

// definition
int strcmp(const char* str1,const char* str2){
    int ret = 0;
    int flag = 1;
    while((0 != *str1) && (0 != *str2) && (flag == 1)){
        if((*str1 - *str2) > 0){
            ret = 1;
            flag = 0;
        } else if ((*str1 - *str2) < 0) {
            ret = -1;
            flag = 0;
        }
        ++str1;
        ++str2;
    }

    return ret;
}

int getMode(const char* mode){
    const char* table[2]= {"1", "2"};
    int ret = -1;
    if( 0 == strcmp(table[0], mode)) {
        ret = 1;
    } else if ( 0 == strcmp(table[1], mode) ) {
        ret = 2;
    }
    return ret;
}

// use export to import enviroment variable
void main(int argc, char* argv[]){

    int mode;
    char *cmd[2][2]= {  {"/bin/ls"  ,NULL},
                        {"/bin/date",NULL}};

    if( argc > 0) {
        mode = getMode(argv[1]);
    } else {
        mode = -1;
    }

    printf("mode: %d\n",mode);

    if(( mode == 1) || ( mode == 2)){
        execvp(cmd[mode - 1][0], cmd[mode-1]);
    }

    printf("process a2 end here\n");    // -> not execute command when replace to another process
    while (1);                          // -> not execute command when replace to another process
}
#include <stdio.h>
#include "control.h"

static E_STATE_PROCESS state = E_STATE_NONE;

E_STATE_PROCESS* ctrl_getState(){
    return &state;
}

typedef struct {
    char command[10];
    char arg1[16];
    char arg2[100];
} parseCMD_t;



static parseCMD_t parser = {0};
static const char cmd [8][10]={ "help",
                                "myip",
                                "myport",
                                "connect",
                                "list",
                                "terminate",
                                "send",
                                "exit"};

void ctrl_bfTsk(){

}

void ctrl_afTsk(){

}

void ctrl_control(){

}

void ctrl_getInput(){
    
    char buff[256] = {0};

    fflush(stdin);
    fgets(buff, sizeof(buff), stdin);
    sscanf("%s %s %s", parser.command, parser.arg1, parser.arg2);


}

void ctrl_getDisplay(){
    str
}


void
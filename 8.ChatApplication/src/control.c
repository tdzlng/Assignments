#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "control.h"
#include "tcpsocket.h"

static void s_help();
static void s_myip();
static void s_myport();
static void s_connect();
static void s_list();
static void s_terminate();
static void s_send();
static void s_exit();


#define D_NUM_CMD (8)

static E_STATE_PROCESS state = E_STATE_NONE;

typedef struct {
    char command[10];
    char arg1[16];
    char arg2[100];
} parseCMD_t;

typedef void (*operation_t)(void);


static parseCMD_t parser = {0};

static const char cmd [D_NUM_CMD][10] = {
    "help",
    "myip",
    "myport",
    "connect",
    "list",
    "terminate",
    "send",
    "exit"
};

static operation_t task[D_NUM_CMD] = {
    s_help,
    s_myip,
    s_myport,
    s_connect,
    s_list,
    s_terminate,
    s_send,
    s_exit
};

E_STATE_PROCESS* ctrl_getState(){
    return &state;
}

void ctrl_bfTsk(){

}

void ctrl_afTsk(){

}

void ctrl_waitClientAccept(){
    ts_acceptClient();
}
#include <stdio.h>
void ctrl_control(){
    int flag = -1;

    for(int i=0; i<D_NUM_CMD; ++i){
        if(strcmp(cmd[i], parser.command) == 0){
            flag = i;
            i=D_NUM_CMD; // break loop
        }
    }

    if ((flag>-1) &&
        (flag<D_NUM_CMD)){
        task[flag]();
    } else {
        /* TODO invald cmd*/
    }
}

void ctrl_getInput(){
    char buff[100] = {0};
    
    fflush(stdin);
    fgets(buff, sizeof(buff), stdin);
    sscanf(buff,"%s %s %s", parser.command, parser.arg1, parser.arg2);
}

void ctrl_initHost(int port){
    ts_initHost(port);
}

void ctrl_deinitHost(){
    ts_deinitHost();
}

static void s_help(){
    gui_drawCMD();
}

static void s_myip(){
    char ip[16] = {0};

    ts_getHostIP(ip);
    gui_showIP(ip);
}

static void s_myport(){
    int port;

    port = ts_getHostPort();
    gui_showPort(port);
}

static void s_connect(){

}

static void s_list(){

}

static void s_terminate(){

}

static void s_send(){
    
}

static void s_exit(){
    ts_destroyAllPeerMachine();
    gui_exit();
    state = E_STATE_EXIT;
}


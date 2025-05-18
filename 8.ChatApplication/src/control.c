#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "gui.h"
#include "control.h"
#include "tcpsocket.h"
#include "log.h"

extern int ts_dataFD;

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

void* ctrl_read(void* arg){
    char* buff = NULL;
    int length; 
    char* ip = NULL;
    int port;
    int status = 1;
    int sockFD;

    sockFD = ts_dataFD;
    ts_dataFD = 0;

    while(status > 0){
        status = ts_recvMsg(sockFD, &buff, &ip, &port);
        length = strlen(buff);
        gui_drawMsg(buff, length, ip, port);
    }
}



void ctrl_bfTsk(){
    memset((char*)&parser,0, sizeof(parser));
}

void ctrl_afTsk(){

}

void ctrl_waitClientAccept(){
    ts_acceptClient();
}

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
    char buff[256] = {0};
    char* status = NULL;
    
    fflush(stdin);
    status = fgets(buff, sizeof(buff), stdin);
    sscanf(buff,"%s %s %s", parser.command, parser.arg1, parser.arg2);
}

void ctrl_initHost(int port){
    ts_initHost(port);
    ts_initCb(ctrl_read);
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
    int port = atoi(parser.arg2);
    char* ip = parser.arg1;
    int status;

    /** TODO them dieu kien check valid ip */
    if(1){
        status = ts_connectPeer(ip, port);
    } else {
        status = -1;
    }

    /* Notify Success */
    if(status == 0){
        gui_notify(E_NOTIFY_CONNECT_SUCC);
    } else {
        gui_notify(E_NOTIFY_ERROR);
    }
}

static void s_list(){
    gui_list();
}

static void s_terminate(){
    int status;
    int id = atoi(parser.arg1);

    if(1){
        status = ts_removePeerSocket(id);
    } else {

    }
}

static void s_send(){
    int id;
    char* msg = NULL;
    int status;

    id = atoi(parser.arg1);
    if(1){
        msg = parser.arg2;
        status = ts_sendMsg(msg, id);
    } else {

    }
}

static void s_exit(){
    ts_destroyAllPeerMachine();
    gui_notify(E_NOTIFY_EXIT);
    state = E_STATE_EXIT;
}
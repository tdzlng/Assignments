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


typedef struct {
    char command[10];
    char arg1[16];
    char arg2[100];
} parseCMD_t;

typedef void (*operation_t)(void);


static E_STATE_PROCESS state = E_STATE_NONE;
static E_NOTIFY_GUI_MESSAGE flagNotify = E_NOTIFY_GUI_NONE;
static E_ERROR_GUI_CODE flagError = E_ERROR_GUI_NONE;
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

void ctrl_initControl(int port){
    /* init host socket */
    ts_initHost(port);

    /* register callback */
    ts_initCbRead(ctrl_read);
    ts_initCbConnect(ctrl_notifyPeerConnected);
    ts_initCbSetAddr(ctrl_notifySetAddr);
}

void ctrl_deinitControl(){
    ts_deinitHost();
}

void* ctrl_read(void* arg){
    int sockFD;
    char* buff = NULL;
    int length; 
    char* ip = NULL;
    int port;
    int status = 1;

    sockFD = ts_dataFD;
    ts_dataFD = 0;
    
    while(status > D_EOF){
        status = ts_recvMsg(sockFD, &buff, &ip, &port);
        if( status > D_EOF) {
            length = strlen(buff);
            gui_drawMsg(buff, length, ip, port);
        }
    }

}

void ctrl_bfTsk(){
    memset((char*)&parser,0, sizeof(parser));
    flagNotify = E_NOTIFY_GUI_NONE;
    flagError = E_ERROR_GUI_NONE;
}

void ctrl_afTsk(){

}

void ctrl_waitClientAccept(){
    ts_acceptClient();
}

void ctrl_getInput(){
    char buff[256] = {0};
    
    fflush(stdin);
    fgets(buff, sizeof(buff), stdin);
    sscanf(buff,"%s %s %s", parser.command, parser.arg1, parser.arg2);
}

void ctrl_control(){
    int cmdID = -1;

    for(int i=0; i<D_NUM_CMD; ++i){
        if(strcmp(cmd[i], parser.command) == 0){
            cmdID = i;
            i = D_NUM_CMD; // break loop
        }
    }

    if ((cmdID > -1) &&
        (cmdID < D_NUM_CMD)){
        /* if flag between command 0~7 process main task */
        task[cmdID]();
    } else {
        flagError = E_ERROR_GUI_INVALID_COMMAND;
    }
}

void ctrl_notifyWarning(){
    gui_notify(flagNotify);
    gui_error(flagError);
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

    status = ts_connectPeer(ip, port);
    
    switch (status){
    case E_ERROR_TS_DUPLICATE_CONECTION:
        flagError = E_ERROR_GUI_DUPLICATE_CONECTION;
        break;

    case E_ERROR_TS_INVALID_IP:
        flagError = E_ERROR_GUI_INVALID_IP;
        break;
    
    case E_ERROR_TS_SELF_CONNECT:
        flagError = E_ERROR_GUI_SELF_CONNECT;
        break;

    case E_ERROR_TS_NONE:
        gui_setAddrNotify(ip, port);
        flagNotify = E_NOTIFY_GUI_CONNECT_SUCCESS;
        break;

    default:
        break;
    }
}

void ctrl_notifyPeerConnected(char* ip, int port){
    gui_setAddrNotify(ip, port);
    gui_notify(E_NOTIFY_GUI_CONNECT_SUCCESS);
}

void ctrl_notifySetAddr(char* ip, int port){
    gui_setAddrNotify(ip, port);
}

static void s_list(){
    gui_list();
}

static void s_terminate(){
    int status;
    int id = atoi(parser.arg1);

    if(id > 0){
        status = ts_removePeerSocket(id-1);

        if(D_OK == status){
            flagNotify = E_NOTIFY_GUI_PEER_DISCONNECTED;
        } else {
            flagError = E_ERROR_GUI_INVALID_IP;
        }
    } else {
        flagError = E_ERROR_GUI_INVALID_ID;
    }
}

static void s_send(){
    int id;
    char* msg = NULL;
    int status;

    id = atoi(parser.arg1);
    if(id > 0){
        msg = parser.arg2;
        status = ts_sendMsg(msg, id-1);

        if( status > 0){
            flagNotify = E_NOTIFY_GUI_SEND_SUCCESS;
        } else {
            flagError = E_ERROR_GUI_INVALID_ID;
        }
    } else {
        flagError = E_ERROR_GUI_INVALID_ID;
    }
}

static void s_exit(){
    ts_destroyAllPeerMachine();
    gui_notify(E_NOTIFY_GUI_EXIT);
    state = E_STATE_EXIT;
}
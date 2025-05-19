#ifndef CONTROL_H
#define CONTROL_H

/* DEFINE CONTSTANTS */
typedef enum {
    E_STATE_NONE = -1,
    E_STATE_EXIT,
    E_STATE_CONTROL,
    E_STATE_GET_INPUT,
    E_STATE_DISPLAY
} E_STATE_PROCESS;

/* PUBLIC FUNCTION PROTOTYPE */
E_STATE_PROCESS* ctrl_getState();
void ctrl_bfTsk();
void ctrl_control();
void ctrl_getInput();
void ctrl_notifyWarning();
void ctrl_initControl(int port);
void ctrl_deinitControl();
void ctrl_waitClientAccept();
void* ctrl_read(void* arg);
void ctrl_notifyPeerConnected(char* ip, int port);
void ctrl_notifySetAddr(char* ip, int port);

#endif
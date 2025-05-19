#ifndef GUI_H
#define GUI_H

/* DEFINE CONSTANTS */
typedef enum {
    E_ERROR_GUI_NONE = 0,
    E_ERROR_GUI_INVALID_CONECTION,
    E_ERROR_GUI_INVALID_ID,
    E_ERROR_GUI_INVALID_IP,
    E_ERROR_GUI_SELF_CONNECT,
    E_ERROR_GUI_DUPLICATE_CONECTION,
    E_ERROR_GUI_INVALID_COMMAND
} E_ERROR_GUI_CODE;

typedef enum {
    E_NOTIFY_GUI_NONE = 0,
    E_NOTIFY_GUI_EXIT,
    E_NOTIFY_GUI_CONNECT_SUCCESS,
    E_NOTIFY_GUI_PEER_DISCONNECTED,
    E_NOTIFY_GUI_SEND_SUCCESS
} E_NOTIFY_GUI_MESSAGE;

/* PUBLIC TYPEDEF */
typedef struct {
    int port;
    int valid;
    char ip[16];
} listConnection_t;

/* PUBLIC FUNCTION PROTOTYPE */
void gui_drawMsg(char* buff, int length, char* ip, int port);
void gui_drawCMD();
void gui_showIP(char* ip);
void gui_showPort(int port);
void gui_list();
void gui_error(E_ERROR_GUI_CODE errorCode);
void gui_notify(E_NOTIFY_GUI_MESSAGE type);
void gui_setAddrNotify(char* ip, int port);

#endif
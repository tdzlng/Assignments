#define D_MAX_LENGTH        (100)
#define D_MAX_CONNECTION    (10)

typedef enum {
    E_ERROR_GUI_INVALID_CONECTION = 0,
    E_ERROR_GUI_INVALID_IP,
    E_ERROR_GUI_SELF_CONNECT,
    E_ERROR_GUI_DUPLICATE_CONECTION,
    E_ERROR_GUI_NONE
} E_ERROR_GUI_CODE;

typedef enum {
    E_NOTIFY_ERROR = 0,
    E_NOTIFY_EXIT,
    E_NOTIFY_CONNECT_SUCC
} E_NOTIFY_MESSAGE;

typedef struct {
    int port;
    int valid;
    char ip[16];
} listConnection_t;

void gui_drawMsg(char* buff, int length, char* ip, int port);
void gui_drawCMD();
void gui_showIP(char* ip);
void gui_showPort(int port);
void gui_list();
void gui_error(E_ERROR_GUI_CODE errorCode);
void gui_notify(E_NOTIFY_MESSAGE type);
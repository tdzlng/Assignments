#define D_MAX_ACCEPT_MACHINE   (5U)

typedef enum {
    E_ERROR_TS_INVALID_CONECTION = 0,
    E_ERROR_TS_INVALID_IP,
    E_ERROR_TS_SELF_CONNECT,
    E_ERROR_TS_DUPLICATE_CONECTION,
    E_ERROR_TS_MAX_CONNECTION,
    E_ERROR_TS_NONE
} E_ERROR_TS_CODE;

typedef enum {
    E_CB_TS_CONNECT = 0,
    E_CB_TS_SET_ADDR,
    E_CB_TS_MAX
} E_CB_TS_CODE;

typedef void* (*cbFuncV_t)(void*);
typedef void (*cbFuncCInt_t)(char*, int);

void ts_initCbRead(cbFuncV_t ptr);
void ts_initCbConnect(cbFuncCInt_t ptr);
void ts_initCbSetAddr(cbFuncCInt_t ptr);
void ts_initHost(int port);
void ts_deinitHost();
int ts_getHostPort();
void ts_getHostIP(char* hostIP);
int ts_getDataFD();
int ts_getDataPeer(char (*ip)[16], int *port);
void ts_acceptClient();
int ts_connectPeer(char* ip, int port);
int ts_sendMsg(char* msg, int id);
int ts_recvMsg(int socketFD, char** msg, char** ip, int* port);
void ts_destroyAllPeerMachine();
int ts_removePeerSocket(int id);

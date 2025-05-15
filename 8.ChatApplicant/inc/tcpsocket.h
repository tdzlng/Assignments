#define D_MAX_ACCEPT_MACHINE   (5U)

void ts_initHost(int port);
void ts_deinitHost();
void ts_acceptClient();
void ts_connectPeer(char* ip, int port);
void ts_connectPeer(char* ip, int port);
void ts_acceptClient();
int ts_sendMsg(char* msg, char* ip, int port);
int ts_recvMsg(int socketFD, char** msg, char** ip, int* port);
int ts_getHostPort();
void ts_getHostIP(char* hostIP);
void ts_destroyAllPeerMachine();
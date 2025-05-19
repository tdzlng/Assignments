#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "tcpsocket.h"

/* DEFINE CONSTANTS */
#define D_MAX_LENGTH        (100)
#define D_MAX_CONNECTION    (5)

/* STATIC VARIABLES */
static const char listCMD[] = "1. help\n2. myip\n3. myport\n4. connect <destination> <port no>\n5. list\n6. terminate <connection id.>\n7. send <connection id.> <message>\n\n";
static char peerIP[D_MAX_CONNECTION][16];
static int peerPort[D_MAX_CONNECTION];

/* PUBLIC FUNCTION */
void gui_drawMsg(char* buff, int length, char* ip, int port){
    if(length > D_MAX_LENGTH){
        length = D_MAX_LENGTH;
    }
    buff[length] = 0;

    printf("Message received from %s\n", ip);
    printf("Sender's Port: %d\n", port);
    printf("Message: %s\n\n", buff);
}

void gui_drawCMD(){
    printf("%s", listCMD);
}

void gui_showIP(char* ip){
    printf("IP: %s\n", ip);
}

void gui_showPort(int port){
    printf("Port: %d\n", port);
}

void gui_list(){
    int maxPeer;

    maxPeer = ts_getDataPeer(peerIP, peerPort);
    if(maxPeer > 0){
        printf("id:\tIP address\tPort No.\n");
        for(int i=0; i<maxPeer; ++i){
            printf("%2d:\t%.15s\t%d\n",i+1, peerIP[i], peerPort[i]);
        }
    } else {
        printf("List chat is empty\n");
    }
    printf("\n");
}

void gui_error(E_ERROR_GUI_CODE errorCode){
    switch (errorCode){
    case E_ERROR_GUI_INVALID_CONECTION:
        printf("Connection does not exist\n");
        break;

    case E_ERROR_GUI_INVALID_IP:
        printf("Invalid IP address\n");
        break;

    case E_ERROR_GUI_SELF_CONNECT:
        printf("Self connect to host\n");
        break;

    case E_ERROR_GUI_INVALID_COMMAND:
        printf("Invalid command line\n");
        break;

    case E_ERROR_GUI_DUPLICATE_CONECTION:
        printf("Peer is alrealdy connected\n");
        break;

    case E_ERROR_GUI_INVALID_ID:
        printf("Invalid ID\n");
        break;

    default:
        break;
    }
}

void gui_setAddrNotify(char* ip, int port){
    memcpy(peerIP[0], ip, 16);
    peerPort[0] = port;
}

void gui_notify(E_NOTIFY_GUI_MESSAGE type){
    switch (type){
    case E_NOTIFY_GUI_CONNECT_SUCCESS:
        printf("Address %s:%d Connected\n\n", peerIP[0], peerPort[0]);
        break;

    case E_NOTIFY_GUI_EXIT:
        printf("Terminate all connection and exit program\n");
        break;

    case E_NOTIFY_GUI_SEND_SUCCESS:
        printf("Sent to %s:%d\n", peerIP[0], peerPort[0]);
        break;

    case E_NOTIFY_GUI_PEER_DISCONNECTED:
        printf("%s:%d disconnected\n", peerIP[0], peerPort[0]);
        break;

    default:
        break;
    }
}
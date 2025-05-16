#include <stdio.h>
#include "gui.h"
#include "tcpsocket.h"


static const char listCMD[] = "1. help\n2. myip\n3. myport\n4. connect <destination> <port no>\n5. list\n6. terminate <connection id.>\n7. send <connection id.> <message>\n\n";
static char peerIP[D_MAX_CONNECTION][16];
static int peerPort[D_MAX_CONNECTION];

/* TODO: xu ly khi co nhieu thread in ra cli */
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

    maxPeer = ts_getDataPeer((char**)peerIP, peerPort);
    if(maxPeer > 0){
        printf("id:\tIP address\tPort No.\n");
        for(int i=0; i<maxPeer; ++i){
            printf("%d:\t%s\t%d\n",i, peerIP[i], peerPort[i]);
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
    case E_ERROR_GUI_DUPLICATE_CONECTION:
        printf("Alrealdy connect to this peer\n");
        break;
    default:
        break;
    }
}

void gui_notify(E_NOTIFY_MESSAGE type){
    switch (type){
    case E_NOTIFY_CONNECT_SUCC:
        printf("Connect to Peer Successfully\n");
        break;
    case E_NOTIFY_EXIT:
        printf("Terminate all conection and exit program\n");
        break;
    default:
        /* TODO error process anouncement */
        gui_error(1);
        break;
    }
}
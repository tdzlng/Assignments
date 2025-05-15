#include <stdio.h>
#include "gui.h"


static const char listCMD[] = "1. help\n2. myip\n3. myport\n4. connect <destination> <port no>\n5. list\n6. terminate <connection id.>\n7. send <connection id.> <message>\n\n";
static listConnection_t peer[D_MAX_CONNECTION];

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
    /* TODO get data from queue*/
    // queue_getDataPeer();
    printf("id:\tIP address\tPort No.\n");
    for(int i=0; i<D_MAX_CONNECTION; ++i){
        if(peer[i].valid == 1){
            printf("%d:\t%s\t%d\n",peer[i].id, peer[i].ip, peer[i].port);
        }
    }
    printf("\n");
}

void gui_error(int errorCode){
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
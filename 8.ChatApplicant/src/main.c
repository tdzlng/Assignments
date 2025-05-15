#include "control.h"
#include "tcpsocket.h"
#include <pthread.h>
#include <stdlib.h>

#define D_PORT              (1)
#define D_MAX_CONECTTION    (10)

void main(int argc, char* argv[]){
    int port;
    pthread_t threadAcceptID;
    // pthread_t peerConnectedID[D_MAX_CONECTTION];
    E_STATE_PROCESS* state = NULL; 
    
    /* init state of screen = E_STATE_NONE */
    state = ctrl_getState(); 

    if (argc > 1){
        /* main process here */
        port = atoi(argv[D_PORT]);
        if(port == 0){
            /* TODO process invalid arg */
        } else {
            ts_initHost(port);

            pthread_create(&threadAcceptID, NULL, (void * (*)(void *))ts_acceptClient, NULL);

            *state = E_STATE_DISPLAY;
            while ((E_STATE_NONE != *state) &&
                    (E_STATE_EXIT != *state)) {
                
                ctrl_bfTsk();
                ctrl_getInput();
                ctrl_control();
                ctrl_getDisplay();
                ctrl_afTsk();
            }
        }
    } else {
        /* TODO error handler error command */

    }

    

    ts_deinitHost();
    exit(*state);
}
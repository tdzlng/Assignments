#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "control.h"

#define _BSD_SOURCE
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
            ctrl_initHost(port);

            pthread_create(&threadAcceptID, NULL, (void * (*)(void *))ctrl_waitClientAccept, NULL);

            
            *state = E_STATE_DISPLAY;
            while ((E_STATE_NONE != *state) &&
                    (E_STATE_EXIT != *state)) {

                ctrl_bfTsk();
                ctrl_getInput();
                ctrl_control();
                ctrl_afTsk();

                usleep(10); /* process all task in 10ms */
            }
        }
    } else {
        /* TODO error handler error command */

    }

    pthread_cancel(threadAcceptID);
    ctrl_deinitHost();
    exit(*state);
}
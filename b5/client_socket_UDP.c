#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/un.h>

// preprocessor directives
#if defined(D_UNIX_STREAM) || defined(D_UNIX_DATAGRAM)
// Do nothing
#else
// Defualt mode
#define D_UNIX_DATAGRAM
#endif

// define constant
#define D_PATH_SERVER          "./Socket_server"
#ifdef D_UNIX_DATAGRAM
#define D_PATH_CLIENT          "./Socket_client"
#endif

#define D_BUFF_SIZE     (256U)
#define D_ERROR         (-1)

// macro function
#define M_HANDLE_ERROR(msg) \
    { perror(msg); exit(EXIT_FAILURE);}
#define M_LOG(msg) \
    { printf("Client Log: %s\n",msg);}

// static globlal variable
static int client_fd;
static struct sockaddr_un serv_addr = { 0 };
static struct sockaddr_un client_addr = { 0 };
static char bufferSend[D_BUFF_SIZE];
static char bufferRev[D_BUFF_SIZE];

// function definition
void init_client(){
    int optval;
    char path[] = D_PATH_SERVER;

    if ((strlen(path)) > sizeof(serv_addr.sun_path)) {
        M_HANDLE_ERROR("Path longer than 108 charactor\n");
    }
    client_addr.sun_family = AF_UNIX;
    char path2[] = D_PATH_CLIENT;
    memcpy(serv_addr.sun_path, path, strlen(path));

    // init socket
#ifdef D_UNIX_STREAM
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0); // protocol luon la 0
#endif
#ifdef D_UNIX_DATAGRAM
    client_fd = socket(AF_UNIX, SOCK_DGRAM, 0); // protocol luon la 0
#endif
    if (D_ERROR == client_fd) {
        M_HANDLE_ERROR("Error Socket()\n");
    }

    // init path
    serv_addr.sun_family = AF_UNIX;
    memcpy(serv_addr.sun_path, path, strlen(path));

#ifdef D_UNIX_STREAM
    // setup conection to server
    if( D_ERROR == connect(client_fd, (const struct sockaddr *)(&serv_addr), sizeof serv_addr)){
        M_HANDLE_ERROR("Error connect()\n");
    }
#endif

#ifdef D_UNIX_DATAGRAM
    optval = 1;
    setsockopt(client_fd, SOL_SOCKET, SO_PASSCRED, &optval, sizeof(optval));   // set option send with credentials. The receiving process can then retrieve these credentials
#endif

}

void deinit_client(){
    remove(D_PATH_SERVER);
    remove(D_PATH_CLIENT);
}

void client(){
    char testData[] = "Ping Server";
    int opt;
    size_t msgLength;
    memset(bufferSend, 0, D_BUFF_SIZE);
    memset(bufferRev, 0, D_BUFF_SIZE);

    msgLength = sizeof testData;
    memcpy(bufferSend, testData, msgLength);

    // send to server
#ifdef D_UNIX_STREAM
    if( D_ERROR == write(client_fd, bufferSend, msgLength) ){
        M_HANDLE_ERROR("Error write()\n");
    }
#endif
#ifdef D_UNIX_DATAGRAM
    msgLength = strlen(bufferSend);
    if ( msgLength != sendto(client_fd, bufferSend, msgLength, 0, (const struct sockaddr*)(&serv_addr), sizeof serv_addr)) {
        M_HANDLE_ERROR("Error sendto()\n");
    }
#endif

#ifdef D_UNIX_DATAGRAM
    // received from server
    if ( D_ERROR == recvfrom(client_fd, bufferRev, D_BUFF_SIZE, 0, NULL, NULL)) {
        M_HANDLE_ERROR("Error recvfrom()\n");
    }
#endif

#ifdef D_UNIX_STREAM
    // read back from server
    if( D_ERROR == read(client_fd, bufferRev, D_BUFF_SIZE)) {
        M_HANDLE_ERROR("Error read()\n");
    }
#endif
    M_LOG("data received!: ");
    M_LOG(bufferRev);
}



// main function
void main(int argc, char* argv[])
{
    M_LOG("Client running:\n");
    init_client();
    client();
    // while(1){
    //     sleep(1);
    //     client(); 
    // }
    deinit_client();
    exit(EXIT_SUCCESS);
}
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// preprocessor directives
#if defined(D_IPV4_STREAM) || defined(D_IPV4_DATAGRAM) || defined(D_UNIX_STREAM) || defined(D_UNIX_DATAGRAM)
// Do nothing
#else
// Defualt mode
#define D_IPV4_DATAGRAM
#endif

// define constant
#define D_SERVER_IP     "127.0.0.1"
#define D_PORT          (8080U)
#define D_BUFF_SIZE     (256U)
#define D_ERROR         (-1)

// macro function
#define M_HANDLE_ERROR(msg) \
    { perror(msg); exit(EXIT_FAILURE);}
#define M_LOG(msg) \
    { printf("Log message: %s\n",msg);}

// static globlal variable
static int client_fd;
static struct sockaddr_in serv_addr = { 0 };
static char bufferSend[D_BUFF_SIZE];
static char bufferRev[D_BUFF_SIZE];

// function definition
void init_client(){

    // init socket
#ifdef D_IPV4_STREAM
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
#endif
#ifdef D_IPV4_DATAGRAM
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
#endif
    if (D_ERROR == client_fd) {
        M_HANDLE_ERROR("Error Socket()\n");
    }

    // init address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(D_PORT);
    
    if ( D_ERROR == inet_pton(AF_INET, D_SERVER_IP, &(serv_addr.sin_addr)) ){
        M_HANDLE_ERROR("Error inet_pton()\n");
    }

    // setup conection to server
#ifdef D_IPV4_STREAM
    if ( D_ERROR == connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ){
        M_HANDLE_ERROR("Error connect()\n");
    }
#endif

}

void deinit_client(){
    close(client_fd);
}

void client(){
    char testData[] = "Ping Server";
    size_t msgLength;
    memset(bufferSend, 0, D_BUFF_SIZE);
    memset(bufferRev, 0, D_BUFF_SIZE);

    memcpy(bufferSend, testData, sizeof testData);

    // send to server
#ifdef D_IPV4_STREAM
    if( D_ERROR == write(client_fd, bufferSend, D_BUFF_SIZE) ){
        M_HANDLE_ERROR("Error write()\n");
    }
#endif
#ifdef D_IPV4_DATAGRAM
    msgLength = strlen(bufferSend);
    if ( msgLength != sendto(client_fd, bufferSend, msgLength, 0, (const struct sockaddr*)(&serv_addr), sizeof serv_addr)) {
        M_HANDLE_ERROR("Error sendto()\n");
    }
#endif

#ifdef D_IPV4_DATAGRAM
    // received from server
    // note: client co the biet thang nao gui qua recvfrom
    if ( D_ERROR != recvfrom(client_fd, bufferRev, D_BUFF_SIZE, 0, NULL, NULL)) {
        M_HANDLE_ERROR("Error recvfrom()\n");
    }
#endif

#ifdef D_IPV4_STREAM
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
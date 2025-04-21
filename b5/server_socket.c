#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// preprocessor directives
#if defined(D_IPV4_STREAM) && defined(D_IPV4_DATAGRAM) && defined(D_UNIX_STREAM) && defined(D_UNIX_DATAGRAM)
// Do nothing
#else
#define D_IPV4_DATAGRAM
#endif

// define constant
#define D_SERVER_IP     "127.0.0.1"
#define D_PORT          (8080U)
#define D_BUFF_SIZE     (256U)
#define D_ERROR         (-1)
#define D_BACK_LOG      (3)

// macro function
#define M_HANDLE_ERROR(msg) \
    { perror(msg); exit(EXIT_FAILURE);}
#define M_LOG(msg) \
    { printf("Log message: %s\n",msg);}


// static globlal variable
static int server_fd;
static int new_socket_fd;
static int opt;
static struct sockaddr_in server_addr = { 0 };
static struct sockaddr_in client_addr = { 0 };
static char bufferSend[D_BUFF_SIZE];
static char bufferRev[D_BUFF_SIZE];

// function definition
void init_server(){

    // init socket
    if(D_ERROR == socket(AF_INET, SOCK_STREAM, 0)){
        M_HANDLE_ERROR("Error socket()\n");
    }

    /* Ngăn lỗi : “address already in use” */
    if(D_ERROR == setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof opt)) {
        M_HANDLE_ERROR("Error setsockopt()\n");
    }

    // init server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; //inet_addr("192.168.243.128");
    server_addr.sin_port = htons(D_PORT);

    // bind address with server
    if ( D_ERROR == bind(server_fd, (const struct sockaddr*)(&server_addr), sizeof server_addr) ){
        M_HANDLE_ERROR("Error bind()\n");
    }

#ifdef D_IPV4_STREAM
    // nghe toi da 3 ket noi trong hang doi
    if (D_ERROR == listen(server_fd, D_BACK_LOG) ) {
        M_HANDLE_ERROR("Error listen()\n");
    }
#endif
}

void deinit_server(){
    close(server_fd);
}

void server(){
    char testData[] = "Return to client";
    size_t msgLength;
    memset(bufferSend, 0, D_BUFF_SIZE);
    memset(bufferRev, 0, D_BUFF_SIZE);

#ifdef D_IPV4_STREAM
    // block until client is connect
    new_socket_fd = accept(server_fd, (struct sockaddr*)(&server_addr), (socklen_t*)(&msgLength));
    if(D_ERROR == new_socket_fd) {
        M_HANDLE_ERROR("Error accept()\n");
    }

    // read from client
    if(D_ERROR == read(new_socket_fd, bufferRev, D_BUFF_SIZE)){
        M_HANDLE_ERROR("Error read()\n");
    }
    M_LOG("data received!: ");
    M_LOG(bufferRev);

    // feedback to client
    msgLength = strlen(bufferRev);
    memcpy(bufferSend, bufferRev, msgLength);
    if(D_ERROR == write(new_socket_fd, bufferSend, msgLength)) {
        M_HANDLE_ERROR("Error write()\n");
    }
    close(new_socket_fd);
#endif

#ifdef D_IPV4_DATAGRAM
    // read from client and client's address
    if (D_ERROR == recvfrom(server_fd, bufferRev, D_BUFF_SIZE, 0, (struct sockaddr*)(&new_socket_fd), NULL)) {
        M_HANDLE_ERROR("Error recvfrom()\n");
    }

    M_LOG("data received!: ");
    M_LOG(bufferRev);

    // send data to client
    msgLength = strlen(testData);
    memcpy(bufferSend, testData, msgLength);
    if ( msgLength != sendto(server_fd, bufferSend, msgLength, 0, (const struct sockaddr*)(&server_addr), sizeof server_addr)) {
        M_HANDLE_ERROR("Error sendto()\n");
    }
#endif

}   


// main function
void main(int argc, char* argv[])
{
    M_LOG("Server running:\n");
    init_server();
    server();
    // while(1){
    //     sleep(1);
    //     server(); 
    // }
    deinit_server();
    exit(EXIT_SUCCESS);
}
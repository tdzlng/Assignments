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
#define D_PATH          "./Server_Socket"
#define D_BUFF_SIZE     (256U)
#define D_ERROR         (-1)
#define D_BACK_LOG      (3)

// macro function
#define M_HANDLE_ERROR(msg) \
    { perror(msg); exit(EXIT_FAILURE);}
#define M_LOG(msg) \
    { printf("Server Log: %s\n",msg);}


// static globlal variable
static int server_fd;
static int new_socket_fd;
static int opt;
static struct sockaddr_un server_addr = { 0 };
static struct sockaddr_un client_addr = { 0 };
static char bufferSend[D_BUFF_SIZE];
static char bufferRev[D_BUFF_SIZE];

// function definition
void init_server(){
    char path[] = D_PATH;

    if ((strlen(path)) > sizeof(server_addr.sun_path)) {
        M_HANDLE_ERROR("Path longer than 108 charactor\n");
    }

    // init socket
#ifdef D_UNIX_STREAM
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
#endif
#ifdef D_UNIX_DATAGRAM
    server_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
#endif
    if(D_ERROR == server_fd){
        M_HANDLE_ERROR("Error socket()\n");
    }

    // init server path
    remove(D_PATH);
    server_addr.sun_family = AF_UNIX;
    memcpy(server_addr.sun_path, path, strlen(path));

    // /* Ngăn lỗi : “address already in use” */
    // if(D_ERROR == setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof opt)) {
    //     M_HANDLE_ERROR("Error setsockopt()\n");
    // }

    // bind address with server
    if ( D_ERROR == bind(server_fd, (const struct sockaddr*)(&server_addr), sizeof server_addr) ){
        M_HANDLE_ERROR("Error bind()\n");
    }

#ifdef D_UNIX_STREAM
    // nghe toi da 3 ket noi trong hang doi
    if (D_ERROR == listen(server_fd, D_BACK_LOG) ) {
        M_HANDLE_ERROR("Error listen()\n");
    }
#endif
}

void deinit_server(){
    remove(D_PATH);
}

void server(){
    char testData[] = "Return to client ";
    size_t msgLength;
    socklen_t sockLength;
    memset(bufferSend, 0, D_BUFF_SIZE);
    memset(bufferRev, 0, D_BUFF_SIZE);

#ifdef D_UNIX_STREAM
    // block until client is connect
    new_socket_fd = accept(server_fd, (struct sockaddr*)(&server_addr), &sockLength);
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
    memcpy(bufferSend, testData, strlen(testData));
    msgLength = strlen(bufferRev);
    memcpy(bufferSend + strlen(testData), bufferRev, msgLength);
    if(D_ERROR == write(new_socket_fd, bufferSend, msgLength + strlen(testData))) {
        M_HANDLE_ERROR("Error write()\n");
    }
    close(new_socket_fd);
#endif

#ifdef D_UNIX_DATAGRAM
    // read from client and client's address
    if (D_ERROR == recvfrom(server_fd, bufferRev, D_BUFF_SIZE, 0, (struct sockaddr*)(&client_addr), &sockLength)) {
        M_HANDLE_ERROR("Error recvfrom()\n");
    }

    M_LOG("data received!: ");
    M_LOG(bufferRev);

    // send data to client
    msgLength = strlen(testData);
    memcpy(bufferSend, testData, msgLength);
    if ( msgLength != sendto(server_fd, bufferSend, msgLength, 0, (const struct sockaddr*)(&client_addr), sockLength)) {
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
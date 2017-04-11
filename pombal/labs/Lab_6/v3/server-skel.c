#include "storyserver.h"

#include <string.h>
#include <signal.h>

static volatile int keepRunning = 1;

void sigIntHandler(int);

void sigIntHandler(int sig)
{
    keepRunning = 0;
}

int main(void)
{
    int i = 0;
    message m;
    bool message_big = false;
    char* story = NULL;
    char* story_to_send = NULL;

    int socket_fd = 0;
    int ret_val_bind = 0;
    long int ret_val_sendto = 0;
    long int ret_val_recv_from = 0;
    struct sockaddr_un server_socket_address;   // address of the server socket
    struct sockaddr_un cli_socket_addr;
    unsigned int cli_addr_len = 0;

    struct sigaction sigint_action;

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

        /* socket: create a new communication endpoint */
        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if(socket_fd == -1){
            fprintf(stderr, "Error opening socket.\n");
            exit(EXIT_FAILURE);
        }

        /* connect socket to socket address */
        unlink(SOCKET_NAME);    // unlink file in case previously the server exited badly
        memset(&server_socket_address, 0, sizeof(server_socket_address));   // first we reset the struct
        server_socket_address.sun_family = AF_UNIX; // specify the sun_family parameter
        strncpy(server_socket_address.sun_path, SOCKET_NAME, sizeof(server_socket_address.sun_path) - 1);   // specify the sun_path

        /* bind: attach a local address to a socket */
        ret_val_bind = bind(socket_fd, (struct sockaddr *)&server_socket_address, sizeof(struct sockaddr_un));  // call bind
        if(ret_val_bind != 0){  // check for error
            fprintf(stderr, "Error binding socket\n");
            exit(EXIT_FAILURE);
        }

        // TODO: listen
        // TODO: send/receive data
        // TODO: close




    exit(EXIT_SUCCESS);
}

#include "gateway.h"
#include "storyserver.h"

#include <signal.h>

static volatile int keepRunning = 1;

void sigIntHandler(int);

void sigIntHandler(int sig)
{
    keepRunning = 0;
}

int main(void)
{
    int socket_dgram_fd = 0;
    struct sockaddr_in gateway_socket_address;  // address of the gateway socket
    struct sockaddr_in server_socket_address;
    int gateway_port = 0;
    char gateway_ip[16];
    char char_buffer[1024];
    int ret_val_bind = 0;
    int ret_val_recv = 0;

    // signal realated variables
    struct sigaction sigint_action;

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

        //socket call
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(socket_dgram_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        // get gateway address info
        fprintf(stdout, "Insert gateway IP address:\n");
        /*gateway_ip = */ fgets(char_buffer, sizeof(char_buffer), stdin);
        sscanf(char_buffer, "%s", gateway_ip);
        fprintf(stdout, "Insert gateway port:\n");
        /*char_buffer = */ fgets(char_buffer, sizeof(char_buffer), stdin);
        sscanf(char_buffer, "%d", &gateway_port);

        fprintf(stdout, "Got IP %s and port %d\n", gateway_ip, gateway_port);

        /* set gateway address struct */
        memset(&gateway_socket_address, 0, sizeof(gateway_socket_address));   // first we reset the struct
        gateway_socket_address.sin_family = AF_INET;
        // TODO: on dgram, bind can be inaddr_any
        inet_aton(gateway_ip, &gateway_socket_address.sin_addr);
        gateway_socket_address.sin_port = htons(gateway_port);

        // call bind
        ret_val_bind = bind(socket_dgram_fd, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));
        if(ret_val_bind == -1){  // check for error
            perror("bind");
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }

        memset(&server_socket_address, 0, sizeof(server_socket_address));
        // TODO: while keepRunning
        ret_val_recv = recv(socket_dgram_fd, &server_socket_address, sizeof(server_socket_address), 0);

        // DEBUG
        fprintf(stderr, "server ip: %s\nserver port: %d\n", inet_ntoa(server_socket_address.sin_addr), ntohs(server_socket_address.sin_port));

    return 0;
}

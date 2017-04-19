#include "storyserver.h"

#include <signal.h>

static volatile int keepRunning = 1;

void sigIntHandler(int);

void sigIntHandler(int sig)
{
    fprintf(stdout, "Caught signal %d\n", sig);
    keepRunning = 0;

    return;
}

int main(void)
{
    int socket_dgram_fd = 0;
    struct sockaddr_in gateway_socket_address;  // address of the gateway socket
    struct sockaddr_in server_socket_address;
    int gateway_port = 0;
    char* char_buffer = NULL;
    int ret_val_bind = 0;
    int ret_val_recv = 0;
    int ret_val_sscanf = 0;

    // signal realated variables
    struct sigaction sigint_action;

        char_buffer = malloc(CHAR_BUFFER_SIZE * sizeof(char));

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

        /* get gateway address info and set variables accordingly */
        fprintf(stdout, "Insert gateway port:\n");
        fgets(char_buffer, sizeof(char_buffer), stdin);
        ret_val_sscanf = sscanf(char_buffer, "%d", &gateway_port);
        if(ret_val_sscanf != 1){
            fprintf(stderr, "sscanf: error reading port number!\n");
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "Gateway port: %d\n", gateway_port);
        memset(&gateway_socket_address, 0, sizeof(gateway_socket_address));   // first we reset the struct
        gateway_socket_address.sin_family = AF_INET;
        gateway_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_socket_address.sin_port = htons(gateway_port);


        /* bind datagram socket, since we will be receiving */
        ret_val_bind = bind(socket_dgram_fd, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }

        // TODO: while keepRunning
        while(true == keepRunning){
            memset(&server_socket_address, 0, sizeof(server_socket_address));
            ret_val_recv = recv(socket_dgram_fd, &server_socket_address, sizeof(server_socket_address), NO_FLAGS);
            /*
            if(ret_val_recv == -1){
                fprintf(stderr, "Error receiving server address\n");
                exit(EXIT_FAILURE);
            }*/
            // DEBUG
            fprintf(stdout, "gateway ip: %s\ngateway port: %d\n-\nserver ip: %s\nserver port: %d\n", inet_ntoa(gateway_socket_address.sin_addr), ntohs(gateway_socket_address.sin_port), inet_ntoa(server_socket_address.sin_addr), ntohs(server_socket_address.sin_port));
        }

    // close and free stuff
    close(socket_dgram_fd);
    free(char_buffer);

    return 0;
}

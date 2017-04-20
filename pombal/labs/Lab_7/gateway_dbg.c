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
    struct sockaddr_in server_socket_address_temp; // TODO: remove when LL is implemented
    struct sockaddr_in client_socket_address;
    int gateway_port = 0;
    char* char_buffer = NULL;
    int ret_val_bind = 0;
    int ret_val_recv = 0;
    int ret_val_send_to = 0;
    int ret_val_sscanf = 0;

    Message_gw message_gw;

    // signal realated variables
    struct sigaction sigint_action;

        char_buffer = malloc(CHAR_BUFFER_SIZE * sizeof(char));

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

        //socket call
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
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
        memset((void*)&gateway_socket_address, 0, sizeof(gateway_socket_address));   // first we reset the struct
        gateway_socket_address.sin_family = AF_INET;
        gateway_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_socket_address.sin_port = htons(gateway_port);

        // we assume client and server family will always be AF_INET
        server_socket_address.sin_family = AF_INET;
        client_socket_address.sin_family = AF_INET;

        /* bind datagram socket, since we will be receiving */
        ret_val_bind = bind(socket_dgram_fd, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }

        // TODO: while keepRunning
        while(true == keepRunning){
            memset((void*)&server_socket_address, 0, sizeof(server_socket_address));
            memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
            ret_val_recv = recv(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS);
            fprintf(stderr, "recdd\n");
            /*
            if(ret_val_recv == -1){
                fprintf(stderr, "Error receiving server address\n");
                exit(EXIT_FAILURE);
            }*/
            if(message_gw.type == SERVER_ADDRESS){
                fprintf(stdout, "Received a message of type SERVER\n");
                server_socket_address.sin_addr.s_addr = htonl(message_gw.address);
                server_socket_address.sin_port = htons(message_gw.port);

                // TOOD: remove when LL is implemented
                server_socket_address_temp.sin_family = AF_INET;
                server_socket_address_temp.sin_addr.s_addr = server_socket_address.sin_addr.s_addr;
                server_socket_address_temp.sin_port = server_socket_address.sin_port;

                // DEBUG
                fprintf(stdout, "gateway ip: %s\ngateway port: %d\n-\nserver ip: %s\nserver socket stream port: %d\n", inet_ntoa(gateway_socket_address.sin_addr), ntohs(gateway_socket_address.sin_port), inet_ntoa(server_socket_address.sin_addr), ntohs(server_socket_address.sin_port));
                // TODO: add server to linked list
            }else if(message_gw.type == CLIENT_ADDRESS){
                fprintf(stdout, "Received a message of type CLIENT\n");
                client_socket_address.sin_addr.s_addr = message_gw.address;
                client_socket_address.sin_port = message_gw.port;

                message_gw.type = SERVER_ADDRESS;
                message_gw.address = server_socket_address_temp.sin_addr.s_addr;
                message_gw.port = ntohs(server_socket_address_temp.sin_port);

                ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
                if(ret_val_send_to == -1){
                    fprintf(stderr, "Failed to send server address to client\n");
                    exit(EXIT_FAILURE);
                }
                // DEBUG
                fprintf(stdout, "gateway ip: %s\ngateway port: %d\n-\nclient ip: %s\nclient port: %d\n", inet_ntoa(gateway_socket_address.sin_addr), ntohs(gateway_socket_address.sin_port), inet_ntoa(client_socket_address.sin_addr), ntohs(client_socket_address.sin_port));
            }
        }

    // close and free stuff
    close(socket_dgram_fd);
    free(char_buffer);

    return 0;
}

#include "../include/gateway.h"

void sigIntHandler(int sig)
{
    fprintf(stdout, "Caught signal %d\n", sig);
    keepRunning = 0;

    return;
}

void setupInterrupt(void)
{
    // signal realated variables
    struct sigaction sigint_action;

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

    return;
}

int getGatewayPort(void)
{
    char* char_buffer;
    int gateway_port = 0;
    int ret_val_sscanf = 0;

        char_buffer = (char*)malloc(CHAR_BUFFER_SIZE * sizeof(char));

        while((gateway_port < 1024) || (gateway_port > USHRT_MAX) || (ret_val_sscanf != 1)){
            fprintf(stdout, "Insert gateway port:\n");
            fgets(char_buffer, sizeof(char_buffer), stdin);
            ret_val_sscanf = sscanf(char_buffer, "%d\n", &gateway_port);

            if ((gateway_port < 1024) || (gateway_port > USHRT_MAX)){
                fprintf(stderr, "Invalid gateway port\n\n");
            }else if(ret_val_sscanf != 1){
                fprintf(stderr, "Error reading port\n\n");
            }
        }

        free(char_buffer);

    return gateway_port;
}

int peersDgramSocketSetup(void)
{
    int socket_dgram_peers_fd = 0;
    int ret_val_bind = 0;
    struct sockaddr_in gateway_peers_dgram_socket_address;

        //socket call peers
        socket_dgram_peers_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_peers_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        setupGatewayAddressPeers(&gateway_peers_dgram_socket_address);

        /* bind datagram socket, since we will be receiving PEERS*/
        ret_val_bind = bind(socket_dgram_peers_fd, (struct sockaddr *)&gateway_peers_dgram_socket_address, sizeof(gateway_peers_dgram_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }

    return socket_dgram_peers_fd;
}

int clientDgramSocketSetup(void)
{
    int socket_dgram_clients_fd = 0;
    int ret_val_bind = 0;
    struct sockaddr_in gateway_clients_dgram_socket_address;

        socket_dgram_clients_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_clients_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        // gateway dgram socket port for clients is the one after the peers'
        setupGatewayAddressClients(&gateway_clients_dgram_socket_address);

        /* bind datagram socket, since we will be receiving CLIENTS*/
        ret_val_bind = bind(socket_dgram_clients_fd, (struct sockaddr *)&gateway_clients_dgram_socket_address,  sizeof(gateway_clients_dgram_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }

    return socket_dgram_clients_fd;
}

void setupGatewayAddressPeers(struct sockaddr_in * gsa)
{
    int gateway_port = 0;

        memset((void*)gsa, 0, sizeof(*gsa));   // first we reset the struct
        gsa->sin_family = AF_INET;
        gsa->sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_port = BASE_PORT + getpid();
        if(gateway_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        gsa->sin_port = htons(gateway_port);
        fprintf(stdout, "Gateway port for peers is on port: %d\n", gateway_port);

    return;
}

void setupGatewayAddressClients(struct sockaddr_in * gsa)
{
    int gateway_port = 0;

        memset((void*)gsa, 0, sizeof(*gsa));   // first we reset the struct
        gsa->sin_family = AF_INET;
        gsa->sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_port = BASE_PORT + getpid() + 1;
        if(gateway_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        gsa->sin_port = htons(gateway_port);
        fprintf(stdout, "Gateway port for clients is on port: %d\n", gateway_port);

    return;
}

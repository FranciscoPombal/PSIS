#include "../include/peer.h"

void sigIntHandler(int sig)
{
    fprintf(stdout, "Caught signal %d\n", sig);
    keepRunning = false;

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

void getGatewayIPv4(char* gateway_ipv4)
{
    int i = 0;
    int v = 0;
    int ip[4] = {-1, -1, -1, -1};
    char* buffer = NULL;
    int ret_val_sscanf = 0;
    bool valid_ip = false;

        buffer = (char*)malloc(CHAR_BUFFER_SIZE * sizeof(char));

        while((ret_val_sscanf != 4) || (false == valid_ip)){
            printf("Insert Gateway IP address (0.0.0.0 or 127.0.0.1 if running locally):\n");
            fgets(buffer, 100, stdin);
            ret_val_sscanf = sscanf(buffer, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
            if(ret_val_sscanf != 4){
                fprintf(stderr, "Error getting gateway IP\n\n");
            }
            for (i = 0; i < 4; i++){
                if((ip[i] < 0) || (ip[i] > 255)){
                    v += 1;
                }
            }
            if(v == 0){
                valid_ip = true;
            }else{
                valid_ip = false;
            }
            if (false == valid_ip) {
                fprintf(stderr, "Invalid gateway ip\n\n");
            }
            v = 0;
        }

        sprintf(gateway_ipv4, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        free(buffer);

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

void setupPeerAddress(struct sockaddr_in * psa)
{
    int peer_port = 0;

        memset((void*)&psa, 0, sizeof(psa));   // first we reset the struct
        psa->sin_family = AF_INET;
        psa->sin_addr.s_addr = htonl(INADDR_ANY);
        peer_port = BASE_PORT + getpid();
        if(peer_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        psa->sin_port = htons(peer_port);
        fprintf(stdout, "Server stream socket is on port: %d\n", peer_port);

    return;
}

void setupGatewayAddress(struct sockaddr_in * gsa)
{
    char* gateway_ipv4;
    int gateway_port = 0;

        gateway_ipv4 = (char*)malloc(IPV4_STRING_SIZE * sizeof(char));
        getGatewayIPv4(gateway_ipv4);
        gateway_port = getGatewayPort();
        fprintf(stdout, "\nGateway IP address: %s\nGateway port: %d\n", gateway_ipv4, gateway_port);

        /* set gateway address struct */
        memset((void*)&gsa, 0, sizeof(gsa));   // first we reset the struct
        gsa->sin_family = AF_INET;
        gsa->sin_addr.s_addr = htonl(INADDR_ANY);
        gsa->sin_port = htons(gateway_port);

    return;
}

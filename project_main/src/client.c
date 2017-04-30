#include "../include/client.h"

int main(void)
{
    char* gateway_ipv4;
    int gateway_port;
    int choice = 0;
    int socket_stream_fd = 0;

        gateway_ipv4 = (char*)malloc(IPV4_STRING_SIZE * sizeof(char));

        //Get gateway information
        getGatewayIPv4(gateway_ipv4);
        gateway_port = getGatewayPort();
        fprintf(stdout, "\nGateway IP address: %s\nGateway port: %d\n", gateway_ipv4, gateway_port);

        socket_stream_fd = gallery_connect(gateway_ipv4, gateway_port);
        if(socket_stream_fd == 0){
            fprintf(stderr, "Unable to connect to gallery\n");
            exit(EXIT_FAILURE);
        }

        // TODO: rest of client

        free(gateway_ipv4);

    // TODO: remove early exit (here for testing purposes only)
    exit(EXIT_SUCCESS);

        //Print client options
        choice = showMenu();

        //Execute option
        switch (choice) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            default:
                break;
        }

    exit(EXIT_SUCCESS);
}

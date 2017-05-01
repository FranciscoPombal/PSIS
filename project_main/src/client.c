#include "../include/client.h"

static volatile bool keepRunning = true;

void sigIntHandler(int);

void sigIntHandler(int sig)
{
    fprintf(stdout, "I think I caught SIGINT: %d; exiting\n", sig);
    keepRunning = false;

    return;
}

int main(void)
{
    char* gateway_ipv4;
    int gateway_port;
    int choice = 0;
    int socket_stream_fd = 0;

    // signal realated variables
    struct sigaction sigint_action;

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

        gateway_ipv4 = (char*)malloc(IPV4_STRING_SIZE * sizeof(char));

        //Get gateway information
        getGatewayIPv4(gateway_ipv4);
        gateway_port = getGatewayPort();
        fprintf(stdout, "\nGateway IP address: %s\nGateway port: %d\n", gateway_ipv4, gateway_port);

        socket_stream_fd = gallery_connect(gateway_ipv4, gateway_port);
        if(socket_stream_fd == -1){
            fprintf(stderr, "Unable to properly communicate with gateway, exiting. Try again later.\n");
            exit(EXIT_FAILURE);
        }else if (socket_stream_fd == 0){
            fprintf(stderr, "Gateway says no peers are available. Try again later.\n");
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "Client has contated the gateway and is connected to a peer.\n");

// TODO: rest of client

        while(true == keepRunning){
            //Print client options
            choice = showMenu();

            //Execute option
            switch(choice){
                case 1:
                    fprintf(stdout, "You chose to add a photo to the gallery\n");
                    break;
                case 2:
                    fprintf(stdout, "You chose to add a keyword to a photo in the gallery\n");
                    break;
                case 3:
                    fprintf(stdout, "You chose to search for a photo in the gallery\n");
                    break;
                case 4:
                    fprintf(stdout, "You chose to delete a photo from the gallery\n");
                    break;
                case 5:
                    fprintf(stdout, "You chose to get the name of a photo in the gallery\n");
                    break;
                case 6:
                    fprintf(stdout, "You chose to  get (download) a photo from the gallery\n");
                    break;
                default:
                    fprintf(stdout, "Invalid option number. Try again\n");
                    break;
            }
        }

        free(gateway_ipv4);

    exit(EXIT_SUCCESS);
}

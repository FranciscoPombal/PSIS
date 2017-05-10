#include "../include/client.h"

int main(void)
{
    int choice = 0;
    int socket_stream_fd = 0;
    char* gateway_ipv4 = NULL;
    int gateway_port = 0;

        //setup SIGINT handler
        setupInterrupt();

        //Get gateway information
        gateway_ipv4 = (char*)malloc(IPV4_STRING_SIZE * sizeof(char));
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
                case 1:{ //ADD PHOTO TO GALLERY
                    char photo_name[100];
                    char buffer[100];
                    fprintf(stdout, "You chose to add a photo to the gallery\n");
                    fprintf(stdout, "Please insert path to photo you want to add:\n");
                    fgets(buffer, 100, stdin);
                    sscanf(buffer, "%s\n", photo_name);

                    gallery_add_photo(socket_stream_fd,photo_name);

                    free(photo_name);
                    free(buffer);
                    break;}
                case 2:
                    fprintf(stdout, "You chose to add a keyword to a photo in the gallery\n");
                    break;
                case 3:{
                    char keyword[100];
                    char buffer[100];
                    fprintf(stdout, "You chose to search for a photo in the gallery\n");
                    fprintf(stdout, "Insert a keyword:\n");
                    fgets(buffer, 100, stdin);
                    sscanf(buffer, "%s\n", keyword);

                    gallery_search_photo(socket_stream_fd, keyword, NULL);

                    free(keyword);
                    free(buffer);
                    break;}
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

#include "../include/client.h"

bool keepRunning = true;

int main(void)
{
    int choice = 0;
    bool closeConnection = false;
    int socket_stream_fd = 0;
    char* gateway_ipv4 = NULL;
    int gateway_port = 0;

    int photo_id = 0;
    char photo_name[CHAR_BUFFER_SIZE];
    char buffer[CHAR_BUFFER_SIZE];
    char keyword[CHAR_BUFFER_SIZE];

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

        while((true == keepRunning) && (false == closeConnection)){
            //Print client options
            choice = showMenu();

            //Execute option
            switch(choice){
                case ADD_PHOTO:
                {
                    fprintf(stdout, "Please insert name of photo you want to add:\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%s\n", photo_name);

                    gallery_add_photo(socket_stream_fd,photo_name);

                    break;
                }
                case ADD_KEYWORD:
                {
                    fprintf(stdout, "Not implemented (yet?), choose another option\n");
                    break;
                }
                case SEARCH_PHOTO:
                {
                    fprintf(stdout, "Insert a keyword to search for (blank to list all):\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%s\n", keyword);

                    gallery_search_photo(socket_stream_fd, keyword, NULL);

                    break;
                }
                case DELETE_PHOTO:
                {
                    fprintf(stdout, "Insert the id of the photo to delete\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%d", &photo_id);

                    gallery_delete_photo(socket_stream_fd, photo_id);

                    break;
                }
                case GET_PHOTO_NAME:
                {
                    fprintf(stdout, "Insert the id of the photo to get the name of (0 to get all):\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%d", &photo_id);

                    gallery_get_photo_name(socket_stream_fd, photo_id, NULL);

                    break;
                }
                case GET_PHOTO:
                {
                    fprintf(stdout, "Insert the id of the photo to download:\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%d", &photo_id);

                    gallery_get_photo(socket_stream_fd, photo_id, NULL);

                    break;
                }

                case CLOSE_CONNECTION:
                {
                    fprintf(stdout, "Closed connection with the peer. Exiting.\n");
                    closeConnection = gallery_close_connection(socket_stream_fd);

                    break;
                }
                default:
                {
                    fprintf(stdout, "Invalid option number. Try again\n");
                    break;
                }
            }
        }

        fprintf(stdout, "Cleaning up...\n");

        free(gateway_ipv4);

    exit(EXIT_SUCCESS);
}

#include "../include/client.h"

bool keepRunning = true;

int main(void)
{
    uint32_t i = 0;
    int choice = 0;
    bool closeConnection = false;
    int socket_stream_fd = 0;
    char* gateway_ipv4 = NULL;
    int gateway_port = 0;

    uint32_t photo_id = 0;
    char buffer[CHAR_BUFFER_SIZE];
    char keyword[CHAR_BUFFER_SIZE];
    char** photo_names = NULL;
    char* photo_name = NULL;
    uint32_t num_photos = 0;
    int delete_response = 0;
    int get_response = 0;

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

                    photo_id = gallery_add_photo(socket_stream_fd,photo_name);
                    if(photo_id != 0){
                        fprintf(stdout, "Photo added to peer with id: %u\n", photo_id);
                    }else{
                        fprintf(stderr, "Error adding photo \"%s\" to peer\n", photo_name);
                    }

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
                    sscanf(buffer, "%u", &photo_id);

                    delete_response = gallery_delete_photo(socket_stream_fd, photo_id);
                    if(delete_response == -1){
                        fprintf(stdout, "Error deleting photo from peer\n");
                    }else if(delete_response == PHOTO_NOT_FOUND){
                        fprintf(stderr, "Can't delete because photo does not exist\n");
                    }else{
                        fprintf(stdout, "Photo with id %u was deleted successfully\n", photo_id);
                    }

                    break;
                }
                case GET_PHOTO_NAME:
                {
                    fprintf(stdout, "Insert the id of the photo to get the name of (0 to get all):\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%u", &photo_id);

                    num_photos = gallery_get_photo_name(socket_stream_fd, photo_id, photo_names);
                    if((int)num_photos == -1){
                        fprintf(stdout, "Error getting photo from peer\n");
                    }else if(num_photos == 0){
                        fprintf(stderr, "Can't get name because photo does not exist\n");
                    }else if(num_photos == 1){
                        fprintf(stdout, "Photo with id %u exists and its name is:%s\n", photo_id, photo_names[0]);
                        free(photo_names[0]);
                        free(photo_names);
                    }else if(num_photos >= 1){
                        fprintf(stdout, "These are the names of all the photos in the peer:\n");
                        for(i = 0; i < num_photos; i++) {
                            fprintf(stdout, "%s\n", photo_names[i]);
                            free(photo_names[i]);
                        }
                        free(photo_names);
                    }

                    break;
                }
                case GET_PHOTO:
                {
                    fprintf(stdout, "Insert the id of the photo to download:\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%u", &photo_id);

                    get_response = gallery_get_photo(socket_stream_fd, photo_id, photo_name);
                    if(get_response == -1){
                        fprintf(stderr, "Error getting photo\n");
                    }else if(get_response == PHOTO_NOT_FOUND){
                        fprintf(stderr, "Photo with id %u not found in peer.\n", photo_id);
                    }else{
                        fprintf(stdout, "File with id %u has been downloaded as file %s\n", photo_id, photo_name);
                        free(photo_name);
                    }

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
        if(photo_name != NULL){
            free(photo_name);
        }

    exit(EXIT_SUCCESS);
}

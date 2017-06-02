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
    int ret_val_sscanf = 0;

    uint32_t photo_id = 0;
    char buffer[CHAR_BUFFER_SIZE];
    char* keyword;
    char* photo_name = NULL;
    char** aux_photo_names = NULL;
    long int num_photos = 0;
    int delete_response = 0;
    int get_response = 0;
    int add_keyword_response = 0;
    uint32_t* photo_ids = NULL;

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

        fprintf(stdout, "Client has contacted the gateway and is connected to a peer.\n");

        while((true == keepRunning) && (false == closeConnection)){
            //Print client options
            choice = showMenu();

            //Execute option
            switch(choice){
                case ADD_PHOTO:
                {
                    fprintf(stdout, "Please insert name of photo you want to add:\n");
                    photo_name = malloc(CHAR_BUFFER_SIZE * sizeof(char));
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%s\n", photo_name);

                    photo_id = gallery_add_photo(socket_stream_fd, photo_name);
                    if(photo_id != 0){
                        fprintf(stdout, "Photo added to peer with id: %u\n", photo_id);
                    }else{
                        fprintf(stderr, "Error adding photo \"%s\" to peer\n", photo_name);
                    }

                    if(photo_name != NULL){
                        free(photo_name);
                        photo_name = NULL;
                    }

                    break;
                }
                case ADD_KEYWORD:
                {
                    while (ret_val_sscanf != 1){
                        fprintf(stdout, "Insert the id of the photo to add a keyword to:\n");
                        fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                        sscanf(buffer, "%u\n", &photo_id);
                    }

                    add_keyword_response = gallery_add_keyword(socket_stream_fd, photo_id, NULL);
                    if(add_keyword_response == -1){
                        fprintf(stderr, "There was an error adding that keyword to photo with id %u.", photo_id);
                    }else if(add_keyword_response == PHOTO_NOT_FOUND){
                        fprintf(stderr, "Peer says photo with id %u does not exist.", photo_id);
                    }else{
                        fprintf(stdout, "keyword added successfully to the photo\n");
                    }

                    break;
                }
                case SEARCH_PHOTO:
                {
                    num_photos = 0;
                    keyword = (char*)malloc(CHAR_BUFFER_SIZE * sizeof(char));
                    fprintf(stdout, "Insert a keyword to search for (blank to list all):\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%s\n", keyword);

                    num_photos = gallery_search_photo(socket_stream_fd, keyword, &photo_ids);
                    if(num_photos == 0){
                        fprintf(stdout, "Peer says no photos were found\n");
                        free(keyword);
                        keyword = NULL;
                        if(photo_ids != NULL){
                            free(photo_ids);
                            photo_ids = NULL;
                        }
                        break;
                    }else if(num_photos < 0){
                        fprintf(stdout, "Peer there was an error trying to search for photos\n");
                        free(keyword);
                        keyword = NULL;
                        if(photo_ids != NULL){
                            free(photo_ids);
                            photo_ids = NULL;
                        }
                        break;
                    }

                    fprintf(stdout, "Found the keyword \"%s\" in %ld photos with ids:\n", keyword, num_photos);
                    for(i = 0; i < num_photos; i++){
                        fprintf(stdout, "%u\n", photo_ids[i]);
                    }

                    free(keyword);
                    keyword = NULL;
                    free(photo_ids);
                    photo_ids = NULL;

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

                    num_photos = gallery_get_photo_name(socket_stream_fd, photo_id, &photo_name);
                    if(num_photos == -1){
                        fprintf(stdout, "Error getting photo from peer\n");
                    }else if(num_photos == 0){
                        fprintf(stderr, "Can't get name because photo does not exist\n");
                    }else if(num_photos == 1){
                        fprintf(stdout, "Photo with id %u exists and its name is: %s\n", photo_id, photo_name);
                        free(photo_name);
                        photo_name = NULL;
                    }else if(num_photos >= 1){ //this is wrong, TODO
                        fprintf(stdout, "These are the names of all the photos in the peer:\n");
                        aux_photo_names = (char**)photo_name;
                        for(i = 0; i < num_photos; i++){
                            fprintf(stdout, "%s\n", aux_photo_names[i]);
                            if(aux_photo_names[i] != NULL){
                                free(aux_photo_names[i]);
                                aux_photo_names[i] = NULL;
                            }
                        }
                        free(aux_photo_names);
                        aux_photo_names = NULL;
                    }

                    break;
                }
                case GET_PHOTO:
                {
                    fprintf(stdout, "Insert the id of the photo to download (0 to get all):\n");
                    fgets(buffer, CHAR_BUFFER_SIZE, stdin);
                    sscanf(buffer, "%u", &photo_id);

                    get_response = gallery_get_photo(socket_stream_fd, photo_id, &photo_name);
                    if(get_response == -1){
                        fprintf(stderr, "Error getting photo\n");
                    }else if(get_response == PHOTO_NOT_FOUND){
                        fprintf(stderr, "Photo with id %u not found in peer.\n", photo_id);
                    }else if(get_response == 1){
                        fprintf(stdout, "File with id %u has been downloaded as file %s\n", photo_id, photo_name);
                        free(photo_name);
                        photo_name = NULL;
                    }else if(get_response >= 1){
                        fprintf(stdout, "All photos in the peer were downloaded (TODO)\n");
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
        gateway_ipv4 = NULL;
        if(photo_name != NULL){
            free(photo_name);
        }
        if(keyword != NULL){
            free(keyword);
        }

    exit(EXIT_SUCCESS);
}

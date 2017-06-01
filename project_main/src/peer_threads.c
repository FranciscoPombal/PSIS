#include "../include/peer.h"

pthread_mutex_t photo_list_mutex = PTHREAD_MUTEX_INITIALIZER;

void* pingerThread(void* args)
{
    int socket_dgram_fd = 0;
    int ret_val_send_to = 0;
    int ret_val_recvfrom = 0;

    Message_ping message_ping;
    struct sockaddr_in gateway_socket_address;
    socklen_t gateway_socket_address_len = sizeof(struct sockaddr_in);

        socket_dgram_fd = *((int*)(args));

        while(true){
            ret_val_recvfrom = recvfrom(socket_dgram_fd, &message_ping, sizeof(Message_ping), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, &gateway_socket_address_len);

            // we just send back
            if(message_ping.type == MESSAGE_TYPE_PEER_PING){
                fprintf(stdout, "Received a ping\n"); // DEBUG
                ret_val_send_to = sendto(socket_dgram_fd, &message_ping, sizeof(Message_ping), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, gateway_socket_address_len);
                fprintf(stdout, "Pinged back the gateway\n"); // DEBUG
            }else{
                fprintf(stdout, "Pinger thread: bad message type received\n");
                exit(EXIT_FAILURE);
            }
        }

    pthread_exit(NULL);
}

void* syncRecvThread(void* args)
{

        while(true){
            // TODO receive dgram stuff from gw and update list
            pthread_mutex_lock(&photo_list_mutex);
            pthread_mutex_unlock(&photo_list_mutex);
        }

    pthread_exit(NULL);
}


void* clientHandlerThread(void* args)
{
    bool closeConnection = false;
    int ret_val_recv_from = 0;
    int ret_val_recv = 0;
    int ret_val_send = 0;
    struct sockaddr_in client_socket_address;
    socklen_t client_socket_address_len = sizeof(struct sockaddr_in);
    Message_api_op_type message_api_op_type;
    ClientHandlerThreadArgs* clientHandlerThreadArgs = NULL;
    int socket_fd = 0;

    SinglyLinkedList* photo_list_head = NULL;
    SinglyLinkedList* aux_photo_list_node = NULL;
    PhotoProperties* photoProperties = NULL;
    long int file_size = 0;
    void* file_buffer = NULL;
    uint32_t id = 0;
    int delete_response = 0;
    int num_photos = 0;
    char* photo_name = NULL;
    char* photo_storage_name = NULL;
    int name_str_len = 0;
    int ret_val_retrievePhoto = 0;
    int i = 0;
    int ret_val_get_photo_name = 0;
    char** photo_names = NULL;
    int keyword_str_len = 0;
    char* keyword = NULL;
    bool photo_exists = false;
    uint32_t* photo_ids = NULL;

        clientHandlerThreadArgs = (ClientHandlerThreadArgs*)args;
        socket_fd = clientHandlerThreadArgs->socket_fd;
        photo_list_head = clientHandlerThreadArgs->photo_list_head;

        while(false == closeConnection){
            ret_val_recv_from = recvfrom(socket_fd, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS, (struct sockaddr *)&client_socket_address, &client_socket_address_len);

            switch(message_api_op_type.type){
                case GALLERY_API_ADD_PHOTO:
                {
                    fprintf(stdout, "Client wants to add a photo\n"); // DEBUG
                    // size of image
                    ret_val_recv = recv(socket_fd, &file_size, sizeof(long int), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Add photo: Error receiving size of image\n");
                        break;
                    }
                    // image
                    file_buffer = malloc(file_size);
                    ret_val_recv = recv(socket_fd, file_buffer, file_size, NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Add photo: Error receiving image.\n");
                        free(file_buffer);
                        file_buffer = NULL;
                        break;
                    }else if(ret_val_recv != file_size){
                        fprintf(stderr, "Add photo: Wrong number of image bytes received: %d\n", ret_val_recv);
                        free(file_buffer);
                        file_buffer = NULL;
                        break;
                    }

                    // image metadata
                    photoProperties = malloc(sizeof(PhotoProperties));
                    ret_val_recv = recv(socket_fd, photoProperties, sizeof(PhotoProperties), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Add photo: Error receiving image metadata\n");
                        free(photoProperties);
                        photoProperties = NULL;
                        break;
                    }

                    writePhotoToDisk(file_buffer, file_size, photoProperties->storage_name);
                    pthread_mutex_lock(&photo_list_mutex);
                    addPhotoToList(photo_list_head, photoProperties);
                    pthread_mutex_unlock(&photo_list_mutex);
                    free(file_buffer);
                    file_buffer = NULL;
                    break;
                }
                case GALLERY_API_SEARCH_PHOTO:
                {
                    fprintf(stdout, "Client wants to search for a photo\n"); // DEBUG

                    ret_val_recv = recv(socket_fd, &keyword_str_len, sizeof(keyword_str_len), NO_FLAGS);
                    if (ret_val_recv == -1){
                        fprintf(stderr, "Error receiving search keyword length\n");
                        break;
                    }

                    keyword = (char*)malloc((keyword_str_len + 1) * sizeof(char));

                    ret_val_recv = recv(socket_fd, keyword, keyword_str_len + 1, NO_FLAGS);
                    if (ret_val_recv == -1){
                        fprintf(stderr, "Error receiving search keyword\n");
                        break;
                    }

                    // search
                    // we need the number of matches and array of ids of the matching photos
                    pthread_mutex_lock(&photo_list_mutex);
                    findPhotoByKeyword(photo_list_head, keyword, &photo_ids, &num_photos);
                    pthread_mutex_unlock(&photo_list_mutex);

                    // send number of matches
                    ret_val_send = send(socket_fd, &num_photos, sizeof(int), NO_FLAGS);
                    if (ret_val_send == -1){
                        fprintf(stderr, "Error sending number of matches\n");
                        free(keyword);
                        keyword = NULL;
                        break;
                    }

                    // send matches
                    if(num_photos > 0){
                        ret_val_send = send(socket_fd, photo_ids, num_photos * sizeof(uint32_t), NO_FLAGS);
                        if(ret_val_send == -1){
                            fprintf(stderr, "Error sending number of matches\n");
                            free(keyword);
                            keyword = NULL;
                            free(photo_ids);
                            photo_ids = NULL;
                            break;
                        }
                    }

                    free(keyword);
                    keyword = NULL;
                    free(photo_ids);
                    photo_ids = NULL;

                    break;
                }
                case GALLERY_API_DELETE_PHOTO:
                {
                    fprintf(stdout, "Client wants to delete a photo\n"); // DEBUG
                    ret_val_recv = recv(socket_fd, &id, sizeof(uint32_t), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Delete photo: Error receiving id of image\n");
                        break;
                    }

                    pthread_mutex_lock(&photo_list_mutex);
                    delete_response = deletePhotoFromList(id, photo_list_head);
                    pthread_mutex_unlock(&photo_list_mutex);

                    ret_val_send = send(socket_fd, &delete_response, sizeof(int), NO_FLAGS);
                    if(ret_val_send == -1){
                        fprintf(stderr, "Delete photo: error sending response to client\n");
                        break;
                    }

                    break;
                }
                case GALLERY_API_GET_PHOTO_NAME:
                {
                    fprintf(stdout, "Client wants to get the name of a photo\n"); // DEBUG
                    ret_val_recv = recv(socket_fd, &id, sizeof(uint32_t), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Get photo name: error receiving id\n");
                        break;
                    }
                    // if id is zero, we will retrieve and send all the names
                    if(id == 0){
                        //find number of photos
                        pthread_mutex_lock(&photo_list_mutex);
                        num_photos = SinglyLinkedList_getNumberOfNodesWithItem(photo_list_head);
                        pthread_mutex_unlock(&photo_list_mutex);

                        //send number of photos
                        ret_val_send = send(socket_fd, &num_photos, sizeof(int), NO_FLAGS);
                        if(ret_val_send == -1){
                            fprintf(stderr, "Get photo name: error sending number of photos\n");
                            break;
                        }

                        photo_names = (char**)malloc(num_photos * sizeof(char*));

                        // for all photos, send the length of the name and the name string
                        aux_photo_list_node = photo_list_head;
                        for(i = 0; i < num_photos; i++){
                            pthread_mutex_lock(&photo_list_mutex);
                            ret_val_get_photo_name = getPhotoName(aux_photo_list_node, &name_str_len, &photo_names[i]);
                            pthread_mutex_unlock(&photo_list_mutex);

                            ret_val_send = send(socket_fd, &name_str_len, sizeof(name_str_len), NO_FLAGS);
                            if(ret_val_send == -1){
                                //TODO what to do in case of error?
                                fprintf(stderr, "Error sending photo names\n");
                                break;
                            }

                            if(ret_val_get_photo_name != -1){
                                ret_val_send = send(socket_fd, photo_names[i], name_str_len + 1, NO_FLAGS);
                                if(ret_val_send == -1){
                                    //TODO what to do in case of error?
                                    fprintf(stderr, "Error sending photo names\n");
                                    break;
                                }
                                pthread_mutex_lock(&photo_list_mutex);
                                aux_photo_list_node = SinglyLinkedList_getNextNode(aux_photo_list_node);
                                pthread_mutex_unlock(&photo_list_mutex);
                                free(photo_names[i]);
                                photo_names[i] = NULL;
                            }
                        }
                        free(photo_names);
                        photo_names = NULL;
                    }else{
                        name_str_len = 0;
                        pthread_mutex_lock(&photo_list_mutex);
                        findPhotoName(photo_list_head, id, &name_str_len, &photo_name, &photo_storage_name);
                        pthread_mutex_unlock(&photo_list_mutex);
                        if(name_str_len == 0){
                            ret_val_send = send(socket_fd, &name_str_len, sizeof(name_str_len), NO_FLAGS);
                            if(ret_val_send == -1){
                                fprintf(stderr, "Get photo name: error sending name string length\n");
                                break;
                            }
                        }else{
                            ret_val_send = send(socket_fd, &name_str_len, sizeof(name_str_len), NO_FLAGS);
                            if(ret_val_send == -1){
                                fprintf(stderr, "Get photo name: error sending name string length\n");
                                break;
                            }
                            ret_val_send = send(socket_fd, photo_name, name_str_len + 1, NO_FLAGS);
                            if(ret_val_send == -1){
                                fprintf(stderr, "Get photo name: error sending name\n");
                                break;
                            }
                            free(photo_name);
                            photo_name = NULL;
                        }
                    }
                    break;
                }
                case GALLERY_API_GET_PHOTO:
                {
                    fprintf(stdout, "Client wants to get a photo\n"); // DEBUG
                    // get the id of the photo to get
                    ret_val_recv = recv(socket_fd, &id, sizeof(uint32_t), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Get photo name: error receiving id\n");
                        break;
                    }
                    if(id != 0){
                        // get and send: name strlen, name, photo size and photo
                        name_str_len = 0;
                        pthread_mutex_lock(&photo_list_mutex);
                        findPhotoName(photo_list_head, id, &name_str_len, &photo_name, &photo_storage_name);
                        pthread_mutex_unlock(&photo_list_mutex);
                        if(name_str_len == 0){
                            ret_val_send = send(socket_fd, &name_str_len, sizeof(name_str_len), NO_FLAGS);
                            if(ret_val_send == -1){
                                fprintf(stderr, "Get photo: error sending name string length\n");
                                break;
                            }
                        }else{
                            ret_val_send = send(socket_fd, &name_str_len, sizeof(name_str_len), NO_FLAGS);
                            if(ret_val_send == -1){
                                fprintf(stderr, "Get photo: error sending name string length\n");
                                break;
                            }

                            ret_val_send = send(socket_fd, photo_name, name_str_len + 1, NO_FLAGS);
                            if(ret_val_send == -1){
                                fprintf(stderr, "Get photo: error sending name\n");
                                perror("socket send");
                                break;
                            }

                            ret_val_retrievePhoto = retrievePhoto(photo_storage_name, &file_size, &file_buffer);
                            if(ret_val_retrievePhoto == -1){
                                fprintf(stderr, "Get photo: error retrieving photo from disk.\n");
                                free(photo_name);
                                photo_name = NULL;
                                break;
                            }

                            ret_val_send = send(socket_fd, &file_size, sizeof(long int), NO_FLAGS);
                            if(ret_val_send == -1){
                                fprintf(stderr, "Get photo: error sending file size.\n");
                                break;
                            }

                            ret_val_send = send(socket_fd, file_buffer, file_size, NO_FLAGS);
                            if (ret_val_send == -1){
                                fprintf(stderr, "Get photo: error sending photo.\n");
                                break;
                            }

                            free(photo_name);
                            photo_name = NULL;
                            free(file_buffer);
                            file_buffer = NULL;
                        }
                    }else{ /// get all photos
                        //TODO
                    }

                    break;
                }
                case GALLERY_API_ADD_KEYWORD:
                {
                    fprintf(stdout, "Client wants to add a keyword to a photo\n");
                    // get the id of the photo to add keyword to
                    ret_val_recv = recv(socket_fd, &id, sizeof(uint32_t), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Add keyword: error receiving id\n");
                        break;
                    }

                    // check if photo exsists and send the information
                    pthread_mutex_lock(&photo_list_mutex);
                    aux_photo_list_node = findPhotoById(photo_list_head, id);
                    pthread_mutex_unlock(&photo_list_mutex);
                    if(aux_photo_list_node == NULL){
                        photo_exists = false;
                        ret_val_send = send(socket_fd, &photo_exists, sizeof(bool), NO_FLAGS);
                        if(ret_val_send == -1){
                            fprintf(stderr, "Error sending info that photo exists.\n");
                            break;
                        }
                        break;
                    }else{
                        photo_exists = true;
                        ret_val_send = send(socket_fd, &photo_exists, sizeof(photo_exists), NO_FLAGS);
                        if(ret_val_send == -1){
                            fprintf(stderr, "Error sending info that photo exists.\n");
                            break;
                        }
                    }

                    //receive string length
                    ret_val_recv = recv(socket_fd, &keyword_str_len, sizeof(keyword_str_len), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Error receiving keyword str len.\n");
                        break;
                    }

                    // alloc string
                    keyword = (char*)malloc((keyword_str_len + 1) * sizeof(char));

                    //receive string
                    ret_val_recv = recv(socket_fd, keyword, keyword_str_len + 1, NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Error receiving keyword.\n");
                        free(keyword);
                        keyword = NULL;
                        break;
                    }

                    pthread_mutex_lock(&photo_list_mutex);
                    addKeywordtoPhoto(aux_photo_list_node, keyword_str_len, keyword);
                    pthread_mutex_unlock(&photo_list_mutex);
                    free(keyword);
                    keyword = NULL;

                    break;
                }
                case GALLERY_API_CLOSE_CONNECTION:
                {
                    fprintf(stdout, "Client wants to close the connection\n"); // DEBUG
                    closeConnection = true;
                    break;
                }
                default:
                {
                    fprintf(stderr, "Error in client<->peer communication. The request operation is unknown. Received %d\n", message_api_op_type.type);
                    closeConnection = true;
                    break;
                }
            }
        }

        // close connection with current client
        close(socket_fd);
        free(clientHandlerThreadArgs);
        clientHandlerThreadArgs = NULL;
        fprintf(stdout, "Peer has closed the connection with the client and this thread is exiting\n");

    pthread_exit(NULL);
}

#include "../include/messages.h"
#include "../include/gallery_api.h"

int gallery_connect(char* host, in_port_t port)
{
    struct sockaddr_in client_socket_address;
    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    struct sockaddr_in peer_socket_address;
    struct sockaddr_in gateway_socket_address;
    int ret_val_bind = 0;
    int ret_val_send_to = 0;
    int ret_val_recvfrom = 0;
    int ret_val_connect = 0;
    Message_gw message_gw;
    socklen_t gateway_socket_address_len = 0;
    struct timeval client_send_to_gateway_timeout;
    struct timeval client_recv_from_gateway_timeout;

        // timeout for dgram socket
        client_send_to_gateway_timeout.tv_sec = CLIENT_SEND_TO_GATEWAY_TIMEOUT;
        client_send_to_gateway_timeout.tv_usec = 0;

        client_recv_from_gateway_timeout.tv_sec = CLIENT_RECV_FROM_GATEWAY_TIMEOUT;
        client_recv_from_gateway_timeout.tv_usec = 0;

        socket_stream_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
        if(socket_stream_fd == -1){
            fprintf(stderr, "Error creating stream socket\n");
            exit(EXIT_FAILURE);
        }

        // dgram socket
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_fd == -1){
            fprintf(stderr, "Error creating dgram socket\n");
            exit(EXIT_FAILURE);
        }

        // set timeouts for dgram socket
        setsockopt(socket_dgram_fd, SOL_SOCKET, SO_SNDTIMEO, (void *)&client_send_to_gateway_timeout, sizeof(client_send_to_gateway_timeout));
        setsockopt(socket_dgram_fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&client_recv_from_gateway_timeout, sizeof(client_recv_from_gateway_timeout));

        setupClientAddress(&client_socket_address);
        setupGatewayAddress(&gateway_socket_address, host, port);

        /* bind datagram socket, since we will be receiving */
        ret_val_bind = bind(socket_dgram_fd, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }

        message_gw.type = CLIENT_ADDRESS;
        message_gw.port = client_socket_address.sin_port;
        message_gw.address = client_socket_address.sin_addr.s_addr;

        ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));
        if(ret_val_send_to == -1){
            return -1;
        }

        gateway_socket_address_len = sizeof(gateway_socket_address);
        ret_val_recvfrom = recvfrom(socket_dgram_fd, &message_gw, sizeof(message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, &gateway_socket_address_len);
        if(ret_val_recvfrom == -1){
            return -1;
        }

        if(message_gw.type == PEER_ADDRESS){
            setupPeerAddress(&peer_socket_address, message_gw.address, message_gw.port);
        }else if(message_gw.type == PEER_UNAVAILABLE){
            return 0;
        }else{
            fprintf(stderr, "Wrong type of message received from gateway\n");
            exit(EXIT_FAILURE);
        }

        /* Connect socket to socket address (server) */
        /* connect */
        ret_val_connect = connect(socket_stream_fd, (struct sockaddr *)&peer_socket_address, sizeof(peer_socket_address));
        if(ret_val_connect != 0){  // check for error
            fprintf(stderr, "Error connecting socket.\n");
            perror("connect");
            exit(EXIT_FAILURE);
        }

        close(socket_dgram_fd);

    return socket_stream_fd;
}

// TODO: find better method to come up with random ports, maybe with random numbers
void setupClientAddress(struct sockaddr_in * csa)
{
    int client_port = 0;

        memset((void*)csa, 0, sizeof(*csa));  // reset struct
        csa->sin_family = AF_INET;
        csa->sin_addr.s_addr = htonl(INADDR_ANY);
        client_port = BASE_PORT + getpid();
        if(client_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        csa->sin_port = htons(client_port);

    return;
}
void setupGatewayAddress(struct sockaddr_in * gsa, char* gateway_IP, int gateway_port)
{
    memset((void*)gsa, 0, sizeof(*gsa));   // first we reset the struct
    gsa->sin_family = AF_INET;
    inet_aton(gateway_IP, &(gsa->sin_addr));
    gsa->sin_port = htons(gateway_port);

    return;
}
void setupPeerAddress(struct sockaddr_in * psa, unsigned int address, int port)
{
    memset((void*)psa, 0, sizeof(*psa));  // reset struct
    psa->sin_family = AF_INET;
    psa->sin_addr.s_addr = htonl(address);
    psa->sin_port = htons(port);

    return;
}

uint32_t gallery_add_photo(int peer_socket, char* file_name)
{
    // communication  variables
    Message_api_op_type message_api_op_type = {.type = GALLERY_API_ADD_PHOTO};
    int ret_val_send = 0;

    // for calculation of the id
    char* command = NULL;
    char* id_buffer = NULL;
    FILE* fp_id = NULL;

    // file to send
    uint32_t id = 0;
    FILE* fp = NULL;
    long int file_size = 0;
    void* file_buffer = NULL;
    PhotoProperties* photoProperties = NULL;
    char id_str[11];

        if(file_name == NULL){
            fprintf(stderr, "ERROR: file name is null\n");
            return 0;
        }

        command = malloc(CHAR_BUFFER_SIZE * sizeof(char));
        id_buffer = malloc(CHAR_BUFFER_SIZE * sizeof(char));

        //GET ID
        strncpy(command, "crc32 ", COMMAND_STRING_LENGTH);
        strncat(command, file_name, CHAR_BUFFER_SIZE - COMMAND_STRING_LENGTH);
        fp_id = popen(command, "r");
        fgets(id_buffer, CHAR_BUFFER_SIZE, fp_id);
        id = (uint32_t)strtol(id_buffer, NULL, 16);

        free(command);
        free(id_buffer);
        fclose(fp_id);

        // file is assumed to be in the same directory as the program
        fp = fopen(file_name, "rb");
        if(fp == NULL){
            fprintf(stderr, "No such file\n");
            return 0;
        }

        // find file size
        // could use fstat instead, but we would need to create 2 variables and call fileno()
        fseek(fp, 0L, SEEK_END);
        file_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        // 20 megabytes - same limit as imgur
        if(file_size > IMAGE_SIZE_LIMIT){
            fprintf(stderr, "File is too big.\n");
            fclose(fp);
            return 0;
        }

        // read from file to buffer in memory
        file_buffer = malloc(file_size);
        fread(file_buffer, 1, file_size, fp);

        photoProperties = malloc(sizeof(PhotoProperties));
        photoProperties->photo_id = id;
        strncpy(photoProperties->photo_name, file_name, CHAR_BUFFER_SIZE - 11);
        snprintf(id_str, 11, "%u", id);
        strncat(file_name, id_str, 11);
        strncpy(photoProperties->storage_name, file_name, CHAR_BUFFER_SIZE);
        photoProperties->keywords = NULL;
        photoProperties->num_keywords = 0;

        //SEND MESSAGE TO PEER: 4-phase send
        // message type
        ret_val_send = send(peer_socket, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_add_photo function (the operation type).\n");
            free(file_buffer);
            fclose(fp);
            return 0;
        }

        // size of the image
        ret_val_send = send(peer_socket, &file_size, sizeof(file_size), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_add_photo function (the size of the image).\n");
            free(file_buffer);
            fclose(fp);
            return 0;
        }

        // the image itself
        ret_val_send = send(peer_socket, file_buffer, file_size, NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_add_photo function (the image).\n");
            free(file_buffer);
            fclose(fp);
            return 0;
        }

        // image metadata
        ret_val_send = send(peer_socket, photoProperties, sizeof(PhotoProperties), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_add_photo function (the image).\n");
            free(file_buffer);
            fclose(fp);
            return 0;
        }

        free(file_buffer);
        fclose(fp);

    return id;
}

int gallery_add_keyword(int peer_socket, uint32_t id_photo, char* keyword)
{

}

int gallery_search_photo(int peer_socket, char* keyword, uint32_t** id_photos)
{
    int ret_val_send = 0;
    int ret_val_recv = 0;
    Message_api_op_type message_api_op_type = {.type = GALLERY_API_SEARCH_PHOTO};
    int num_photos = 0;
    int i = 0;

    //id_photos** = (int*)calloc(100, 100*sizeof(uint32_t*));

    //SEND MESSAGE TO PEER (One with the message_type and another with a string of keywords)
    ret_val_send = send(peer_socket, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS);
    if(ret_val_send == -1){
        fprintf(stderr, "Error sending message in gallery_search_photo function\n");
        return 0;
    }
    ret_val_send = send(peer_socket, keyword, sizeof(keyword), NO_FLAGS);
    if (ret_val_send == -1){
        fprintf(stderr, "Error sending message in gallery_search_photo function\n");
        exit(EXIT_FAILURE);
    }

    //RECEIVE MESSAGE FROM PEER
    ret_val_recv = recv(peer_socket, *id_photos, sizeof(*id_photos), NO_FLAGS);
    if (ret_val_recv == -1){
        fprintf(stderr, "Error receiving message in gallery_search_photo function\n");
        exit(EXIT_FAILURE);
    }

    for(i = 0; *id_photos[i] != '\0'; i++){
        num_photos++;
    }

    return num_photos;
}

int gallery_delete_photo(int peer_socket, uint32_t id_photo)
{
    // communication  variables
    Message_api_op_type message_api_op_type = {.type = GALLERY_API_DELETE_PHOTO};
    int ret_val_send = 0;
    int ret_val_recv = 0;
    uint32_t deleted = -1;

        // send message type
        ret_val_send = send(peer_socket, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_delete_photo function\n");
            return -1;
        }

        // send id to delete
        ret_val_send = send(peer_socket, &id_photo, sizeof(id_photo), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_delete_photo function\n");
            return -1;
        }

        // receive confirmation message
        ret_val_recv = recv(peer_socket, &deleted, sizeof(deleted), NO_FLAGS);
        if(ret_val_recv == -1){
            fprintf(stderr, "Error receiving confirmation message in gallery_delete_photo function\n");
            return -1;
        }
        if((int)deleted == -1){
            fprintf(stderr, "Peer reports error on photo deletion\n");
        }else if(deleted == PHOTO_NOT_FOUND){
            fprintf(stderr, "Peer says photo does not exist\n");
        }

    return deleted;
}

int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char** photo_name)
{
    int i = 0;
    int j = 0;
    int name_str_len = 0;
    Message_api_op_type message_api_op_type = {.type = GALLERY_API_GET_PHOTO_NAME};
    int ret_val_send = 0;
    int ret_val_recv = 0;
    int num_photos = 0;
    char** aux_photo_names = NULL;

        // send message type to peer
        ret_val_send = send(peer_socket, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS);
        if (ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_get_photo_name function\n");
            return -1;
        }

        // send id of photo
        ret_val_send = send(peer_socket, &id_photo, sizeof(id_photo), NO_FLAGS);
        if (ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_get_photo_name function\n");
            return -1;
        }

        // if id is 0 we have to receive every photo, so we need the number of photos we will receive
        if(id_photo == 0){
            ret_val_recv = recv(peer_socket, &num_photos, sizeof(num_photos), NO_FLAGS);
            if(ret_val_recv == -1){
                fprintf(stdout, "gallery_get_photo_name: Error receiving number of photos.\n");
                return -1;
            }
            if(num_photos == 0){
                return PHOTO_NOT_FOUND;
            }
            aux_photo_names = (char**)malloc(num_photos * sizeof(char*));
            for(i = 0; i < num_photos; i++){
                ret_val_recv = recv(peer_socket, &name_str_len, sizeof(name_str_len), NO_FLAGS);
                if(name_str_len != 0){
                    aux_photo_names[i] = malloc((name_str_len + 1) * sizeof(char));
                    ret_val_recv = recv(peer_socket, aux_photo_names[i], name_str_len + 1, NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Error receiving photo names\n");
                        for(j = 0; j < i; j++){
                            if(aux_photo_names[j] != NULL){
                                free(aux_photo_names[j]);
                            }
                        }
                        free(aux_photo_names);
                        return -1;
                    }
                }else{
                    aux_photo_names[i] = NULL;
                }
            }
            *photo_name = (char*)aux_photo_names;
            return num_photos;
        }else{
            // receive photo names
            ret_val_recv = recv(peer_socket, &name_str_len, sizeof(name_str_len), NO_FLAGS);
            if(ret_val_recv == -1){
                fprintf(stderr, "Error receiving name length in gallery_get_photo_name function\n");
                return -1;
            }
            if(name_str_len == 0){
                return PHOTO_NOT_FOUND;
            }
            *photo_name = malloc((name_str_len + 1) * sizeof(char));
            ret_val_recv = recv(peer_socket, *photo_name, name_str_len + 1, NO_FLAGS);
            if(ret_val_recv == -1){
                fprintf(stderr, "gallery_get_photo_name: Error receiving photo name.\n");
                free(*photo_name);
                return -1;
            }

            return 1;
        }

    return -1;
}

int gallery_get_photo(int peer_socket, uint32_t id_photo, char** file_name)
{
    Message_api_op_type message_api_op_type = {.type = GALLERY_API_GET_PHOTO};
    int ret_val_send = 0;
    int ret_val_recv = 0;
    int ret_val_fwrite = 0;
    FILE* fp = NULL;
    void* file_buffer = NULL;
    long int file_size = 0;
    int name_str_len = 0;
    int num_photos = 0;

        //send message type
        ret_val_send = send(peer_socket, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_get_photo function\n");
            return -1;
        }

        // send the id of the photo
        ret_val_send = send(peer_socket, &id_photo, sizeof(id_photo), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_get_photo function\n");
            return -1;
        }

        // If the id is not 0, then we will only get one photo
        if(id_photo != 0){
            // receive the strlen of the name
            ret_val_recv = recv(peer_socket, &name_str_len, sizeof(name_str_len), NO_FLAGS);
            if(ret_val_recv == -1){
                fprintf(stderr, "get_photo: Error receiving name string length.\n");
                return -1;
            }
            if(name_str_len == 0){
                return PHOTO_NOT_FOUND;
            }
            // alloc memory for the name and receive the name
            *file_name = malloc((name_str_len + 1) * sizeof(char));
            ret_val_recv = recv(peer_socket, *file_name, name_str_len + 1, NO_FLAGS);
            if(ret_val_recv == -1){
                fprintf(stderr, "get_photo: Error receiving name. %s\n", *file_name);
                free(*file_name);
                return -1;
            }

            // receive the size of the image
            ret_val_recv = recv(peer_socket, &file_size, sizeof(file_size), NO_FLAGS);
            if(ret_val_recv == -1){
                fprintf(stderr, "get_photo: Error receiving file size.\n");
                free(*file_name);
                return -1;
            }

            // alloc memory for the file and receive the file
            file_buffer = malloc(file_size);
            ret_val_recv = recv(peer_socket, file_buffer, file_size, NO_FLAGS);
            if(ret_val_recv == -1){
                fprintf(stderr, "get_photo: Error receiving photo.\n");
                free(*file_name);
                free(file_buffer);
            }

            //write the file to disk
            fp = fopen(*file_name, "wb");
            if(fp == NULL){
                fprintf(stderr, "get_photo: Error creating file on disk. %s\n", *file_name);
                free(*file_name);
                free(file_buffer);
                return -1;
            }
            ret_val_fwrite = fwrite(file_buffer, 1, file_size, fp);
            if(ret_val_fwrite != file_size){
                fprintf(stderr, "get_photo: Error writing file to disk. Expected bytes: %ld | Bytes written: %d\n", file_size, ret_val_fwrite);
                free(*file_name);
                free(file_buffer);
                fclose(fp);
                return -1;
            }
            fclose(fp);

            return 1;
        }else{
        // Get all photos
            //TODO
            num_photos = 5;
            return num_photos;
        }

    return -1;
}

bool gallery_close_connection(int peer_socket)
{
    int ret_val_send = 0;
    Message_api_op_type message_api_op_type = {.type = GALLERY_API_CLOSE_CONNECTION};

        ret_val_send = send(peer_socket, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS);
        if(ret_val_send == -1){
            fprintf(stderr, "Error sending message in gallery_close_connection function (the operation type).\n");
            return false;
        }

    return true;
}

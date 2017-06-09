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

int clientStreamSocketSetup(void)
{
    int socket_stream_fd = 0;
    struct sockaddr_in peer_socket_address;   // address of the peer socket
    int ret_val_bind = 0;
    int ret_val_listen = 0;

        socket_stream_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
        if(socket_stream_fd == -1){
            fprintf(stderr, "Error opening stream socket.\n");
            exit(EXIT_FAILURE);
        }

        /* connect socket to socket address */
        setupPeerAddress(&peer_socket_address);

        // stream socket
        ret_val_bind = bind(socket_stream_fd, (struct sockaddr *)&peer_socket_address, sizeof(struct sockaddr_in));  // call bind
        if(ret_val_bind == -1){  // check for error
            fprintf(stderr, "Error binding socket stream\n");
            exit(EXIT_FAILURE);
        }

        /* listen on the socket */
        ret_val_listen = listen(socket_stream_fd, BACKLOG_MAX);
        if(ret_val_listen == -1){  // check for error
            fprintf(stderr, "Error listening for connections.\n");
            exit(EXIT_FAILURE);
        }

    return socket_stream_fd;
}

int gatewayConnect(int peerStreamSocket)
{
    int socket_dgram_fd = 0;
    struct sockaddr_in gateway_socket_address;  // address of the gateway socket
    struct sockaddr_in peer_socket_address_stream;
    struct sockaddr_in peer_socket_address_dgram;
    socklen_t peer_socket_address_len = sizeof(struct sockaddr_in);
    int ret_val_send_to = 0;
    int ret_val_bind = 0;
    Message_gw message_gw;

        //dgram socket
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        setupPeerAddressDgram(&peer_socket_address_dgram);

        /* bind: attach a local address to a socket */
        ret_val_bind = bind(socket_dgram_fd, (struct sockaddr *)&peer_socket_address_dgram, sizeof(struct sockaddr_in));  // call bind
        if(ret_val_bind == -1){  // check for error
            fprintf(stderr, "Error binding socket stream\n");
            exit(EXIT_FAILURE);
        }

        /* get gateway address info */
        setupGatewayAddress(&gateway_socket_address);

        // this only gets us the port, the adress comes out as 0.0.0.0
        getsockname(peerStreamSocket, (struct sockaddr *)&peer_socket_address_stream, &peer_socket_address_len);

        // the gateway will get the adress from the dgram socket
        message_gw.address = 0;
        message_gw.type = PEER_ADDRESS;
        message_gw.port = ntohs(peer_socket_address_stream.sin_port);

        // send peer address to gateway
        ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, sizeof(struct sockaddr_in));

    return socket_dgram_fd;
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
            fgets(buffer, CHAR_BUFFER_SIZE, stdin);
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
        buffer = NULL;

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
            fgets(char_buffer, CHAR_BUFFER_SIZE, stdin);
            ret_val_sscanf = sscanf(char_buffer, "%d\n", &gateway_port);

            if ((gateway_port < 1024) || (gateway_port > USHRT_MAX)){
                fprintf(stderr, "Invalid gateway port\n\n");
            }else if(ret_val_sscanf != 1){
                fprintf(stderr, "Error reading port\n\n");
            }
        }

        free(char_buffer);
        char_buffer = NULL;

    return gateway_port;
}

void setupPeerAddress(struct sockaddr_in * psa)
{
    int peer_port = 0;

        memset((void*)psa, 0, sizeof(struct sockaddr_in));   // first we reset the struct
        psa->sin_family = AF_INET;
        psa->sin_addr.s_addr = htonl(INADDR_ANY);
        peer_port = BASE_PORT + getpid();
        if(peer_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        psa->sin_port = htons(peer_port);
        fprintf(stdout, "peer stream socket is on port: %d\n", peer_port);

    return;
}

void setupPeerAddressDgram(struct sockaddr_in * psa_dgram)
{
    int peer_port = 0;

        memset((void*)psa_dgram, 0, sizeof(struct sockaddr_in));   // first we reset the struct
        psa_dgram->sin_family = AF_INET;
        psa_dgram->sin_addr.s_addr = htonl(INADDR_ANY);
        peer_port = BASE_PORT + getpid() + 1;
        if(peer_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        psa_dgram->sin_port = htons(peer_port);
        fprintf(stdout, "peer datagram socket is on port: %d\n", peer_port);

    return;
}

void setupGatewayAddress(struct sockaddr_in * gsa)
{
    char* gateway_ipv4 = NULL;
    int gateway_port = 0;

        gateway_ipv4 = (char*)malloc(IPV4_STRING_SIZE * sizeof(char));
        getGatewayIPv4(gateway_ipv4);
        gateway_port = getGatewayPort();
        fprintf(stdout, "\nGateway IP address: %s\nGateway port: %d\n", gateway_ipv4, gateway_port);

        /* set gateway address struct */
        memset((void*)gsa, 0, sizeof(struct sockaddr_in));   // first we reset the struct
        gsa->sin_family = AF_INET;
        inet_aton(gateway_ipv4, &(gsa->sin_addr));
        gsa->sin_port = htons(gateway_port);

    free(gateway_ipv4);
    gateway_ipv4 = NULL;

    return;
}

void addPhotoToList(SinglyLinkedList* list_head, PhotoProperties* photo_metadata)
{
    SinglyLinkedList* aux_photo_list_node = NULL;

        for(aux_photo_list_node = list_head; SinglyLinkedList_getNextNode(aux_photo_list_node) != NULL; aux_photo_list_node =   SinglyLinkedList_getNextNode(aux_photo_list_node)){}

        if(SinglyLinkedList_getItem(aux_photo_list_node) == NULL){
            SinglyLinkedList_setItem(aux_photo_list_node, photo_metadata);
        }else{
            SinglyLinkedList_insertAtEnd(aux_photo_list_node, SinglyLinkedList_newNode(photo_metadata));
        }

    return;
}

void writePhotoToDisk(void* photo, long int size, char storage_name[CHAR_BUFFER_SIZE])
{
    FILE* fp = NULL;

        fp = fopen(storage_name, "wb");
        fwrite(photo, 1, size, fp);
        fclose(fp);

    return;
}

int deletePhotoFromList(uint32_t id, SinglyLinkedList* list_head)
{
    SinglyLinkedList* aux_photo_list_node = NULL;
    PhotoProperties* aux_photo_properties_item = NULL;
    int name_len = 0;
    char* name = NULL;
    char* rm_str = NULL;
    char rm_cmd[4] = "rm ";

        for(aux_photo_list_node = list_head; aux_photo_list_node != NULL; aux_photo_list_node =   SinglyLinkedList_getNextNode(aux_photo_list_node)){
            if(SinglyLinkedList_getItem(aux_photo_list_node) != NULL){
                aux_photo_properties_item = (PhotoProperties*)SinglyLinkedList_getItem(aux_photo_list_node);
                if(aux_photo_properties_item->photo_id == id){
                    name_len = strlen(aux_photo_properties_item->storage_name);
                    name = malloc((name_len + 1) * sizeof(char));
                    strncpy(name, aux_photo_properties_item->storage_name, name_len + 1);
                    rm_str = malloc((strlen(name) + 1 + strlen(rm_cmd)) * sizeof(char));
                    strncpy(rm_str, rm_cmd, strlen(rm_cmd) + 1);
                    strncat(rm_str, name, name_len);
                    system(rm_str);
                    SinglyLinkedList_deleteNode(aux_photo_list_node, NULL); // TODO: make deleting func
                    free(name);
                    free(rm_str);
                    name = NULL;
                    rm_str = NULL;
                    return PHOTO_DELETE_SUCCESS;
                }
            }
        }

        free(name);
        free(rm_str);
        name = NULL;
        rm_str = NULL;

    return PHOTO_NOT_FOUND;
}

void findPhotoName(SinglyLinkedList* photo_list_head, uint32_t id, int* name_str_len, char** photo_name, char** photo_storage_name)
{
    SinglyLinkedList* aux_photo_list_node = NULL;
    PhotoProperties* aux_photo_properties_item = NULL;
    int photo_storage_name_len = 0;

        for(aux_photo_list_node = photo_list_head; aux_photo_list_node != NULL; aux_photo_list_node =     SinglyLinkedList_getNextNode(aux_photo_list_node)){
            if(SinglyLinkedList_getItem(aux_photo_list_node) != NULL){
                aux_photo_properties_item = (PhotoProperties*)SinglyLinkedList_getItem(aux_photo_list_node);
                if(aux_photo_properties_item->photo_id == id){
                    *name_str_len = strlen(aux_photo_properties_item->photo_name);
                    photo_storage_name_len = strlen(aux_photo_properties_item->storage_name);
                    *photo_storage_name = malloc((photo_storage_name_len + 1) * sizeof(char));
                    *photo_name = malloc((*name_str_len + 1) * sizeof(char));
                    strncpy(*photo_name, aux_photo_properties_item->photo_name, *name_str_len + 1);
                    strncpy(*photo_storage_name, aux_photo_properties_item->storage_name, photo_storage_name_len + 1);
                    return;
                }
            }
        }

    return;
}

int retrievePhoto(char* photo_name, long int* file_size, void** file_buffer)
{
    FILE* fp = NULL;
    int ret_val_fread = 0;

        fp = fopen(photo_name, "rb");
        if(fp == NULL){
            // TODO: delete metadata from list in this case
            fprintf(stderr, "No such file on disk to retrieve.\n");
            return -1;
        }

        fseek(fp, 0L, SEEK_END);
        *file_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        *file_buffer = malloc(*file_size);

        ret_val_fread = fread(*file_buffer, 1, *file_size, fp);
        if(ret_val_fread != *file_size){
            fprintf(stderr, "Error retrieving file from disk.\n");
            free(*file_buffer);
            (*file_buffer) = NULL;
            return -1;
        }

        fclose(fp);

    return 0;
}

int getPhotoName(SinglyLinkedList* photo_node, int* name_str_len, char** photo_name){
// TODO

        if(SinglyLinkedList_getItem(photo_node) != NULL){
            *name_str_len = strlen(((PhotoProperties*)SinglyLinkedList_getItem(photo_node))->photo_name);
            *photo_name = malloc((*name_str_len + 1) * sizeof(char));
            strncpy(*photo_name, ((PhotoProperties*)SinglyLinkedList_getItem(photo_node))->photo_name, *name_str_len + 1);
            return 0;
        }else{
            *name_str_len = 0;
            return -1;
        }
}

SinglyLinkedList* findPhotoById(SinglyLinkedList* photo_list_head, uint32_t id)
{
    SinglyLinkedList* aux_photo_list_node = NULL;
    PhotoProperties* aux_photo_properties_item = NULL;

    for(aux_photo_list_node = photo_list_head; aux_photo_list_node != NULL; aux_photo_list_node =     SinglyLinkedList_getNextNode(aux_photo_list_node)){
        if(SinglyLinkedList_getItem(aux_photo_list_node) != NULL){
            aux_photo_properties_item = (PhotoProperties*)SinglyLinkedList_getItem(aux_photo_list_node);
            if(aux_photo_properties_item->photo_id == id){
                return aux_photo_list_node;
            }
        }
    }

    return NULL;
}

void addKeywordtoPhoto(SinglyLinkedList* photo_list_node, int keyword_str_len, char* keyword)
{
    PhotoProperties* photoProperties = NULL;
    int* num_keywords = NULL;
    char*** keywords = NULL;

        photoProperties = (PhotoProperties*)SinglyLinkedList_getItem(photo_list_node);
        num_keywords = &(photoProperties->num_keywords);
        keywords = &(photoProperties->keywords);

        (*num_keywords) += 1;
        *keywords = realloc(*keywords, (*num_keywords) * sizeof(char*));
        (*keywords)[(*num_keywords)-1] = malloc((keyword_str_len + 1) * sizeof(char));
        strncpy((*keywords)[(*num_keywords)-1], keyword, keyword_str_len + 1);


    return;
}

void findPhotoByKeyword(SinglyLinkedList* photo_list_head, char* keyword, uint32_t** photo_ids, int* num_photos)
{
    int i = 0;
    SinglyLinkedList* aux_photo_list_node = NULL;
    PhotoProperties* aux_photo_properties_item = NULL;
    int aux_num_keywords = 0;
    char** aux_keywords = NULL;

        *num_photos = 0;

        for(aux_photo_list_node = photo_list_head; aux_photo_list_node != NULL; aux_photo_list_node = SinglyLinkedList_getNextNode(aux_photo_list_node)){
            if(SinglyLinkedList_getItem(aux_photo_list_node) != NULL){
                aux_photo_properties_item = (PhotoProperties*)SinglyLinkedList_getItem(aux_photo_list_node);
                aux_num_keywords = aux_photo_properties_item->num_keywords;
                if(aux_num_keywords > 0){
                    aux_keywords = aux_photo_properties_item->keywords;
                    for(i = 0; i < aux_num_keywords; i++){
                        if((strncmp(aux_keywords[i], keyword, strlen(aux_keywords[i])) == 0) || (strstr(aux_keywords[i], keyword) != NULL)){
                            *num_photos += 1;
                            *photo_ids = realloc(*photo_ids, (*num_photos) * sizeof(uint32_t));
                            (*photo_ids)[(*num_photos)-1] = aux_photo_properties_item->photo_id;
                        }
                    }
                }
            }
        }

    return;
}

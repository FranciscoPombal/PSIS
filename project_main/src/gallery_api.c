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
        ret_val_recvfrom = recvfrom(socket_dgram_fd, &message_gw, sizeof(message_gw), NO_FLAGS, (struct sockaddr        *)&gateway_socket_address, &gateway_socket_address_len);
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
    struct client_message message;
    int* message_type = (int*) ADD_PHOTO;
    message.id = -1; //Must define id in gateway
    int lastbar = -1;
    char* lastdot;
    char* ret_val_strcpy;
    int ret_val_send = -2;
    int i = 0;
    char command[1000];

    //GET ID
    char id_buffer[50];
    FILE* fp_id;
    strcpy(command, "crc32 ");
    strcat(command, file_name);
    fp_id = popen(command, "r");
    fgets(id_buffer, sizeof(id_buffer), fp_id);

    message.id = (int) strtol(id_buffer, NULL, 16);

    fclose(fp_id);
    free(command);

    //MESSAGE.FILENAME (eliminate the path and the extension)
    if(file_name == NULL){
        fprintf(stderr, "File name is NULL\n");
        exit(EXIT_FAILURE);
    }

    if(strrchr(file_name, '/') == NULL){
        ret_val_strcpy = strcpy(message.filename, file_name);
        if(ret_val_strcpy == NULL){
            fprintf(stderr, "strcpy error in gallery_add_photo function\n");
            exit(EXIT_FAILURE);
        }
    }

    if(strrchr(file_name, '/') != NULL){
        for ( i = 0; file_name[i] != '\0'; i++) {
            if (file_name[i] == '/') {
                lastbar = i;
            }
        }
        for ( i = 0; file_name[lastbar + i + 1] != '\0'; i++){
            message.filename[i] = file_name[lastbar + i + 1];
        }
        message.filename[i + 1] = '\0';
    }

    lastdot = strrchr(message.filename, '.');

    if(lastdot != NULL){
        *lastdot = '\0';
    }

    //FOPEN
    FILE* fp = fopen(file_name, "r");

    //SEND MESSAGE TO PEER
    ret_val_send = send(peer_socket, message_type, sizeof(message_type), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_add_photo function\n");
        exit(EXIT_FAILURE);
    }
    ret_val_send = send(peer_socket, fp, sizeof(*fp), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_add_photo function\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    return message.id;
}

int gallery_add_keyword(int peer_socket, uint32_t id_photo, char* keyword)
{

}

int gallery_search_photo(int peer_socket, char* keyword, uint32_t** id_photos)
{
    int ret_val_send = 0;
    int ret_val_recv = 0;
    int* message_type = (int*) SEARCH_PHOTO;
    int num_photos = 0;
    int i = 0;

    //id_photos** = (int*)calloc(100, 100*sizeof(uint32_t*));

    //SEND MESSAGE TO PEER (One with the message_type and another with a string of keywords)
    ret_val_send = send(peer_socket, message_type, sizeof(message_type), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_search_photo function\n");
        exit(EXIT_FAILURE);
    }
    ret_val_send = send(peer_socket, keyword, sizeof(keyword), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_search_photo function\n");
        exit(EXIT_FAILURE);
    }

    //RECEIVE MESSAGE FROM PEER
    ret_val_recv = recv(peer_socket, *id_photos, sizeof(*id_photos), NO_FLAGS);
    if (ret_val_recv < 0) {
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
    int* message_type = (int*) DELETE_PHOTO;
    int ret_val_send = 0;
    int ret_val_recv = 0;
    int deleted = -1;

    //SEND MESSAGES TO PEER
    ret_val_send = send(peer_socket, message_type, sizeof(message_type), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_delete_photo function\n");
        exit(EXIT_FAILURE);
    }
    ret_val_send = send(peer_socket, &id_photo, sizeof(&id_photo), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_delete_photo function\n");
        exit(EXIT_FAILURE);
    }

    //RECEIVE CONFIRMATION MESSAGE
    ret_val_recv = recv(peer_socket, &deleted, sizeof(&deleted), NO_FLAGS);
    if(ret_val_recv < 0){
        fprintf(stderr, "Error receiving confirmation message in gallery_delete_photo function\n");
        exit(EXIT_FAILURE);
    }

    return deleted;
}

int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char** photo_name)
{
    int* message_type = (int*) GET_PHOTO_NAME;
    int ret_val_send = 0;
    int ret_val_recv = 0;

    //SEND MESSAGES TO PEER
    ret_val_send = send(peer_socket, message_type, sizeof(message_type), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_get_photo_name function\n");
        exit(EXIT_FAILURE);
    }
    ret_val_send = send(peer_socket, &id_photo, sizeof(&id_photo), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_get_photo_name function\n");
        exit(EXIT_FAILURE);
    }

    //RECEIVE MESSAGE FROM PEER
    ret_val_recv = recv(peer_socket, *photo_name, sizeof(*photo_name), NO_FLAGS);
    if(ret_val_recv < 0){
        fprintf(stderr, "Error receiving confirmation message in gallery_get_photo_name function\n");
        exit(EXIT_FAILURE);
    }

    if(photo_name == NULL){
        return 0;
    }

    fprintf(stdout, "Photo name: %s\n", *photo_name);

    return 1;
}

int gallery_get_photo(int peer_socket, uint32_t id_photo, char** file_name)
{
    int* message_type = (int*) GET_PHOTO;
    int ret_val_send = 0;
    int ret_val_recv = 0;
    FILE* fp = fopen(*file_name, "w");

    //SEND MESSAGES TO PEER
    ret_val_send = send(peer_socket, message_type, sizeof(message_type), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_get_photo function\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    ret_val_send = send(peer_socket, &id_photo, sizeof(&id_photo), NO_FLAGS);
    if (ret_val_send < 0) {
        fprintf(stderr, "Error sending message in gallery_get_photo function\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    //RECEIVE MESSAGE FROM PEER
    ret_val_recv = recv(peer_socket, fp, sizeof(fp), NO_FLAGS);
    if(ret_val_recv < 0){
        fprintf(stderr, "Error receiving confirmation message in gallery_get_photo_name function\n");
        exit(EXIT_FAILURE);
    }

    if(fp == NULL){
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 0;
}

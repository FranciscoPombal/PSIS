#include "../include/peer.h"

pthread_mutex_t photo_list_mutex = PTHREAD_MUTEX_INITIALIZER;

void* pingerThread(void* args)
{
    int socket_dgram_fd = 0;
    int ret_val_send_to = 0;
    int ret_val_recvfrom = 0;

    Message_ping message_ping;
    struct sockaddr_in gateway_socket_address;
    socklen_t gateway_socket_address_len = sizeof(gateway_socket_address);

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


void* clientHandlerThread(void* args)
{
    bool closeConnection = false;
    int ret_val_recv_from = 0;
    int ret_val_recv = 0;
    int ret_val_send = 0;
    struct sockaddr_in client_socket_address;
    socklen_t client_socket_address_len = sizeof(client_socket_address);
    Message_api_op_type message_api_op_type;
    ClientHandlerThreadArgs* clientHandlerThreadArgs = NULL;
    int socket_fd = 0;

    SinglyLinkedList* photo_list_head = NULL;
    PhotoProperties* photoProperties = NULL;
    long int file_size = 0;
    void* file_buffer = NULL;
    uint32_t id = 0;
    int delete_response = 0;

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
                    ret_val_recv = recv(socket_fd, &file_size, sizeof(file_size), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Add photo: Error receiving sizeof image\n");
                        break;
                    }
                    // image
                    file_buffer = malloc(file_size);
                    ret_val_recv = recv(socket_fd, file_buffer, file_size, NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Add photo: Error receiving image.\n");
                        free(file_buffer);
                        break;
                    }else if(ret_val_recv != file_size){
                        fprintf(stderr, "Add photo: Wrong number of image bytes received: %d\n", ret_val_recv);
                        free(file_buffer);
                        break;
                    }

                    // image metadata
                    photoProperties = malloc(sizeof(PhotoProperties));
                    ret_val_recv = recv(socket_fd, photoProperties, sizeof(PhotoProperties), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Add photo: Error receiving image metadata\n");
                        free(photoProperties);
                        break;
                    }

                    writePhotoToDisk(file_buffer, file_size, photoProperties->storage_name);
                    pthread_mutex_lock(&photo_list_mutex);
                    addPhotoToList(photo_list_head, photoProperties);
                    pthread_mutex_lock(&photo_list_mutex);
                    free(file_buffer);
                    break;
                }
                case GALLERY_API_SEARCH_PHOTO:
                {
                    //call function for this
                    fprintf(stdout, "Client wants to search for a photo\n"); // DEBUG
                    break;
                }
                case GALLERY_API_DELETE_PHOTO:
                {
                    fprintf(stdout, "Client wants to delete a photo\n"); // DEBUG
                    ret_val_recv = recv(socket_fd, &id, sizeof(id), NO_FLAGS);
                    if(ret_val_recv == -1){
                        fprintf(stderr, "Delete photo: Error receiving id of image\n");
                        break;
                    }

                    pthread_mutex_lock(&photo_list_mutex);
                    delete_response = deletePhotoFromList(id, photo_list_head);
                    pthread_mutex_unlock(&photo_list_mutex);

                    ret_val_send = send(socket_fd, &delete_response, sizeof(delete_response), NO_FLAGS);
                    if(ret_val_send == -1){
                        fprintf(stderr, "Delete photo: error sending response to client\n");
                        break;
                    }

                    break;
                }
                case GALLERY_API_GET_PHOTO_NAME:
                {
                    //call function for this
                    fprintf(stdout, "Client wants to get the name of a photo\n"); // DEBUG
                    break;
                }
                case GALLERY_API_GET_PHOTO:
                {
                    fprintf(stdout, "Client wants to get a photo\n"); // DEBUG
                    break;
                }
                case GALLERY_API_ADD_KEYWORD:
                {
                    //call function for this
                    fprintf(stdout, "Client wants to add a keyword to a photo\n"); // DEBUG
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
                    fprintf(stderr, "Error in client<->peer communication. The request operation is unknown.\n");
                    break;
                }
            }
        }

        // close connection with current client
        close(socket_fd);
        fprintf(stdout, "Peer has closed the connection with the client and this thread is exiting\n");

    pthread_exit(NULL);
}

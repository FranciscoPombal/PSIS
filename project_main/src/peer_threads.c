#include "../include/peer.h"

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
    int socket_fd = 0;
    bool closeConnection = false;
    int ret_val_recv_from = 0;
    struct sockaddr_in client_socket_address;
    socklen_t client_socket_address_len = sizeof(client_socket_address);
    Message_api_op_type message_api_op_type;

        socket_fd = *((int*)args);

        while(false == closeConnection){
            ret_val_recv_from = recvfrom(socket_fd, &message_api_op_type, sizeof(Message_api_op_type), NO_FLAGS, (struct sockaddr *)&client_socket_address, &client_socket_address_len);

            switch (message_api_op_type.type){
                case GALLERY_API_ADD_PHOTO:
                    //call function for this
                    fprintf(stdout, "Client wants to add a photo\n"); // DEBUG
                case GALLERY_API_SEARCH_PHOTO:
                    //call function for this
                    fprintf(stdout, "Client wants to search for a photo\n"); // DEBUG
                case GALLERY_API_DELETE_PHOTO:
                    //call function for this
                    fprintf(stdout, "Client wants to delete a photo\n"); // DEBUG
                case GALLERY_API_GET_PHOTO_NAME:
                    //call function for this
                    fprintf(stdout, "Client wants to get the name of a photo\n"); // DEBUG
                case GALLERY_API_GET_PHOTO:
                    //call function for this
                    fprintf(stdout, "Client wants to get a photo\n"); // DEBUG
                case GALLERY_API_ADD_KEYWORD:
                    //call function for this
                    fprintf(stdout, "Client wants to add a keyword to a photo\n"); // DEBUG
                case GALLERY_API_CLOSE_CONNECTION:
                    fprintf(stdout, "Client wants to close the connection\n"); // DEBUG
                    closeConnection = true;
                default:
                    fprintf(stderr, "Error in client<->peer communication. The request operation is unknown.\n");
                }
        }

        // close connection with current client
        close(socket_fd);
        fprintf(stdout, "Peer has closed the connection with the client and this thread is exiting\n");

    pthread_exit(NULL);
}

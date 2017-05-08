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
            }
        }

    pthread_exit(NULL);
}


void* clientHandlerThread(void* args)
{

    pthread_exit(NULL);
}

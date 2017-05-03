#include "../include/messages.h"

int getGatewayPort(void);


int getGatewayPort(void)
{
    char* char_buffer;
    int gateway_port = 0;
    int ret_val_sscanf = 0;

        char_buffer = (char*)malloc(CHAR_BUFFER_SIZE * sizeof(char));

        while((gateway_port < 1024) || (gateway_port > USHRT_MAX) || (ret_val_sscanf != 1)){
            fprintf(stdout, "Insert gateway port:\n");
            fgets(char_buffer, sizeof(char_buffer), stdin);
            ret_val_sscanf = sscanf(char_buffer, "%d\n", &gateway_port);

            if ((gateway_port < 1024) || (gateway_port > USHRT_MAX)){
                fprintf(stderr, "Invalid gateway port\n\n");
            }else if(ret_val_sscanf != 1){
                fprintf(stderr, "Error reading port\n\n");
            }
        }

        free(char_buffer);

    return gateway_port;
}

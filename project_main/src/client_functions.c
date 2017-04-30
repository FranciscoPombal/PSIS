#include "../include/client.h"

void getGatewayIPv4(char* gateway_ipv4)
{
    int i = 0;
    int v = 0;
    int ip[4] = {-1, -1, -1, -1};
    char* buffer = NULL;
    int ret_val_sscanf = 0;
    bool valid_ip = false;

        buffer = (char*)malloc(CHAR_BUFFER_SIZE * sizeof(char));

        while(ret_val_sscanf != 4 || false == valid_ip ){
            printf("Insert Gateway IP address (default: XXX):\n");
            fgets(buffer, 100, stdin);
            ret_val_sscanf = sscanf(buffer, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
            if(ret_val_sscanf != 4){
                fprintf(stderr, "Error getting gateway IP\n\n");
            }
            for (i = 0; i < 4; i++) {
                if(ip[i] < 0 || ip[i] > 255){
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
        // TODO: Must not forget to free IP_v4 in main()

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

int showMenu(void)
{
    int option = 0;

        printf("\nChoose an option (number):\n");
        printf("1 - \n");
        printf("2 - \n");
        printf("3 - \n");
        printf("4 - \n");
        printf("5 - \n");

        scanf("%d\n", &option);

    return option;
}

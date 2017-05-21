#include "../include/client.h"

void sigIntHandler(int signal)
{
    switch(signal){
        case 2:
            fprintf(stdout, "\nCaught SIGINT\n");
            keepRunning = false;
            break;
        default:
            fprintf(stdout, "\nCaught other signal (should not happen!)\n");
            break;
    }

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
            fgets(buffer, 100, stdin);
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
            fgets(char_buffer, sizeof(char_buffer), stdin);
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

int showMenu(void)
{
    int option = 0;
    int ret_val_sscanf = 0;
    char* char_buffer = NULL;

        char_buffer = (char*)malloc(CHAR_BUFFER_SIZE * sizeof(char));

        fprintf(stdout, "\nChoose an option (number):\n");
        fprintf(stdout, "1 - Add (upload) a photo to the gallery\n");
        fprintf(stdout, "2 - Add a keyword to a photo in the gallery\n");
        fprintf(stdout, "3 - Search for a photo in the gallery\n");
        fprintf(stdout, "4 - Delete a photo from the gallery\n");
        fprintf(stdout, "5 - Get the name of a photo in the gallery\n");
        fprintf(stdout, "6 - Get (download) a photo from the gallery\n");
        fprintf(stdout, "7 - Close connection with the peer\n");

        fgets(char_buffer, sizeof(char_buffer), stdin);
        ret_val_sscanf = sscanf(char_buffer, "%d\n", &option);
        if(ret_val_sscanf != 1){
            fprintf(stdout, "Error reading option, try again.\n");
        }

        free(char_buffer);
        char_buffer = NULL;

    return option;
}

#include "storyserver.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>

int main(){

    message m;
    struct sockaddr_un server_addr;
  	int nbytes;

  	int sock_fd= socket(AF_UNIX, SOCK_DGRAM, 0);

  	if (sock_fd == -1){
  		perror("socket: ");
  		exit(-1);
  	}

  	printf(" socket created \n Ready to send\n");

  	server_addr.sun_family = AF_UNIX;
  	strcpy(server_addr.sun_path, SOCKET_NAME);

    int i = 0;

    printf("message: ");
    fgets(m.buffer, MESSAGE_LEN, stdin);

    /* write message */
    nbytes = sendto(sock_fd,
  	                    m.buffer, strlen(m.buffer)+1, 0,
  	                    (const struct sockaddr *) &server_addr, sizeof(server_addr));


    /* receive story */

    printf("OK\n");
    exit(0);

}

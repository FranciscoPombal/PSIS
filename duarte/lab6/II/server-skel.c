#include "storyserver.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(){

    struct sockaddr_un local_addr;
    struct sockaddr_un server_addr;
    char buff[100];
    int nbytes;
    message m;
    char * story;


	story = strdup("");

  int sock_fd= socket(AF_UNIX, SOCK_DGRAM, 0);

  if (sock_fd == -1){
    perror("socket: ");
    exit(-1);
  }

  local_addr.sun_family = AF_UNIX;
	strcpy(local_addr.sun_path, SOCKET_NAME);
	int err = bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if(err == -1) {
		perror("bind");
		exit(-1);
	}

    while(1){
        /* read message */
        nbytes = recv(sock_fd, buff, 100, 0);
        printf("Received bytes: %d\n", nbytes);
        printf("%s\n", buff );
        /* process message */
		story = strcat(story, m.buffer);
    nbytes = sendto(sock_fd,
  	                    story, strlen(story)+1, 0,
  	                    (const struct sockaddr *) &server_addr, sizeof(server_addr));

     }
    printf("OK\n");
    close(sock_fd);
    unlink(SOCKET_NAME);
    exit(0);

}

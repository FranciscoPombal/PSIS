#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>

#include "sock_dg.h"

int main(){
	struct sockaddr_un server_addr;
	char buff[100];
	int nbytes;
	
	int sock_fd= socket(AF_UNIX, SOCK_DGRAM, 0);
	
	if (sock_fd == -1){
		perror("socket: ");
		exit(-1);
	}
	
	printf(" socket created \n Ready to send\n");

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCK_ADDRESS);

	sprintf(buff, "first message");
	nbytes = sendto(sock_fd, 
	                    buff, strlen(buff)+1, 0, 
	                    (const struct sockaddr *) &server_addr, sizeof(server_addr));
	printf("sent %d %s\n", nbytes, buff);

sleep(1);

	sprintf(buff, "second message");
	nbytes = sendto(sock_fd, 
	                    buff, strlen(buff)+1, 0, 
	                    (const struct sockaddr *) &server_addr, sizeof(server_addr));
	printf("sent %d %s\n", nbytes, buff);

sleep(1);
	sprintf(buff, "third message");
	nbytes = sendto(sock_fd, 
	                    buff, strlen(buff)+1, 0, 
	                    (const struct sockaddr *) &server_addr, sizeof(server_addr));
	printf("sent %d %s\n", nbytes, buff);

	close(sock_fd);
	exit(0);
}

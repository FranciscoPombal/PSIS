#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>

#include "sock_dg.h"

int main(){
	struct sockaddr_un local_addr;
	struct sockaddr_un client_addr;
	socklen_t size_addr;
	char buff[100];
	int nbytes;
	
	int sock_fd= socket(AF_UNIX, SOCK_DGRAM, 0);
	
	if (sock_fd == -1){
		perror("socket: ");
		exit(-1);
	}
	

	local_addr.sun_family = AF_UNIX;
	strcpy(local_addr.sun_path, SOCK_ADDRESS);
	int err = bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if(err == -1) {
		perror("bind");
		exit(-1);
	}
	
	printf(" socket created and binded \n Ready to receive messages\n");

while(1){
	nbytes = recvfrom(sock_fd, buff, 100, 0, 
	          (struct sockaddr *) & client_addr, &size_addr);
	printf("received %d bytes from %s --- %s ---\n", 
	      nbytes, client_addr.sun_path, buff);
	      
	 sprintf(buff, "reply to %s", client_addr.sun_path);
	nbytes = sendto(sock_fd, 
	                    buff, strlen(buff)+1, 0, 
	                    (const struct sockaddr *) &client_addr, 
	                    sizeof(client_addr));
	 printf("replying %d bytes\n", nbytes);
}	                    
	close(sock_fd);
	//unlink(SOCK_ADDRESS);
	exit(0);
}

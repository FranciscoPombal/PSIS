#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #include <unistd.h>
 
int main(){
	int sv[2];
	char buffer[100];
	int sock_fd = socketpair(AF_UNIX, SOCK_DGRAM, 0, sv);
	
	if(fork() == 0){
		
		fork();
		fork();
		while(1){
			sleep(1);
			sprintf(buffer, "ping");
			send(sv[0], buffer, strlen(buffer)+1, 0);
			recv(sv[0], buffer, 100, 0);
			printf("child!!! %d %s\n", getpid(), buffer);

		}
	}else{
		
		while(1){
			recv(sv[1], buffer, 100, 0);
			printf("parent!!!  2 %s\n",buffer);
			sprintf(buffer, "pong");
			send(sv[1], buffer, strlen(buffer)+1, 0);

		}
	}
}

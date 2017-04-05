#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>      
#include <sys/mman.h>

char *ptr;

int main(){
	char * filename = "int_file";
	int fd;
	int shmid;
	key_t key;
 

	fd = open(filename, O_RDWR | O_CREAT,  S_IRWXU);
	ptr = mmap(NULL, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	while(1){
		printf(" %c \n", *ptr);
		sleep(1);
	}


    exit(0);
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>      
#include <sys/mman.h>

int *ptr;

void stop_handler(int s){
	printf("Exiting the last value written is %x\n", *ptr);
	exit(-1);
}
int main(){
    char * filename = "int_file";
    int fd;
    int shmid;
    key_t key;
 
//	signal(SIGINT, stop_handler);

	fd = open(filename, O_RDWR | O_CREAT,  S_IRWXU);
	ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	int i = 1;
	while(1){
		*ptr = i++;
		i = i%127;
	}


    exit(0);
}

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main(){
    int *shm;


shm = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANON, -1, 0);

	if(fork()!=0){
		if(fork()!=0){

			int i = 1;
			while(1){
				*shm = i++;
			}
		}else{
			while(1){
				printf("%d \n", *shm);
				sleep(1);
			}
		}		
	}else{
		while(1){
			printf("%d \n", *shm);
			sleep(1);
		}
	}

    exit(0);
}

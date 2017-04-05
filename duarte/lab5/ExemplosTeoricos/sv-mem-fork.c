#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ     27

int main(){
    char c;
    int shmid;
    key_t key;
    int *shm, *shm2, *s;

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5678;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }
 shm2 = malloc(sizeof(int));
 
 *shm2 = 12;
	if(fork()==0){
		int i = 1;
		while(1){
			*shm = i++;
			//*shm2 = i;
		}
	}else{
		while(1){
			printf("%d \n", *shm);
			//printf("%d \n", *shm2);
			//sleep(1);
		}
	}
		
    exit(0);
}

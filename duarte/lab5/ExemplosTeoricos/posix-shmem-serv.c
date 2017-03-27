
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
     #include <sys/mman.h>
     #include <fcntl.h>

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(){

    int fd = shm_open("my_shmem", O_CREAT | O_TRUNC| O_RDWR, 0666);
if (fd == -1)
                error_and_die("shm_open");
    int r = ftruncate(fd, sizeof(int));
        if (r != 0)
                error_and_die("ftruncate");
		
    int *ptr = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);    
        if (ptr == MAP_FAILED)
                error_and_die("mmap");
    close(fd);

    int i = 1;
    while(1){
	    *ptr = i++;
    }
    printf("Exiting the last value written is %x\n", *ptr);
    munmap (ptr, sizeof(int));
    shm_unlink("my_shmem");
    exit(0);
}

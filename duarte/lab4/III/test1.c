#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/wait.h>

#define NUM_CHILDS 3

typedef struct{
		unsigned start, end;
    unsigned int m_7_array;
    unsigned int m_19_array;
    unsigned int m_7_19_array;
} com_struct;

int main(int argc, char const *argv[]) {
  unsigned int  m_7, m_19, m_7_19;
	m_7 = m_19 = m_7_19 = 0;
	com_struct limits;
	int i=0;
  char* memname = "shmlab4";

  limits.start = 0;
	limits.end = UINT_MAX;


  /*int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
  if (fd == -1) {
    printf("shm_open ERROR!\n");
    exit(EXIT_FAILURE);
  }*/

  /*int r = ftruncate(fd, sizeof(com_struct));
  if (r != 0) {
    error_and_die("shm_open");
  }*/

  com_struct* sh_package = mmap(NULL, NUM_CHILDS*sizeof(com_struct), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  for (i = 0; i < 3; i++) {
    sh_package[i].start = 0 + (UINT_MAX/3 *i);
    sh_package[i].end = UINT_MAX/3 + (UINT_MAX/3 *i);
    sh_package[i].m_7_array = 0;
    sh_package[i].m_19_array = 0;
    sh_package[i].m_7_19_array = 0;
  }

  pid_t pid[3];

  for (i = 0; i < 3; i++) {
    if ((pid[i] = fork()) == -1) {
      return 1;
    }
    if(pid[i] == 0){
      for (int j = sh_package[i].start; j <  sh_package[i].end; j++){
    		if(j%7 == 0){
    			sh_package[i].m_7_array ++;
    		}
    		if(j%19 == 0){
    		 sh_package[i].m_19_array ++;
    		}
    		if((j%7 == 0)	&& (i%19 == 0)){
    			sh_package[i].m_7_19_array ++;
    		}
    	}
      exit(0);
    }
  }

   int n = NUM_CHILDS;

   while(n > 0){
     int wait_status = wait(NULL);
     printf("Child %d exitted\n", wait_status);
     n--;
   }

    for(i=0; i<3; i++){
      m_7 += sh_package[i].m_7_array;
      m_19 += sh_package[i].m_19_array;
      m_7_19 += sh_package[i].m_7_19_array;
    }

    printf("m 7    %d\n", m_7);
  	printf("m   19 %d\n", m_19);
  	printf("m 7 19 %d\n\n", m_7_19);
    exit(0);

}

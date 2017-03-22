#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/wait.h>
#include <time.h>

int main(int argc, char const *argv[]) {

  time_t start=0, end=0;
  double elapsed=0;  // seconds
  start = time(NULL);
  int terminate = 1;
  int i=0, five=1;
  int even=0, odd=0;
  srand(time(NULL));


  //Create static shared memory int vector
  int* v = mmap(NULL, 1000000000*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  printf("Generating numbers, wait 10 seconds\n");
  while(terminate){

    v[i] = rand();
    if (v[i]%2 == 0) {
      even++;
    }
    else if (v[i]%2 != 0) {
      odd++;
    }
    i++;
    end = time(NULL);
    elapsed = difftime(end, start);
    if (elapsed == 5.0 && five){
       printf("Halfway there!\n");
       five=0;
  }
    if (elapsed >= 10.0 /* seconds */){
       terminate = 0;
  }
}

  printf("\nEven numbers: %d ||| Odd numbers: %d\n", even, odd);
  exit(0);
}

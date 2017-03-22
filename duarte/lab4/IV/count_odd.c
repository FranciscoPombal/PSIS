#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/wait.h>
#include <time.h>

int main(int argc, char const *argv[]) {

  int i=0, odd=0;

  while(v[i] != -1){
    if (v[i]%2 == 0) {
      odd++;
    }
  }

  exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

int verify_prime(int n){
  int i;
  for(i=2; i <n; i++){
    if(n%i==0)
      return 0;
  }
  return 1;
}

int main(int argc, char const *argv[]) {
  int  i =0, i1, i2, i3;
  int m_7_19, m_19, m_7;
  int pid;
  int x1=0, x2=0, x3=0;
  int fd1[1];
  int n = 0;
  int n_child = 0;


  if (argc == 1){
    n_child = 0;
  }
  if(argc == 2){
    if (sscanf(argv[1], "%d", &n_child) == 0)
      n_child = 0;
  }

  for(int j=0; j<=n_child; j++){ //Does this for all children
  pipe(fd1);
  pid = fork();
  if(pid == 0){
    while(n != -1){
      read(fd1[0], &n, sizeof(int));
      if (verify_prime(n))
        printf("%d is prime\n", n);
    }
      close(fd1[0]);
      exit(0);
    }
  }

        for (i = 0; i < 999999; i++) {//Send numbers to children
          n = random()%999999;
          for(int i1=0; i1<=n_child; i1++){
          write(fd1[1], &n, sizeof(int));
          }

          n = -1;
          for(int i1=0; i1<=n_child; i1++){//Terminate children
          write(fd1[1], &n, sizeof(int));
          }
        }
        wait(NULL);
  return 0;
}

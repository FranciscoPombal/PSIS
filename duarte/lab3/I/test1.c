#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {

  //pid_t pid1, pid2, pid3;
  int i = 0;
  int sleepTime = 0;
  int pid[10];

  printf("Parent process ID : %d\n", getpid());

  pid[0] = fork();

  while(i<7){
    if (pid[i] == 0) {
      printf("Child id:%d\n", getpid());
    }

    if (pid[i] > 0) {
      printf("Parent id:%d\n", getpid());
      pid[i] = fork();
    }
    i++;
  }
  sleepTime = random() % 11;
  sleep(sleepTime);

  wait();

  printf("%d slept for %d seconds\n", getpid(), &sleepTime);

  return 0;
}

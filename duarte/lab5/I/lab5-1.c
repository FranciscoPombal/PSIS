#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char const *argv[]) {
  int  i =0, i1, i2, i3;
  int m_7_19, m_19, m_7;
  int pid;
  int x1=0, x2=0, x3=0;
  int fd1[3], fd2[3], fd3[3];

  m_7_19= m_19= m_7=0;

  pipe(fd1);
  pid = fork();
  if(pid == 0){
    for (i1 = 0; i1 < 999999; i1++) {
      read(fd1[0], &x1, sizeof(int));
      if(x1%7 == 0){
          m_7 ++;
        }
      }
      printf("Process1 count (m_7): %d\n", m_7);
      close(fd1[0]);
      exit(0);
    }

    pipe(fd2);
    pid = fork();
    if(pid == 0){
      for (i2 = 0; i2 < 999999; i2++) {
        read(fd2[0], &x2, sizeof(int));
        if(x2%19 == 0){
            m_19 ++;
          }
        }
        printf("Process2 count (m_19): %d\n", m_19);
        close(fd2[0]);
        exit(0);
      }

      pipe(fd3);
      pid = fork();
      if(pid == 0){
        for (i3 = 0; i3 < 999999; i3++) {
          read(fd3[0], &x3, sizeof(int));
          if((x3%7 == 0) && (x3%19 == 0)){
              m_7_19 ++;
            }
          }
          printf("Process3 count (m_7_19): %d\n", m_7_19);
          close(fd3[0]);
          exit(0);
        }

        for (i = 0; i < 999999; i++) {
          write(fd1[1], &i, sizeof(int));
          write(fd2[1], &i, sizeof(int));
          write(fd3[1], &i, sizeof(int));
        }
        wait(NULL);
  return 0;
}

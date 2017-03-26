#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char const *argv[]) {
  int  i =0;
  int m_7_19, m_19, m_7;

  m_7_19= m_19= m_7=0;

  do{
    i++;

    if(i%7 == 0){
         m_7 ++;
    }
    if(i%19 == 0){
         m_19 ++;
    }
    if((i%19  == 0) && (i%7  == 0)){
         m_7_19 ++;
    }
  }while (i<999999);

  printf("m 7    %d\n", m_7);
  printf("m 19    %d\n", m_19);
  printf("m 7 19    %d\n", m_7_19);

  return 0;
}

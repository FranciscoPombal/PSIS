#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
		unsigned start, end;
} com_struct;

int main(){
	unsigned int  m_7, m_19, m_7_19;
	m_7 = m_19 = m_7_19 = 0;
	com_struct limits;
  int pid=0;

	limits.start = 0;
	limits.end = UINT_MAX/3;

while(1){
  fork();
  if (pid < 0) {
    printf("fork() error!\n");
    exit(-1);
  }
  if(pid==0){
	for (unsigned int  i = limits.start; i <  limits.end; i++){
		if(i%7 == 0){
			m_7 ++;
		}
		if(i%19 == 0){
			m_19 ++;
		}
		if((i%7 == 0)	&& (i%19 == 0)){
			m_7_19++;
		}
	}
  printf("m 7    %d\n", m_7);
	printf("m   19 %d\n", m_19);
	printf("m 7 19 %d\n\n", m_7_19);
  exit(0);
}
  if (pid > 0) {
    limits.start += UINT_MAX/3;
  	limits.end += UINT_MAX/3;
    if (limits.start > UINT_MAX-5) {
      exit(1);
    }
  }
}

	printf("m 7    %d\n", m_7);
	printf("m   19 %d\n", m_19);
	printf("m 7 19 %d\n", m_7_19);

}

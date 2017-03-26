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
	int i=0;

	limits.start = 0;
	limits.end = UINT_MAX/3;

while(i<3){

	fork(); //REPRODUCE

  if (pid < 0) {//Fork FAIL!
    printf("fork() error!\n");
    exit(-1);
  }

  if(pid==0){//CHILD
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
	printf("Child %d\n", i+1);
  printf("m 7    %d\n", m_7);
	printf("m   19 %d\n", m_19);
	printf("m 7 19 %d\n\n", m_7_19);
  exit(0);
}//END OF CHILD PROCESS

  if (pid > 0) {//PARENT
    limits.start += UINT_MAX/3;
  	limits.end += UINT_MAX/3;
    i++;
  }
}

	printf("Processes finished\n");
	//printf("m 7    %d\n", m_7);
	printf("m   19 %d\n", m_19);
	printf("m 7 19 %d\n", m_7_19);

	exit(6);
}

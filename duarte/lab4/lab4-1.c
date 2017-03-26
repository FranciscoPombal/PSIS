#include <limits.h>
#include <stdio.h>

typedef struct{
		unsigned start, end;
} com_struct;

int main(){
	unsigned int  m_7, m_19, m_7_19;
	m_7 = m_19 = m_7_19 = 0;
	com_struct limits;

	limits.start = 0;
	limits.end = UINT_MAX;
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
	printf("m 7 19 %d\n", m_7_19);

}

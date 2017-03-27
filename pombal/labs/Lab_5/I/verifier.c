#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i = 0;
    int m_7_19 = 0;
    int m_19 = 0;
    int m_7 = 0;

        do{
            i++;

            if((i % 7) == 0){
                m_7++;
            }
            if((i % 19) == 0){
                m_19++;
            }
            if(((i % 19) == 0) && ((i % 7) == 0)){
                m_7_19++;
            }
        }while(i < 999999);

        fprintf(stdout,"m 7\t%d\n", m_7);
        fprintf(stdout,"m 19\t%d\n", m_19);
        fprintf(stdout,"m 7 19\t%d\n", m_7_19);

    return 0;
}

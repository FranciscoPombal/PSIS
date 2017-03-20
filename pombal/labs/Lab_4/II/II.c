#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// TODO

typedef struct{
    unsigned int start, middle1, middle2, end;
} com_struct;

int main(void){
    unsigned int m_7, m_19, m_7_19;
    m_7 = m_19 = m_7_19 = 0;
    com_struct limits;

    int pid0 = 0;
    int pid1 = 0;
    int pid2 = 0;

    unsigned int i = 0;

        limits.start = 0;
        limits.middle1 = UINT_MAX / 3;
        limits.middle2 = UINT_MAX * (2 / 3);
        limits.end = UINT_MAX;

        pid0 = fork();
        if(pid0 < 0){
            fprintf(stderr,"Fork error.\n");
            exit(EXIT_FAILURE);
        }else if(pid0 == 0){
            // child 0
            for(i = limits.start; i < limits.middle1; i++){
                //etc
            }
        }else{
            // parent
            pid1 = fork();
            if(pid1 < 0){
                fprintf(stderr,"Fork error.\n");
                exit(EXIT_FAILURE);
            }else if(pid1 == 0){
                // child 1
            }else{
                // parent
                pid2 = fork();
                if(pid2 < 0){
                    fprintf(stderr,"Fork error.\n");
                    exit(EXIT_FAILURE);
                }else if(pid2 == 0){
                    // child 2
                }else{
                    //parent
                }
            }
        }




        for(i = limits.start; i < limits.end; i++){
            if(i % 7 == 0){
                m_7++;
            }
            if(i % 19 == 0){
                m_19++;
            }
            if((i % 7 == 0) && (i % 19 == 0)){
                m_7_19++;
            }
        }
        fprintf(stdout,"m 7    %d\n", m_7);
        fprintf(stdout,"m   19 %d\n", m_19);
        fprintf(stdout,"m 7 19 %d\n", m_7_19);

    return 0;
}

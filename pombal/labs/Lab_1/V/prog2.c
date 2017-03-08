#include "include/test.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(){

    int a;
    char* error;

    // this will will point to the shared object (library)
    void* handle;

    // these will point to the functions inside the shared object
    void (*f_1)();
    void (*f_2)();

        printf("What version of the functions you whant to use?\n");
        printf("\t1 - Normal    (test1)\n");
        printf("\t2 - Optimized (test2)\n");
        scanf("%d", &a);

        if(a == 1){
            /* load library test1 */
            handle = dlopen("lib/libtest1.so", RTLD_LAZY);

            // check for errors
            if(handle == NULL){
                fprintf(stderr, "%s\n", dlerror());
                exit(EXIT_FAILURE);
            }

            printf("running the normal versions from test1\n");
        }else{
            if(a == 2){
                /* load library test2 */
                handle = dlopen("lib/libtest2.so", RTLD_LAZY);

                // check for errors
                if(handle == NULL){
                    fprintf(stderr, "%s\n", dlerror());
                    exit(EXIT_FAILURE);
                }

                printf("running the optimized versions from test2\n");
            }else{
                printf("Not running anything\n");
                exit(EXIT_FAILURE);
            }
        }

        // obtain addresses of functions and check for errors
        f_1 = dlsym(handle, "func_1");
        if((error = dlerror()) != NULL){
            fprintf(stderr, "%s\n", error);
            exit(EXIT_FAILURE);
        }
        f_2 = dlsym(handle, "func_2");
        if((error = dlerror()) != NULL){
            fprintf(stderr, "%s\n", error);
            exit(EXIT_FAILURE);
        }
        /* call func_1 and func_2 from whichever library was loaded */
        (*f_1)();
        (*f_2)();

    // clean up and close
    dlclose(handle);
    exit(EXIT_SUCCESS);
}

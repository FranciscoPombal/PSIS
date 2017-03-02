/*
 * implement a program that creates a copy of argv with all its contents in uppercase.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char* argv[]) {

    int i = 0;
    int j = 0;
    int k = 0;
    char c = 'a';

    // This will hold argv's contents
    char** argv2;

        // argv and argv2 are arrays of strings (strings are arrays of chars); there are argc strings in argv in total.
        argv2 = (char**)malloc(argc * sizeof(char*));

        // Now we iterate over all the strings in argv
        // First we find the number of chars in each string and allocate the memory for each
        // Then we copy the contents from argv to argv2
        for(i = 0; i < argc; i++){
            c = 'a';
            for(j = 0; c != '\0' ; j++){
                c = argv[i][j];
                k++;
            }

            // allocation
            argv2[i] = (char*)malloc(k * sizeof(char));

            // assignment of values to new array
            c = 'a';
            for(j = 0; c != '\0' ; j++){
                c = argv[i][j];
                argv2[i][j] = toupper(argv[i][j]);
            }
        }

        // Print the result
        for(i = 0; i < argc; i++){
            fprintf(stdout, "%s\n", argv2[i]);
        }

        // Free allocated memory
        for(i = 0; i < argc; i++){
            free(argv2[i]);
        }
        free(argv2);

    return 0;
}

/*
 *Implement a program that concatenates all its arguments into a single string. Do not use any string manipulation function.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    int i = 0;
    int j = 0;
    int k = 0;
    char c = 'a';
    int str_len = 0;
    char* result_str;

        //Iterate over argv to find lenght of xoncatenated string
        // argv[0] is the program's name, so we start from 1 to print and concatenate all the arguments
        for(i = 1; i < argc; i++){
            c = 'a';
            for(j = 0; c != '\0' ; j++){
                c = argv[i][j];
                if(c != '\0'){
                    str_len++;
                }
            }
        }

        // Allocate the concatenated string; add 1 to length to account for NULL char
        result_str = (char*)malloc((str_len + 1) * sizeof(char));

        // assing value to new array
        for(i = 1; i < argc; i++){
            c = 'a';
            for(j = 0; c != '\0' ; j++){
                c = argv[i][j];
                if(c != '\0'){
                    result_str[k] = c;
                    k++;
                }
            }
        }

        // Print the string
        fprintf(stdout, "%s\n", result_str);

        // Free allocated memory
        free(result_str);

    return 0;
}

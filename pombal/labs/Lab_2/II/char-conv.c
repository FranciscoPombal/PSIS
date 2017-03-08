#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 100

int main(){
    char v1[BUFFER_SIZE];
    char *v2 = (char*)malloc(BUFFER_SIZE*sizeof(char));
    int i;

    printf("Write a word");
    fgets(v1, BUFFER_SIZE, stdin);

    for (i=0; v1[i]!='\0'; i++){
        v2[i] = (char)toupper(v1[i]);
    }

    printf("Converted string: %s", v2);

    free(v2);

    exit(EXIT_SUCCESS);
}

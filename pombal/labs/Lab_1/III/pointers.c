#include <stdlib.h>
#include <stdio.h>

int a;

int main(){
    int b;

    // just a small experiment to verify pointer-array equivalency
    char v[10] = "123456789\0";

    printf("%c\n", *v); //v[0] = 1
    printf("%c\n", *(v+1)); //v[1] = 2

    char *p = malloc(1);

    printf("&a - %p\n", &a);
    printf("&b - %p\n", &b);

    printf("v - %p\n", v);
    printf("&v - %p\n", &v);

    printf("&main - %p\n", main);
    printf("&exit %p\n", exit);

    printf("&p - %p\n", &p);
    printf("p - %p\n", p);

    exit(0);
}

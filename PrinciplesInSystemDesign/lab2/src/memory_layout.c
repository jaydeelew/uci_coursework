#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv, char **env) {
    int A;
    int *B = malloc(4);

    printf("Address of A: %p\nAddress of B: %p\n", &A, B);
    printf("Address of main: %p\n", &main);    
    printf("Address of main: %p\n", (void*)main);    
    printf("Address of the array of arguments: %p\n", (void *)argv);
    printf("Addresses of the arguments:\n\t");
    int i;
    for (i = 0; i < argc; i++) 
        printf("[%s]:%p ", argv[i], argv[i]);
     printf("\n");


  return 0;
}

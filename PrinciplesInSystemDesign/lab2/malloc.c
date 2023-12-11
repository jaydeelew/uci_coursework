#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// void foo(char *p){
    
// }

int main(int agrc, char **argv) {
    int arrayA[3] = {0,1,2}, arrayB[3] = {3,4,5};
    char a, b, c;
    int i, j, k, l;
    int *stuff = malloc(3*sizeof(int));    //declare space to hold 3 integers
    printf("\n");
    printf("address of a:         %p\t" "%" PRIuPTR "\n"
           "address of b:         %p\t" "%" PRIuPTR "\n"
           "address of c:         %p\t" "%" PRIuPTR "\n"
           "address of arrayA[0]: %p\t" "%" PRIuPTR "\n" 
           "address of arrayA[1]: %p\t" "%" PRIuPTR "\n" 
           "address of arrayA[2]: %p\t" "%" PRIuPTR "\n" 
           "address of i:         %p\t" "%" PRIuPTR "\n"
           "address of j:         %p\t" "%" PRIuPTR "\n"
           "address of k:         %p\t" "%" PRIuPTR "\n"
           "address of l:         %p\t" "%" PRIuPTR "\n"
           "address of arrayB[0]: %p\t" "%" PRIuPTR "\n" 
           "address of arrayB[1]: %p\t" "%" PRIuPTR "\n" 
           "address of arrayB[2]: %p\t" "%" PRIuPTR "\n" 
           "address of stuff[0]:  %p\t" "%" PRIuPTR "\n" 
           "address of stuff[1]:  %p\t" "%" PRIuPTR "\n" 
           "address of stuff[2]:  %p\t" "%" PRIuPTR "\n", 
            &a, (uintptr_t)&a,
            &b, (uintptr_t)&b,
            &c, (uintptr_t)&c,
            &arrayA[0], (uintptr_t)&arrayA[0], 
            &arrayA[1], (uintptr_t)&arrayA[1], 
            &arrayA[2], (uintptr_t)&arrayA[2], 
            &i, (uintptr_t)&i,
            &j, (uintptr_t)&j,
            &k, (uintptr_t)&k,
            &l, (uintptr_t)&l,
            &arrayB[0], (uintptr_t)&arrayB[0], 
            &arrayB[1], (uintptr_t)&arrayB[1], 
            &arrayB[2], (uintptr_t)&arrayB[2], 
            &stuff[0], (uintptr_t)&stuff[0],
            &stuff[1], (uintptr_t)&stuff[1],
            &stuff[2], (uintptr_t)&stuff[2]);
    
    
    for(i = 0; i < 3; i++)
        printf("stuff[%d]: %d\n", i, stuff[i]);
    printf("\n");

    // free(stuff); //Uncomment for Question 26 

    stuff[0] = 0xFFFFFFFF;
    stuff[1] = 0xCAFEBABE;
    stuff[2] = 0XDEADBEEF;

    for(i = 0; i < 3; i++)
        printf("stuff[%d]: %u\n", i, stuff[i]);
    free(stuff); //Comment out this line for Question 26. Then uncomment it for Question 27

    int *arrayC = malloc(5*sizeof(int));
    printf("\naddress of arrayC: %p\n", &arrayC[0]);
    *arrayC = 9;
    *(arrayC + 1) = 8;
    
    for(i = 0; i < 5; i++)
        printf("arrayC[%d]: %d\n", i, arrayC[i]);

    free(arrayC);
    return 0;
}

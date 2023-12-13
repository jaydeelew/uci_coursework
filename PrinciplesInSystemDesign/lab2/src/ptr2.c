#include<stdio.h>
#include<stdlib.h>

int main (void)
{
    int num = 0xdead;
    int a[5] = {16, 32, 48, 64, 80};
    int othernum = 0xbeef;
    char letter = 100;

    int* p = a;
    printf("a: %x\t&a: %x\t&a[0]: %x\n", a, &a, &a[0]);
    printf("p points to: %p\n", p);

    p = p + 5;
    printf("p points to: %p\n", p);
    printf("p dereferenced is: %x\n", *p);

    *p = 0xcafe;
    printf("p dereferenced is: %x\n", *p);

    p = *p + 3;
    printf("p points to: %p\n", p);

    p = a;
    printf("p points to: %p\n", p);
    printf("p dereferenced is: %x\n", *p);

    p = p + 3;
    printf("p points to: %p\n", p);
    printf("p dereferenced is: %x\n", *p);

    return 0;
}

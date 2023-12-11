#include<stdio.h>
#include<stdlib.h>

int main (void) 
{
    // Reference: http://www.asciitable.com/
    char abc[12] = {'a','b','c', 100, 101, 102, 103, 104, 0x69, 0x6A, 0x6B, 0x6C};
    char abc2[] = "aa";
    char abc3[5] = "B123";
    printf("abc: %p\n", abc);
    printf("&abc: %p\n", &abc);
    printf("abc[0]: %p\n", abc[0]); //simply prints hex value of contents of abc[o]
    printf("&abc[0]: %p\n\n", &abc[0]);
    printf("&abc[0] to &abc[11]:\n %p,\n %p,\n %p,\n %p,\n %p,\n %p,\n %p,\n %p,\n %p,\n %p,\n %p,\n %p\n", &abc[0], &abc[1], &abc[2], &abc[3], &abc[4], &abc[5], &abc[6], &abc[7], &abc[8], &abc[9], &abc[10], &abc[11]);
    printf(" %p, %s\n", abc2, abc2);
    printf(" %p, %s\n", abc3, abc3);
    printf("Answer Question 10 & 11 & 12\n\n");

    char *p = &abc[3];
    printf("*p: %c\n", *p);
    printf("Answer Question 13\n\n");

    char* strings[4] = {"abcdefg", "123", "!@#", "000"};
    printf("Size of string[0] in bytes: %ld\n", sizeof(strings[0]));
    printf("Size of string[1] in bytes: %ld\n", sizeof(strings[1]));
    printf("Size of string[2] in bytes: %ld\n", sizeof(strings[2]));
    printf("Size of string[3] in bytes: %ld\n", sizeof(strings[3]));
    printf("Answer Question 14 & 15\n\n");

    int nums[9] = {97, 98, 99, 100, 101, 102, 103, 104, 0};
    printf("Answer Question 16 & 17\n\n");

    return 0;
}

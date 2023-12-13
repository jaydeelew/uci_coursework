#include <stdio.h>
#include <stdlib.h>

int main()
{
	char i = 'A';
	char *p  = &i;
    printf("size of any pointer on 64 bit system: %ld\n", sizeof(void*));
    printf("current int value of *p, ascii of *p: %d, %c\n", *p, *p);

    i = (*p)++;
    printf("current int value of *p, ascii of *p: %d, %c\n", *p, *p);
    printf("current int value of *p, ascii of *p: %d, %c\n", *p, *p);

    i = (*p)+1;
    printf("current int value of *p, ascii of *p: %d, %c\n", *p, *p);

    i = ++(*p);
    printf("current int value of *p, ascii of *p: %d, %c\n", *p, *p);
    printf("current int value of i, ascii of i: %d, %c\n", i, i);

    return 0;
}

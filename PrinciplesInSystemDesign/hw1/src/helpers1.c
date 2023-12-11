#include "helpers1.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


void process_L(char* msg) {
    int c;
    char *m;
    m = msg;
    while (c != EOF) {
        c = tolower(getchar());
        while (((isalpha(*m)) == 0) && (*m != '\0')) { // loop until *m is in the alphabet
            m++;
            if (*m == '\0') {
                break;
            }
        }
        if (c == tolower(*m)) {
            putchar(c - 32);
            m++;
        }
        else {
            putchar(c);
        }
    }
    printf("\n");
}

void process_W(char* MSG) {
    printf("My message is: %s \n", MSG);
}

void process_S(int NUM) {
    printf("My number is: %d \n", NUM);
}

void process_O() {
    printf("This is processO\n");
}

void process_P() {
    printf("This is processP\n");
}

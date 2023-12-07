#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void printStars(int s) {
    for (int i = 0; i < s; i++)
        printf("*");
    printf("\n");
}

int main() {

    int arr[11] = {0};
    int roll_1, roll_2, total;
    srand(time(NULL));

    for (int i = 0; i < 100; i++) {
        roll_1 = rand() % 6 + 1;
        roll_2 = rand() % 6 + 1;
        total = roll_1 + roll_2;

        arr[total - 2]++;
    }

    for (int i = 0; i <= 10; i++) {
        printf("%d: %d\t", i + 2, arr[i]);
        printStars(arr[i]);
    }
   
   return 0;
}
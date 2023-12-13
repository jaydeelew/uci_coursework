// roll dice n times to see their outcomes distributed with astericks
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void printStars(int s) {
    for (int i = 0; i < s; i++)
        printf("*");
    printf("\n");
}

int main() {
    char str[128];
    printf("Enter the number times to roll the dice: ");
    fgets(str, 255, stdin);
    printf("\n");

    int arr[11] = {0};
    int roll_1, roll_2, total;
    srand(time(NULL));
    int rolls = atoi(str);

    for (int i = 0; i < rolls; i++) {
        roll_1 = rand() % 6 + 1;
        roll_2 = rand() % 6 + 1;
        total = roll_1 + roll_2;

        arr[total - 2]++;
    }

    for (int i = 0; i <= 10; i++) {
        printf("Rolled a %d: ", i + 2);
        printStars(arr[i]);
    }
   
   return 0;
}
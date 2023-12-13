// take any input, tokenize it via spaces, return the string with each token on a new line
#include <stdio.h>
#include <string.h>

int main() {

    char str1[255];
    char str2[255];
    char *token;
    printf("Type anything: ");
    fgets(str1, 255, stdin);
    printf("\n");

    int i = 0;
    while (str1[i] != '\0') {
        str2[i] = str1[i];
        i++;
    }

    token = strtok(str1, " \n");
   
    do {
        printf("%s\n", token);
    } while (token = strtok(NULL, " \n"));

    return 0;
}
#include <stdio.h>
#include <string.h>

int main() {
    char netid[] = "jdlewis1";
    int ClassNum = 53;
    int myLen = strlen(netid);

    printf("Hello, %s! Welcome to ICS%d!\n",netid, ClassNum);

    for(int i = myLen; i >= 0 ; i--){
        printf("%d\n",i );
    }
    return 0;
}

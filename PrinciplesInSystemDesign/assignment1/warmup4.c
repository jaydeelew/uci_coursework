// add two matrices together and print the result
#include <stdio.h>

void madd(int *a, int *b, int m, int n);
void printresult(int *a, int m, int n); 

int main () {

    int matrix1[][3] = {{1,2,3},{4,5,6},{7,8,9}};
    int matrix2[][3] = {{1,1,1},{1,1,1},{1,1,1}};

    madd((int*)matrix1, (int*)matrix2, 3, 3);
    printresult((int*)matrix1, 3, 3);

    return 0;
}

void madd(int *a, int *b, int m, int n) {
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
           *(a + i * n + j) = *(a + i * n + j) + *(b + i * n + j);
        }
    }
}

// Alternative madd:
// void madd(int *a, int *b, int m, int n) {
//     int len = m * n;
//     for (int i = 0; i < len; i++) {
//         *a = *a + *b;
//         a++;
//         b++;
//     }
// }

/*  Notes:
 *  (i * n)          row number x length of row
 *  (a + i * n)      array pointer 'a' incremented by (row number x length of row) 
 *  (a + i * n + j)  array pointer 'a' incremented by (row number x length of row) and then by column position 
 *  *(a + i * n + j) the above pointer position dereferenced
 */

void printresult(int *a, int m, int n) {
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", *(a + i * n + j));
        }
        printf("\n");
    }
}
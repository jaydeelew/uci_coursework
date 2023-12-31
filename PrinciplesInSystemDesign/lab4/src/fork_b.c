#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid;
  int x = 1000, i;

  pid = fork();
  if (pid == 0) {
    // child
    for (i = 0; i <= x; i++) {
      printf("child : %d\n", i);
    }
    exit(0);
  }

  // parent    
  for (i = x; i >= 0; i--) {
    printf("parent : %d\n", i);
  }
  exit(0);
}

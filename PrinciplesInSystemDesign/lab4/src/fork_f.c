#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  if (fork() == 0) {
    // child
    printf("Terminating child : PID=%d\n", getpid());
    exit(0);
  }

  // parent
  while (1) {
    printf("Running parent : PID=%d\n", getpid());
    sleep(10);
  }
}

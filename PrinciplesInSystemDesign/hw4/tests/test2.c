#include "icsmm.h"
#include "debug.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>

#define VALUE1_VALUE 320
#define VALUE2_VALUE 0xDEADBEEFF00D

void press_to_cont() {
    printf("Press Enter to Continue");
    while (getchar() != '\n')
      ;
    printf("\n");
}

void null_check(void* ptr, long size) {
    if (ptr == NULL) {
      error(
          "Failed to allocate %lu byte(s) for an integer using ics_malloc.\n",
          size);
      error("%s\n", "Aborting...");
      assert(false);
    } else {
      success("ics_malloc returned a non-null address: %p\n", (void *)(ptr));
    }
}

void payload_check(void* ptr) {
    if ((unsigned long)(ptr) % 16 != 0) {
      warn("Returned payload address is not divisble by a quadword. %p %% 16 "
           "= %lu\n",
           (void *)(ptr), (unsigned long)(ptr) % 16);
    }
}

int main(int argc, char *argv[]) {
  ics_mem_init();

  ics_mem_fini();

  return EXIT_SUCCESS;
}

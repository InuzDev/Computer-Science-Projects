#include "binary.h"
#include <stdio.h>
/**
 * Function: binary_switcher
 * Objective: switch an specific binary from off to on, or on to off, switching the original value
 */
void binary_switcher(int *n, int *b) {
   *n = *n | (1 << *b);
}

/**
 * Function: printBinary()
 * Objective:
 * Arguments: (int) n
 */
void printBinary(int n) {
   for (int index = 7; index >= 0; index--) {
      printf("%d", (n >> index) & 1);
   }
   printf("\n");
}

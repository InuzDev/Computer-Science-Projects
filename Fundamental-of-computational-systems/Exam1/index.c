/*
 * Carlos David - Code for Midterm exam in Computational Systems fundamentals.
 */

#include <stdio.h>

void regresive_count(unsigned char counter);

int main() {
   regresive_count(4);
   return 0;
}

void regresive_count(unsigned char counter) {
   // Iterate until zero inclusive
   while (counter != 0) {
      // Print counter value
      printf("Counter - %u\n", counter);

      // Decrement counter
      counter -= 1;
   }
   printf("Counter - 0\n");
}

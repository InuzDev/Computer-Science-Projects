#include <stdio.h>

/*
 * Function: swap
 * Objective: Change the value of A to B. E.g: A = 3, B = 2, with this function,
 * A = 2 and B = 3
 * Arguments: (float)(pointers) a, b
 */
void swap(float *a, float *b) {
   if (*a == *b) {
      printf("Same value ; AB: %.4f", *a);
   } else {
      int StoreA = *a;
      int StoreB = *b;

      *b = StoreA;
      *a = StoreB;
      printf("a: %.4f\nb: %.4f", *a, *b);
   }
}

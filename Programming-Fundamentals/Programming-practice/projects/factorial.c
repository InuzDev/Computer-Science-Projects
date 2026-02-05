/*
 * Charles David ; Write a progrsam that write the factorial of n in the values from 1 to 10.
 *
 * factorial.c
 */
#include <stdio.h>

#define S 10

int main() {
   long int n, m, fact;

   for (n = 1; n <= S; n++) {
      fact = 1;
      for (m = n; m > 1; m--) {
         fact *= m;
      }
      printf("\t %ld! = %ld\n", n, fact);
   }
   return 0;
}

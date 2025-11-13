/**
 *
 * Data types and expressions.
 * This program shows the basic types. The objective is to improve my learning on how the computer interpretate and compile the data.
 *
 */

#include <stdio.h>

int main()
{
   int a = 8, b = 3, c = 12, d = 1, e = 5;
   float x = 3.5, y = 2.1, z = 5;
   char m = 'C', n = '2';

   float result;

   result = z / b + 1; // 5.00 / 3.00 + 1 -> 1.66 + 1 = 2.66
   printf("%.2f", result);

   return 0;
}
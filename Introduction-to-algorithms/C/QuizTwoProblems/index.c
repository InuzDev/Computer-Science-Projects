#include <stdio.h>

int main()
{
   // Going to check the data and how computers understand this value.
   int a = 3, b = 2, c = 4, d = 1;
   float x = 0.5, y = 1.2, z = 2.25;

   int result = a % b++ + (int)(c / x) % b;
   printf("Result of the exercise number 1: %d", result);
   printf("\n"); // Break line

   float resultB = !(x = a * ((a - b) % c) > -x * z * y) ? a / d / (int)(x + 1) : y / x;
   printf("Result of exercise number 2: %f", resultB);

   return 0;
}
/*

Los números de Pitágoras pueden ser descritos de la siguiente
manera: a2 + b2 = c2, donde a, b y c son enteros y b y c son
consecutivos. a y b son catetos y c es la hipotenusa. Escriba
un programa para encontrar cinco ternas de números de
Pitágoras. Por ejemplo:
32 + 42 = 52 , 52 + 122 = 132

*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
   int count = 0;
   int a, b, c;

   // Search for the first 5 ternas.
   for (a = 1; count < 5; a++)
   {
      // Verify that (a^2 - 1) is even
      if ((a * a - 1) % 2 == 0)
      {
         b = (a * a - 1) / 2;
         c = b + 1;

         if (a * a + b * b == c * c)
         {
            printf("%d^2 + %d^2 = %d^2\n", a, b, c);
            count++;
         }
      }
   }
   return 0;
}
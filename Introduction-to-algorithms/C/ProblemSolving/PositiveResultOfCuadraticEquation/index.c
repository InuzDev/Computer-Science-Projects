/*
Haga una función float eqcuad(a,b,c) la cual retorne la
solución positiva de una ecuación cuadrática cuyos parámetros
con a,b y c respectivamente.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float eqcuad(float a, float b, float c);
// To solve this problem, we going to approach the general formula solution.
int main()
{
   float a, b, c, result = 0.00f;

   printf("Enter the coefficients a, b, and c: ");
   scanf("%f %f %f", &a, &b, &c);

   // result += eqcuad(a, b, c);
   printf("%.2f", eqcuad(a, b, c));

   // printf("Resultado de la ecuaci%cn: %.2f\n", 162, result);
   return 0;
}

float eqcuad(float a, float b, float c)
{
   float discriminant, Solution1, Solution2;
   float PositiveResult = 0.00f;
   discriminant = b * b - 4 * a * c; // We check if we getting an imaginary number.

   // If we get a positive number, we just continue the quadratic formula
   if (discriminant > 0)
   {
      Solution1 = (-b - sqrt(discriminant)) / (2 * a);
      Solution2 = (-b + sqrt(discriminant)) / (2 * a);

      if (Solution1 > 0 && Solution2 > 0)
      {
         PositiveResult = fmax(Solution1, Solution2);
      }
      else if (Solution1 > 0)
      {
         PositiveResult = Solution1;
      }
      else if (Solution2 > 0)
      {
         PositiveResult = Solution2;
      }
   }
   // If the root is 0.
   else if (discriminant == 0)
   {
      Solution1 = -b / (2 * a);
      if (Solution1 > 0)
      {
         PositiveResult = Solution1;
      }
   }
   // If the discriminant is negative, we need to work with complex numbers
   else
   {
      return 0;
   }

   return PositiveResult;
}
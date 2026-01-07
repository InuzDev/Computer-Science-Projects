/*

Todo número entero puede ser factorizado en potencias de
factores primos. Escriba un programa que pida números
enteros hasta digitar 0. Para cada número, escriba por pantalla
la descomposición de la siguiente manera.
Digite un número: 60
60 =
2^2*
3*
5
Digite un número: 0

*/

#include <stdio.h>
#include <stdlib.h>

void Factorization(int num);

int main()
{
   int Num;
   do
   {
      printf("ingrese un n%cmero entero: ", 163);
      scanf("%d", &Num);

      if (Num > 0)
      {
         printf("%d = \n", Num);
         Factorization(Num);
      }
   } while (Num != 0);

   printf("Program termination. . .\n");
   return 0;
}

void Factorization(int num)
{
   int Factor = 2;
   while (num > 1)
   {
      int count = 0;
      while (num % Factor == 0)
      {
         count++;
         num /= Factor;
      }

      if (count > 0)
      {
         printf("%d^%d\n", Factor, count);
      }
      Factor++;
   }
   printf("\n");
}
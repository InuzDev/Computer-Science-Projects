/*

Números dorsales son aquellos que como 1233 son iguales a la
suma de los cuadrados de las unidades de centenas: 1233 =
12*12 + 33*33. Realice la función int dorsal(int n) que
retorne 1 si n es dorsal y 0 si no lo es.

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int dorsal(int num);

int main()
{
   int num;
   printf("Introduzca un valor entero: ");
   scanf("%d", &num);

   if (dorsal(num))
   {
      printf("%d es dorsal\n", num);
   }
   else
   {
      printf("%d no es dorsal\n", num);
   }
   return 0;
}

int dorsal(int num)
{
   int Units = num % 100;
   int Centenas = num / 100;
   int Result = (Centenas * Centenas) + (Units * Units);

   if (Result == num)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}
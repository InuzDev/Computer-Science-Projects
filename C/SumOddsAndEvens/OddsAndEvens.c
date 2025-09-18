#include <stdio.h>

int main()
{

   //    Hallar la suma de los números pares e impares comprendidos en un rango de números
   // 	Ejemplo
   // 	Rango 1, 15
   // 	Suma de los pares: (2+4+6+8+10+12+14) = 56
   // 	Suma de los impares: (1,3,5,7,9,11,13,15) = 64
   int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
   int arrlength = sizeof(arr) / sizeof(arr[0]);

   int SumEven = 0; // This store the result of the sum of even numbers.
   int SumOdd = 0;  // This store the result of the sum of odd numbers.
   for (int i = 0; i < arrlength; i++)
   {
      if (arr[i] % 2 == 0) // Check if the divisor equal 0.
      {
         SumEven += arr[i];
      }
      else
      {
         SumOdd += arr[i];
      }
   }
   printf("Resultado de la suma de los pares: %d", SumEven);
   printf("\nResultado de la suma de los impares: %d", SumOdd);

   return 0;
}
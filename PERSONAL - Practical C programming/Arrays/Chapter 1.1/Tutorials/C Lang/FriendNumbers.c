#include <stdio.h>

int sumdiv(int);

int main() {
   int num1 = 0, num2 = 0, sumd1 = 0, sumd2 = 0;
   printf("Digite dos valores: (A, B)\n");

   while (num1 <= 0) {
      printf("Digite el primer valor: ");
      scanf("%d", &num1);

      if (num1 <= 0) {
         printf("Digite un numero mayor a 0.\n\n");
      }
   }

   while (num2 <= 0) {
      printf("Digite el segundo valor: ");
      scanf("%d", &num2);

      if (num2 <= 0) {
         printf("Digite un valor, mayor a cero.\n\n");
      }
   }

   sumd1 = sumdiv(num1);
   sumd2 = sumdiv(num2);

   if (sumd1 == num2 && sumd2 == num1) {
      printf("Son amigos");
   } else {
      printf("No son amigos");
   }

   return 0;
}

int sumdiv(int num) {
   int suma, divisor;

   suma = 0;
   divisor = 1;

   for (; divisor < num; divisor++) {
      if (num % divisor == 0) {
         suma += divisor;
      }
   }

   return suma;
}

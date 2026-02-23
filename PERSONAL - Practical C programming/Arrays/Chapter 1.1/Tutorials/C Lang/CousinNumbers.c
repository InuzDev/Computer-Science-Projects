#include <stdio.h>

int EvaluarPrimero(int);

int main() {
   int LimInf = 0, limsup = 0;
   int SumPrim = 0, par = 0, impar = 0, Esprimo = 0;

   printf("Digite el rango: [Limite inferior, limite superior]\n");
   while (LimInf <= 0 || limsup <= 0 || LimInf == limsup || LimInf > limsup) {
      printf("Limite inferior: ");
      scanf("%d", &LimInf);

      printf("Limite superior: ");
      scanf("%d", &limsup);

      if (LimInf <= 0 || limsup <= 0 || LimInf == limsup || LimInf > limsup) {
         printf("Rango invalido.\n\n");
         if (LimInf <= 0 || limsup <= 0) {
            printf("Debe ser mayor a 0.\n\n");
         } else if (LimInf == limsup || LimInf > limsup) {
            printf("Los limites no pueden ser iguales, ni el limite inferior mayor al superior\n\n");
         }
      }
   }

   for (int index = LimInf; index <= limsup; index++) {
      if (EvaluarPrimero(index)) {
         SumPrim++;
      }
      if (index % 2 == 0) {
         par++;
      } else {
         impar++;
      }
   }
   printf("Total de primos: %d\nTotal de pares: %d\nTotal de impares: %d\n", SumPrim, par, impar);
   return 0;
}

int EvaluarPrimero(int num) {
   int Contador = 2;

   for (; Contador < num; Contador++) {
      if (num % Contador == 0) {
         return 0;
      } else {
         return 1;
      }
   }

   return 0;
}

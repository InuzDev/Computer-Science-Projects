// Follow this example to create and solve other problems.

#include <stdio.h>

int sumfact(int); // Definir el prototipo para evitar problemas

int main()
{
   // Inicializar las variables de valores enteros de los limites, el valor y la suma de los progresistas
   int liminf, limpsup, val, sumfacts, _sumfacts, sumprogresistas = 0;

   do
   {
      printf("Ingrese el limite inferior: ");
      scanf("%d", &liminf);

   } while (liminf <= 0);

   do
   {
      printf("Ingrese el limite superior: ");
      scanf("%d", &limpsup);
   } while (limpsup <= 0);

   for (val = liminf; val <= limpsup; val++)
   {
      sumfacts = sumfact(val);

      if (sumfacts < val)
         _sumfacts = sumfact(sumfacts);
      if (sumfact(sumfacts) > sumfacts)
         printf("%d (%d) [%d]\n", val, sumfacts, _sumfacts);
   }

   return 0;
}

int sumfact(int num)
{
   int div, totdiv = 0, topediv = num / 2;

   for (div = 1; div <= topediv; div++)
      if (num % div == 0)
         totdiv += div;

   return totdiv;
}
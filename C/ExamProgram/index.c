#include <stdio.h>
#include <time.h>
#include <math.h>

// Calculate how much time does the program takes to run

int numprim(int num);
int cantnumprim(int num);
int sumanumprim(int num);

int main()
{
   clock_t start = clock();
   int liminf, limsup, val, suma_num_prim, cant_num_prim, cant_num_pari, suma_num_pari;
   float prom;

   do
   {
      printf("Ingrese limite inferior: ");
      scanf("%d", &liminf);
      if (liminf <= 0)
      {
         printf("Se ha definido el limite inferior automaticamente a 1.\n");
         liminf = 1;
      }
   } while (liminf == NULL);

   do
   {
      printf("Ingrese el limite superior: ");
      scanf("%d", &limsup);
      if (limsup <= 0 || limsup == liminf)
         printf("Limite inferior es 1, por favor un numero mayor a limite inferior (1).\n");
   } while (limsup == NULL);

   val = liminf;
   cant_num_pari = 0;
   suma_num_pari = 0;

   for (; val <= limsup; val++)
   {
      cant_num_prim = cantnumprim(val);
      suma_num_prim = sumanumprim(val);

      if (cant_num_prim % 2 == 0 && suma_num_prim % 2 == 0)
      {
         cant_num_pari++;
         suma_num_pari += val;
      }
      printf("\n%d", val);
   }

   prom = (float)suma_num_pari / cant_num_pari;
   printf("\nEl promedio de los numeros con paridad prima en el rango [%d,%d] es: %3.f", liminf, limsup, prom);

   clock_t end = clock();
   printf("\n\nTime used to run the program: %3.f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
   return 0;
}

int sumanumprim(int num)
{
   int div = 1;
   int suma_num_prim = 0;
   for (; div >= num / 2; div++)
   {
      if (num % div == 0)
      {
         int num_prim = numprim(div);
         if (num_prim == 1)
            suma_num_prim += div;
      }
   }
   return suma_num_prim;
}

int cantnumprim(int num)
{
   int div = 1;
   int cant_num_prim = 0;

   for (; div <= num / 2; div++)
   {
      if (num % div == 0)
      {
         int num_prim = numprim(div);
         if (num_prim == 1)
            cant_num_prim++;
      }
   }
   return cant_num_prim;
}

int numprim(int num)
{
   if (num < 2)
      return 0;
   for (int div = 2; div <= sqrt(num); div++)
   {
      if (num % div == 0)
         return 0;
   }
   return 1;
}
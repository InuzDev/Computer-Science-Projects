#include <stdio.h>

// Función que calcula la suma de los factores propios de un número
int sumaFactores(int n)
{
   int suma = 0;
   for (int i = 1; i < n; i++)
   {
      if (n % i == 0)
      {
         suma += i;
      }
   }
   return suma;
}

// Función que determina si un número es abundante
int esAbundante(int num)
{
   int suma = sumaFactores(num);
   return (suma > num);
}

// Función principal
int main()
{
   int inicio, fin;
   int sumaExitosos = 0, contadorExitosos = 0;

   printf("Ingrese el limite inferior del rango: ");
   scanf("%d", &inicio);
   printf("Ingrese el limite superior del rango: ");
   scanf("%d", &fin);

   printf("\nNumeros exitosos encontrados:\n");

   for (int n = inicio; n <= fin; n++)
   {
      int suma = sumaFactores(n);

      // Determinar tipo de número
      if (suma > n)
      { // Es abundante
         int factoresAbundantes = 0;

         // Buscar factores de n y contar cuántos son abundantes
         for (int i = 1; i < n; i++)
         {
            if (n % i == 0 && esAbundante(i))
            {
               factoresAbundantes++;
            }
         }

         // Verificar si el número es exitoso
         if (factoresAbundantes >= 3 && factoresAbundantes <= 8)
         {
            printf("%d (factores abundantes: %d)\n", n, factoresAbundantes);
            sumaExitosos += n;
            contadorExitosos++;
         }
      }
   }

   if (contadorExitosos > 0)
   {
      float promedio = (float)sumaExitosos / contadorExitosos;
      printf("\nPromedio de numeros exitosos: %.2f\n", promedio);
   }
   else
   {
      printf("\nNo se encontraron numeros exitosos en el rango.\n");
   }

   // Getchar is used to avoid inmediate shutdown of the program in the terminal
   getchar();
   return 0;
}

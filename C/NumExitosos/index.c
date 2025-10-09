#include <stdio.h>
// Esta libreria es para llamar system("pause");, no es muy util
// en code:blocks debido a que este lo detiene por defecto, pero si se corre el .exe normal,
// notaremos que se cierra automaticamente termina el programa.
#include <unistd.h>

// Función que calcula la suma de los factores propios de un número
int SumDiv(int num)
{
   int suma = 0;
   for (int i = 1; i < num; i++)
   {
      if (num % i == 0)
      {
         suma += i;
      }
   }
   return suma;
}

// Función que determina si un número es abundante
int esAbundante(int num)
{
   int suma = SumDiv(num);
   return (suma > num);
}

// Función principal
int main()
{
   // Datos insertados por el usuario
   int LimInf, LimSup;
   // Variables del sistema.
   int sumaExitosos = 0, contadorExitosos = 0;

   printf("Ingrese el limite inferior del rango: ");
   scanf("%d", &LimInf);
   printf("Ingrese el limite superior del rango: ");
   scanf("%d", &LimSup);
   while (LimInf <= 0 || LimSup <= 0 || LimSup <= LimInf)
   {
      if (LimInf <= 0 || LimSup <= 0 || LimSup <= LimInf)
      {
         printf("Limites invalidos, favor de insertar limites validos\n");
      }
      printf("Ingrese el limite inferior del rango: ");
      scanf("%d", &LimInf);
      printf("Ingrese el limite superior del rango: ");
      scanf("%d", &LimSup);
   }

   printf("\nNumeros exitosos encontrados:\n");

   for (int num = LimInf; num <= LimSup; num++)
   {
      int suma = SumDiv(num);

      // Determinar tipo de número
      if (suma > num)
      { // Es abundante
         int FactAbund = 0;

         // Buscar factores de n y contar cuántos son abundantes
         for (int i = 1; i < num; i++)
         {
            if (num % i == 0 && esAbundante(i))
            {
               FactAbund++;
            }
         }

         // Verificar si el número es exitoso
         if (FactAbund >= 3 && FactAbund <= 8)
         {
            printf("%d (Factores abundantes: %d)\n", num, FactAbund);
            sumaExitosos += num;
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

   system("pause");
   return 0;
}
#include <stdio.h>

#define INVALID 0
#define STARTLIMIT 1

int SetArrSize(int LimInf, int LimSup);
void CleanBuffer();

int main()
{
   int LimSup = 0, arrSize;

   while (LimSup == INVALID)
   {
      printf("Ingrese el limite superior del rango: ");
      scanf("%d", &LimSup);
      if (LimSup <= INVALID || LimSup <= STARTLIMIT)
      {
         printf("Limite invalido, favor de ingresar un limite mayor al limite inferior [%d]\n", STARTLIMIT);
         LimSup = 0;
      }
   }

   CleanBuffer();

   arrSize = SetArrSize(STARTLIMIT, LimSup);
   int Storage[arrSize];

   for (int index = 0; index < arrSize; index++)
   {
      // Storage[index] = (index + LimInf);
      printf("Ingrese un valor para el indice [%d] almacenar en el arreglo: ", index);
      scanf("%d", &Storage[index]);
   }

   return 0;
}

/**
 * Funcion: SetArrSize
 * Argumentos: LimInf, LimSup
 * Objetivo: Establecer el tamaño de una matriz
 * Retorna: ArrSize, que viene siendo el tamaño del arreglo.
 */
int SetArrSize(int LimInf, int LimSup)
{
   int ArrSize = LimSup - LimInf + 1;
   return ArrSize;
}

/*
Funcion: CleanBuffer();
Objetivo: Limpiar la memoria temporal, para evitar bugs con scanf(); cuando se presiona enter
*/
void CleanBuffer()
{
   while (getchar() != '\n')
      ;
}

#include <stdio.h>

#define INVALID 0

void CleanBuffer();

int main()
{
   int LimInf = 0, LimSup = 0;

   while (LimInf == INVALID)
   {
      printf("Ingrese el limite inferior del rango: ");
      scanf("%d", &LimInf);
      if (LimInf <= INVALID)
      {
         printf("Limite invalido, favor de ingresar un limite mayor a 0\n");
         LimInf = 0;
      }
   }

   CleanBuffer();

   while (LimSup == INVALID)
   {
      printf("Ingrese el limite superior del rango: ");
      scanf("%d", &LimSup);
      if (LimSup <= INVALID || LimSup <= LimInf)
      {
         printf("Limite invalido, favor de ingresar un limite mayor al limite inferior [%d]\n", LimInf);
         LimSup = 0;
      }
   }

   return 0;
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

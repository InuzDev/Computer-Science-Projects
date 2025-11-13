#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAXFIL 20
#define MAXCOL 20
#define VAL_INI 3
#define VAL_END 3225

#define CANTDIG(X) (int)log10(X) + 1

int randrange(int, int);
int sumMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);
int maxMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);
int traceMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);
void getMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);
void genMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol], int, int);
void showMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);

int main()
{
   int cantfil, cantcol;

   do
   {
      printf("Cantidad de filas: ");
      scanf("%d", &cantfil);

      if (cantfil <= 0 || cantfil > MAXFIL)
      {
         printf("Cantidad de filas debe estar entre [1, %d]\n", MAXFIL);
      }
   } while (cantfil <= 0 || cantfil > MAXFIL);

   do
   {
      printf("Cantidad de columnas: ");
      scanf("%d", &cantcol);

      if (cantcol <= 0 || cantcol > MAXCOL)
      {
         printf("Cantidad de columnas debe estar entre [1, %d]\n", MAXCOL);
      }
   } while (cantcol <= 0 || cantcol > MAXCOL);

   int matrix[cantfil][cantcol];

   // getMatrix(cantfil, cantcol, matrix);
   genMatrix(cantfil, cantcol, matrix, VAL_INI, VAL_END);
   system("cls");
   printf("Valores capturados: \n");
   showMatrix(cantfil, cantcol, matrix);

   int total = sumMatrix(cantfil, cantcol, matrix);
   int max = maxMatrix(cantfil, cantcol, matrix);
   int trace = traceMatrix(cantfil, cantcol, matrix);
   int cantDig = CANTDIG(total);

   printf("Total: %*d\n", cantDig, total);
   printf("M%cximo: %d\n", 160, cantDig, max);
   if (cantfil != cantcol)
   {
      printf("No hay traza");
   }
   else
   {
      printf("Traza: %*d\n", cantDig, trace);
   }

   return 0;
}

int randrange(int liminf, int limsup)
{
   return rand() % (limsup - liminf + 1) + liminf;
}

int sumMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol])
{
   int total = 0;

   for (int indexFil = 0; indexFil < cantfil; indexFil++)
   {
      for (int indexCol = 0; indexCol < cantcol; indexCol++)
      {
         total += matrix[indexFil][indexCol];
      }
   }
}
int maxMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol])
{
   int maxVal = matrix[0][0];

   for (int indexFil = 0; indexFil < cantfil; indexFil++)
   {
      for (int indexCol = 0; indexCol < cantcol; indexCol++)
      {
         if (maxVal < matrix[indexFil][indexCol])
         {
            maxVal = matrix[indexFil][indexCol];
         }
      }
   }
}
int traceMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol])
{
   if (cantfil != cantcol)
   {
      return -1;
   }

   int valTrace = 0;

   for (int index = 0; index < cantfil; index++)
   {
      valTrace += matrix[index][index];
   }

   return valTrace;
}
void getMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol])
{
   for (int indexFil = 0; indexFil < cantfil; indexFil++)
   {
      for (int indexCol = 0; indexCol < cantcol; indexCol++)
      {
         printf("Val[%d][%d]: ", indexFil + 1, indexCol + 1);
         scanf("%d", &matrix[indexFil][indexCol]);
      }
   }
}
void genMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol], int, int)
{
   for (int indexFil = 0; indexFil < cantfil; indexFil++)
   {
      for (int indexCol = 0; indexCol < cantcol; indexCol++)
      {
         matrix[indexFil][indexCol] = randrange(VAL_INI, VAL_END);
      }
   }
}
void showMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol])
{
   int cantDig = (int)log10(maxMatrix(cantfil, cantcol, matrix)) + 1;
   for (int indexFil = 0; indexFil < cantfil; indexFil++)
   {
      for (int indexCol = 0; indexCol < cantcol; indexCol++)
      {
         printf("%*d ", cantDig, matrix[indexFil][indexCol]);
      }
      printf("\n");
   }
}
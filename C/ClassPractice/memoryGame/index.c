#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DIM 6

#define VAL_INI 3
#define VAL_END 3225

#define CANTDIG(X) (int)log10(X) + 1

int randrange(int, int);
int maxMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);
int traceMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);
void genMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol], int, int);
void showMatrix(int cantfil, int cantcol, int matrix[cantfil][cantcol]);

int main()
{
   int figures[] = {1, 2, 3, 4, 5, 6, 15, 16, 17, 18, 127, 143, 224, 225, 231, 232, 233, 234, 235, 236, 237, 238, 0};

   printf("%c", figures);
   return 0;

   srand(time(NULL));

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
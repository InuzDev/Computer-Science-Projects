#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAXFIL 20
#define MAXCOL 20

#define VALINI 25
#define VALFIN 3245
#define CANTDIG(X) (int)log10(X) + 1

int summat(int cantfil, int cantcol, int mat[cantfil][cantcol]);
int maxmat(int cantfil, int cantcol, int mat[cantfil][cantcol]);
int trazamat(int cantfil, int cantcol, int mat[cantfil][cantcol]);
void getmat(int cantfil, int cantcol, int mat[cantfil][cantcol]);
void genmat(int cantfil, int cantcol, int mat[cantfil][cantcol], int valini, int valfin);
void showmat(int cantfil, int cantcol, int mat[cantfil][cantcol]);
int randrange(int, int);

int main()
{
   int cantfil, cantcol;

   do
   {
      printf("Cantidad de filas: ");
      scanf("%d", &cantfil);

      if (cantfil <= 0 || cantfil > MAXFIL)
         printf("Cantidad de fila debe estar entre [1,%d]", MAXFIL);
   } while (cantfil <= 0 || cantfil > MAXFIL);

   do
   {
      printf("Cantidad de columnas: ");
      scanf("%d", &cantcol);

      if (cantcol <= 0 || cantcol > MAXCOL)
         printf("Cantidad de columnas debe estar entre [1,%d]", MAXCOL);
   } while (cantcol <= 0 || cantcol > MAXCOL);

   srand(time(NULL));

   int mat[cantfil][cantcol];

   // getmat(cantfil,cantcol,mat);
   genmat(cantfil, cantcol, mat, VALINI, VALFIN);
   system("cls");
   printf("Valores capturados:\n");
   showmat(cantfil, cantcol, mat);
   int total = summat(cantfil, cantcol, mat);
   int maximo = maxmat(cantfil, cantcol, mat);
   int traza = trazamat(cantfil, cantcol, mat);
   int cantdig = CANTDIG(total);
   printf("Total : %*d\n", cantdig, total);
   printf("M%cximo: %*d\n", 160, cantdig, maximo);
   if (cantfil != cantcol)
      printf("Traza : %*s\n", cantdig, "N/A");
   else
      printf("Traza : %*d\n", cantdig, traza);

   return 0;
}

int summat(int cantfil, int cantcol, int mat[cantfil][cantcol])
{
   int total = 0;

   for (int indfil = 0; indfil < cantfil; indfil++)
      for (int indcol = 0; indcol < cantcol; indcol++)
         total += mat[indfil][indcol];

   return total;
}
int maxmat(int cantfil, int cantcol, int mat[cantfil][cantcol])
{
   int maxval = mat[0][0];

   for (int indfil = 0; indfil < cantfil; indfil++)
      for (int indcol = 0; indcol < cantcol; indcol++)
         if (maxval < mat[indfil][indcol])
            maxval = mat[indfil][indcol];
   return maxval;
}

int trazamat(int cantfil, int cantcol, int mat[cantfil][cantcol])
{
   int valtraza = 0;

   for (int ind = 0; ind < cantfil; ind++)
      valtraza += mat[ind][ind];

   return valtraza;
}

void getmat(int cantfil, int cantcol, int mat[cantfil][cantcol])
{
   for (int indfil = 0; indfil < cantfil; indfil++)
      for (int indcol = 0; indcol < cantcol; indcol++)
      {
         printf("Val[%d][%d]: ", indfil + 1, indcol + 1);
         scanf("%d", &mat[indfil][indcol]);
      }
   return;
}

void genmat(int cantfil, int cantcol, int mat[cantfil][cantcol], int valini, int valfin)
{
   for (int indfil = 0; indfil < cantfil; indfil++)
      for (int indcol = 0; indcol < cantcol; indcol++)
         mat[indfil][indcol] = randrange(valini, valfin);
   return;
}

void showmat(int cantfil, int cantcol, int mat[cantfil][cantcol])
{
   int cantdig = (int)log10(maxmat(cantfil, cantcol, mat)) + 1;
   for (int indfil = 0; indfil < cantfil; indfil++)
   {
      for (int indcol = 0; indcol < cantcol; indcol++)
         printf("%*d ", cantdig, mat[indfil][indcol]);
      printf("\n");
   }
   return;
}

int randrange(int liminf, int limsup)
{
   return rand() % (limsup - liminf + 1) + liminf;
}

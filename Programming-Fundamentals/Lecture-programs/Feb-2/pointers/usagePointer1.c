/*
 * Charles David ; Pointers exercise
 *
 * Here we study their usage.
 *
 * main.c
 */

#include <stdio.h>

#define MAXFIL 3
#define MAXCOL 3

int maxval(int fil, int col, int mat[fil][col], int *posfil, int *poscol);
void showmat(int fil, int col, int mat[fil][col]);

int main() {
   printf("Pointer usage\n");

   int mat[MAXFIL][MAXCOL] = {{18, 80, 30}, {21, 15, 7}, {64, 72, 5}};

   showmat(MAXFIL, MAXCOL, mat);
   int indfilmayor, indcolmayor, mayor = maxval(MAXFIL, MAXCOL, mat, &indfilmayor, &indcolmayor);

   printf("El mayor es %d, y esta en la posici%cn: (%d, %d)\n", mayor, 161, indfilmayor, indcolmayor);

   return 0;
}

int maxval(int fil, int col, int mat[fil][col], int *posfil, int *poscol) {
   int valmax = mat[0][0];
   *posfil = *poscol = 0;

   for (int indfil = 0; indfil < fil; indfil++) {
      for (int indcol = 0; indcol < col; indcol++) {
         if (valmax < mat[indfil][indcol]) {
            valmax = mat[indfil][indcol];
            *posfil = indfil;
            *poscol = indcol;
         }
      }
   }
   return valmax;
}

void showmat(int fil, int col, int mat[fil][col]) {
   for (int indfil = 0; indfil < fil; indfil++) {
      for (int indcol = 0; indcol < col; indcol++) {
         printf("%d ", mat[indfil][indcol]);
      }
      printf("\n");
   }
}

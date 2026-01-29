#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Exercise one:
void llenartablero(int dim, int tablero[dim * dim]) {
   // initialize the cells to 0 or space
   for (int index = 0; index < dim * dim; index++) {
      tablero[index] = ' ';
   }

   // seed the randoms
   srand(time(NULL));

   int pos1 = rand() % (dim * dim);
   int pos2;

   do {
      pos2 = rand() % (dim * dim);
   } while (pos2 == pos1);

   tablero[pos1] = 'A' + (rand() % 26);
   tablero[pos2] = 'A' + (rand() % 26);
}

// DEBUGGING function
void printTablero(int dim, int tablero[dim * dim]) {
   for (int index = 0; index < dim; index++) {
      for (int jindex = 0; jindex < dim; jindex++) {
         printf("%c ", tablero[index * dim + jindex]);
      }
      printf("\n");
   }
}

// Exercise two:
// - Prototype
void completar_tablero(int dim, char tablero[dim][dim]);
// - Function
void completar_tablero(int dim, char tablero[dim][dim]) {
   for (int index = 0; index < dim; index++) {
      for (int jindex = 0; jindex < dim; jindex++) {
         if (tablero[index][jindex] == ' ') {
            int contador = 0;

            // Check all 8 neighbors
            for (int dx = -1; dx <= 1; dx++) {
               for (int dy = -1; dy <= 1; dy++) {

                  if (dx == 0 && dy == 0) {
                     continue;
                  }

                  int ni = index + dx;
                  int nj = jindex + dy;

                  if (ni >= 0 && ni < dim && nj >= 0 && nj < dim) {
                     if (tablero[ni][nj] >= 'A' && tablero[ni][nj] <= 'Z') {
                        contador++;
                     }
                  }
               }
            }
         }
      }
   }
}

int main() {
   int dim = 5;
   int tablero[dim * dim];
   llenartablero(dim, tablero);
   printTablero(dim, tablero);
}

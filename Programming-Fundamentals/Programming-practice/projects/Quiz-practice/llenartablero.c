#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DIM 5

void llenartablero(int dim, int tablero[dim * dim]);
void printTablero(int dim, int tablero[dim * dim]);

int main() {
   int tablero[DIM * DIM];

   llenartablero(DIM, tablero);
   printTablero(DIM, tablero);

   return 0;
}

void llenartablero(int dim, int tablero[dim * dim]) {
   // initialize the cells
   for (int index = 0; index < dim; index++) {
      tablero[index] = 0;
   }

   // Initialize the random seed
   srand(time(NULL));

   // pick two random **DIFFERENT** positions
   int pos1 = rand() % (dim * dim);
   int pos2;

   do {
      pos2 = rand() % (dim * dim);
   } while (pos2 == pos1);

   // Assign random uppercase letter
   tablero[pos1] = 'A' + (rand() % 20);
   tablero[pos2] = 'A' + (rand() % 20);
}

// Debugging printing
void printTablero(int dim, int tablero[dim * dim]) {
   for (int index = 0; index < dim; index++) {
      for (int jindex = 0; jindex < dim; jindex++) {
         printf("%c ", tablero[index * dim + jindex]);
      }
      printf("\n");
   }
}

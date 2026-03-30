#include <math.h>
#include <stdio.h>

#define FIL 2
#define COL 2

int CountDigits(int fila, int columna, int GetMatrix[][COL]);

int main() {
   int TwoDimensionsMatrix[FIL][COL] = {{12, 112}, {8483, 342}};

   for (int Fila = 0; Fila < FIL; Fila++) {
      for (int Columna = 0; Columna < COL; Columna++) {
         printf("Numero: %d, digitos: %d\n", TwoDimensionsMatrix[Fila][Columna], CountDigits(Fila, Columna, TwoDimensionsMatrix));
      }
   }

   return 0;
}

int CountDigits(int fila, int columna, int GetMatrix[][COL]) {
   int Digits = 0;
   if (GetMatrix[fila][columna] == 0) {
      Digits = 1;
   } else {
      Digits = log10(GetMatrix[fila][columna]) + 1;
   }
   return Digits;
}

/*
 * Charles David ; Swap Row
 *
 * SwapRow.c
 */

#include <stdio.h>

#define ROW 3
#define COL 3

void swapRow(int row, int col, char (*matrix)[col]);

int main() {
   /**
    * Swap the first with the last row.
    */
   char grid[ROW][COL] = {
       {'1', '2', '3'},
       {'4', '5', '6'},
       {'7', '8', '9'}};

   for (int ind = 0; ind < ROW; ind++) {
      for (int jind = 0; jind < COL; jind++) {
         printf("%c ", grid[ind][jind]);
      }
      printf("\n");
   }

   char (*GridPtr)[COL] = grid;

   swapRow(ROW, COL, GridPtr);

   return 0;
}

void swapRow(int row, int col, char (*matrix)[col]) {
   int TempRow[col];

   // Copy first row into temp
   for (int ind = 0; ind < col; ind++) {
      TempRow[ind] = matrix[0][ind];
   }

   // copy last row into first row
   for (int ind = 0; ind < col; ind++) {
      matrix[row - 1][ind] = TempRow[ind];
   }
}

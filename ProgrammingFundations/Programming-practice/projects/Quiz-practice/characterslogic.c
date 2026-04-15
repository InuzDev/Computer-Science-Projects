/**
 * Characters logic theory ; Charles David
 *
 * characterslogic.c
 */

#include <stdio.h>

#define ROW 3
#define COL 3

void FillTable(int row, int col, char (*table)[col]);

int main() {
   char CharMatrix[ROW][COL] = {{'A', ' ', 'B'},
                                {'B', 'C', ' '},
                                {' ', 'Z', ' '}};

   FillTable(ROW, COL, CharMatrix);

   return 0;
}

void FillTable(int row, int col, char (*table)[col]) {
   for (int ind = 0; ind < row; ind++) {
      for (int jind = 0; jind < col; jind++) {
         if (*(*(table + ind) + jind) == ' ') {
            // You can use table[ind][jind] instead of the complex pointer operation.
            int counter = 0;

            // Sweep the surroundings
            for (int Xview = -1; Xview <= 1; Xview++) {
               for (int Yview = -1; Yview <= 1; Yview++) {
                  if (Xview == 0 && Yview == 0) {
                     continue;
                  }

                  int ni = ind + Xview;
                  int nj = jind + Yview;

                  if (ni >= 0 && ni < row && nj >= 0 && nj < col) {
                     if (*(*(table + ni) + nj) >= 'A' && *(*(table + ni) + nj) <= 'Z') {
                        counter++;
                     }
                  }
               }
            }
            *(*(table + ind) + jind) = '0' + counter;
         }
      }
   }
}

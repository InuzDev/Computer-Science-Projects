/**
 * Charles David ; Moving in a matrix with pointers
 *
 * movingMatrix.c
 */

/*
 * p is a pointer;
 * p + 2 -> move to row 2
 * *(p + 2) -> get that row
 * + 1 -> move inside the row
 * * -> read the value
 */

#include <stdio.h>

#define COL 3
#define FIL 2

void PrintMatrix(int *Matrix);

int main() {
   int Matrix[FIL][COL] = {
       {1, 2, 3},
       {4, 5, 6}};

   int *ptr = &Matrix[0][0];

   // this print in one linear dimension
   for (int index = 0; index < (COL * FIL); index++) {
      printf("%d ", *(ptr + index));
   }

   // To move in a 2D using pointer logic, use this formula: *(ptr + (index * Col + jindex))
   int Value = *(ptr + (1 * COL + 2)); // This would be Matrix[1][2]

   return 0;
}

void PrintMatrix(int *Matrix) {
}

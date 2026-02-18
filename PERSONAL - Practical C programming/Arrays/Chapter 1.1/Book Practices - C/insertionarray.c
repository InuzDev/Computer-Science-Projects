/*
 * Charles David - Practice
 *
 * Inserting an element indo a one-dimensional array
 *
 * main.c
 */
#include <stdio.h>

#define MAX 100

int main() {
   int arr[MAX];
   int ArrLength, ArrPosition;

   while (1) {
      printf("Enter length of array: ");
      scanf("%d", &ArrLength);
      if (ArrLength > 100) {
         printf("Max length is 100, please insert a length less than 100\n");
         continue;
      } else {
         break;
      }
   }
   printf("Enter %d elements of array\n", ArrLength);
   for (int ind = 0; ind <= ArrLength - 1; ind++) {
      scanf("%d", &arr[ind]);
   }

   printf("\nEnter the position where to insert: ");
   scanf("%d", &ArrPosition);

   ArrPosition--;

   for (int jnd = ArrLength; jnd >= ArrPosition; jnd--) {
      arr[jnd + 1] = arr[jnd];
   }
   printf("\nEnter the value to insert: ");
   scanf("%d", &arr[ArrPosition]);

   printf("\nArray after insertion of element: \n");
   for (int ind = 0; ind <= ArrLength; ind++) {
      printf("%d\n", arr[ind]);
   }
   return 0;
}

#include <stdio.h>

int main() {
   // We need to create an array that will contian the numbers
   int numbers[8] = {4,5,10,34,2,12,4,1}; // Arrays start from 0 index.

   // If I ask to print the number 34, I gotta ask the index.

   // To print a intenger, we need to specify the data type with "%d"
   printf("%d", numbers[3]);
   printf("\n");

   printf("This is the MaxNum.c File \n");

   return 0;
}
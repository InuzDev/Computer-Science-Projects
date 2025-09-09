#include <stdio.h>

int main()
{
   // We now going to try and create the workflow to make the MaxNum program

   /*
   This programs is suppose to determine what the biggest number in an array is.
   In this example, the array got a fixed size of 8 and the data is integer.
   */

   // We need to create an array that will contian the numbers

   int numbers[8] = {4, 5, 10, 34, 2, 12, 4, 1, 2}; // Arrays start from 0 index.
   int Max = 0;
   int numbersLength = sizeof(numbers) / sizeof(numbers[0]); // This will give us the lenght of the array

   for (int Index = 0; Index < numbersLength; Index++)
   {
      if (numbers[Index] > Max)
      {
         Max = numbers[Index];
      }
   }

   printf("%d", Max); // Keep this, so we receive an output.

   return 0;
}
/**
 * Charles David - Pointer, Arrays and matrix
 * !Note: The functions to manipulate the arrays can be implemented using
 * indexes and pointers. Consider the pointer exercises
 */

#include <stdio.h>

#define MAX_EXERCISES 12

void swap(int a, int b);

int main() {
   int userSelection = 0;

   printf("Programming exercises - Main menu\n\n");
   printf("Select from 1 to 12 to see each program made for the exercises\n");
   while (!userSelection || userSelection > MAX_EXERCISES) {
      printf("User Input: ");
      scanf_s("%d", userSelection);
      if (!userSelection || userSelection > MAX_EXERCISES) {
         printf("Over the limit, please select from: 1 to 12");
      }
   }

   if (userSelection == 1) {
      int a, b;
      printf(
          "This function objective is to swap the value of A to the value of B, meaning A will equal B and B will equal A\n");
      printf("Enter a value for the A: ");
      scanf_s("%d", a);
      printf("");
   }

   return 0;
}

/*
 * Function: swap
 * Objective: Change the value of A to B. E.g: A = 3, B = 2, with this function,
 * A = 2 and B = 3
 */

void swap(int a, int b) {
   if (a == b) {
      printf("Same value");
   } else {
      int StoreA = a;
      int StoreB = b;

      b = StoreA;
      a = StoreB;
   }
   printf("a: %d\nb: %d", a, b);
}

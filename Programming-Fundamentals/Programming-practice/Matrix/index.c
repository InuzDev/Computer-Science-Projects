/**
 * Charles David - Pointer, Arrays and matrix
 * !Note: The functions to manipulate the arrays can be implemented using
 * indexes and pointers. Consider the pointer exercises
 */

#include <stdio.h>
#include <unistd.h>

// Typedef area
typedef enum {
   TYPE_INT,
   TYPE_FLOAT,
   TYPE_DOUBLE,
   TYPE_LONG,
   TYPE_LONGLONG,
   TYPE_LONGDOUBLE
} DataType;

#define MAX_EXERCISES 12
#define MIN_EXERCISES 1

// Exercises library
#define SWAP 1
#define BINARY_SWITCHER 2

// Additional function used as userPrompt in the terminal.
int userPrompt(void *Value, DataType type);
// Prototypes functions for the exercises.
void swap(float *a, float *b);
void binary_switcher(int *n, int *b);
void printBinary(int n);

int main() {
   int userSelection = 0;

   printf("Programming exercises - Main menu\n\n");
   printf("Select from 1 to 12 to see each program made for the exercises\n");
   while (!userSelection || userSelection > MAX_EXERCISES) {
      userPrompt(&userSelection, TYPE_INT);

      while (getchar() != '\n')
         ;

      if (userSelection > MAX_EXERCISES) {
         printf("Over the limit, please select from: %d to %d\n\n", MIN_EXERCISES, MAX_EXERCISES);
      } else if (userSelection < MIN_EXERCISES) {
         printf("Please, enter a valid input [%d to %d]\n\n", MIN_EXERCISES, MAX_EXERCISES);
      }
   }

   if (userSelection == 1) {
      float x, y;
      printf(
          "This function objective is to swap the value of A to the value of B, meaning A will equal B and B will equal A\n");
      printf("Enter a value for A:\n");
      userPrompt(&x, TYPE_FLOAT);
      printf("Enter a value for B:\n");
      userPrompt(&y, TYPE_FLOAT);
      swap(&x, &y);
   } else if (userSelection == 2) {
      int x, bit;

      printf("Enter a value for X\n");
      userPrompt(&x, TYPE_INT);

      printf("Before change of binary: %d -> ", x);
      printBinary(x);

      printf("Specify what bit you want to change: [1 to 8]\n");
      userPrompt(&bit, TYPE_INT);
      bit = bit - 1; // Make sure is it from 0 to 7.
      binary_switcher(&x, &bit);

      printf("After change of binary: %d -> ", x);
      printBinary(x);
   }

   return 0;
}

/**
 * Function: UserPrompt
 * Description: Give a simple user prompt as interface in ther terminal.
 * Arguments: (void)(pointer) Value, (DataType) type
 * return: A scanf_s that await the user input.
 */
int userPrompt(void *Value, DataType type) {
   printf("User prompt: ");
   switch (type) {
   case TYPE_INT:
      return scanf_s("%d", (int *)Value);
   case TYPE_FLOAT:
      return scanf_s("%f", (float *)Value);
   case TYPE_DOUBLE:
      return scanf_s("%lf", (double *)Value);
   case TYPE_LONG:
      return scanf_s("%ld", (long *)Value);
   case TYPE_LONGLONG:
      return scanf_s("%lld", (long long *)Value);
   case TYPE_LONGDOUBLE:
      return scanf_s("%Lf", (long double *)Value);
   default:
      return 0;
   }
}

/*
 * Function: swap
 * Objective: Change the value of A to B. E.g: A = 3, B = 2, with this function,
 * A = 2 and B = 3
 * Arguments: (float)(pointers) a, b
 */
void swap(float *a, float *b) {
   if (*a == *b) {
      printf("Same value ; AB: %.4f", *a);
   } else {
      int StoreA = *a;
      int StoreB = *b;

      *b = StoreA;
      *a = StoreB;
      printf("a: %.4f\nb: %.4f", *a, *b);
   }
}

// we need to make a helper to output the data in this function

/**
 * Function: binary_switcher
 * Objective: switch an specific binary from off to on, or on to off, switching the original value
 */
void binary_switcher(int *n, int *b) {
   *n = *n | (1 << *b);
}

/**
 * Function: printBinary()
 * Objective:
 * Arguments: (int) n
 */
void printBinary(int n) {
   for (int index = 7; index >= 0; index--) {
      printf("%d", (n >> index) & 1);
   }
   printf("\n");
}

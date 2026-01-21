/**
 * Carlos Jorge ; Quiz.c
 *
 * File used to understand the quiz exercise I struggle with
 */

#include <stdio.h>

int main() {
   printf("Exercise 1: ");

   // The question is: What is the last value of varI?
   int varI = 30;
   for (varI = 'Z'; varI >= 'A'; varI -= 5)
      printf("%c\n", varI); // The last answer is A. Which is 60.

   printf("\n\nExercise 2: ");
   // Second point: An array of values, what is the value of total?
   int total = 0;
   float arrNum[10] = {2.0, 1.0, 4.0, 2.0, 4.0, 6.0, 2.0, 4.0, 7.0};

   for (int index = 1; index < 9; index++) {
      if (index % 2 == 1) {
         total += arrNum[index];
      }
   }
   printf("%d", total);

   printf("\n\nExercise 3: ");
   // We will simulate a function output with variable
   int FuncionReturnSim = 0, num = 4;

   for (int index = 0; index < 2; index++) {
      if (FuncionReturnSim == 0 && num % 2 < 1) {
         printf("Abundant");
         FuncionReturnSim = 1;
         printf("\t");
      } else if (!FuncionReturnSim + (num % 2) == 0) {
         printf("Abundant");
      }
   }

   // Exercise 4 is skipped as I got it correct/

   printf("\n\nExercise 5: ");

   // which code snippet print even and ods nnumbers equal to 10 and less or equal to 20;

   for (int index = 9; index++ <= 19; index++) {
      printf("%d\t", index);
   }

   // The exercise 6 is the same as one, but this time ask the last value as a character (For some reason, the first question is "asking" for the integer value)

   return 0;
}

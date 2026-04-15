#include <math.h>
#include <stdio.h>

/*
 * Program: This is a practice for rcursirve functions.
 * Charles David ; Computer Science Practice.
 * */

int main() {
   printf("Practica sobre funciones recursivas: ");
   printf("Select the problem to debug: ");
   return 0;
}

// Make a function to find An = An-1 + 2^n
int GetNextNum(int An) {
   if (An == 0) {
      return 1;
   }

   return GetNextNum(An - 1) + pow(2, An);
}

// Make the following functions recursive
// - int contarchar(char str[], char char);
// - int contarchar(char *str);
// - int mcd(int num1, int num2);

int contarchar(char str[], char Character) {

   return 0;
}

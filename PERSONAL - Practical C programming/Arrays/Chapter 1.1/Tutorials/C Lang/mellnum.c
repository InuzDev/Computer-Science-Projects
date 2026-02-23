#include <stdio.h>

int EsPrimo(int n) {
   int counter = 0;
   for (int ind = 1; ind <= n; ind++) {
      if (n % ind == 0) {
         counter++;
      }
   }

   if (counter > 2) {
      return 0;
   } else {
      return 1;
   }
   return 0;
}

int CheckMellizos(int num1, int num2) {
   if ((EsPrimo(num1) && EsPrimo(num2)) && ((num1 - num2 == 2 || num1 - num2 == -2))) {
      return 1;
   } else {
      return 0;
   }
}

int main() {
   int num1 = 5, num2 = 7, _checkMellizos = 0;

   _checkMellizos = CheckMellizos(num1, num2);

   printf("Num1: %d, Num2: %d\n\n", num1, num2);
   printf("Son mellizos? %d", _checkMellizos);

   return 0;
}

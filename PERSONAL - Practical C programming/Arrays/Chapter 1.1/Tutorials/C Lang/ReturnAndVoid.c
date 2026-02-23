#include <stdio.h>

int EsPrimo(int num);
void TempStoreData(int num, int num2, int *Result);

int main() {
   int num1 = 1, num2 = 2, *Result = 0;
   TempStoreData(num1, num2, Result);
   printf("%d", Result);

   return 0;
}

void TempStoreData(int num, int num2, int *Result) {
   *(Result) = num + num2;
}

int EsPrimo(int num) {
   int counter = 0;
   for (int ind = 1; ind <= num; ind++) {
      if (num % ind == 0) {
         counter++;
      }
   }

   if (counter > 2) {
      return 1;
   } else {
      return 0;
   }
   return 0;
}

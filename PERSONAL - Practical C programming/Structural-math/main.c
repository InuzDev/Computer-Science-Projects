#include <math.h>
#include <stdio.h>

int CheckPrime(int num);

int main() {
   // Check if a number is prime or not
   int num = 0;
   printf("Ingrese el numero que quiere verificar: ");
   scanf("%d", &num);

   int _checkPrime = 0;
   _checkPrime = CheckPrime(num);
   printf("%d", _checkPrime);
   if (_checkPrime == 1) {
      printf("El numero es primo");
   } else {
      printf("El numero no es primo");
   }

   return 0;
}

int CheckPrime(int num) {
   int _num = (int)sqrt((float)num);

   for (int ind = 0; ind < _num; ind++) {
      if (num % ind == 0) {
         return 0;
      } else {
         return 1;
      }
   }
   return 0;
}

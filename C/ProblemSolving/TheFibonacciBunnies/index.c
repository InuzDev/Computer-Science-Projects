/*

Los conejos de Fibonacci. El matemático Leonardo Fibonacci
expuso el siguiente problema, haga un programa para
solucionarlo. Suponga que una pareja de conejos tiene una par
de crías cada mes y a la vez las crías se hacen fértiles al cabo de
un mes. Si comenzamos con una pareja fértil y no muere,
¿Cuántos pares de conejos se tendrían al cabo de un año?

Fibonacci sucession is: F(n) = F(n - 1) + F(n - 2)

*/
#include <stdlib.h>
#include <stdio.h>

#define MONTH 12

int main()
{
   long long f1 = 1, f2 = 1, fn;

   printf("Mes 1: %lld\n", f1);
   printf("Mes 1: %lld\n", f2);

   for (int index = 3; index <= MONTH; index++)
   {
      fn = f1 + f2;
      printf("Mes %d: %lld\n", index, fn);
      f1 = f2;
      f2 = fn;
   }
   return 0;
}

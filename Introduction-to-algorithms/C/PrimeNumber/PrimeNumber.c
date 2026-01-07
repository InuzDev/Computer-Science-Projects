#include <stdio.h>
#include <math.h>

int main()
{
   int num = 0, cantNum = 0;

   printf("Ingrese un n%cmero -> ", 162);
   scanf("%d", &num);

   if (num <= 1)
   {
      printf("%d is NOT prime", num);
   }
   else
   {
      // Check how many numbers divide n in
      // range 2 to sqrt(n)
      for (int i = 2; i * i <= num; i++)
      {
         if (num % i == 0)
         {
            cantNum++;
         }
      }

      // if cnt is greater than 0 then n is not prime
      if (cantNum > 0)
      {
         printf("%d is NOT prime", num);
      }
      else
      {
         printf("%d is prime", num);
      }
   }

   return 0;
}
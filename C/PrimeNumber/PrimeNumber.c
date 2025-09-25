#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int main()
{
   // Optimized approach - Thanks to GeeksforGeeks
   int n = 21;
   int cnt = 0;

   if (n <= 1)
   {
      printf("%d is NOT prime", n);
   }
   else
   {
      // Check how many numbers divide n in
      // range 2 to sqrt(n)
      for (int i = 2; i * i <= n; i++)
      {
         if (n % i == 0)
         {
            cnt++;
         }
      }

      // if cnt is greater than 0 then n is not prime
      if (cnt > 0)
      {
         printf("%d is NOT prime", n);
      }
      else
      {
         printf("%d is prime", n);
      }
   }

   return 0;
}
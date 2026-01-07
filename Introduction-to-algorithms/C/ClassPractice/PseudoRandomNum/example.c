#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define CANTNUM 10
#define LIMINF 25
#define LIMSUP 50

int randrange(int liminf, int limsup);

//! NOTE: Formula to have a random number from a range: rand() % (limsup-liminf + 1) + liminf

// #define SEED 349857

// int main()
// {
//    srand(SEED);

//    for (int index = 1; index <= 10; index++)
//    {
//       printf("%d\n", rand());
//    }

//    return 0;
// }

int main()
{
   long sec = time(NULL);
   printf("Cant. segundos: %ld", sec);
   srand(sec);

   for (int index = 1; index <= CANTNUM; index++)
   {
      // Random numbers from a given range with a specific seed.
      printf("%ld\n", randrange(LIMINF, LIMSUP));
      // Random numbers from a given seed
      printf("%ld\n", rand() % sec);
   }

   return 0;
}

int randrange(int liminf, int limsup)
{
   return rand() % (limsup - liminf + 1) + liminf;
}
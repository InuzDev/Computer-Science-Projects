/*

Se desea jugar con la máquina a “adivinar el número”. Para
ello, elabore un programa en el cual la máquina seleccione
internamente un número aleatorio entre 0 y 200. El programa
solicitará al usuario que adivine el número que escogió el
programa. Dicho programa debe informar si el número digitado
por el usuario es mayor o menor que el número a adivinar.
Esto se repetirá hasta que el usuario haya adivinado el número.
El programa informará la cantidad de intentos realizados por el
usuario.

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_RANGE 200
#define MIN_RANGE 0

int randrange(int MinRange, int MaxRange);

int main()
{
   // Generate random seeds every second.
   srand(time(NULL));

   // User input
   int UserGuess, TriesCounter = 0;
   int RandomNum = randrange(MIN_RANGE, MAX_RANGE);
   do
   {
      printf("adivina el n%cmero secreto: [ENTER] ", 163);
      scanf("%d", &UserGuess);
      TriesCounter++;
      if (UserGuess == RandomNum)
      {
         printf("Excelente!!! %d es el n%cmero secreto.\n", RandomNum, 163);
         printf("N%cmero de intentos: %d\n", 163, TriesCounter);
      }
   } while (UserGuess != RandomNum);
   return 0;
}

// Function to generate a random number based on a seed.
int randrange(int MinRange, int MaxRange)
{
   return rand() % (MaxRange - MinRange + 1) + MinRange;
}
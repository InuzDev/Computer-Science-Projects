/*

similar al ejemplo anterior, esta vez la computadora es la que va a adivinar

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_RANGE 200
#define MIN_RANGE 0

#define ENTER 13

int randrange(int MinRange, int MaxRange);

int main()
{
   srand(time(NULL));

   char StartInput, PlayerInput;
   int GuessNum, Low = MIN_RANGE, High = MAX_RANGE, TriesCounter = 0;

   GuessNum = randrange(MIN_RANGE, MAX_RANGE);

   printf("Tome un n%cmero de la mente entre 0 y 200\n", 163);
   printf("Pulse [ENTER] cuando est%c listo", 130);
   scanf("%c", &StartInput);

   while (1)
   {
      TriesCounter++;
      printf("Es su n%cmero secreto el %d?\n", 163, GuessNum);
      printf("Si es menor, pulse <\n");
      printf("Si es mayor, pulse >\n");
      printf("Si adivin%c, pulse =\n", 130);
      printf("Respuesta: ");
      scanf(" %c", &PlayerInput);

      if (PlayerInput == '=')
      {
         printf("Adivin%c en %d oportunidades!!", 130, TriesCounter);
         break;
      }
      else if (PlayerInput == '<')
      {
         High = GuessNum - 1;
         if (Low > High)
         {
            printf("Rango inv%clido, revise sus respuestas.\n", 160);
            break;
         }
         GuessNum = randrange(Low, High);
      }
      else if (PlayerInput == '>')
      {
         Low = GuessNum + 1;
         if (Low > High)
         {
            printf("Rango inv%clido, revise sus respuestas.\n", 160);
            break;
         }
         GuessNum = randrange(Low, High);
      }
   }
   return 0;
}

int randrange(int MinRange, int MaxRange)
{
   return rand() % (MaxRange - MinRange + 1) + MinRange;
}
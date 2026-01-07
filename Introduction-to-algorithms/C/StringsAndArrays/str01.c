/*
This file has been provided by my teacher
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXSTR 80

int main()
{
   char frase[MAXSTR];
   char otra[] = "Alex";
   char caracts[] = {'A', 'l', 'e', 'x'};
   int vals[] = {10, 15, 18, 20, 30};
   int nums[10] = {0};

   printf("Frase: ");
   gets(frase);

   printf("Frase digitada:\n%s\n", frase);

   frase[7] = '\0'; // NULL, 0

   printf("Frase digitada:\n%s\n", frase);
   return 0;
}
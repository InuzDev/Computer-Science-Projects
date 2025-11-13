#include <stdio.h>

#define MAXCHAR 128

void repeatName(char[], int);

// Program that prints the name of the user n times.
int main()
{
   int Num;
   char Name[MAXCHAR];

   printf("Ingrese un n%cmero: ", 163);
   scanf("%d", &Num);

   printf("Ingrese su nombre: ");
   scanf("%s", &Name);

   repeatName(Name, Num);
   return 0;
}

void repeatName(char Name[], int Num)
{
   for (int index = 0; index < Num; index++)
   {
      printf("%s\n", Name);
   }
}
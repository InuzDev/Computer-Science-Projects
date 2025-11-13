# C Extras

Here we practice exercise given by my teacher. In this readme, I will update it to explain some concepts and programs I made to solve the exercises.

## Program that prints your name

This program is designed to print your name N times.

The structure of the program is very simple.

First, let understand the problem first.

* "Write a program to which a number is typed and print per screen your name as many times as the number indicates Entered."

So, I thought that the problem is asking the developer "Make a program that ask the user a number, then the name of the user. Then it will print the name *n* times". So if the user input is 10, and Charles it will print 10 times the Charles. Very simple.

```c
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
```

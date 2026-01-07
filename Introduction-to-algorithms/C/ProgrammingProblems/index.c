// This include is used for every single option.
#include <stdio.h>

// Used to pause the program after running the terminal
#include <unistd.h>
#define INVALID_LIMIT 0
// In this file we going to solve multiple programming problems from ICC class.

// The following exercise are required to study:
// 10, 22, 15, 29, 30, 36, 37, 45, 46, 49, 51, 52
// Evaluate and use the good practices shown by the teacher

// The most important five points
/*
Standard documentation for the functions
Identation of 3 spaces
No Global variables
Use monotechnicals names for the variables
Instead of literal contants, use the Macros or constants.
*/

/*
Program number 1: Escriba un programa que imprima una tabla de conversión de
grados Farenheit a Celcius. La fórmula de conversión es como
sigue : 1.8 °C = °F - 32
*/

// Prototype function
int FahrenheitConversion(float Fahr);

int main()
{
   // The program needs to read a range of values
   int LimInf, LimSup;       // This variables are the one to store the range data.
   float Fahr = 0, Celcious; // Variables that are used for the conversion.
   // Let ask for the user input
   do
   {
      printf("Introduzca un valor del rango inicial para la tabla: ");
      scanf("%d", &LimInf);
      if (LimInf <= 0)
      {
         printf("Limite invalido, reintroduzca un limite mayor a 0.");
      }
   } while (LimInf <= INVALID_LIMIT); // this is a way to do it.
   do
   {
      printf("Introduzca un valor del rango final para la tabla: ");
      scanf("%d", &LimSup);
   } while (LimSup <= INVALID_LIMIT);

   printf("Farenheit\t|\tCelcius\n");            // Table header
   printf("---------------|----------------\n"); // Table line separator.

   // the loop to cycle on each number from the user input.
   for (Fahr = LimInf; Fahr <= LimSup; Fahr++)
   {
      Celcious = FahrenheitConversion(Fahr);
      printf("%10.1f\t|\t%7.2f\n", Fahr, Celcious);
   }

   // this command is used to pause the terminal, so the user can test it out in a
   // real CMD application
   system("pause"); // the use will be prompted to press any key to continue, in this case close the program.
   return 0;
}

int FahrenheitConversion(float Fahr)
{
   return (Fahr - 32) / 1.8;
}
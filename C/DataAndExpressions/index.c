#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// A whole function to break a line
void BreakLine()
{
   printf("\n\n");
}

void ABC_Arithmetic_operations(int a, int b, int c)
{
   // This char is for testing what data to use and how to make an actual string
   char *StringData = "Suponga que a, b y c son variables enteras que tienen los valores 8, 3, y -5, respectivamente. Determine el valor de cada una de las siguientes expresiones aritmeticas.\n\n"; // \n is a line breaker
   printf("%s", StringData);

   // Print the values used
   printf("Values used: %d, %d, %d", a, b, c);
   printf("\n\n");
   // We need to determine every single following arithmetic operation
   printf("A: %d", a + b + c);
   printf("\n"); // Line breaker
   printf("B: %d", 2 * b + 3 * (a - c));
   printf("\n");
   printf("C: %d", a / b);
   printf("\n");
   printf("D: %d", a % b);
   printf("\n");
   printf("E: %d", a / c);
   // Line breaker
   printf("\n\n");
   // Second columb
   printf("F: %d", a % c);
   printf("\n");
   printf("G %d", a * b / c);
   printf("\n");
   printf("H: %d", a * (b / c));
   printf("\n");
   printf("I: %d", (a * c) % b);
   printf("\n");
   printf("J: %d", a * (c % b));

   BreakLine();
}

void XYZ_float_arithmetic_operations(float x, float y, float z)
{
   // Next stage point 2
   char *StringData_point2 = "Suponga que x, y y z son variables flotantes que tienen los valores 8.8, 3.5, y -5.2, respectivamente. Determine el valor de cada una de las siguientes expresiones aritmeticas.";
   printf("%s", StringData_point2);

   BreakLine();

   printf("A: %f", x + y + z);
   printf("\n");
   printf("B: %f", 2 * y + 3 * (x - z));
   printf("\n");
   printf("C: %f", x / y);
   printf("\n");
   // For "modulo" operations with float type data, we need to import <math.h>
   // printf("D: %f", x % y);  <- Instead of this; Do the code below
   printf("D: fmod(%.2f, %.2f) = %.2f\n", x, y, fmod(x, y)); // In this scenerario, x = 8.3 for example

   BreakLine();
}

int main()
{
   // Function calls
   ABC_Arithmetic_operations(8, 3, -5);
   XYZ_float_arithmetic_operations(8.8, 3.5, -5.2);

   return 0;
}
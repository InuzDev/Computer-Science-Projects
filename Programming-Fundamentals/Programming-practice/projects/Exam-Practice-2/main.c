#include <string.h>

// Struct used in the third example.
typedef struct {
   char matricula[8], nombre[20], carrera[3];
   float puntos;
   int creditos;
} EST;

// Third example
int EliminarEstudiantes(EST *data, int num, char *matborrar);
// Second example
double serie(double x, int index, int num);
// First example
int strrepite(char *string, char *RepitedString);

int EliminarEstudiantes(EST *data, int num, char *matborrar) {
   int Escritura = 0;

   for (int lectura = 0; lectura < num; lectura++) {
      if (strstr(data[lectura].matricula, matborrar) == NULL) {
         data[Escritura] = data[lectura];
         Escritura++;
      }
   }
   return Escritura;
}

// Normal factorial without recursion
//
// Knowing we can do it with a factorial function, but... the reason to use this one, is to avoid usage of space.
// If we use two factorial functions, the space complexity increases exponentially.
int factorial(int num) {
   int fact = 1, index;

   for (index = 1; index <= num; index++) {
      fact *= index;
   }

   return fact;
}

// Using recursion in the factorial;
// If the number is 1, then it returns 1,
// If the number isn't one, it will return the num multiplied by them minus 1.
int RecursiveFactorial(int num) {
   if (num == 1) {
      return 1;
   }
   return num * RecursiveFactorial(num - 1);
}

double serie(double x, int index, int num) {
   if (index > num) {
      return 0;
   }

   double termino = 1.0;

   if (index % 2 == 0) {
      termino = 1.0;
   } else {
      termino = -1.0;
   }

   // Multiplicar por x^1 / 1!
   double xi = 1.0;
   for (int kindex; kindex < 1; kindex++) {
      xi *= x;
   }

   termino = termino * (xi / factorial(index));

   return termino + serie(x, index + 1, num);
}

// This recursive function is supposed to find a repeated word in a string.
int strrepite(char *string, char *RepitedString) {
   // Si el string es demasiado corto.
   if (strlen(string) < strlen(RepitedString)) {
      return 0;
   }

   // Si RepitedString aparece al inicio de String?
   if (strncmp(string, RepitedString, strlen(RepitedString)) == 0) {
      return 1 + strrepite(string + 1, RepitedString);
   } else {
      return strrepite(string + 1, RepitedString); // No se ha encontrado nada
   }
   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros used in the fourth example
#define MAXNOMASIG 41
#define MAXCOD 11

// Struct used in the fourth example
typedef struct {
   char codigo[MAXCOD], nombre[MAXNOMASIG];
   int ht, hp, cr; // Horas teoricas, horas practicas y total de creditos.
} ASG;

// Struct used in the third example.
typedef struct {
   char matricula[8], nombre[20], carrera[3];
   float puntos;
   int creditos;
} EST;

// Fourth example
int estaEnLista(ASG asig, ASG *asigselim, int num);
int obtenerasig(FILE *pf, ASG *asigselim, ASG **asigsfiltro, int num);
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

int estaEnLista(ASG asig, ASG *asigselim, int num) {
   for (int index = 0; index < num; index++) {
      if (strcmp(asig.codigo, asigselim[index].codigo) == 0) {
         return 1; // It is in the list.
      }
   }
   return 0; // It isn't in the list.
}
int obtenerasig(FILE *pf, ASG *asigselim, ASG **asigsfiltro, int num) {
   int cantidad = 0;
   *asigsfiltro = NULL; // Initialize an empty pointer.
   ASG TempStorage;     // Buffer to read the file.

   while (fread(&TempStorage, sizeof(ASG), 1, pf) == 1) {
      if (!estaEnLista(TempStorage, asigselim, num)) {
         // If it isn't in the list, we will include, so we ask for more memory
         *asigsfiltro = realloc(*asigsfiltro, (cantidad + 1) * sizeof(ASG));
         (*asigsfiltro)[cantidad] = TempStorage;
         cantidad++;
      }
   }

   return cantidad;
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

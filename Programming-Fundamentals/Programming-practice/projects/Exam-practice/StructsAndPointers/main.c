#include <stdio.h>
#include <string.h>

#define LENISBN 14
#define LENTITULO 51
#define LENAUTOR 40

typedef struct {
   int dd, mm, yyyy;
} FECHA;

typedef struct {
   char ISBN[LENISBN];
   char titulo[LENTITULO];
   char autor[LENAUTOR];
   FECHA fechaedicion;
} LIBRO;

int obtenerOrden(char *isbn, int *order, int n) {
   char tipo[5];
   strncpy(tipo, &isbn[6], 4);
   tipo[4] = '\0';

   for (int i = 0; i < n; i++) {
      char tipoOrder[5];
      sprintf(tipoOrder, "%d", order[i]);
      if (strcmp(tipo, tipoOrder) == 0) {
         return i;
      }
   }
   return n;
}

void getorder(char **list, int n, int **order) {
   LIBRO *libros = (LIBRO *)list;
   int *ordenArray = *order;
   int numOrdenes = 7;

   for (int i = 0; i < n - 1; i++) {
      for (int j = 0; j < n - 1 - i; j++) {
         int pos1 = obtenerOrden(libros[j].ISBN, ordenArray, numOrdenes);
         int pos2 = obtenerOrden(libros[j + 1].ISBN, ordenArray, numOrdenes);

         if (pos1 > pos2) {
            LIBRO temp = libros[j];
            libros[j] = libros[j + 1];
            libros[j + 1] = temp;
         }
      }
   }
}

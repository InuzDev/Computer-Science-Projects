// This exam is an example practice, not the original one.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Utilities used through the exam.
#define LENGTHID 6
#define LENGTHNAME 21
#define LENGTHLASTNAME 16
#define LENGTHCITY 11

typedef struct {
   int day, month, year;
} DATE;

typedef struct {
   char id[LENGTHID];
   char firstname[LENGTHNAME];
   char lastname[LENGTHLASTNAME];
   DATE birthday;
   char cityofbirth[LENGTHCITY];
} PERSON;

float edad(DATE birthday);

// First point exercise, second exam
int getpersons(PERSON **persons, FILE **pf, int EdadIni, int EdadFin) {
   *pf = fopen("personas.info", "rb");
   if (*pf == NULL) {
      return 1; // There was an error opening it.
   }

   int cantidad = 0;
   *persons = NULL;
   PERSON temp;

   while (fread(&temp, sizeof(PERSON), 1, *pf) == 1) {
      float edad = edad(temp.birthday); // edad(); function is an imaginary given function.

      if (edad >= EdadIni && edad <= EdadFin) {
         // It is in the range, so we add them.
         *persons = realloc(*persons, (cantidad + 1) * sizeof(PERSON));

         (*persons)[cantidad] = temp;
         cantidad++;
      }
   }

   // Optimal use, if nobody qualify, free the pointer.
   if (cantidad == 0) {
      free(*persons);
      *persons = NULL;
   }

   fclose(*pf);
   return cantidad;
}

// fourth exercise, second exam.
FILE *filter(char *filename, PERSON *persons, int npersons, int bdayfirst, int bdaylast) {
   FILE *pf = fopen(filename, "rb");

   if (pf == NULL) {
      return NULL;
   }

   PERSON *filtrados = NULL;
   int cantidad = 0;
   PERSON temp;

   while (fread(&temp, sizeof(PERSON), 1, pf) == 1) {
      float edad_persona = edad(temp.birthday);

      if (edad_persona >= bdayfirst && edad_persona <= bdaylast) {
         filtrados = realloc(filtrados, (cantidad + 1) * sizeof(PERSON));
         filtrados[cantidad] = temp;
         cantidad++;
      }
   }

   fclose(pf);
   pf = fopen(filename, "wb");

   if (filtrados != NULL) {
      fwrite(filtrados, sizeof(PERSON), cantidad, pf);
      free(filtrados);
   }

   return pf;
}

// Second point exercise
int yaExiste(PERSON *data, int hasta, char *id) {
   for (int index = 0; index < hasta; index++) {
      if (strcmp(data[index].id, id) == 0) {
         return 1;
      }
   }
   return 0;
}

int distinct(PERSON *data, int num) {
   int escritura = 0;
   for (int lectura = 0; lectura < num; lectura++) {
      // We check if the ID isn't already registed.
      if (!yaExiste(data, escritura, data[lectura].id)) {
         data[escritura] = data[lectura];
         escritura++;
      }
   }

   return escritura;
}

float edad(DATE birthday) {
   time_t today = time(NULL);
   struct tm *actual_date = localtime(&today);

   int actual_year = actual_date->tm_year + 1900;
   int actual_month = actual_date->tm_mon + 1;
   int actual_day = actual_date->tm_mday;

   float years = actual_year - birthday.year;

   if (actual_month < birthday.month || (actual_month == birthday.month && actual_day < birthday.day)) {
      years -= 1.0f;
   }

   return years;
}

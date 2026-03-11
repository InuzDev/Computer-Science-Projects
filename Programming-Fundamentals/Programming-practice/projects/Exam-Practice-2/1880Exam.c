#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Utilities for the exam.
#define LENGTHID 6
#define LENGTHNAME 21
#define LENGTHLASTNAME 16
#define LENGTHCITY 11

typedef struct
{
   int day, month, year;
} DATE;

typedef struct
{
   char id[LENGTHID];
   char firstname[LENGTHNAME];
   char lastname[LENGTHLASTNAME];
   DATE birthday;
   char cityofbirth[LENGTHCITY];
} PERSON;

float edad(DATE birthday);

// First exercise, third exam practice
char *getcities(PERSON *data, int num) {
   char *resultado = NULL;
   int longitud = 0;

   for (int index = 0; index < num; index++) {
      // Check if the city in the result
      if (resultado == NULL || strstr(resultado, data[index].cityofbirth) == NULL) {
         int lenCiudad = strlen(data[index].cityofbirth);

         if (resultado == NULL) {
            // Primera ciudad, sin coma.
            resultado = realloc(resultado, lenCiudad + 1);
            strcpy(resultado, data[index].cityofbirth);
            longitud = lenCiudad;
         } else {
            resultado = realloc(resultado, longitud + 2 + lenCiudad + 1);

            strcat(resultado, ", ");
            strcat(resultado, data[index].cityofbirth);
            longitud += 2 + lenCiudad;
         }
      }
   }

   return resultado;
}

// Exercise two
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
      if (!yaExiste(data, escritura, data[lectura].id)) {
         data[escritura] = data[lectura];
         escritura++;
      }
   }

   return escritura;
}

FILE *filter(char *filename, PERSON *persons, int numPersons, int bdayfirst, int bdaylast) {
   FILE *pf = fopen(filename, "rb");
   if (pf == NULL) {
      return NULL;
   }

   PERSON *filtrados = NULL;
   int cantidad = 0;
   PERSON temp;

   while (fread(&temp, sizeof(PERSON), 1, pf) == 1) {
      float edad = edad(temp.birthday);
      if (edad >= bdayfirst && edad <= bdayfirst) {
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

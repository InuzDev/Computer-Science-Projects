#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXID 6
#define LENNOMB 31
#define LENPWD 20

typedef struct {
   int dd, mm, yyyy;
} FECHA;

typedef struct artista {
   char id[MAXID];
   char name[LENNOMB];
   FECHA fnacim;
} ART;

int CalcEdad(ART *arts);
int getarts(ART **arts);
void getart(ART *art);
void imparts(ART *arts, int narts);

int main() {
   ART *arts = NULL; // Requires memory reallocation.

   int cantarts = getarts(&arts);
   imparts(arts, cantarts);

   free(arts);

   return 0;
}

void getart(ART *art) {
   fflush(stdin);
   printf("Nombre del artista: ");
   scanf("%30[^\n]s", art->name);

   fflush(stdin);
   printf("Fecha de nacimiento: ");
   scanf("%d/%d/%d", &art->fnacim.dd, &art->fnacim.mm, &art->fnacim.yyyy);
}

int getarts(ART **arts) {
   char UserInput;
   int Artcount = 0;

   while (1) {
      printf("Quieres capturar un arista?\n[S/N]: ");
      fflush(stdin);
      scanf("%c", &UserInput);
      UserInput = tolower(UserInput);

      if (UserInput == 'n' || UserInput == 's') {
         break;
      }
      printf("Por, favor. Entrar un aletra valida\n\n");
   }

   if (UserInput == 's') {
      while (UserInput == 's') {
         *arts = (ART *)realloc(*arts, (Artcount + 1) * sizeof(ART));
         if (*arts == NULL) {
            fprintf(stderr, "Error de memoria");
            exit(1);
         }

         printf("Ingrese el ID del artista: ");
         scanf("%5s", (*arts + Artcount)->id);
         getart((*arts + Artcount));
         Artcount++;

         printf("Quieres agregar otro artista? [S/N]: ");
         fflush(stdin);
         scanf("%c", &UserInput);
         UserInput = tolower(UserInput);
      }
   }
   return Artcount;
}

int CalcEdad(ART *art) {
   time_t t = time(NULL);
   struct tm *fecha = localtime(&t);

   int edad = (fecha->tm_year + 1900) - art->fnacim.yyyy;

   if (art->fnacim.mm > (fecha->tm_mon + 1) ||
       (art->fnacim.mm == (fecha->tm_mon + 1) && art->fnacim.dd > fecha->tm_mday)) {
      edad--;
   }

   return edad;
}

void imparts(ART *arts, int narts) {
   printf("Id\t Nombre del artista\t Edad\n");
   for (int ind = 0; ind < narts; ind++) {
      int edad = CalcEdad(&arts[ind]);
      printf("%s\t %s\t %d\n", (arts + ind)->id, (arts + ind)->name, edad);
   }

   printf("Total de artistaS: %d", narts);
}

#include <stdio.h>
#include <string.h>

#define MAXID 6
#define MAXNAME 60
#define MAXTITULO 100
#define MAXISBN 20
#define TOTAL_LIBROS 100

typedef struct {
   int dd, mm, yyyy;
} FECHA;

typedef struct {
   char idlibro[MAXID];
   char titulo[MAXTITULO];
   char autor[MAXNAME];
   FECHA fecha_pub;
   FECHA fecha_nac_autor;
   char isbn[MAXISBN];
} INFOLIBRO;

// The desire objective is get the author data without repetition.
typedef struct {
   char idautor[MAXID], nombre[MAXNAME];
   FECHA fecnacim;
} AUTOR;

int authorExists(AUTOR *Authors, int total, char *Name);
int GetAuthors(char *InputFileName, INFOLIBRO *Buffer);
int WriteAuthorsData(char *OutputFileName, AUTOR *authorData, int total);

int main() {
   INFOLIBRO Libro[TOTAL_LIBROS];
   AUTOR AuthorData[TOTAL_LIBROS];

   int total = GetAuthors("listalibros.txt", Libro);
   if (total <= 0) {
      printf("Problemas leyendo el archivo\"listalibros.txt\"");
      return 1;
   }

   int AuthorCount = 0;
   for (int index = 0; index < total; index++) {
      if (!authorExists(AuthorData, AuthorCount, Libro[index].autor)) {
         sprintf(AuthorData[AuthorCount].idautor, "A%d", AuthorCount + 1);
         strcpy(AuthorData[AuthorCount].nombre, Libro[index].autor);
         AuthorData[AuthorCount].fecnacim = Libro[index].fecha_nac_autor;
         AuthorCount++;
      }
   }

   if (WriteAuthorsData("AutoresDeLibros.dat", AuthorData, AuthorCount)) {
      printf("Se ha creado el archivo AutoresDeLibros\n");
   }

   return 0;
}

int authorExists(AUTOR *Authors, int total, char *Name) {
   for (int index = 0; index < total; index++) {
      if (strcmp(Authors[index].nombre, Name) == 0)
         return 1;
   }
   return 0;
}

int GetAuthors(char *InputFileName, INFOLIBRO *Buffer) {
   FILE *InputFile = fopen(InputFileName, "rb");
   if (!InputFile) {
      return 0;
   }

   int count = fread(Buffer, sizeof(INFOLIBRO), TOTAL_LIBROS, InputFile);

   fclose(InputFile);
   return count;
}

int WriteAuthorsData(char *OutputFileName, AUTOR *authorData, int total) {
   FILE *AuthorDataFile = fopen(OutputFileName, "wb");
   if (!AuthorDataFile) {
      return 0;
   }

   fwrite(authorData, sizeof(AUTOR), total, AuthorDataFile);

   fclose(AuthorDataFile);
   return 1;
}

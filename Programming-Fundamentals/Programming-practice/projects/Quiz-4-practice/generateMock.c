#include <stdio.h>

#define MAXID 6
#define MAXNAME 60
#define MAXTITULO 100
#define MAXISBN 20
#define TOTAL_LIBROS 100

typedef struct {
   char ID[MAXID];
   char Nombre[MAXNAME];
   char Apellido[MAXNAME];
   char Dept[MAXISBN];
} DOCENTE;

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

int main() {
   FILE *f = fopen("listalibro.txt", "wb");
   if (!f) {
      printf("Error opening file\n");
      return 1;
   }

   INFOLIBRO books[] = {
       {"L001",
        "Cien Anos de Soledad",
        "Gabriel Garcia Marquez",
        {5, 6, 1967},
        {6, 3, 1927},
        "978-0-06-088328-7"},
       {"L002",
        "Don Quijote de la Mancha",
        "Miguel de Cervantes",
        {1, 1, 1605},
        {29, 9, 1547},
        "978-84-376-0494-7"},
       {"L003",
        "El Principito",
        "Antoine de Saint-Exupery",
        {6, 4, 1943},
        {29, 6, 1900},
        "978-0-15-601219-5"},
       {"L004",
        "1984",
        "George Orwell",
        {8, 6, 1949},
        {25, 6, 1903},
        "978-0-452-28423-4"},
       {"L005",
        "Rayuela",
        "Julio Cortazar",
        {28, 6, 1963},
        {26, 8, 1914},
        "978-84-204-8285-1"}};

   int n = sizeof(books) / sizeof(INFOLIBRO);
   fwrite(books, sizeof(INFOLIBRO), n, f);
   fclose(f);

   printf("Mock file created with %d books.\n", n);
   return 0;
}

#include <stdio.h>
#include <string.h>

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

typedef struct
{
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

typedef struct {
   char idautor[MAXID], nombre[MAXNAME];
   FECHA fecnamic;
} AUTOR;

typedef struct {
   char idlibro[MAXID], titulo[MAXTITULO], isbn[MAXISBN];
   FECHA publicacion;
} LIBRO;

typedef struct {
   char idautor[MAXID];
   char idlibro[MAXID];
} AUTORLIBRO;

int main() {
   FILE *FileInput, *FileLibro, *FileAutor, *FileAutorLibro;
   INFOLIBRO info;
   LIBRO libro;
   AUTOR autor;
   AUTORLIBRO rel;

   int index = 0;

   FileInput = fopen("listalibro.txt", "rb");
   FileLibro = fopen("libros.dat", "wb");
   FileAutor = fopen("autores.dat", "wb");
   FileAutorLibro = fopen("autorlibro.dat", "wb");

   while (fread(&info, sizeof(INFOLIBRO), 1, FileInput) == 1) {
      strcpy(libro.idlibro, info.idlibro);
      strcpy(libro.titulo, info.titulo);
      strcpy(libro.isbn, info.isbn);
      libro.publicacion = info.fecha_pub;
      fwrite(&libro, sizeof(LIBRO), 1, FileLibro);

      // Rellenar el autor
      sprintf(autor.idautor, "A%04d", index); // Generar un ID unico.
      strcpy(autor.nombre, info.autor);
      fwrite(&rel, sizeof(AUTORLIBRO), 1, FileAutorLibro);

      index++;
   }

   fclose(FileInput);
   fclose(FileLibro);
   fclose(FileAutor);
   fclose(FileAutorLibro);

   return 0;
}

// Function for the quiz:

void eliminarDocente(char *idToDelete) {
   FILE *fileInput, *TempFile, *FileDelete;
   DOCENTE doc;

   fileInput = fopen("docentes.dat", "rb");
   TempFile = fopen("temp.dat", "wb");
   FileDelete = fopen("eliminados.del", "ab");

   if (!fileInput || !TempFile || !FileDelete) {
      printf("Error opening files.\n");
      return;
   }

   int found = 0;
   while (fread(&doc, sizeof(DOCENTE), 1, fileInput) == 1) {
      if (strcmp(doc.ID, idToDelete) == 0) {
         fwrite(&doc, sizeof(DOCENTE), 1, FileDelete);
         found++;
      } else {
         fwrite(&doc, sizeof(DOCENTE), 1, TempFile);
      }
   }

   fclose(fileInput);
   fclose(TempFile);
   fclose(FileDelete);

   if (found) {
      remove("docentes.dat");
      rename("temp.dat", "docentes.dat");
      printf("Docente eliminado correctamente.\n");
   } else {
      remove("temp.dat");
      printf("Docente no encontrado.");
   }
}

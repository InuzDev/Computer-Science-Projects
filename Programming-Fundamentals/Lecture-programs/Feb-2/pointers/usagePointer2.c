#include <stdio.h>

#define MAXSTR 81

void aminus(char[], int);

// int contarchr(char *str, char chr), que determina cuantas veces está
// el caracter 'chr' en la cadena 'str'
// int wordcount(char *str).  Que detemina cuantas palabras tiene str.
// Una palabra es un conjunto de caracteres que termina con un espacio o un nulo.

// sprintf, sscanf, strcpy, strncpy, strcmp, strncmp, strnicmp, strtok.

int main() {
   char frase[MAXSTR] = "FUNDAMENTOS DE PROGRAMACION";

   printf("%s\n", frase);

   aminus(frase + 12, 2);

   printf("%s\n", frase);

   return 0;
}

void aminus(char str[], int nchars) {
   int chr;

   // NULL = 0 = '\0'

   for (int ind = 0; ind < nchars; ind++) {
      chr = str[ind];

      if (chr >= 'A' && chr <= 'Z')
         str[ind] += ' ';
   }
}

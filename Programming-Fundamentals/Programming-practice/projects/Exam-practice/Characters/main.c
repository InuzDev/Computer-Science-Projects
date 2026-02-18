#include <ctype.h>
#include <string.h>

void Mayusculas(char *str, int n) {
   for (int ind = 0; ind < n; ind++) {
      str[ind] = toupper(str[ind]);
   }
}

void Minusculas(char *str, int n) {
   for (int ind = 0; ind < n; ind++) {
      str[ind] = tolower(str[ind]);
   }
}

void TitleCase(char *str, int n) {
   if (n > 0) {
      str[0] = toupper(str[0]);
      for (int ind = 1; ind < n; ind++) {
         str[ind] = tolower(str[ind]);
      }
   }
}

void TitleInvertCase(char *str, int n) {
   for (int ind = 0; ind < n; ind++) {
      if (isupper(str[ind])) {
         str[ind] = tolower(str[ind]);
      } else if (islower(str[ind])) {
         str[ind] = toupper(str[ind]);
      }
   }
}

void buscarReemplazar(char *str, char *buscar, char *reemplazar) {
   int lenStr = strlen(str);
   int lenSearch = strlen(buscar);
   int lenReemplazar = strlen(reemplazar);

   for (int ind = 0; ind <= lenStr - lenSearch; ind++) {
      int coincide = 1;

      for (int jind = 0; jind < lenSearch; jind++) {
         if (tolower(str[ind + jind]) != tolower(buscar[jind])) {
            coincide = 0;
            break;
         }
      }

      if (coincide) {
         for (int jind = 0; jind < lenReemplazar && jind < lenSearch; jind++) {
            if (isupper(str[ind + jind])) {
               str[ind + jind] = toupper(reemplazar[jind]);
            } else {
               str[ind + jind] = tolower(reemplazar[jind]);
            }
         }
      }
   }
}

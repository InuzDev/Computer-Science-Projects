/*
This file has been provided by my teacher
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXSTR 80

int countchr(char[], char);
char modachr(char[]); // Working on it

void amayus(char[]);
void aminus(char[]);
int wordcount(char[]);
void titlecase(char[]); // Primer mayúscula, resto minúsculas
void WordCase(char[]);  // Primera letra de cada palabra en mayúsculas y
                        // el resto de la palabra en minúsculas.

int main()
{
   char frase[MAXSTR], chr = 'a';
   char otra[MAXSTR] = "Esta sí";
   printf("Frase: ");
   // scanf("%[^\n]s",frase);
   gets(frase);

   printf("Estado 1: %s\n\n", otra); // This will be printed as "Esta si"
   // This is copy one string into another. strcpy(string2, string1);
   strcpy(otra, frase); // Now "otra" is equal to what we type in the phrase
   printf("Estado 2: %s\n\n", otra);
   // What if we want to copy just some characters? We use strncpy();
   strncpy(otra, frase, sizeof(otra) - 1);
   otra[sizeof(otra) - 1] = '\0';
   printf("Estado 3: %s\n", otra);

   amayus(frase);
   aminus(frase);
   titlecase(frase);
   WordCase(frase);

   printf("Frase digitada:\n%s(%d)\n", frase, strlen(frase));
   printf("El carater '%c' est%c %d veces en:\n%s\n", chr, 160,
          countchr(frase, chr), frase);
   printf("Cantidad de palabras: %d\n", wordcount(frase));
   printf("La letra que mas se repite es: %c\n", modachr(frase));

   return 0;
}

/*
   for ( int ind = 0; str[ind] != NULL; ind++ )
   for ( int ind = 0; str[ind] != '\0'; ind++ )
   for ( int ind = 0; str[ind] != 0; ind++ )
   for ( int ind = 0; str[ind]; ind++ )
   sprintf
   sscanf
   strncmp
   strnicmp
   stricmp
*/

int countchr(char str[], char chr)
{
   int cont = 0;

   for (int ind = 0; str[ind]; ind++)
      if (tolower(str[ind]) == tolower(chr))
         cont++;

   return cont;
}

/*
   Función: modachr
   Argumento: char str[], Cadena de caracteres a evaluar
   Objetivo: Determinar la letra del alfabeto que más se repite.
   Retorno: (char) El caracter que más se repite.

char modachr(char str[]);*/
char modachr(char str[])
{
   int Alphabet[26] = {0}, max = 0;
   char most = 0;

   for (int index = 0; str[index]; index++)
   {
      char character = tolower(str[index]);
      // char character = str[index] + ('a' - 'A');
      if (character >= 'a' && character <= 'z')
      {
         Alphabet[character - 'a']++;

         if (Alphabet[character - 'a'] > max)
         {
            max = Alphabet[character - 'a'];
            most = character;
         }
      }
   }

   return most ? most : '?';
}

void amayus(char str[])
{
   char chr;

   for (int ind = 0; str[ind]; ind++)
   {
      chr = str[ind];
      if (chr >= 'a' && chr <= 'z') // islower(chr)
         str[ind] -= 'a' - 'A';     // toupper(str[ind]);
                                    /*
                                             'a' - 97  32  'A' - 65
                                             'b' - 98  32  'B' - 66
                                             'c' - 99  32  'C' - 67
                                             ...       32  ...
                                             'z' -122  32  'Z' - 90
                                    */
   }
   printf("AMAYUS: %s\n", str);
}

void aminus(char str[])
{
   char chr;
   for (int ind = 0; str[ind]; ind++)
   {
      chr = str[ind];
      if (chr >= 'A' && chr <= 'Z')
      {
         str[ind] += 'a' - 'A';
      }
   }
   printf("aminus: %s\n", str);
}

void titlecase(char str[])
{
   if (str[0] >= 'a' && str[0] <= 'z')
   {
      // str[0] = toupper(str[0]); This is also correct.
      str[0] = str[0] - ('a' - 'A');
   }
   printf("Titlecase: %s\n", str);
}

void WordCase(char str[])
{
   int newWord = 1;
   for (int index = 0; str[index]; index++)
   {
      char character = str[index];
      if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z')) // This is like the function isalpha();
      {
         if (newWord)
         {
            if (character >= 'a' && character <= 'z')
            {
               str[index] = character - ('a' - 'A');
               // you use x - (a - A) to convert from lower to upper
               // And x + (a - A) from upper to lower
            }
            newWord = 0;
         }
         else
         {
            if (character >= 'A' && character <= 'Z')
            {
               str[index] = character + ('a' - 'A');
            }
         }
      }
      else
      {
         newWord = 1;
      }
   }
   printf("WordCase: %s\n", str);
}

int wordcount(char str[])
{
   int cont = 0;
   for (int ind = 0; str[ind]; ind++)
      if (str[ind] != ' ' && (str[ind + 1] == ' ' || str[ind + 1] == 0))
         cont++;
   return cont;
}

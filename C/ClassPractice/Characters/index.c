#include <stdio.h>

// char modachar(char[]); [TAREA]
void amayus(char str[]);
void aminus(char[]);
int wordcount(char[]);
void titlecase(char[]); // Primera mayuscala, resto minuscula.
void WordCase(char[]);  // Primera letra de cada palabra, en mayuscula, y el resto de la palabra minuscula.

int main()
{
   char frase[] = "Hola mundo.";

   printf("%[^\n]s\n", frase);
   printf("Cantidad de palabras: %d", wordcount(frase));
   return 0;
}

void amayus(char str[])
{
   char chr;
   for (int index = 0; str[index]; index++)
   {
      chr = str[index];
      if (chr >= 'a' && chr <= 'z')
      { // str[index] - 97 - 65
         str[index] -= 'a' - 'A';
      }
   }
}

int wordcount(char str[])
{
   int cont = 0;
   for (int index = 0; str[index]; index++)
   {
      if (str[index] != ' ' && str[index + 1] == ' ' || !str[index + 1])
      {
         cont++;
      }
   }
   return 0;
}
/**
 * funcion: modachr
 * Argumento: char str[], Cadena de caracteres a evaluar
 * Objetivo: Determinar la letra del alfabeto que mas se repite.
 * Return: un caracter
 */
// char modachr(char[])
// {
// }
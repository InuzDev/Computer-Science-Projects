#include <stdio.h>
#include <ctype.h>

int main()
{
   char String[128];
   int n = 10;
   printf("Ingrese su nombre: ");
   fgets(String, n, stdin); // This prevent buffer overflow, avoiding exploits.
   // gets() is vulnerable.
   printf("Su nombre es: ");
   puts(String);

   // Test getchar, and putchar.
   char car, pre = '#';
   while ((car = getchar()) != EOF) // EOF es ctrl + z
   {
      if (pre == ' ' || pre == '#')
         putchar(toupper(car));
      else
         putchar(car);
      pre = car;
   }
   return 0;
}
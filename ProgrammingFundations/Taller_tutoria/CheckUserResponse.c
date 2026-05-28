#include <ctype.h>
#include <stdio.h>

int main() {
   char Res;

   printf("Respuesta usuario: (S o N)");
   scanf("%c", &Res);
   fflush(stdin);
   Res = tolower(Res);
   while (Res != 's' || Res != 'n') {

      if (Res == 's') {
         // Pase algo cuando S
         break;
      } else if (Res == 'n') {
         // Pase algo cuando sea N
         break;
      } else {
         printf("Solo N o S\n");

         scanf("%c", &Res);
         fflush(stdin);
         Res = tolower(Res);
      }
   }

   printf("%c", Res);

   return 0;
}

// Comprobar si la entrada es una v o una H

#include <ctype.h>
#include <stdio.h>

int main() {
   char resp; // Respuesta del usuario
   char character;

   printf("Es un var%cn o una hembra (V/H)?: ", 162);
   scanf("%c ", &resp);

   resp = toupper(resp);

   switch (resp) {
   case 'V':
      puts("Es un enfermero");
      break;
   case 'H':
      puts("Es una maestra");
      break;
   default:
      puts("No es ni enfermero ni maestra");
      break;
   }

   return 0;
}

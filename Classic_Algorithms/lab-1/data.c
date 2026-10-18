#include <stdio.h>
#include <stdlib.h>

int main(void) {
   int *data = malloc(100 * sizeof(int)); // Indices validos: 0 -> 99
   if (data == NULL) {
      return 1;
   }

   data[100] = 0; // Indice fuera de rango.
   printf("Listo\n");
   free(data);

   return 0;
}

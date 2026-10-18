#include <stdio.h>
#include <stdlib.h>

int main(void) {
   int *pointer = malloc(100 * sizeof(int));
   if (pointer == NULL) {
      return 1;
   }

   pointer[0] = 42;
   printf("Reservado, pointer[0] - %d\n", pointer[0]);
   return 0; // Nunca se llama free(pointer);
}

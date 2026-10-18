#include <stdio.h>
#include <stdlib.h>

int main(void) {
   int *data = malloc(100 * sizeof(int));

   if (data == NULL) {
      return 1;
   }

   data[0] = 1;

   free(data + 50);
   printf("Termino sin abortar?\n");

   return 0;
}

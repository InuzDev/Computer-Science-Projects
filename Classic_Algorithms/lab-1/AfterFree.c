#include <stdio.h>
#include <stdlib.h>

int main(void) {
   int *data = malloc(100 * sizeof(int));

   if (data == NULL) {
      return 1;
   }

   data[5] = 123;
   free(data);
   printf("data[5] = %d\n", data[5]);

   return 0;
}

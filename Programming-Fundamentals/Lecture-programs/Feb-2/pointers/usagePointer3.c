#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INIT_VAL 10
#define INC_VAL 5

#define LIMINF 5
#define LIMSUP 90

void genvals(int *, int, int, int);
void showvals(int *, int);
int randrange(int, int);

int main() {
   int *vals;

   srand(time(NULL));

   vals = (int *)malloc(sizeof(int) * INIT_VAL);
   // vals = (int*) calloc(MAX_VALS, sizeofint))

   genvals(vals + INIT_VAL, INC_VAL, LIMINF, LIMSUP);
   printf("\n");
   showvals(vals, INIT_VAL + INC_VAL);

   free(vals);
   return 0;
}

void genvals(int *vals, int nvals, int liminf, int limsup) {
   for (int index = 0; index < nvals; index++) {
      *(vals + index) = randrange(liminf, limsup);
   }
}

void showvals(int *vals, int nvals) {
   for (int index = 0; index < nvals; index++) {
      printf("%d\n", *(vals + index));
   }
}

int randrange(int liminf, int limsup) {
   return rand() % (limsup - liminf + 1) + liminf;
}

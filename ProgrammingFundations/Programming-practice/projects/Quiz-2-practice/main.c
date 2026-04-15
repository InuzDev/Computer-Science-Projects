/**
 * Charles David ; Quiz 2 Practice - Student name matrix management
 *
 * main.c
 */
#include <stdio.h>
#include <string.h>

#define NAME_LEN 32

void initMatrix(int Students, char (*ptr)[NAME_LEN]);
void search(char (*ptr)[NAME_LEN], int Students, char target[]);

int main() {
   int Students = 0;

   do {
      printf("Please, enter a amount of students you want to type in: ");
      scanf("%d", &Students);
      if (Students <= 0) {
         printf("Enter a value higher than 0\n\n");
      }
   } while (Students <= 0);

   /**
    * Use <data type> *ptr_name = &var; for a lineal reading
    * And <data type> (*ptr_name)[J] = matrix for a bidimensional reading
    */

   char names[Students][NAME_LEN];
   char (*ptr)[NAME_LEN] = names;
   // char *ptr = &names[0][0];

   for (int ind = 0; ind < Students; ind++) {
      printf("Enter name %d: ", ind + 1);
      scanf("%31s", *(ptr + ind));
   }

   // ptr + ind jump rows, *(ptr + i) gives that row and scanf writes into it.
   printf("\nAll students:\n");
   for (int ind = 0; ind < Students; ind++) {
      printf("%s\n", *(ptr + ind));
   }

   char target[NAME_LEN];
   printf("\nEnter name to search: ");
   scanf("%31s", target);

   search(ptr, Students, target);

   return 0;
}

void search(char (*ptr)[NAME_LEN], int Students, char target[]) {
   for (int ind = 0; ind < Students; ind++) {
      if (strcmp(*(ptr + ind), target) == 0) {
         printf("Found at position %d\n", ind);
      } else {
         printf("Not found\n");
         return;
      }
   }
}

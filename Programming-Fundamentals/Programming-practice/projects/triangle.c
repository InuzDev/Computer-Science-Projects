/*
 * Charles David ; A program that visualize a triangle
 */

#include <stdio.h>

int main() {
   int num_lines = 5;
   char white = ' ';
   char drawing_point = '*';

   int row, whites, count_as;
   puts(" "); // leave a white space

   for (row = 0; row <= num_lines; row++) {
      putchar('\t');
      // Write spaces
      for (whites = num_lines - row; whites > 0; whites--) {
         putchar(white);
      }

      for (count_as = 1; count_as < 2 * row; count_as++) {
         putchar(drawing_point);
      }
      puts(" ");
   }
   return 0;
}

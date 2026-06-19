#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BASE_BIN 2
#define BASE_OCT 8
#define BASE_DEC 10
#define BASE_HEX 16
#define MAX_ARR_SIZE 65

int getBase();
long convertToDecimal(char *numValue, int base);

int main() {
   char num_1[MAX_ARR_SIZE], num_2[MAX_ARR_SIZE];
   int base;
   long value_1, value_2;

   base = getBase();

   printf("Ingrese el primer valor en base %d: ", base);
   scanf("%s", num_1);

   printf("Ingrese el segundo valor en base %d: ", base);
   scanf("%s", num_2);

   value_1 = convertToDecimal(num_1, base);
   value_2 = convertToDecimal(num_2, base);

   if (value_1 == -1 || value_2 == -1) {
      printf("Error de entrada: Hay valores invalidos\n");
      return 1;
   }

   printf("Resultado de `%s + %s` en base `%d` = %ld (decimal)\n", num_1, num_2, base, (value_1 + value_2));

   return 0;
}

int getBase() {
   int base;

   do {
      printf("Ingresa la base [2, 8, 10, 16]\n");
      scanf("%d", &base);

      if (base != BASE_BIN && base != BASE_OCT && base != BASE_DEC && base != BASE_HEX)
         printf("Base invalida\n");
   } while (base != BASE_BIN && base != BASE_OCT && base != BASE_DEC && base != BASE_HEX);

   return base;
}

long convertToDecimal(char *numValue, int base) {
   long result = 0;
   int len = strlen(numValue);

   for (int index = 0; index < len; index++) {
      char _character = toupper(numValue[index]);
      int digit;

      if (_character >= '0' && _character <= '9') {
         digit = _character - '0';
      } else if (_character >= 'A' && _character <= 'F') {
         digit = _character - 'A' + 10;
      } else {
         printf("Caracter invalido '%c' para base %d\n", numValue[index], base);
         return -1;
      }

      if (digit >= base) {
         printf("El digito '%c' no existen en base %d\n", numValue[index], base);
         return -1;
      }

      result = result * base + digit;
   }

   return result;
}

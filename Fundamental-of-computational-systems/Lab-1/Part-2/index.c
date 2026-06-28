/*
 * Lab 1 ; Part 2 ; Carlos David Jorge ; 1016-5529
 *
 * The main objective of the homework is to
 * implement a stack using linked list (simple or double),
 * using malloc & free, without memory leak.
 *
 * Subobjective: Convert values of different bases (binary, octagesimal, decimal and hexadecimal)
 * - Remember: Successive division [decimal -> (any) base]
 * - Positional evaluation [(any) base -> decimal]
 * Another subobjective, is to identify if there any little-endian or big-endina from the system.
 *
 * The program's objective is:
 * - Show a menu which showcase different options.
 * * Option A: Decimal to (any) base
 * * Option B: (any) base to decimal
 * * Option C: Detect Little-endian or big-endian,
 *   also show the value in 32 bytes, print 4 bytes in memory in the following direction; (Low to High)
 *   And for final, calculate bswap32(x) and print the result and his bytes.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_VAL 0
#define BIT_LENGTH 33
#define MAX_VALIDRESP 4
#define MIN_VALIDRESP 1

#define BIN_BASE 2
#define OCT_BASE 8
#define HEX_BASE 16

typedef struct Node {
   int data;
   struct Node *prev;
   struct Node *next;
} Node;

typedef struct {
   Node *top;
   int size;
} stack;

void PushStack(stack *stk, int value);
bool CheckStack(stack *stk);
void InitStack(stack *stk);
void FreeStack(stack *stk);
int PopStack(stack *stk);

void ResetStack(stack *stk);

void Check_endiannes(int value);

void Convert_bin(int Value, stack *stk);
void Convert_oct(int value, stack *stk);
void Convert_hex(int value, stack *stk);

int Char_to_val(char _character);
int Base_to_dec(const char *input, int base);

int main() {
   char input[64];
   int User_Response, base, converted, value = 0;
   char User_Response_c;
   bool MenuStatus = true, ValidUserResp = false;

   stack stk;
   InitStack(&stk);

   while (MenuStatus) {
      printf("\n\nSelecciona una opci%cn\n", 162);
      printf("\n1: Convertir de decimal a [A] binario, [B] octal, o [C] hexadecimal\n2: Convertir de una base ([A] binario, [B] octal, o [C] hexadecimal) a decimal\n3: Encontrar los endianismo, valor en 32 bytes, y hacer swap de bytes.\n4: Salir\n\n");

      scanf("%d", &User_Response);

      if (User_Response >= MIN_VALIDRESP && User_Response <= MAX_VALIDRESP) {
         ValidUserResp = true;
      }

      while (!ValidUserResp) {
         printf("Por, favor. Elige un valor valido\n");
         scanf("%d", &User_Response);

         // Check again the user response, so the loop breaks.
         if (User_Response >= MIN_VALIDRESP && User_Response <= MAX_VALIDRESP) {
            ValidUserResp = true;
         }
      }

      switch (User_Response) {
      case 1:
         printf("Selecciona la base a la que quiere convertir: [A] Binario, [B] Octal, [C] Hexadecimal\n");
         scanf(" %c", &User_Response_c);

         User_Response_c = toupper(User_Response_c);
         printf("Ingrese un valor en decimal: ");
         scanf("%d", &value);

         if (User_Response_c == 'A') {
            Convert_bin(value, &stk);
         } else if (User_Response_c == 'B') {
            Convert_oct(value, &stk);
         } else if (User_Response_c == 'C') {
            Convert_hex(value, &stk);
         } else {
            printf("Opcion invalida\n");
         }
         break;
      case 2:
         printf("Selecciona la base de origen: [A] Binario, [B] Octal, [C] Hexadecimal\n");
         scanf(" %c", &User_Response_c);
         User_Response_c = toupper(User_Response_c);

         printf("Ingresa un valor: ");
         scanf("%s", input);

         if (User_Response_c == 'A') {
            base = BIN_BASE;
            converted = Base_to_dec(input, base);
         } else if (User_Response_c == 'B') {
            base = OCT_BASE;
            converted = Base_to_dec(input, base);
         } else if (User_Response_c == 'C') {
            base = HEX_BASE;
            converted = Base_to_dec(input, base);
         } else {
            printf("Opcion invalida\n");
         }

         if (converted != -1) {
            printf("Resultado: %d\n", converted);
         }

         break;

      case 3:
         printf("Ingrese un valor entero: ");
         scanf("%d", &value);
         Check_endiannes(value);

         break;
      case 4:
         printf("\nSaliendo\n");
         MenuStatus = false;
      default:
         printf("Opcion invalida");
      }
   }

   return 0;
}

/*
 * Funcion: InitStack
 * Objetivo: inicializar el stack
 */
void InitStack(stack *stk) {
   stk->top = NULL;
}

bool CheckStack(stack *stk) {
   return stk->top == NULL;
}

void PushStack(stack *stk, int value) {
   Node *node = malloc(sizeof(Node));

   node->data = value;
   node->prev = NULL;
   node->next = stk->top;

   if (stk->top != NULL) {
      stk->top->prev = node;
   }

   stk->top = node;
   stk->size++;
}

void FreeStack(stack *stk) {
   while (!CheckStack(stk)) {
      PopStack(stk);
   }
   stk->top = NULL;
   stk->size = 0;
}

int PopStack(stack *stk) {
   if (CheckStack(stk))
      return -1;

   Node *temp = stk->top;
   int value = temp->data;

   stk->top = temp->next;

   if (stk->top != NULL) {
      stk->top->prev = NULL;
   }

   free(temp);
   stk->size--;

   return value;
}

void Convert_bin(int value, stack *stk) {
   if (value == 0) {
      printf("0\n");
      return;
   }

   int temp = value;
   while (temp > 0) {
      PushStack(stk, temp % 2);
      temp /= 2;
   }

   while (!CheckStack(stk)) {
      printf("%d", PopStack(stk));
   }
   printf("\n");
}

void Convert_oct(int value, stack *stk) {
   if (value == 0) {
      printf("0\n");
      return;
   }

   int temp = value;
   while (temp > 0) {
      PushStack(stk, temp % 8);
      temp /= 8;
   }

   while (!CheckStack(stk)) {
      printf("%d", PopStack(stk));
   }
   printf("\n");
}

void Convert_hex(int value, stack *stk) {
   if (value == 0) {
      printf("0\n");
      return;
   }

   char digits[] = "0123456789ABCDEF";
   int temp = value;
   while (temp > 0) {
      PushStack(stk, temp % 16);
      temp /= 16;
   }

   while (!CheckStack(stk)) {
      printf("%c", digits[PopStack(stk)]);
   }
   printf("\n");
}

void Check_endiannes(int value) {
   int probe = 1;
   char *ptr_probe = (char *)&probe;

   printf("Sistema: %s-endian\n", ptr_probe[0] == 1 ? "little" : "big");

   printf("Valor: %d (0x%08X)\n", value, (unsigned int)value);
   printf("Bytes en memoria (low -> high): ");

   unsigned char *bytes = (unsigned char *)&value;
   for (int index = 0; index < 4; index++) {
      printf("[%d]: 0x%02X  ", index, bytes[index]);
   }
   printf("\n");

   unsigned int ux = (unsigned int)value;
   unsigned int swapped = ((ux & 0xFF000000) >> 24) |
                          ((ux & 0x00FF0000) >> 8) |
                          ((ux & 0x0000FF00) << 8) |
                          ((ux & 0x000000FF) << 24);

   printf("bswap32(%d) = %u (0x%08X)\n", value, swapped, swapped);
   printf("Bytes swapped (low -> high): ");

   unsigned char *sbytes = (unsigned char *)&swapped;
   for (int index = 0; index < 4; index++) {
      printf("[%d]: 0x%02X  ", index, sbytes[index]);
   }
   printf("\n");
}

int Char_to_val(char _character) {
   if (_character >= '0' && _character <= '9') {
      return _character - '0';
   }
   if (_character >= 'A' && _character <= 'F') {
      return _character - 'A' + 10;
   }
   if (_character >= 'a' && _character <= 'f') {
      return _character - 'a' + 10;
   }
   return -1;
}

int Base_to_dec(const char *input, int base) {
   int result = 0;
   int power = 1;
   int len = 0;

   while (input[len] != '\0') {
      len++;
   }

   for (int index = len - 1; index >= 0; index--) {
      int digit = Char_to_val(input[index]);

      if (digit == -1) {
         printf("Error: '%c' no es un caracter valid.\n", input[index]);
         return -1;
      }

      if (digit >= base) {
         printf("Error: '%c' no es valido en base %d.\n", input[index], base);
         return -1;
      }

      result += digit * power;
      power *= base;
   }

   return result;
}

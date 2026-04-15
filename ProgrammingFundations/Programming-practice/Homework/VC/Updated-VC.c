#include <conio.c>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CT YELLOW
#define CF BLUE
#define CTS BLUE
#define CFS YELLOW
#define CET BLACK
#define CEF MAGENTA

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

#define ESC 27
#define ENTER 13
#define BKSP 8

#define OPTIONS 5
#define OPT_LENGTH 80

#define ALPHANUM_CAPTURE 0
#define DATE_CAPTURE 1
#define NUM_CAPTURE 2
#define PASS_CAPTURE 3
#define EXIT 4

#define PASSWORD_MIN 8
#define PASSWORD_MAX 20
#define SPECIAL_CHAR "@!$%&*()-+_~"

#define BACKGROUND_COLOR LIGHTGRAY
#define TEXT_COLOR BLUE

#define CSEL_BACKGROUND YELLOW
#define CSEL_TEXT BLUE

#define DEFAULT_CT LIGHTGRAY
#define DEFAULT_CF BLACK

typedef struct {
   int dd, mm, yyyy;
} FECHA;

void show_header(char[][OPT_LENGTH], int, int, int, int);
int menu_movement(char[][OPT_LENGTH], int, int, int, int);
void set_color(int, int);
void color_default();

void show_field(char *, int, int, int, int);
void capture_alphanumeric(char *, int, int, int);
FECHA captureDate(char *, int, int, int);
void CaptureInteger(char *, int *, int, int, int);
void CaptureFloat(char *, int *, int, int, int);
void capture_numeric(char *, int, int, int, int, int);
void capture_password(char *, int, int, int);
int vcapture(int, int, const char *, ...);

int validate_date(int, int, int);
int validate_password(char *);
int validate_alpha_char(char);
int has_special(char *);
int has_upper(char *);
int has_lower(char *);
int has_number(char *);
int valid_length(char *);

/*
Funcion: vcapture
Argumentos:
   int pos_x        : posicion horizontal (columna) donde se mostrara el campo
   int pos_y        : posicion vertical   (fila)    donde se mostrara el campo
   const char *fmt  : cadena de formato que indica el tipo de captura:

      %[ancho]s      - Alfanumerico. Ancho opcional indica el maximo de caracteres.
                       Ejemplo: %30s captura hasta 30 caracteres en un char*.

      %D             - Fecha en formato dd/mm/yyyy con validacion automatica.
                       Requiere un puntero FECHA* como argumento.
                       Ejemplo: %D  ->  vcapture(x, y, "%D", &mi_fecha)

      %[ancho]i      - Entero. Ancho opcional indica el maximo de digitos.
                       Requiere un int* como argumento.
                       Ejemplo: %5i captura hasta 5 digitos en un int*.

      %[antes].[despues]lf
                     - Double con precision controlada.
                       'antes'   = maximo de digitos antes del punto decimal.
                       'despues' = digitos despues del punto decimal.
                       Requiere un double* como argumento.
                       Ejemplo: %8.2lf captura un numero de hasta 8.2 digitos.
                       Nota: para precision dinamica, construya el formato con sprintf:
                             sprintf(fmt, "%%%d.%dlf", antes, despues);

      %p             - Password con validacion de requisitos de seguridad
                       (mayusculas, minusculas, numeros, caracteres especiales,
                       longitud entre PASSWORD_MIN y PASSWORD_MAX).
                       Requiere un char* como argumento.
                       Ejemplo: %p  ->  vcapture(x, y, "%p", clave)

   ...  : punteros a las variables donde se guardaran los resultados,
          en el mismo orden que los especificadores en fmt.

Objetivo:
   Funcion unificada de captura de datos con formato estilo scanf.
   Permite capturar distintos tipos de dato con una sola llamada,
   delegando internamente a la funcion de captura correspondiente.
   Mantiene la misma interfaz visual interactiva (flechas, BKSP, ENTER, ESC)
   que las funciones individuales.

Retorno:
   int - numero de campos capturados exitosamente.

Ejemplo de uso:
   char nombre[31]  = {0};
   FECHA nacimiento = {0};
   int   edad       = 0;
   double precio    = 0.0;
   char  clave[21]  = {0};
   char  num_fmt[20];

   vcapture(10,  5, "%30s",    nombre);
   vcapture(10,  7, "%D",      &nacimiento);
   vcapture(10,  9, "%5i",     &edad);
   vcapture(10, 11, "%8.2lf",  &precio);
   vcapture(10, 13, "%p",      clave);

   // Precision dinamica para doubles:
   int antes = 4, despues = 2;
   sprintf(num_fmt, "%%%d.%dlf", antes, despues);
   vcapture(10, 15, num_fmt, &precio);
*/
int vcapture(int pos_x, int pos_y, const char *fmt, ...) {
   va_list args;
   va_start(args, fmt);

   int i = 0;
   int captured = 0;

   while (fmt[i]) {
      if (fmt[i] != '%') {
         i++;
         continue;
      }
      i++; /* skip '%' */

      /* --- parse optional width --- */
      int width = 0;
      while (fmt[i] >= '0' && fmt[i] <= '9') {
         width = width * 10 + (fmt[i] - '0');
         i++;
      }

      /* --- parse optional precision (.N) --- */
      int precision = 0;
      if (fmt[i] == '.') {
         i++; /* skip '.' */
         while (fmt[i] >= '0' && fmt[i] <= '9') {
            precision = precision * 10 + (fmt[i] - '0');
            i++;
         }
      }

      char spec = fmt[i++];

      switch (spec) {

      /* %[ancho]s  ->  cadena alfanumerica */
      case 's': {
         char *result = va_arg(args, char *);
         int max = (width > 0) ? width : 80;
         capture_alphanumeric(result, max, pos_x, pos_y);
         captured++;
         break;
      }

      /* %D  ->  fecha dd/mm/yyyy (FECHA*) */
      case 'D': {
         FECHA *result = va_arg(args, FECHA *);
         char buf[11] = {0};
         *result = captureDate(buf, 10, pos_x, pos_y);
         captured++;
         break;
      }

      /* %[ancho]i  ->  entero (int*) */
      case 'i': {
         int *result = va_arg(args, int *);
         char buf[20] = {0};
         int max_digits = (width > 0) ? width : 10;
         CaptureInteger(buf, result, max_digits, pos_x, pos_y);
         captured++;
         break;
      }

      /* %[antes].[despues]lf  ->  double (double*) */
      case 'l': {
         if (fmt[i] == 'f') {
            i++; /* skip 'f' */
            double *result = va_arg(args, double *);
            char buf[50] = {0};
            int before = (width > 0) ? width : 10;
            int after = (precision > 0) ? precision : 2;
            int total = before + after + 1; /* +1 por el punto */
            capture_numeric(buf, before, after, total, pos_x, pos_y);
            *result = atof(buf);
            captured++;
         }
         break;
      }

      /* %p  ->  password (char*) */
      case 'p': {
         char *result = va_arg(args, char *);
         capture_password(result, PASSWORD_MAX, pos_x, pos_y);
         captured++;
         break;
      }

      default:
         break;
      }
   }

   va_end(args);
   return captured;
}

int main() {
   system("cls");
   char menu[][OPT_LENGTH] = {"Alfanumerico", "Fecha", "Numerico con Precision", "Password", "Salir"};

   int menu_x = 10;
   int menu_y = 6;
   int selected_option = 0;

   int digits_before_decimal = 0;
   int digits_after_decimal = 0;

   char continue_key;
   FECHA fecha_result;

   do {
      selected_option = menu_movement(menu, OPTIONS, menu_x, menu_y, selected_option);

      char field_buffer[80] = {0};

      if (selected_option == ALPHANUM_CAPTURE) {
         system("cls");
         gotoxy(10, 5);
         printf("=== CAPTURA ALFANUMERICA ===");
         gotoxy(10, 7);
         printf("Campo (max 30 chars):");
         vcapture(menu_x + 25, menu_y + 1, "%30s", field_buffer);

      } else if (selected_option == DATE_CAPTURE) {
         system("cls");
         gotoxy(10, 5);
         printf("=== CAPTURA DE FECHA ===");
         gotoxy(10, 7);
         printf("Fecha (dd/mm/yyyy):");
         vcapture(menu_x + 25, menu_y + 2, "%D", &fecha_result);

      } else if (selected_option == NUM_CAPTURE) {
         system("cls");
         gotoxy(10, 5);
         printf("=== CAPTURA NUMERICA ===");

         gotoxy(10, 7);
         printf("Digitos antes del punto:");
         vcapture(menu_x + 25, menu_y + 3, "%2i", &digits_before_decimal);

         gotoxy(10, 9);
         printf("Digitos despues del punto:");
         vcapture(menu_x + 25, menu_y + 5, "%2i", &digits_after_decimal);

         gotoxy(10, 11);
         printf("Numero (formato %d.%d):", digits_before_decimal, digits_after_decimal);

         char num_fmt[20];
         double num_result = 0.0;
         sprintf(num_fmt, "%%%d.%dlf", digits_before_decimal, digits_after_decimal);
         vcapture(menu_x + 45, menu_y + 3, num_fmt, &num_result);
         sprintf(field_buffer, "%.*f", digits_after_decimal, num_result);

      } else if (selected_option == PASS_CAPTURE) {
         system("cls");
         gotoxy(10, 5);
         printf("=== CAPTURA DE PASSWORD ===");
         gotoxy(10, 7);
         printf("Password (8-20 chars):");
         vcapture(menu_x + 25, menu_y + 4, "%p", field_buffer);

      } else if (selected_option == EXIT) {
         system("cls");
         return 0;
      }

      gotoxy(menu_x, menu_y + 6);
      if (selected_option != 1) {
         printf("Resultado: %s%30c", field_buffer, ' ');
      } else {
         printf("Resultado: %02d/%02d/%04d%30c", fecha_result.dd, fecha_result.mm, fecha_result.yyyy, ' ');
      }

      gotoxy(menu_x, menu_y + 8);
      printf("Presione cualquier tecla para continuar (ESC para salir)...");

      continue_key = getch();

      system("cls");

   } while (continue_key != ESC);

   return 0;
}

/*
Funcion: show_menu
Argumentos:
   char menu[][OPT_LENGTH]: matriz de cadenas con las opciones del menu
   int total_options: cantidad total de opciones en el menu
   int pos_y: posicion vertical (fila) donde se mostrara el menu
   int pos_x: posicion horizontal (columna) donde se mostrara el menu
   int selected_index: indice de la opcion actualmente seleccionada (0 a total_options-1)

Objetivo:
   Mostrar el menu en pantalla, resaltando visualmente la opcion seleccionada
   con colores diferentes al resto de las opciones
*/
void show_header(char menu[][OPT_LENGTH], int total_options, int pos_y, int pos_x, int selected_index) {

   gotoxy(pos_x, pos_y);
   set_color(CET, CEF);
   printf("%25s%5c", "Validador de campos", ' ');

   int i = 0;
   for (; i < total_options; i++) {
      set_color(CT, CF);
      gotoxy(pos_x, pos_y + i + 1);

      if (selected_index == i)
         set_color(CTS, CFS);

      printf("%25s%5c", menu[i], ' ');
   }

   color_default();
}

/*
Funcion: menu_movement
Argumentos:
   char menu[][OPT_LENGTH]: matriz de cadenas con las opciones del menu
   int total_options: cantidad total de opciones disponibles
   int pos_y: posicion vertical donde se muestra el menu
   int pos_x: posicion horizontal donde se muestra el menu
   int selected_index: indice de la opcion inicialmente seleccionada

Objetivo:
   Capturar el movimiento del usuario entre las opciones del menu usando
   las teclas de flecha arriba/abajo, y permitir seleccionar con ENTER

Retorno:
   int - el indice de la opcion seleccionada por el usuario (0 a total_options-1)
*/
int menu_movement(char menu[][OPT_LENGTH], int total_options, int pos_y, int pos_x, int selected_index) {
   char key;

   _setcursortype(0);

   do {
      show_header(menu, total_options, pos_x, pos_y, selected_index);

      do {
         key = getch();
      } while (key != ESC && key != ENTER && key != UP && key != DOWN);

      if (key != ESC) {
         if (key == UP) {
            if (selected_index != 0)
               selected_index--;
            else
               selected_index = total_options - 1;
         }
         if (key == DOWN) {
            if (selected_index < total_options - 1)
               selected_index++;
            else
               selected_index = 0;
         }
      } else {
         selected_index = total_options - 1;
      }
   } while (key != ESC && key != ENTER);

   _setcursortype(100);

   return selected_index;
}

/*
Funcion: show_field
Argumentos:
   char *field_str: puntero al string que contiene el contenido actual del campo
   int cursor_position: posicion actual del cursor dentro del campo (0 a max_length-1)
   int max_length: longitud maxima del campo en caracteres
   int pos_x: posicion horizontal (columna) donde inicia el campo
   int pos_y: posicion vertical (fila) donde se muestra el campo

Objetivo:
   Mostrar visualmente el campo de entrada con fondo de color, el contenido
   actual, y posicionar el cursor en la ubicacion correcta para que el usuario
   pueda ver donde esta escribiendo
*/
void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y) {
   int i;

   set_color(TEXT_COLOR, BACKGROUND_COLOR);

   for (i = 0; i < max_length; i++) {
      gotoxy(pos_x + i + 1, pos_y);
      if (*(field_str + i))
         printf("%c", *(field_str + i));
      else
         printf(" ");
   }

   color_default();
   gotoxy(pos_x + cursor_position + 1, pos_y);
}

/*
Funcion: capture_alphanumeric
Argumentos:
   char *result_str: puntero donde se almacenara el texto capturado
   int max_length: cantidad maxima de caracteres permitidos en el campo
   int pos_x: posicion horizontal donde se mostrara el campo
   int pos_y: posicion vertical donde se mostrara el campo

Objetivo:
   Capturar texto alfanumerico del usuario, permitiendo:
   - Escribir caracteres validos (letras, numeros, simbolos)
   - Moverse con flechas izquierda/derecha
   - Borrar con backspace
   - Confirmar con ENTER o cancelar con ESC

Retorno: void (el resultado se guarda en result_str)
*/
void capture_alphanumeric(char *result_str, int max_length, int pos_x, int pos_y) {
   int cursor_index = 0;
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(result_str, cursor_index, max_length, pos_x, pos_y);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while (!validate_alpha_char(pressed_key) && pressed_key != ENTER && pressed_key != ESC &&
               pressed_key != LEFT && pressed_key != BKSP && pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_length - 1)
            cursor_index++;

      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;

      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BKSP) {
            if (cursor_index) {
               cursor_index--;
               strcpy(result_str + cursor_index, result_str + cursor_index + 1);
            }
         } else {
            if (cursor_index < max_length) {
               *(result_str + cursor_index) = pressed_key;
               cursor_index++;
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC)
      *(result_str + cursor_index) = '\0';
}

/*
Funcion: capture_date
Argumentos:
   char *date_str: puntero donde se almacenara la fecha como string (dd/mm/yyyy)
   int field_length: longitud del campo (debe ser 10 para dd/mm/yyyy)
   int pos_x: posicion horizontal del campo
   int pos_y: posicion vertical del campo

Objetivo:
   Capturar una fecha en formato dd/mm/yyyy con validacion automatica:
   - Inserta automaticamente las barras '/' en posiciones 2 y 5
   - Solo acepta digitos numericos
   - Valida que la fecha sea real (dias por mes, años bisiestos)
   - Permite navegacion con flechas y borrado con backspace

Retorno:
   FECHA - estructura con los campos dd, mm, yyyy de la fecha validada
*/
FECHA captureDate(char *date_str, int field_length, int Xpos, int Ypos) {
   int cursor_index = 0;
   char pressed_key;
   FECHA result_date;

   _setcursortype(100);

   do {
      *(date_str + 2) = '/';
      *(date_str + 5) = '/';
      show_field(date_str, cursor_index, field_length, Xpos, Ypos);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while ((pressed_key < '0' || pressed_key > '9') && pressed_key != ENTER && pressed_key != ESC &&
               pressed_key != LEFT && pressed_key != BKSP && pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < field_length - 1) {
            cursor_index++;
            if (cursor_index == 2 || cursor_index == 5)
               cursor_index++;
         }
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0) {
            cursor_index--;
            if (cursor_index == 2 || cursor_index == 5)
               cursor_index--;
         }
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BKSP) {
            if (cursor_index) {
               cursor_index--;
               if (cursor_index == 2 || cursor_index == 5)
                  cursor_index--;
               strcpy(date_str + cursor_index, date_str + cursor_index + 1);
            }
         } else {
            if (cursor_index != 2 && cursor_index != 5 && cursor_index < field_length) {
               *(date_str + cursor_index) = pressed_key;
               cursor_index++;

               if (cursor_index == 2 || cursor_index == 5)
                  cursor_index++;
            }
         }
      }

      if (pressed_key == ENTER) {
         *(date_str + cursor_index) = '\0';

         char day_str[3] = {date_str[0], date_str[1], '\0'};
         char month_str[3] = {date_str[3], date_str[4], '\0'};
         char year_str[5] = {date_str[6], date_str[7], date_str[8], date_str[9], '\0'};

         result_date.dd = atoi(day_str);
         result_date.mm = atoi(month_str);
         result_date.yyyy = atoi(year_str);

         if (validate_date(result_date.dd, result_date.mm, result_date.yyyy)) {
            gotoxy(Xpos, Ypos + 2);
            set_color(GREEN, BLACK);
            printf("Fecha valida!%20c", ' ');
            color_default();
            break;
         } else {
            gotoxy(Xpos, Ypos + 2);
            set_color(RED, BLACK);
            printf("Fecha invalida! Intente de nuevo.%20c", ' ');
            color_default();
            memset(date_str, 0, field_length);
            cursor_index = 0;
         }
      }
   } while (pressed_key != ESC);

   return result_date;
}

/*
Funcion: capture_numeric_digits
Argumentos:
   char *digit_str: puntero donde se almacena el numero capturado como string
   int *result_value: puntero donde se almacenara el valor numerico convertido
   int max_digits: cantidad maxima de digitos permitidos
   int pos_x: posicion horizontal del campo
   int pos_y: posicion vertical del campo

Objetivo:
   Capturar solo digitos numericos (0-9) del usuario, usado para especificar
   cuantos digitos antes o despues del punto decimal se permiten en un numero

Retorno: void (resultado se guarda en result_value como entero)
*/
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int pos_x, int pos_y) {
   int cursor_index = 0;
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(digit_str, cursor_index, max_digits, pos_x, pos_y);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while ((pressed_key < '0' || pressed_key > '9') && pressed_key != ENTER && pressed_key != ESC &&
               pressed_key != LEFT && pressed_key != BKSP && pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_digits - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BKSP) {
            if (cursor_index) {
               cursor_index--;
               strcpy(digit_str + cursor_index, digit_str + cursor_index + 1);
            }
         } else {
            if (cursor_index < max_digits) {
               *(digit_str + cursor_index) = pressed_key;
               cursor_index++;
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC) {
      *(digit_str + cursor_index) = '\0';
      *result_value = atoi(digit_str);
   }
}

/*
Funcion: capture_numeric_decimals
Argumentos:
   (iguales a capture_numeric_digits)

Objetivo:
   Alias de capture_numeric_digits, usado especificamente para capturar
   la cantidad de decimales deseados

Retorno: void
*/
void CaptureFloat(char *decimal_str, int *result_value, int max_digits, int pos_x, int pos_y) {
   CaptureInteger(decimal_str, result_value, max_digits, pos_x, pos_y);
}

/*
Funcion: capture_numeric
Argumentos:
   char *number_str: puntero donde se almacena el numero capturado
   int max_digits_before: cantidad maxima de digitos antes del punto decimal
   int max_digits_after: cantidad maxima de digitos despues del punto decimal
   int total_length: longitud total del campo (digitos + punto)
   int pos_x: posicion horizontal del campo
   int pos_y: posicion vertical del campo

Objetivo:
   Capturar un numero decimal con precision controlada:
   - Limita digitos antes y despues del punto decimal
   - Inserta automaticamente el punto decimal cuando es necesario
   - Auto-justifica el numero cuando el usuario ingresa el punto
     (ejemplo: si escribe "5." con max 3 digitos antes, lo convierte a "  5.")

Retorno: void (resultado en number_str)
*/
void capture_numeric(char *number_str, int max_digits_before, int max_digits_after,
                     int total_length, int pos_x, int pos_y) {
   int cursor_index = 0;
   int count_before_decimal = 0;
   int count_after_decimal = 0;
   int decimal_inserted = 0;
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(number_str, cursor_index, total_length, pos_x, pos_y);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while ((pressed_key < '0' || pressed_key > '9') && pressed_key != '.' && pressed_key != ENTER &&
               pressed_key != ESC && pressed_key != LEFT && pressed_key != BKSP && pressed_key != RIGHT);

      if (pressed_key == '.' && decimal_inserted == 0) {
         char temp[100];
         char empty[100] = {0};
         strcpy(temp, number_str);
         strcpy(number_str, empty);
         strcpy(number_str + (max_digits_before - cursor_index - 1), temp);
         cursor_index = (max_digits_before - cursor_index);
      }

      if (pressed_key == RIGHT) {
         if (cursor_index < total_length - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BKSP) {
            if (cursor_index) {
               cursor_index--;
               if (decimal_inserted != 0) {
                  count_after_decimal--;
               } else {
                  count_before_decimal--;
               }
               if (*(number_str + cursor_index) == '.') {
                  decimal_inserted = 0;
                  count_after_decimal = 0;
               }

               strcpy(number_str + cursor_index, number_str + cursor_index + 1);
            }
         } else {
            if (cursor_index < total_length) {
               if (count_before_decimal < max_digits_before - 1 && decimal_inserted == 0) {
                  *(number_str + cursor_index) = pressed_key;
                  cursor_index++;
                  count_before_decimal++;
               } else if (count_before_decimal == max_digits_before - 1 && decimal_inserted == 0) {
                  *(number_str + cursor_index) = '.';
                  decimal_inserted++;
                  cursor_index++;
                  *(number_str + cursor_index) = pressed_key;
                  cursor_index++;
                  count_after_decimal++;
               } else if ((count_before_decimal > max_digits_before - 1 || decimal_inserted != 0) && count_after_decimal < max_digits_after) {
                  *(number_str + cursor_index) = pressed_key;
                  count_after_decimal++;
                  cursor_index++;
               }
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC)
      *(number_str + cursor_index) = '\0';
}

/*
Funcion: capture_password
Argumentos:
   char *password_str: puntero donde se almacena la contrasena capturada
   int max_length: longitud maxima de la contrasena (PASSWORD_MAX = 20)
   int pos_x: posicion horizontal del campo
   int pos_y: posicion vertical del campo

Objetivo:
   Capturar una contrasena de forma segura:
   - Muestra asteriscos (*) en lugar de los caracteres reales
   - No permite espacios
   - Valida requisitos: longitud (8-20), mayusculas, minusculas, numeros,
     caracteres especiales
   - Muestra mensajes de error indicando que falta si la contrasena no es valida

Retorno: void (resultado en password_str)
*/
void capture_password(char *password_str, int max_length, int pos_x, int pos_y) {
   int cursor_index = 0;
   char pressed_key;
   char hidden_display[PASSWORD_MAX + 1] = {0};

   _setcursortype(100);

   do {
      show_field(hidden_display, cursor_index, PASSWORD_MAX, pos_x, pos_y);

      gotoxy(pos_x, pos_y + 2);
      set_color(YELLOW, BLACK);
      printf("********************");

      gotoxy(pos_x, pos_y + 3);
      if (has_upper(password_str)) {
         set_color(GREEN, BLACK);
         printf("%c ", 251);
      } else {
         set_color(RED, BLACK);
         printf("  ");
      }
      set_color(WHITE, BLACK);
      printf("Letra mayuscula%20c", ' ');

      gotoxy(pos_x, pos_y + 4);
      if (has_lower(password_str)) {
         set_color(GREEN, BLACK);
         printf("%c ", 251);
      } else {
         set_color(RED, BLACK);
         printf("  ");
      }
      set_color(WHITE, BLACK);
      printf("Letra minuscula%20c", ' ');

      gotoxy(pos_x, pos_y + 5);
      if (has_special(password_str)) {
         set_color(GREEN, BLACK);
         printf("%c ", 251);
      } else {
         set_color(RED, BLACK);
         printf("  ");
      }
      set_color(WHITE, BLACK);
      printf("Caracteres especiales (@,!,$,%%,&,*,(,),-,+,_,~)%20c", ' ');

      gotoxy(pos_x, pos_y + 6);
      if (has_number(password_str)) {
         set_color(GREEN, BLACK);
         printf("%c ", 251);
      } else {
         set_color(RED, BLACK);
         printf("  ");
      }
      set_color(WHITE, BLACK);
      printf("Numero%20c", ' ');

      gotoxy(pos_x, pos_y + 7);
      if (valid_length(password_str)) {
         set_color(GREEN, BLACK);
         printf("%c ", 251);
      } else {
         set_color(RED, BLACK);
         printf("  ");
      }
      set_color(WHITE, BLACK);
      printf("Al menos 8 caracteres%20c", ' ');

      color_default();

      gotoxy(pos_x + cursor_index + 1, pos_y);

      fflush(stdin);
      do {
         pressed_key = getch();
      } while (!validate_alpha_char(pressed_key) && pressed_key != ENTER && pressed_key != ESC &&
               pressed_key != LEFT && pressed_key != BKSP && pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_length - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BKSP) {
            if (cursor_index) {
               cursor_index--;
               strcpy(password_str + cursor_index, password_str + cursor_index + 1);
               strcpy(hidden_display + cursor_index, hidden_display + cursor_index + 1);
            }
         } else {
            if (cursor_index < max_length && pressed_key != ' ') {
               *(password_str + cursor_index) = pressed_key;
               *(hidden_display + cursor_index) = '*';
               cursor_index++;
            }
         }
      }

      if (pressed_key == ENTER) {
         *(password_str + cursor_index) = '\0';

         if (validate_password(password_str)) {
            for (int i = 2; i < 8; i++) {
               gotoxy(pos_x, pos_y + i);
               printf("%80c", ' ');
            }

            gotoxy(pos_x, pos_y + 2);
            set_color(GREEN, BLACK);
            printf("Contrasena valida!%15c", ' ');
            color_default();
            break;
         } else {
            gotoxy(pos_x, pos_y + 9);
            set_color(RED, BLACK);
            printf("Debe cumplir todos los requisitos arriba%20c", ' ');
            color_default();
         }
      }
   } while (pressed_key != ESC);
}

/*
Funcion: validate_date
Argumentos:
   int day: dia del mes (1-31)
   int month: mes del año (1-12)
   int year: año (se valida para años bisiestos)

Objetivo:
   Validar que una fecha sea real y posible:
   - Febrero tiene 28 dias (29 en años bisiestos)
   - Abril, Junio, Septiembre, Noviembre tienen 30 dias
   - El resto de meses tienen 31 dias

Retorno:
   int - 1 si la fecha es valida, 0 si es invalida
*/
int validate_date(int day, int month, int year) {
   if (month >= 1 && month <= 12) {
      if (month == 2) {
         if (day >= 1 && day <= 28) {
            return 1;
         } else if (day == 29 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))) {
            return 1;
         }
      } else if (month == 4 || month == 6 || month == 9 || month == 11) {
         if (day >= 1 && day <= 30) {
            return 1;
         }
      } else {
         if (day >= 1 && day <= 31) {
            return 1;
         }
      }
   }

   return 0;
}

/*
Funcion: validate_alpha_char
Argumentos:
   char test_char: caracter a validar

Objetivo:
   Determinar si un caracter es valido para campos alfanumericos
   (letras, numeros, espacios, simbolos de puntuacion)

Retorno:
   int - 1 si el caracter es valido, 0 si no lo es
*/
int validate_alpha_char(char test_char) {
   char valids[] = " qwertyuiopQWERTYUIOPASDFGHJKLZXCVBNM[]\\|asdfghjkl;'"
                   "zxcvbnm,./1234567890-=%%`~/*-+=!@#$%%^&*()";

   for (int i = 0; valids[i]; i++) {
      if (test_char == valids[i]) {
         return 1;
      }
   }
   return 0;
}

/*
Funcion: validate_password
Argumentos:
   char *password_str: cadena con la contrasena a validar

Objetivo:
   Validar que una contrasena cumpla todos los requisitos de seguridad:
   - Tiene al menos un caracter especial
   - Tiene al menos un numero
   - Tiene al menos una letra minuscula
   - Tiene al menos una letra mayuscula
   - Longitud entre PASSWORD_MIN (8) y PASSWORD_MAX (20)

Retorno:
   int - 1 si la contrasena es valida, 0 si no cumple todos los requisitos
*/
int validate_password(char *password_str) {
   return (has_special(password_str) && has_number(password_str) && has_lower(password_str) &&
           has_upper(password_str) && valid_length(password_str));
}

/*
Funcion: has_special
Argumentos:
   char *test_str: cadena a verificar

Objetivo:
   Verificar si la cadena contiene al menos un caracter especial
   de los definidos en SPECIAL_CHAR (@!$%&*()-+_~)

Retorno:
   int - 1 si tiene al menos un caracter especial, 0 si no tiene ninguno
*/
int has_special(char *test_str) {
   int i, j, count = 0;

   for (i = 0; test_str[i]; i++) {
      for (j = 0; SPECIAL_CHAR[j]; j++) {
         if (test_str[i] == SPECIAL_CHAR[j]) {
            count++;
            break;
         }
      }
   }

   return (count > 0 ? 1 : 0);
}

/*
Funcion: has_upper
Argumentos:
   char *test_str: cadena a verificar

Objetivo:
   Verificar si la cadena contiene al menos una letra mayuscula (A-Z)

Retorno:
   int - 1 si tiene al menos una mayuscula, 0 si no tiene ninguna
*/
int has_upper(char *test_str) {
   int i, count = 0;

   for (i = 0; test_str[i]; i++) {
      if (test_str[i] >= 'A' && test_str[i] <= 'Z') {
         count++;
      }
   }

   return (count > 0 ? 1 : 0);
}

/*
Funcion: has_lower
Argumentos:
   char *test_str: cadena a verificar

Objetivo:
   Verificar si la cadena contiene al menos una letra minuscula (a-z)

Retorno:
   int - 1 si tiene al menos una minuscula, 0 si no tiene ninguna
*/
int has_lower(char *test_str) {
   int i, count = 0;

   for (i = 0; test_str[i]; i++) {
      if (test_str[i] >= 'a' && test_str[i] <= 'z') {
         count++;
      }
   }

   return (count > 0 ? 1 : 0);
}

/*
Funcion: has_number
Argumentos:
   char *test_str: cadena a verificar

Objetivo:
   Verificar si la cadena contiene al menos un digito numerico (0-9)

Retorno:
   int - 1 si tiene al menos un numero, 0 si no tiene ninguno
*/
int has_number(char *test_str) {
   int i, count = 0;

   for (i = 0; test_str[i]; i++) {
      if (test_str[i] >= '0' && test_str[i] <= '9') {
         count++;
      }
   }

   return (count > 0 ? 1 : 0);
}

/*
Funcion: valid_length
Argumentos:
   char *test_str: cadena a verificar

Objetivo:
   Verificar que la longitud de la cadena este entre PASSWORD_MIN (8)
   y PASSWORD_MAX (20) caracteres

Retorno:
   int - 1 si la longitud es valida, 0 si es muy corta o muy larga
*/
int valid_length(char *test_str) {
   int size = strlen(test_str);
   return ((size >= PASSWORD_MIN && size <= PASSWORD_MAX) ? 1 : 0);
}

/*
Funcion: set_color
Argumentos:
   int text_color: color del texto (constantes de conio.c: RED, GREEN, BLUE, etc)
   int background_color: color del fondo

Objetivo:
   Cambiar el color del texto y del fondo para la salida en consola
*/
void set_color(int text_color, int background_color) {
   textcolor(text_color);
   textbackground(background_color);
}

/*
Funcion: color_default
Argumentos: ninguno

Objetivo:
   Restaurar los colores por defecto de la consola (texto gris claro, fondo negro)
*/
void color_default(void) {
   set_color(LIGHTGRAY, BLACK);
}

#include <conio.c>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ALPHA 224
#define __UNDEFINED 0

#define PROVIDER_FIELDS 4
#define ARTICLE_FIELDS 4
#define ARTICLEPROVIDER_FIELDS 3

#define FORM_LABEL_X 10
#define FORM_VALUE_X 30
#define FORM_START_Y 5

#define TITLE_COLOR_BG BLACK
#define TITLE_COLOR_FONT LIGHTGRAY
#define WARNING_CT RED
#define WARNING_CF BLACK
#define CT BLACK
#define CF CYAN
#define CTS BLACK
#define CFS YELLOW

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ESC 27
#define ENTER 13
#define BACKSPACE 8
#define TAB 9

#define INIT_X_POS 1
#define INIT_Y_POS 1

#define OPTS 4
#define OPT_LENGTH 81
#define PROCESS_OPTS 3
#define CONSULT_OPTS 3
#define CRUD_OPTS 4
#define CRUD_SUBOPTS 3

#define BUFFER 81
#define ARTICLEPROVIDER_PRICE_FIELD_BUFFER 8
#define DAY_BUFFER 3
#define MONTH_BUFFER 3
#define YEAR_BUFFER 5
#define QUANTITY_BUFFER 4
#define REORDER_BUFFER 5

#define MAXID 6
#define LENNOMB 21
#define LENAPEL 16
#define LENDESC 26
#define LENDIR 31
#define LENPWD 21

typedef enum {
   FIELD_ALPHA,
   FIELD_INT,
   FIELD_FLOAT,
   FIELD_DATE,
   FIELD_READONLY
} FieldType;

typedef struct {
   int day, month, year;
} __DATE;

typedef struct {
   char Id[MAXID];
   char Nombre[LENNOMB];
   char Apellido[LENAPEL];
   __DATE Nacimiento;
} CLIENTE;

typedef struct {
   char Id[MAXID];
   char Nombre[LENNOMB];
   char Apellido[LENAPEL];
   char PassWord[LENPWD];
   __DATE Nacimiento;
   char Id_jefe[MAXID];
} USUARIO;

typedef struct {
   char Id[MAXID];
   char Descripcion[LENDESC];
   int cantidad;
   int reorden;
} ARTICULO;

typedef struct {
   char Id[MAXID];
   char Descripcion[LENDESC];
   char Direccion[LENDIR];
   __DATE Nacimiento;
} PROVEEDOR;

typedef struct {
   char Id_Art[MAXID];
   char Id_Prov[MAXID];
   float precio;
} ARTICULOPROVEEDOR;

typedef struct {
   char numfactura[MAXID];
   char IdCliente[MAXID];
   char IdUsuario[MAXID];
   __DATE Fecha;
} FACTURA;

typedef struct {
   char numfactura[MAXID];
   char IdArt[MAXID];
   char IdProv[MAXID];
   int cantidad;
   float precio;
} DETALLEFACTURA;

typedef struct {
   char numpago[MAXID];
   char numfactura[MAXID];
   float monto;
} PAGO;

typedef struct {
   char Id_Art[MAXID];
   char Id_Prov[MAXID];
} ArticleProviderKey;

typedef struct {
   char *header;
   void (*getfield)(void *data, char *buff);
} Column;

typedef struct GenericNode {
   void *data;
   struct GenericNode *next;
   struct GenericNode *prev;
} GenericNode;

typedef struct {
   GenericNode *head;
   GenericNode *tail;
   int Quantity;
} GenericList;

typedef struct {
   int size;
   int (*compare)(void *inputA, void *inputB);
   int (*SearchKey)(void *data, void *key);
   void (*show)(void *data);
   void (*read)(void *data);
} CRUD_Operations;

typedef struct {
   char *label;
   void *valuePtr;
   FieldType type;
   int maxLength;
} FormField;

void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char character));
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int xpos, int ypos);
void CaptureFloat(char *decimal_str, int *result_value, int max_digits, int xpos, int ypos);
void CaptureDate(__DATE *date, int xpos, int ypos);
int validateAlphachar(char character);
int ValidDate(int day, int month, int year);

void Cleanup(GenericList *list, FILE *file);

void setcolor(int ct, int cf);
void colordefault();

void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y);
void show_header(char menu[][OPT_LENGTH], int options, int xpos, int ypos, int isSelected);
int ShowForm(FormField *fields, int numFields, char *title);
int show_dropdown(char items[][OPT_LENGTH], int count, int xpos, int ypos);

int get_option_xpos(char menu[][OPT_LENGTH], int targetIndex, int startX);

int ConfirmDelete(char *name);

int GetKeyClient(void *data, void *key);
int CompareClient(void *inputA, void *inputB);
void ReadClient(void *data);
void ShowClient(void *data);

int GetKeyUser(void *data, void *key);
int CompareUser(void *inputA, void *inputB);
void ReadUser(void *data);
void ShowUser(void *data);

void GenerateArticleId(char *newId, FILE *file);

int GetKeyArticle(void *data, void *key);
int CompareArticle(void *inputA, void *inputB);
void ReadArticle(void *data, FILE *file);
void ModifyArticle(FILE *file);
void DeleteArticle(FILE *fileArticulos, FILE *fileArtProv);
void ShowArticle(void *data);

void GenerateProviderId(char *newId, FILE *file);

int GetKeyProvider(void *data, void *key);
int CompareProvider(void *inputA, void *inputB);
void ReadProvider(void *data, FILE *file);
void ModifyProvider(FILE *file);
void DeleteProvider(FILE *fileProveedores, FILE *fileArtProv);
void ShowProvider(void *data);

int GetKeyArtProvByProv(void *data, void *key);

int GetKeyArtProv(void *data, void *key);
int CompareArtProv(void *inputA, void *inputB);
void ReadArtProv(void *data, FILE *file);
void ModifyArtProv(FILE *file);
void DeleteArtProv(FILE *file);
void ShowArtProv(void *data);

int main() {
   GenericList listaClientes = {NULL, NULL, 0};
   GenericList listaUsuarios = {NULL, NULL, 0};
   GenericList listaArticulos = {NULL, NULL, 0};
   GenericList listaProveedores = {NULL, NULL, 0};
   GenericList listaArtProv = {NULL, NULL, 0};
   GenericList listaFacturas = {NULL, NULL, 0};
   GenericList listaDetalles = {NULL, NULL, 0};
   GenericList listaPagos = {NULL, NULL, 0};

   FILE *fileClientes = fopen("clientes.dat", "ab+");
   FILE *fileUsuarios = fopen("usuarios.dat", "ab+");
   FILE *fileArticulos = fopen("articulos.dat", "ab+");
   FILE *fileProveedores = fopen("proveedores.dat", "ab+");
   FILE *fileArtProv = fopen("art_prov.dat", "ab+");
   FILE *fileFacturas = fopen("facturas.dat", "ab+");
   FILE *fileDetalles = fopen("detalles.dat", "ab+");
   FILE *filePagos = fopen("pagos.dat", "ab+");

   if (!fileClientes || !fileUsuarios ||
       !fileArticulos || !fileProveedores ||
       !fileArtProv || !fileFacturas ||
       !fileDetalles || !filePagos) {
      printf("Error abriendo un archivo.");
      return 1;
   }

   system("cls");
   _setcursortype(0);

   char menu[OPTS][OPT_LENGTH] = {"CRUD", "PROCESOS", "CONSULTAS", "SALIR"};
   char crud_dropdown[CRUD_OPTS][OPT_LENGTH] = {"Cliente", "Usuario", "Proveedor", "Articulo"};
   char crud_actions[CRUD_SUBOPTS][OPT_LENGTH] = {"Ingresar", "Modificar", "Eliminar"};
   char process_dropdown[PROCESS_OPTS][OPT_LENGTH] = {"Precio Art.", "Facturar", "Pagar"};
   char consult_dropdown[CONSULT_OPTS][OPT_LENGTH] = {"Fact. Pend", "Pagos", "Balance"};

   int crudXPos = get_option_xpos(menu, 0, INIT_X_POS);

   int SelectedOption = 0;
   int subOption;
   int key;

   show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
   do {
      show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);

      key = getch();
      if (key == __UNDEFINED || key == ALPHA) {
         key = getch();
      }

      if (key == LEFT) {
         SelectedOption = (SelectedOption > 0) ? SelectedOption - 1 : OPTS - 1;
         show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
      }

      if (key == RIGHT) {
         SelectedOption = (SelectedOption < OPTS - 1) ? SelectedOption + 1 : 0;
         show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
      }

      if (key == ENTER) {
         switch (SelectedOption) {
         case 0:
            subOption = show_dropdown(crud_dropdown, CRUD_OPTS, crudXPos, INIT_Y_POS + 1);

            if (subOption != -1) {
               for (int index = 0; index < CRUD_OPTS; index++) {
                  gotoxy(crudXPos, INIT_Y_POS + 1 + index);
                  if (index == subOption)
                     setcolor(CTS, CFS);
                  else
                     setcolor(CT, CF);
                  printf("%-12s", crud_dropdown[index]);
               }
               colordefault();
               int actionOption = show_dropdown(crud_actions, CRUD_SUBOPTS, get_option_xpos(menu, 0, INIT_X_POS) + 12, INIT_Y_POS + 1 + subOption);

               system("cls");
               show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
               if (actionOption == -1)
                  break;

               if (subOption == 2 && actionOption == 0) {
                  PROVEEDOR temp = {0};
                  ReadProvider(&temp, fileProveedores);
               }
               if (subOption == 2 && actionOption == 1)
                  ModifyProvider(fileProveedores);
               if (subOption == 2 && actionOption == 2)
                  DeleteProvider(fileProveedores, fileArtProv);

               if (subOption == 3 && actionOption == 1)
                  ModifyArticle(fileArticulos);
               if (subOption == 3 && actionOption == 2)
                  DeleteArticle(fileArticulos, fileArtProv);

               if (subOption == 3 && actionOption == 0) { /* Articulo → Ingresar */
                  ARTICULO temp = {0};
                  ReadArticle(&temp, fileArticulos);
               }
               if (subOption == 3 && actionOption == 1)
                  ModifyArticle(fileArticulos);
               if (subOption == 3 && actionOption == 2)
                  DeleteArticle(fileArticulos, fileArtProv);
            }

            break;

         case 1:
            subOption = show_dropdown(process_dropdown, PROCESS_OPTS, get_option_xpos(menu, 1, INIT_X_POS), INIT_Y_POS + 1);
            system("cls");
            show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
            if (subOption == 0) {
               ARTICULOPROVEEDOR temp = {0};
               ReadArtProv(&temp, fileArtProv);
            }
            if (subOption == 1) { /* Facturar */
            }
            if (subOption == 2) { /* Pagar */
            }
            break;

         case 2:
            subOption = show_dropdown(consult_dropdown, CONSULT_OPTS, get_option_xpos(menu, 2, INIT_X_POS), INIT_Y_POS + 1);
            system("cls");
            show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
            if (subOption == 0) { /* Facturas pendientes */
            }
            if (subOption == 1) { /* Pagos */
            }
            if (subOption == 2) { /* Balance */
            }
            break;
         case 3:
            key = ESC;
            break;
         }
      }
   } while (key != ESC);

   system("cls");

   Cleanup(&listaClientes, fileClientes);
   Cleanup(&listaUsuarios, fileUsuarios);
   Cleanup(&listaArticulos, fileArticulos);
   Cleanup(&listaProveedores, fileProveedores);
   Cleanup(&listaArtProv, fileArtProv);
   Cleanup(&listaFacturas, fileFacturas);
   Cleanup(&listaDetalles, fileDetalles);
   Cleanup(&listaPagos, filePagos);
   return 0;
}

/*
 * Funcion: captureAlphanumeric
 * Objetivo: Permite al usuario escribir texto (nombres o apellidos) con flechas y backspace.
 * Argumentos: (char) *result_str - Lugar donde se guarda el texto que escribe el usuario.
 *             (int) max_length - Cantidad máxima de caracteres permitidos.
 *             (int) Xpos, Ypos - Posición en pantalla del campo.
 *             (int)(*func)(char) validator - Función que decide qué caracteres son válidos.
 */
void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char character)) {
   int cursor_index = strlen(result_str);
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(result_str, cursor_index, max_length, Xpos, Ypos);
      fflush(stdin);

      do {
         pressed_key = getch();
         if (pressed_key == 0 || pressed_key == (char)224) {
            getch();
            pressed_key = 0;
         }
      } while (!validator(pressed_key) &&
               pressed_key != ENTER &&
               pressed_key != ESC &&
               pressed_key != TAB &&
               pressed_key != LEFT &&
               pressed_key != BACKSPACE &&
               pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_length - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC && pressed_key != TAB) {
         if (pressed_key == BACKSPACE) {
            if (cursor_index > 0) {
               cursor_index--;
               memmove(result_str + cursor_index,
                       result_str + cursor_index + 1,
                       max_length - cursor_index);
               result_str[max_length - 1] = '\0';
            }
         } else {
            if (cursor_index < max_length - 1) {
               memmove(result_str + cursor_index + 1,
                       result_str + cursor_index,
                       max_length - cursor_index - 1);
               result_str[cursor_index] = pressed_key;
               cursor_index++;
               result_str[max_length - 1] = '\0';
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC && pressed_key != TAB);

   _setcursortype(0);
}

/*
 * Funcion: CaptureInteger
 * Objetivo: Permite al usuario escribir solo números (día, mes o año).
 * Argumentos: (char) digit_str - Buffer temporal donde se guarda el número mientras se escribe.
 *             (int) *result_value - Variable donde se guarda el número final como entero.
 *             (int) max_digits - Cantidad máxima de dígitos permitidos.
 *             (int) pos_x, pos_y - Posición en pantalla del campo.
 */
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int pos_x, int pos_y) {
   int cursor_index = strlen(digit_str);
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(digit_str, cursor_index, max_digits, pos_x, pos_y);
      fflush(stdin);

      do {
         pressed_key = getch();
         if (pressed_key == 0 || pressed_key == (char)224) {
            getch();
            pressed_key = 0;
         }
      } while ((pressed_key < '0' || pressed_key > '9') &&
               pressed_key != ENTER &&
               pressed_key != ESC &&
               pressed_key != LEFT &&
               pressed_key != BACKSPACE &&
               pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_digits - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BACKSPACE) {
            if (cursor_index > 0) {
               cursor_index--;
               memmove(digit_str + cursor_index,
                       digit_str + cursor_index + 1,
                       max_digits - cursor_index);
               digit_str[max_digits - 1] = '\0';
            }
         } else {
            if (cursor_index < max_digits - 1) {
               memmove(digit_str + cursor_index + 1,
                       digit_str + cursor_index,
                       max_digits - cursor_index - 1);
               digit_str[cursor_index] = pressed_key;
               cursor_index++;
               digit_str[max_digits - 1] = '\0';
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC) {
      digit_str[cursor_index] = '\0';
      *result_value = atoi(digit_str);
   }
   _setcursortype(0);
}

/*
Funcion: capture_numeric_decimals
Objetivo: Alias de capture_numeric_digits, usado especificamente para capturar
          la cantidad de decimales deseados
Argumentos: (char) *decimal_str, (int) *result_value, (int) max_digits, (int) pos_x, (int) pos_y
*/
void CaptureFloat(char *decimal_str, int *result_value, int max_digits, int pos_x, int pos_y) {
   CaptureInteger(decimal_str, result_value, max_digits, pos_x, pos_y);
}

/*
 * Funcion: CaptureFecha
 * Objetivo: Pide al usuario que ingrese una fecha (día/mes/año) y la valida.
 * Argumentos: (__DATE) date  - Estructura donde se guardará la fecha ingresada.
 *             (int) xpos - Columna donde empieza el campo de fecha en pantalla.
 *             (int) ypos - Fila donde se muestra el campo de fecha en pantalla.
 */
void CaptureDate(__DATE *date, int xpos, int ypos) {
   char buf_dia[DAY_BUFFER] = "";
   char buf_mes[MONTH_BUFFER] = "";
   char buf_year[YEAR_BUFFER] = "";
   int dia_temp = 0, mes_temp = 0, year_temp = 0;
   int valida;

   _setcursortype(100);

   do {
      valida = 1;

      if (date->day > 0)
         sprintf(buf_dia, "%02d", date->day);
      if (date->month > 0)
         sprintf(buf_mes, "%02d", date->month);
      if (date->year > 0)
         sprintf(buf_year, "%04d", date->year);

      gotoxy(xpos + 1, ypos);
      setcolor(CT, CF);
      printf("  /  /    ");
      colordefault();

      CaptureInteger(buf_dia, &dia_temp, 3, xpos, ypos);
      gotoxy(xpos + 3, ypos);
      printf("/");
      CaptureInteger(buf_mes, &mes_temp, 3, xpos + 3, ypos);
      gotoxy(xpos + 6, ypos);
      printf("/");
      CaptureInteger(buf_year, &year_temp, 5, xpos + 6, ypos);

      gotoxy(xpos, ypos + 1);
      setcolor(CT, CF);
      printf("%-40s", "");
      colordefault();

      if (!ValidDate(dia_temp, mes_temp, year_temp)) {
         valida = 0;
         gotoxy(xpos, ypos + 1);
         setcolor(RED, BLACK);
         printf("Fecha invalida. Verifique dia/mes/a%co.", 164);
         colordefault();
         getch();
      }
   } while (!valida);

   date->day = dia_temp;
   date->month = mes_temp;
   date->year = year_temp;
}

/*
 * Funcion: validateAlphachar
 * Objetivo: Verifica si un carácter es letra, espacio o guion.
 * Argumentos: (char) character - Carácter que se quiere validar.
 * Retorna: 1 si es válido, 0 si no lo es.
 */
int validateAlphachar(char character) {
   return (isalpha((char)character) || character == ' ' || character == '-');
}

/*
 * Funcion: ValidDate
 * Objetivo: Verifica si la fecha ingresada es una fecha real y válida (usa time.h).
 * Argumentos: (int) day, month, year - Día, mes y año que se quieren comprobar.
 * Retorna: 1 si la fecha es válida, 0 si no lo es.
 */
int ValidDate(int day, int month, int year) {
   struct tm date_tm = {0};
   date_tm.tm_mday = day;
   date_tm.tm_mon = month - 1;
   date_tm.tm_year = year - 1900;
   date_tm.tm_isdst = -1;

   time_t time_val = mktime(&date_tm);
   if (time_val == -1)
      return 0;

   return (date_tm.tm_mday == day && date_tm.tm_mon == month - 1 && date_tm.tm_year == year - 1900);
}

/**
 * Funcion: Cleanup
 * Objetivo: Liberar memoria y cerrar archivos antes de terminar la corrida del programa.
 * Argumentos: (GenericList) *list, (FILE) *file
 */
void Cleanup(GenericList *list, FILE *file) {
   GenericNode *current = list->head;
   while (current) {
      GenericNode *next = current->next;
      free(current->data);
      free(current);
      current = next;
   }
   list->head = NULL;
   list->tail = NULL;
   list->Quantity = 0;

   if (file) {
      fclose(file);
   }
}

/*
 * funcion: ShowForm
 * Objetivo: Renderizar el formulario para la entrada de datos dependiendo el la estructura
 *           permite tambien al usuario moverse entre las opciones mientras edita, como un
 *           archivo de word.
 * Argumentos: (FormField) *fields, (int) numFields, (char) *title
 */
int ShowForm(FormField *fields, int numFields, char *title) {
   int isSelected = 0;
   int key;
   char buff[BUFFER];

   system("cls");

   while (isSelected < numFields && fields[isSelected].type == FIELD_READONLY) {
      isSelected++;
   }

   do {
      gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
      printf("%s", title);

      for (int index = 0; index < numFields; index++) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + index * 3);
         if (index == isSelected) {
            setcolor(CTS, CFS);
         } else {
            setcolor(CT, CF);
         }

         printf("%-15s", fields[index].label);
         colordefault();

         gotoxy(FORM_VALUE_X, FORM_START_Y + index * 3);
         setcolor(CT, CF);

         switch (fields[index].type) {
         case FIELD_READONLY:
         case FIELD_ALPHA:
            printf("%-*s", fields[index].maxLength, (char *)fields[index].valuePtr);
            break;
         case FIELD_INT:
            printf("%-5d", *(int *)fields[index].valuePtr);
            break;
         case FIELD_FLOAT:
            printf("%-8.2f", *(float *)fields[index].valuePtr);
            break;
         case FIELD_DATE: {
            __DATE *date = (__DATE *)fields[index].valuePtr;
            printf("%02d/%02d/%04d", date->day, date->month, date->year);
            break;
         }
         }
         colordefault();
      }

      gotoxy(FORM_LABEL_X, FORM_START_Y + numFields * 3 + 1);
      if (isSelected == numFields) {
         setcolor(CTS, CFS);
      } else {
         setcolor(CT, CF);
      }
      printf("Guardar");
      colordefault();

      key = getch();
      if (key == __UNDEFINED || key == ALPHA) {
         key = getch();
      }

      if (key == UP) {
         isSelected = (isSelected > 0) ? isSelected - 1 : 0;

         while (isSelected < numFields && fields[isSelected].type == FIELD_READONLY) {
            isSelected--;
         }
      }

      if (key == DOWN) {
         isSelected = (isSelected < numFields) ? isSelected + 1 : 0;
         while (isSelected > 0 && fields[isSelected].type == FIELD_READONLY) {
            isSelected--;
         }
      }

      if (key != UP && key != DOWN && key != ESC && key != ENTER && isSelected < numFields) {
         switch (fields[isSelected].type) {
         case FIELD_ALPHA:
            memset(buff, 0, sizeof(buff));
            captureAlphanumeric((char *)fields[isSelected].valuePtr, fields[isSelected].maxLength, FORM_VALUE_X, FORM_START_Y + isSelected * 3, validateAlphachar);
            break;
         case FIELD_INT:
            memset(buff, 0, sizeof(buff));
            CaptureInteger(buff, (int *)fields[isSelected].valuePtr, fields[isSelected].maxLength, FORM_VALUE_X, FORM_START_Y + isSelected * 3);
            break;
         case FIELD_FLOAT:
            memset(buff, 0, sizeof(buff));
            CaptureFloat(buff, (int *)fields[isSelected].valuePtr, fields[isSelected].maxLength, FORM_VALUE_X, FORM_START_Y + isSelected * 3);
            break;
         case FIELD_DATE: {
            memset(buff, 0, sizeof(buff));
            CaptureDate((__DATE *)fields[isSelected].valuePtr, FORM_VALUE_X, FORM_START_Y + isSelected * 3);
            break;
         }
         default:
            break;
         }
      }

      if (key == ENTER && isSelected == numFields) {
         system("cls");
         return 1;
      }
   } while (key != ESC);

   system("cls");
   return 0;
}

/*
 * Funcion: show_dropdown
 * Objetivo: Renderizar y permitir al usuario moverse por el menu dropdown
 * Argumentos: (char) items[][OPT_LENGTH], (int) count, (int) xpos, (int) ypos
 * Retorna: -1 si la tecla presionada es Escape, o variable entero "isSelected" si el usuario no da Escape.
 */
int show_dropdown(char items[][OPT_LENGTH], int count, int xpos, int ypos) {
   int isSelected = 0;
   int key;

   int width = 0;
   for (int index = 0; index < count; index++) {
      int len = (int)strlen(items[index]);
      if (len > width) {
         width = len;
      }
   }
   width += 2;

   do {
      for (int index = 0; index < count; index++) {
         gotoxy(xpos, ypos + index);

         if (index == isSelected) {
            setcolor(CTS, CFS);
         } else {
            setcolor(CT, CF);
         }

         printf("%-*s", width, items[index]);
      }
      colordefault();

      key = getch();
      if (key == 0 || key == 224) {
         key = getch();
      }

      if (key == UP) {
         isSelected = (isSelected > 0) ? isSelected - 1 : count - 1;
      } else if (key == DOWN) {
         isSelected = (isSelected < count - 1) ? isSelected + 1 : 0;
      }
   } while (key != ENTER && key != ESC);

   for (int index = 0; index < count; index++) {
      gotoxy(xpos, ypos + index);
      colordefault();
      for (int jndex = 0; jndex < width; jndex++) {
         printf(" ");
      }
   }
   colordefault();

   return (key == ESC) ? -1 : isSelected;
}

/*
 * Funcion  : show_field
 * Objetivo : Dibuja el contenido de un campo editable y coloca el cursor.
 * Argumentos:
 *   field_str       - Texto que se muestra en el campo.
 *   cursor_position - Posición actual del cursor dentro del campo.
 *   max_length      - Tamaño máximo del campo.
 *   pos_x, pos_y    - Posición en pantalla donde se dibuja el campo.
 */
void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y) {
   setcolor(CT, CF);
   for (int index = 0; index < max_length; index++) {
      gotoxy(pos_x + index + 1, pos_y);
      printf("%c", field_str[index] ? field_str[index] : ' ');
   }
   colordefault();
   gotoxy(pos_x + cursor_position + 1, pos_y);
}

/*
 * Funcion  : show_header
 * Objetivo : Muestra el menú principal (INGRESAR, MODIFICAR, etc.).
 * Argumentos:
 *   menu       - Arreglo con los textos de las opciones.
 *   options    - Cantidad de opciones en el menú.
 *   Xpos, Ypos - Posición en pantalla donde se dibuja el menú.
 *   isSelected - Opción que está resaltada actualmente.
 */
void show_header(char menu[][OPT_LENGTH], int options, int xpos, int Ypos, int isSelected) {
   gotoxy(xpos, Ypos);
   setcolor(TITLE_COLOR_FONT, TITLE_COLOR_BG);
   printf("Facturaci%cn  ", 162);

   int desplazamiento = (int)strlen("Facturacion  ") + xpos;

   for (int index = 0; index < options; index++) {
      gotoxy(desplazamiento, Ypos);
      if (isSelected == index)
         setcolor(CTS, CFS);
      else
         setcolor(CT, CF);
      printf("%s", menu[index]);
      desplazamiento += (int)strlen(menu[index]) + 2;
      setcolor(CT, CF);
      printf("  ");
   }
   colordefault();
}

/*
 * Funcion: get_option_xpos
 * Objetivo: Obtener la posicion en X de las opciones principales (e.g: CRUD) para renderizar el dropdown debajo de esta.
 * Argumentos: (char) menu[][OPT_LENGTH], (int) TargetIndex, (int) StartX
 * Retorna: La posicion en X de una opcion principal
 */
int get_option_xpos(char menu[][OPT_LENGTH], int targetIndex, int startX) {
   int offset = startX + (int)strlen("Facturacion  ");
   for (int index = 0; index < targetIndex; index++) {
      offset += (int)strlen(menu[index]) + 2;
   }
   return offset;
}

/*
 * Funcion: ConfirmDelete
 * Objetivo: Confirmar la removida de un dato.
 * Retorna: 1 si confirma, 0 si cancela.
 */
int ConfirmDelete(char *name) {
   char key;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);
   setcolor(WARNING_CT, WARNING_CT);

   printf("Eliminar: %s", name);
   colordefault();

   gotoxy(FORM_LABEL_X, FORM_START_Y);
   printf("Esta seguro? [S/N]: ");
   do {
      key = getch();
      key = toupper(key);
   } while (key != 'S' && key != 'N');
   return key == 'S';
}

/**
 * Funcion: GenerateArticleId
 * Objetivo: Generar automaticamente el Id de un nuevo articulo
 * Argumentos: (char) *newId - Nuevo ID, (FILE) *file - Archivo de articulos dado.
 */
void GenerateArticleId(char *newId, FILE *file) {
   ARTICULO last;
   int lastNum = 0;

   rewind(file);
   while (fread(&last, sizeof(ARTICULO), 1, file) == 1)
      ;

   if (last.Id[0] == 'A') {
      lastNum = atoi(last.Id + 1);
   }

   sprintf(newId, "A%03d", lastNum + 1);
}

/**
 * Funcion GetKeyArticle
 * Objetivo: Comparar y verificar la existencia de un articulo.
 * Retorna: 1, si ya se repite, 0 si no se repite.
 */
int GetKeyArticle(void *data, void *key) {
   return strcmp(((ARTICULO *)data)->Id, (char *)key) == 0;
}

/*
 * Funcion: CompareArticle
 * Objetivo: Comparar el ID de los articulos para ver que no se repitan.
 */
int CompareArticle(void *inputA, void *inputB) {
   return strcmp(((ARTICULO *)inputA)->Id, ((ARTICULO *)inputB)->Id);
}

/*
 * Funcion: ReadArticle
 * Objetivo: Obtener los datos de un articulo con el usuario.
 * Argumentos: (void) *data, (FILE) file
 */
void ReadArticle(void *data, FILE *file) {
   ARTICULO *article = (ARTICULO *)data;

   GenerateArticleId(article->Id, file);

   FormField fields[] = {
       {"Id", article->Id, FIELD_READONLY, MAXID - 1},
       {"Descripcion", article->Descripcion, FIELD_ALPHA, LENDESC - 1},
       {"Cantidad", &article->cantidad, FIELD_INT, QUANTITY_BUFFER},
       {"Reorden", &article->reorden, FIELD_INT, REORDER_BUFFER},
   };

   if (ShowForm(fields, ARTICLE_FIELDS, "Nuevo Articulo")) {
      rewind(file);
      fseek(file, 0, SEEK_END);
      fwrite(article, sizeof(ARTICULO), 1, file);
   }
}

/*
 * Funcion: ModifyArticle
 * Objetivo: Buscar un articulo por ID y permitir al usuario modificarlo
 * Argumentos: (FILE) *file
 */
void ModifyArticle(FILE *file) {
   ARTICULO record;

   char searchId[MAXID] = {0};
   int position = -1;
   int found = 0;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);

   printf("ID a modificar: ");
   captureAlphanumeric(searchId, MAXID - 1, FORM_LABEL_X + 18, FORM_START_Y, validateAlphachar);

   rewind(file);
   int index = 0;
   while (fread(&record, sizeof(ARTICULO), 1, file) == 1) {
      if (strcmp(record.Id, searchId) == 0) {
         position = index;
         found = 1;
         break;
      }
      index++;
   }

   if (!found) {
      system("cls");

      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);

      printf("Articulo no encontrado.");
      colordefault();
      getch();

      return;
   }

   FormField fields[] = {
       {"Id", record.Id, FIELD_READONLY, MAXID - 1},
       {"Descripcion", record.Descripcion, FIELD_ALPHA, LENDESC - 1},
       {"Cantidad", &record.cantidad, FIELD_INT, QUANTITY_BUFFER},
       {"Reorden", &record.reorden, FIELD_INT, REORDER_BUFFER},
   };

   if (ShowForm(fields, ARTICLE_FIELDS, "Modificar articulo")) {
      fseek(file, position * (long)sizeof(ARTICULO), SEEK_SET);
      fwrite(&record, sizeof(ARTICULO), 1, file);
   }
}

/*
 * Funcion: DeleteArticle
 * Objetivo: Verificar la integridad referencial y eliminar un articulo del archivo, SOLO EN CASO DE QUE SE MANTENGA LA INTEGRIDAD REFERENCIAL.
 * Argumentos: (FILE) *fileArticulos, (FILE) *fileArtProv
 */
void DeleteArticle(FILE *fileArticulos, FILE *fileArtProv) {
   ARTICULO record;

   char searchId[MAXID] = {0};
   int found = 0;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);

   printf("ID a eliminar: ");
   captureAlphanumeric(searchId, MAXID - 1, FORM_LABEL_X + 17, FORM_START_Y, validateAlphachar);

   ARTICULOPROVEEDOR ArticleProvider;
   rewind(fileArtProv);
   while (fread(&ArticleProvider, sizeof(ARTICULOPROVEEDOR), 1, fileArtProv) == 1) {
      if (strcmp(ArticleProvider.Id_Art, searchId) == 0) {
         system("cls");

         gotoxy(FORM_LABEL_X, FORM_START_Y);
         setcolor(WARNING_CT, WARNING_CF);

         printf("No se puede eliminar; el articulo referenciado en articulo-provedor");

         colordefault();
         getch();

         return;
      }
   }

   rewind(fileArticulos);

   while (fread(&record, sizeof(ARTICULO), 1, fileArticulos) == 1) {
      if (strcmp(record.Id, searchId) == 0) {
         found = 1;
         break;
      }
   }

   if (!found) {
      system("cls");

      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);

      printf("Articulo no encontrado");

      colordefault();
      getch();

      return;
   }

   if (!ConfirmDelete(record.Id)) {
      return;
   }

   FILE *temp = fopen("temp.dat", "wb");
   if (!temp)
      return;

   rewind(fileArticulos);
   while (fread(&record, sizeof(ARTICULO), 1, fileArticulos) == 1) {
      if (strcmp(record.Id, searchId) != 0)
         fwrite(&record, sizeof(ARTICULO), 1, temp);
   }
   fclose(temp);

   freopen("articulos.dat", "wb", fileArticulos);
   temp = fopen("temp.dat", "rb");
   while (fread(&record, sizeof(ARTICULO), 1, temp) == 1)
      fwrite(&record, sizeof(ARTICULO), 1, fileArticulos);
   fclose(temp);
   remove("temp.dat");
}

/*
 * Funcion: ShowArticle
 * Objetivo: Mostrar una tabla con los datos de los articulos guardados de un archivo.
 * Argumentos: (void) *data
 */
void ShowArticle(void *data) {
   ARTICULO *article = (ARTICULO *)data;

   printf("ID: %-5s | Descripcion: %-25s | Cantidad: %4d | Reorden: %4d",
          article->Id, article->Descripcion, article->cantidad, article->reorden);
}

/*
 * Funcion: GenerateProviderId
 * Objetivo: Generar el ID de un proveedor de forma automatica
 * Argumentos: (char) *newId, (FILE) *file
 */
void GenerateProviderId(char *newId, FILE *file) {
   PROVEEDOR last;
   int lastNum = 0;
   rewind(file);
   while (fread(&last, sizeof(PROVEEDOR), 1, file) == 1)
      ;
   if (last.Id[0] == 'P')
      lastNum = atoi(last.Id + 1);
   sprintf(newId, "P%03d", lastNum + 1);
}

int GetKeyProvider(void *data, void *key) {
   return strcmp(((PROVEEDOR *)data)->Id, (char *)key) == 0;
}

int CompareProvider(void *inputA, void *inputB) {
   return strcmp(((PROVEEDOR *)inputA)->Id, ((PROVEEDOR *)inputB)->Id);
}

/*
 * funcion: ReadProvider
 * Objetivo: Obtener la informacion del proveedor con el usuario.
 * Argumentos: (void) *data, (FILE) *file
 */
void ReadProvider(void *data, FILE *file) {
   PROVEEDOR *provider = (PROVEEDOR *)data;

   GenerateProviderId(provider->Id, file);

   FormField fields[] = {
       {"Id", provider->Id, FIELD_READONLY, MAXID - 1},
       {"Descripcion", provider->Descripcion, FIELD_ALPHA, LENDESC - 1},
       {"Direccion", provider->Direccion, FIELD_ALPHA, LENDIR - 1},
       {"Nacimiento", &provider->Nacimiento, FIELD_DATE, (DAY_BUFFER + MONTH_BUFFER + YEAR_BUFFER) - 1},
   };

   if (ShowForm(fields, PROVIDER_FIELDS, "Nuevo proveedor")) {
      rewind(file);
      fseek(file, 0, SEEK_END);
      fwrite(provider, sizeof(PROVEEDOR), 1, file);
   }
}
/*
 * Funcion: ModifyProvider
 * Objetivo: Buscar un proveedor por ID y permitir al usuario modificarlo.
 * Argumentos: (FILE) *file
 */
void ModifyProvider(FILE *file) {
   char searchId[MAXID] = {0};
   PROVEEDOR record;
   long position = -1;
   int found = 0;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);
   printf("ID a modificar: ");
   captureAlphanumeric(searchId, MAXID - 1, FORM_LABEL_X + 18, FORM_START_Y, validateAlphachar);

   rewind(file);
   long index = 0;
   while (fread(&record, sizeof(PROVEEDOR), 1, file) == 1) {
      if (strcmp(record.Id, searchId) == 0) {
         position = index;
         found = 1;
         break;
      }
      index++;
   }

   if (!found) {
      system("cls");
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);
      printf("Proveedor no encontrado.");
      colordefault();
      getch();
      return;
   }

   FormField fields[] = {
       {"Id", record.Id, FIELD_READONLY, MAXID - 1},
       {"Descripcion", record.Descripcion, FIELD_ALPHA, LENDESC - 1},
       {"Direccion", record.Direccion, FIELD_ALPHA, LENDIR - 1},
       {"Nacimiento", &record.Nacimiento, FIELD_DATE, 10},
   };

   if (ShowForm(fields, PROVIDER_FIELDS, "Modificar Proveedor")) {
      fseek(file, position * (long)sizeof(PROVEEDOR), SEEK_SET);
      fwrite(&record, sizeof(PROVEEDOR), 1, file);
   }
}

/*
 * Funcion: DeleteProvider
 * Objetivo: Verificar integridad referencial y eliminar un proveedor del archivo.
 * Argumentos: (FILE) *fileProveedores, (FILE) *fileArtProv
 */
void DeleteProvider(FILE *fileProveedores, FILE *fileArtProv) {
   char searchId[MAXID] = {0};
   PROVEEDOR record;
   int found = 0;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);
   printf("ID a eliminar: ");
   captureAlphanumeric(searchId, MAXID - 1, FORM_LABEL_X + 17, FORM_START_Y, validateAlphachar);

   ARTICULOPROVEEDOR ArticleProvider;
   rewind(fileArtProv);
   while (fread(&ArticleProvider, sizeof(ARTICULOPROVEEDOR), 1, fileArtProv) == 1) {
      if (strcmp(ArticleProvider.Id_Prov, searchId) == 0) {
         system("cls");
         gotoxy(FORM_LABEL_X, FORM_START_Y);
         setcolor(WARNING_CT, WARNING_CF);
         printf("No se puede eliminar: proveedor referenciado en Articulo-Proveedor.");
         colordefault();
         getch();
         return;
      }
   }

   rewind(fileProveedores);
   while (fread(&record, sizeof(PROVEEDOR), 1, fileProveedores) == 1) {
      if (strcmp(record.Id, searchId) == 0) {
         found = 1;
         break;
      }
   }

   if (!found) {
      system("cls");
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);
      printf("Proveedor no encontrado.");
      colordefault();
      getch();
      return;
   }

   if (!ConfirmDelete(record.Id))
      return;

   FILE *temp = fopen("temp.dat", "wb");
   if (!temp)
      return;

   rewind(fileProveedores);
   while (fread(&record, sizeof(PROVEEDOR), 1, fileProveedores) == 1) {
      if (strcmp(record.Id, searchId) != 0)
         fwrite(&record, sizeof(PROVEEDOR), 1, temp);
   }
   fclose(temp);

   freopen("proveedores.dat", "wb", fileProveedores);
   temp = fopen("temp.dat", "rb");
   while (fread(&record, sizeof(PROVEEDOR), 1, temp) == 1)
      fwrite(&record, sizeof(PROVEEDOR), 1, fileProveedores);
   fclose(temp);
   remove("temp.dat");
}

/*
 * funcion: ShowProvider
 * Objetivo: Mostrar una tabla con los proveedores, con su respectiva descripcion, direccion y fecha de nacimiento.
 * Argumentos: (void) *data
 */
void ShowProvider(void *data) {
   PROVEEDOR *provider = (PROVEEDOR *)data;
   printf("Id: %-5s | Descripci%cn %-25s | Direcci%cn %-30s | Nac: %02d/%02d/%04d", provider->Id, 162, provider->Descripcion, 162, provider->Direccion, provider->Nacimiento.day, provider->Nacimiento.month, provider->Nacimiento.year);
}

/*
 * Funcion: GetKeyArtProv
 * Objetivo: Obtener la relacion entre un proveedor y articulo.
 * Argumentos: (void) *data, (void) *key
 */
int GetKeyArtProv(void *data, void *key) {
   ARTICULOPROVEEDOR *articleProvider = (ARTICULOPROVEEDOR *)data;
   ArticleProviderKey *artProKey = (ArticleProviderKey *)key;

   return (strcmp(articleProvider->Id_Art, artProKey->Id_Art) == 0 && strcmp(articleProvider->Id_Prov, artProKey->Id_Prov) == 0);
}

int CompareArtProv(void *inputA, void *inputB) {
   ARTICULOPROVEEDOR *ArticleProvider_A = (ARTICULOPROVEEDOR *)inputA;
   ARTICULOPROVEEDOR *ArticleProvider_B = (ARTICULOPROVEEDOR *)inputB;

   int result = strcmp(ArticleProvider_A->Id_Art, ArticleProvider_B->Id_Art);
   if (result != 0) {
      return result;
   }

   return strcmp(ArticleProvider_A->Id_Prov, ArticleProvider_B->Id_Prov);
}

/*
 * Funcion: ReadArtProv
 * Objetivo: Dar la abilidad de cambiar los precios de un articulo dependiendo del proveedor.
 * Argumentos: (void) *data, (FILE) *file
 */
void ReadArtProv(void *data, FILE *file) {
   ARTICULOPROVEEDOR *ArticleProvider = (ARTICULOPROVEEDOR *)data;

   FormField fields[] = {
       {"ID Articulo", ArticleProvider->Id_Art, FIELD_ALPHA, MAXID - 1},
       {"ID Proveedor", ArticleProvider->Id_Prov, FIELD_ALPHA, MAXID - 1},
       {"Precio", &ArticleProvider->precio, FIELD_FLOAT, ARTICLEPROVIDER_PRICE_FIELD_BUFFER},
   };

   if (ShowForm(fields, ARTICLEPROVIDER_FIELDS, "Articulo-Proveedor")) {
      rewind(file);
      fseek(file, 0, SEEK_END);
      fwrite(ArticleProvider, sizeof(ARTICULOPROVEEDOR), 1, file);
   }
}
/*
 * Funcion: ModifyArtProv
 * Objetivo: Buscar una relacion articulo-proveedor y modificar su precio.
 * Argumentos: (FILE) *file
 */
void ModifyArtProv(FILE *file) {
   char searchArt[MAXID] = {0};
   char searchProv[MAXID] = {0};
   ARTICULOPROVEEDOR record;
   long position = -1;
   int found = 0;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);
   printf("ID Articulo: ");
   captureAlphanumeric(searchArt, MAXID - 1, FORM_LABEL_X + 15, FORM_START_Y, validateAlphachar);

   gotoxy(FORM_LABEL_X, FORM_START_Y + 3);
   printf("ID Proveedor: ");
   captureAlphanumeric(searchProv, MAXID - 1, FORM_LABEL_X + 15, FORM_START_Y + 3, validateAlphachar);

   rewind(file);
   long index = 0;
   while (fread(&record, sizeof(ARTICULOPROVEEDOR), 1, file) == 1) {
      if (strcmp(record.Id_Art, searchArt) == 0 &&
          strcmp(record.Id_Prov, searchProv) == 0) {
         position = index;
         found = 1;
         break;
      }
      index++;
   }

   if (!found) {
      system("cls");
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);
      printf("Relacion no encontrada.");
      colordefault();
      getch();
      return;
   }

   FormField fields[] = {
       {"ID Articulo", record.Id_Art, FIELD_READONLY, MAXID - 1},
       {"ID Proveedor", record.Id_Prov, FIELD_READONLY, MAXID - 1},
       {"Precio", &record.precio, FIELD_FLOAT, ARTICLEPROVIDER_PRICE_FIELD_BUFFER},
   };

   if (ShowForm(fields, ARTICLEPROVIDER_FIELDS, "Modificar Articulo-Proveedor")) {
      fseek(file, position * (long)sizeof(ARTICULOPROVEEDOR), SEEK_SET);
      fwrite(&record, sizeof(ARTICULOPROVEEDOR), 1, file);
   }
}

/*
 * Funcion: DeleteArtProv
 * Objetivo: Eliminar una relacion articulo-proveedor del archivo.
 * Argumentos: (FILE) *file
 */
void DeleteArtProv(FILE *file) {
   char searchArt[MAXID] = {0};
   char searchProv[MAXID] = {0};
   ARTICULOPROVEEDOR record;
   int found = 0;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);
   printf("ID Articulo: ");
   captureAlphanumeric(searchArt, MAXID - 1, FORM_LABEL_X + 15, FORM_START_Y, validateAlphachar);

   gotoxy(FORM_LABEL_X, FORM_START_Y + 3);
   printf("ID Proveedor: ");
   captureAlphanumeric(searchProv, MAXID - 1, FORM_LABEL_X + 15, FORM_START_Y + 3, validateAlphachar);

   rewind(file);
   while (fread(&record, sizeof(ARTICULOPROVEEDOR), 1, file) == 1) {
      if (strcmp(record.Id_Art, searchArt) == 0 &&
          strcmp(record.Id_Prov, searchProv) == 0) {
         found = 1;
         break;
      }
   }

   if (!found) {
      system("cls");
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);
      printf("Relacion no encontrada.");
      colordefault();
      getch();
      return;
   }

   char label[MAXID * 2 + 2];
   sprintf(label, "%s/%s", searchArt, searchProv);
   if (!ConfirmDelete(label))
      return;

   FILE *temp = fopen("temp.dat", "wb");
   if (!temp)
      return;

   rewind(file);
   while (fread(&record, sizeof(ARTICULOPROVEEDOR), 1, file) == 1) {
      if (strcmp(record.Id_Art, searchArt) != 0 ||
          strcmp(record.Id_Prov, searchProv) != 0)
         fwrite(&record, sizeof(ARTICULOPROVEEDOR), 1, temp);
   }
   fclose(temp);

   freopen("art_prov.dat", "wb", file);
   temp = fopen("temp.dat", "rb");
   while (fread(&record, sizeof(ARTICULOPROVEEDOR), 1, temp) == 1)
      fwrite(&record, sizeof(ARTICULOPROVEEDOR), 1, file);
   fclose(temp);
   remove("temp.dat");
}

/*
 * Funcion: ShowArtProv
 * Objetivo: Mostrar una tabla con los datos de los articulos y su proveedor, junto con su precio.
 * Argumentos: (void) *data
 */
void ShowArtProv(void *data) {
   ARTICULOPROVEEDOR *ArticleProvider = (ARTICULOPROVEEDOR *)data;
   printf("Articulo: %-5s | Proveedor: %-5s | Precio: %.2f", ArticleProvider->Id_Art, ArticleProvider->Id_Prov, ArticleProvider->precio);
}

/*
 * Funcion  : setcolor
 * Objetivo : Cambia el color del texto y del fondo en la consola.
 * Argumentos:
 *   ct - Color del texto.
 *   cf - Color del fondo.
 */
void setcolor(int ct, int cf) {
   textbackground(cf);
   textcolor(ct);
}

/*
 * Funcion  : colordefault
 * Objetivo : Vuelve a los colores normales de la consola.
 */
void colordefault() {
   setcolor(LIGHTGRAY, BLACK);
}

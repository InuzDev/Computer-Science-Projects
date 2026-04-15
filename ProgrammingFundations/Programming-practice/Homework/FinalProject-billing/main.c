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

#define TITLE_X_POS 1
#define TITLE_Y_POS 1
#define INIT_X_POS 1
#define INIT_Y_POS 2

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
   int (*validator)(char character);
} FormField;

void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char character));
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int xpos, int ypos);
void CaptureFloat(char *decimal_str, float *result_value, int max_digits, int pos_x, int pos_y);
void CaptureDate(__DATE *date, int xpos, int ypos);
int validateAlphachar(char character);
int ValidDate(int day, int month, int year);

void GetCurrentDate(__DATE *date);
int CalculateAge(__DATE birth, __DATE current);

void Cleanup(GenericList *list, FILE *file);

void setcolor(int ct, int cf);
void colordefault();

void show_field(char *field_str, int cursor_position, int max_length, int xpos, int ypos);
void show_header(char menu[][OPT_LENGTH], int options, int xpos, int ypos, int isSelected);
int ShowForm(FormField *fields, int numFields, char *title);
int show_dropdown(char items[][OPT_LENGTH], int count, int xpos, int ypos);

int ShowTable(FILE *file, int recordSize, void (*show)(void *data), int (*filterFn)(void *data, void *key), void *filterKey, int xpos, int ypos);
int get_option_xpos(char menu[][OPT_LENGTH], int targetIndex, int startX);

int ConfirmDelete(char *name);

void GenerateArticleId(char *newId, FILE *file);
void ArtPrice(FILE *fileProveedores, FILE *fileArticulos, FILE *fileArtProv);

void BillingFunc(FILE *ClientFile, FILE *UserFile, FILE *ArtProvFile, FILE *BillFile, FILE *DetailFile);

float CalcPayment(char *BillNum, FILE *PaymentFile);

void ConsultPendingBilling(FILE *ClientFile, FILE *BillFile, FILE *DetailFile, FILE *PaymentFile);
void ConsultPayments(FILE *ClientFile, FILE *BillFile, FILE *PaymentFile);
void ConsultBalance(FILE *ClientFile, FILE *BillFile, FILE *DetailFile, FILE *PaymentFile);

float CalctotalBill(char *billNum, FILE *DetailFile);
void GeneratePaymentId(char *newId, FILE *PaymentFile);
void ShowPendingBill(void *data, FILE *DetailFile, FILE *PaymentFile);
void PayFunc(FILE *ClientFile, FILE *BillFile, FILE *DetailFile, FILE *PaidFile);

int GetKeyArticle(void *data, void *key);
int CompareArticle(void *inputA, void *inputB);
void ReadArticle(void *data, FILE *file);
void ModifyArticle(FILE *file);
void DeleteArticle(FILE *fileArticulos, FILE *fileArtProv);
void ShowArticle(void *data);

void GenerateProviderId(char *newId, FILE *ProviderFile);
void GenerateClientId(char *newId, FILE *ClientFile);
void GenerateUserId(char *newId, FILE *UserFile);
void GenerateBillId(char *newId, FILE *BillFile);

int GetKeyClient(void *data, void *key);
int CompareClient(void *inputA, void *inputB);
void ReadClient(void *data, FILE *file);
void ShowClient(void *data);
void ModifyClient(FILE *file);
void DeleteClient(FILE *ClientFile, FILE *BillFile);

void ShowclientBill(void *data);

int GetKeyUser(void *Data, void *key);
int CompareUser(void *inputA, void *inputB);
void ShowUser(void *data);
void readUser(void *data, FILE *UserFile);
void ModifyUser(FILE *UserFile);
void DeleteUser(FILE *UserFile, FILE *BillFile);

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

void ShowArtProvBill(void *data);

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

               if (subOption == 0 && actionOption == 0) {
                  CLIENTE temp = {0};
                  ReadClient(&temp, fileClientes);
               }
               if (subOption == 0 && actionOption == 1)
                  ModifyClient(fileClientes);
               if (subOption == 0 && actionOption == 2)
                  DeleteClient(fileClientes, fileFacturas);

               if (subOption == 1 && actionOption == 0) {
                  USUARIO temp = {0};
                  readUser(&temp, fileUsuarios);
               }
               if (subOption == 1 && actionOption == 1)
                  ModifyUser(fileUsuarios);
               if (subOption == 1 && actionOption == 2)
                  DeleteUser(fileUsuarios, fileFacturas);

               if (subOption == 2 && actionOption == 0) {
                  PROVEEDOR temp = {0};
                  ReadProvider(&temp, fileProveedores);
               }
               if (subOption == 2 && actionOption == 1)
                  ModifyProvider(fileProveedores);
               if (subOption == 2 && actionOption == 2)
                  DeleteProvider(fileProveedores, fileArtProv);

               if (subOption == 3 && actionOption == 0) {
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
               ArtPrice(fileProveedores, fileArticulos, fileArtProv);
            }
            if (subOption == 1) {
               BillingFunc(fileClientes, fileUsuarios, fileArtProv, fileFacturas, fileDetalles);
            }
            if (subOption == 2) { /* Pagar */
               PayFunc(fileClientes, fileFacturas, fileDetalles, filePagos);
            }
            break;
         case 2:
            subOption = show_dropdown(consult_dropdown, CONSULT_OPTS, get_option_xpos(menu, 2, INIT_X_POS), INIT_Y_POS + 1);
            system("cls");
            show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
            if (subOption == 0) {
               ConsultPendingBilling(fileClientes, fileFacturas, fileDetalles, filePagos);
            }
            if (subOption == 1) {
               ConsultPayments(fileClientes, fileFacturas, filePagos);
            }
            if (subOption == 2) {
               ConsultBalance(fileClientes, fileFacturas, fileDetalles, filePagos);
            }
            break;
         case 3:
            key = ESC;
            break;
         }
      }

      if (key == ESC || SelectedOption == 3) {
         gotoxy(INIT_X_POS + 3, INIT_Y_POS + 3);
         printf("Seguro que quiere salir? Presion [ESC] o [ENTER] para confirmar");

         key = getch();
         if (key == ESC || key == ENTER) {
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
 * Funcion: capture_numeric_decimals
 * Objetivo: Alias de capture_numeric_digits, usado especificamente para capturar
             la cantidad de decimales deseados
 * Argumentos: (char) *decimal_str, (int) *result_value, (int) max_digits, (int) pos_x, (int) pos_y
*/
void CaptureFloat(char *decimal_str, float *result_value, int max_digits, int pos_x, int pos_y) {
   int cursor_index = strlen(decimal_str);
   char pressed_key;

   _setcursortype(100);

   do {
      show_field(decimal_str, cursor_index, max_digits, pos_x, pos_y);
      fflush(stdin);

      int valid_key = 0;
      do {
         pressed_key = getch();
         if (pressed_key == 0 || pressed_key == (char)224) {
            getch();
            pressed_key = 0;
         }

         if ((pressed_key >= '0' && pressed_key <= '9') ||
             pressed_key == ENTER || pressed_key == ESC ||
             pressed_key == LEFT || pressed_key == BACKSPACE ||
             pressed_key == RIGHT) {
            valid_key = 1;
         } else if (pressed_key == '.') {
            if (strchr(decimal_str, '.') == NULL) {
               valid_key = 1;
            }
         }
      } while (!valid_key);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_digits - 1 && cursor_index < strlen(decimal_str)) {
            cursor_index++;
         }
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0) {
            cursor_index--;
         }
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BACKSPACE) {
            if (cursor_index > 0) {
               cursor_index--;
               memmove(decimal_str + cursor_index,
                       decimal_str + cursor_index + 1,
                       max_digits - cursor_index);
               decimal_str[max_digits - 1] = '\0';
            }
         } else {
            if (cursor_index < max_digits - 1) {
               memmove(decimal_str + cursor_index + 1,
                       decimal_str + cursor_index,
                       max_digits - cursor_index - 1);
               decimal_str[cursor_index] = pressed_key;
               cursor_index++;
               decimal_str[max_digits - 1] = '\0';
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC) {
      *result_value = (float)atof(decimal_str);
   }
   _setcursortype(0);
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
   return (isalpha((char)character) || isdigit((char)character));
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

/*
 * Funcion: GetCurrentDate
 * Objetivo: Obtener la fecha actual.
 * Argumentos: (__DATE) *date
 */
void GetCurrentDate(__DATE *date) {
   time_t __time = time(NULL);
   struct tm *tm = localtime(&__time);

   date->day = tm->tm_mday;
   date->month = tm->tm_mon + 1;
   date->year = tm->tm_year + 1900;
}

int CalculateAge(__DATE birth, __DATE current) {
   int age = current.year - birth.year;

   if (current.month < birth.month || (current.month == birth.month && current.day < birth.day)) {
      age--;
   }

   return age;
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
            captureAlphanumeric((char *)fields[isSelected].valuePtr, fields[isSelected].maxLength, FORM_VALUE_X, FORM_START_Y + isSelected * 3, fields[isSelected].validator ? fields[isSelected].validator : validateAlphachar);
            break;
         case FIELD_INT:
            memset(buff, 0, sizeof(buff));
            CaptureInteger(buff, (int *)fields[isSelected].valuePtr, fields[isSelected].maxLength, FORM_VALUE_X, FORM_START_Y + isSelected * 3);
            break;
         case FIELD_FLOAT:
            memset(buff, 0, sizeof(buff));
            CaptureFloat(buff, (float *)fields[isSelected].valuePtr, fields[isSelected].maxLength, FORM_VALUE_X, FORM_START_Y + isSelected * 3);
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
   gotoxy(TITLE_X_POS, TITLE_Y_POS);
   setcolor(TITLE_COLOR_FONT, TITLE_COLOR_BG);
   printf("Facturaci%cn  ", 162);

   int desplazamiento = (int)strlen("") + xpos;

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
 * Funcion: ShowTable
 * Objetivo: Renderizar una tabla navegable desde un archivo binario.
 * Argumentos:
 *   file       - Archivo binario a leer.
 *   recordSize - Tamaño del struct (sizeof).
 *   show       - Funcion que imprime una fila del struct.
 *   filterFn   - Funcion de filtro opcional (NULL = mostrar todo).
 *   filterKey  - Clave que se pasa al filtro (NULL si no hay filtro).
 *   xpos, ypos - Posicion en pantalla.
 * Retorna: indice del registro seleccionado, -1 si ESC.
 */
int ShowTable(FILE *file, int recordSize, void (*show)(void *data), int (*filterFn)(void *data, void *key), void *filterKey, int xpos, int ypos) {
   char *records = NULL;
   int count = 0;

   rewind(file);

   char *temp = malloc(recordSize);
   if (!temp) {
      return -1;
   }

   while (fread(temp, recordSize, 1, file) == 1) {
      if (filterFn == NULL || filterFn(temp, filterKey)) {
         records = realloc(records, (count + 1) * recordSize);
         if (!records)
            return -1;
         memcpy(records + count * recordSize, temp, recordSize);
         count++;
      }
   }

   if (count == 0) {
      gotoxy(xpos, ypos);
      setcolor(WARNING_CT, WARNING_CF);

      printf("No hay regristros para mostrar");
      colordefault();

      getch();
      printf("                                                                                                                                                                                                                     ");

      return -1;
   }

   int selected = 0;
   int key;

   do {
      for (int index = 0; index < count; index++) {
         gotoxy(xpos, ypos + index);
         if (index == selected) {
            setcolor(CTS, CFS);
         } else {
            setcolor(CT, CF);
         }

         show(records + index * recordSize);
         colordefault();
      }

      gotoxy(xpos, ypos + count + 1);
      setcolor(TITLE_COLOR_FONT, TITLE_COLOR_BG);
      printf("ARRIBA/ABAJO: Navegar | ENTER: Seleccionar | ESC: Volver");
      colordefault();

      key = getch();

      if (key == __UNDEFINED || key == ALPHA)
         key = getch();

      if (key == UP) {
         selected = (selected > 0) ? selected - 1 : count - 1;
      }

      if (key == DOWN) {
         selected = (selected < count - 1) ? selected + 1 : 0;
      }
      if (key == ESC) {
         for (int index = 0; index < count; index++) {
            gotoxy(xpos, ypos + index);
            printf("                                                                                                                                                                                                                     ");
         }
      }
   } while (key != ENTER && key != ESC);

   free(records);
   return (key == ESC) ? -1 : selected;
}

/*
 * Funcion: get_option_xpos
 * Objetivo: Obtener la posicion en X de las opciones principales (e.g: CRUD) para renderizar el dropdown debajo de esta.
 * Argumentos: (char) menu[][OPT_LENGTH], (int) TargetIndex, (int) StartX
 * Retorna: La posicion en X de una opcion principal
 */
int get_option_xpos(char menu[][OPT_LENGTH], int targetIndex, int startX) {
   int offset = startX;
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

/*
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

/*
 * Funcion: ArtPrice
 * Objetivo: Proceso de Articulo Proveedor, permite definir un precio de un articulo de cierto proveedor.
 * Argumentos (FILE) *ProviderFile, (FILE) *ArticleProviderFile
 */
void ArtPrice(FILE *fileProveedores, FILE *fileArticulos, FILE *fileArtProv) {
   system("cls");
   int selectedProv = ShowTable(fileProveedores, sizeof(PROVEEDOR), ShowProvider, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selectedProv == -1) {
      return;
   }

   PROVEEDOR prov;
   rewind(fileProveedores);
   for (int index = 0; index <= selectedProv; index++) {
      fread(&prov, sizeof(PROVEEDOR), 1, fileProveedores);
   }

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
   printf("Seleccione un Articulo para: %s", prov.Descripcion);

   int selectedArt = ShowTable(fileArticulos, sizeof(ARTICULO), ShowArticle, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selectedArt == -1) {
      return;
   }

   ARTICULO art;
   rewind(fileArticulos);
   for (int index = 0; index <= selectedArt; index++) {
      fread(&art, sizeof(ARTICULO), 1, fileArticulos);
   }

   ARTICULOPROVEEDOR ArtProv;
   long existingPos = -1;
   long pos = 0;
   rewind(fileArtProv);
   while (fread(&ArtProv, sizeof(ARTICULOPROVEEDOR), 1, fileArtProv) == 1) {
      if (strcmp(ArtProv.Id_Art, art.Id) == 0 &&
          strcmp(ArtProv.Id_Prov, prov.Id) == 0) {
         existingPos = pos;
         break;
      }
      pos++;
   }

   if (existingPos == -1) {
      memset(&ArtProv, 0, sizeof(ARTICULOPROVEEDOR));
   }

   strcpy(ArtProv.Id_Art, art.Id);
   strcpy(ArtProv.Id_Prov, prov.Id);

   FormField fields[] = {
       {"ID Articulo", ArtProv.Id_Art, FIELD_READONLY, MAXID - 1, NULL},
       {"ID Proveedor", ArtProv.Id_Prov, FIELD_READONLY, MAXID - 1, NULL},
       {"Precio", &ArtProv.precio, FIELD_FLOAT, ARTICLEPROVIDER_PRICE_FIELD_BUFFER, NULL},
   };

   if (ShowForm(fields, ARTICLEPROVIDER_FIELDS, "Establecer Precio")) {
      if (existingPos >= 0) {
         fseek(fileArtProv, existingPos * (long)sizeof(ARTICULOPROVEEDOR), SEEK_SET);
         fwrite(&ArtProv, sizeof(ARTICULOPROVEEDOR), 1, fileArtProv);
      } else {
         fseek(fileArtProv, 0, SEEK_END);
         fwrite(&ArtProv, sizeof(ARTICULOPROVEEDOR), 1, fileArtProv);
      }
   }
}

/*
 * Funcion: BillingFunc
 * Objetivo: Permitir el proceso de facturacion, seleccionando el cliente y los articulos con cantidad. Generando al final una factura detallada
 * Argumentos: (FILE) *ClientFile, (FILE) *UserFile, (FILE) *ArtProvFile, (FILE) *BillFile, (FILE) *DetailFile
 */
void BillingFunc(FILE *ClientFile, FILE *UserFile, FILE *ArtProvFile, FILE *BillFile, FILE *DetailFile) {
   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
   printf("Seleccione un Cliente");

   int selectedClient = ShowTable(ClientFile, sizeof(CLIENTE), ShowclientBill, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selectedClient == -1)
      return;

   CLIENTE client;
   rewind(ClientFile);
   for (int index = 0; index <= selectedClient; index++)
      fread(&client, sizeof(CLIENTE), 1, ClientFile);

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
   printf("Seleccione el Usuario que factura");

   int selectedUser = ShowTable(UserFile, sizeof(USUARIO), ShowUser, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selectedUser == -1) {
      return;
   }

   USUARIO user;
   rewind(UserFile);
   for (int index = 0; index <= selectedUser; index++) {
      fread(&user, sizeof(USUARIO), 1, UserFile);
   }

   FACTURA factura = {0};
   GenerateBillId(factura.numfactura, BillFile);
   strcpy(factura.IdCliente, client.Id);
   strcpy(factura.IdUsuario, user.Id);
   GetCurrentDate(&factura.Fecha);

   DETALLEFACTURA detalles[50];
   int detalleCount = 0;
   int key;

   do {
      system("cls");
      gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
      printf("Factura: %-5s  Cliente: %-20s  Items: %d",
             factura.numfactura, client.Nombre, detalleCount);

      for (int index = 0; index < detalleCount; index++) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + index);
         setcolor(CT, CF);
         printf("Art: %-5s  Prov: %-5s  Cant: %3d  Precio: %8.2f", detalles[index].IdArt, detalles[index].IdProv, detalles[index].cantidad, detalles[index].precio);
         colordefault();
      }

      gotoxy(FORM_LABEL_X, FORM_START_Y + detalleCount + 2);
      setcolor(TITLE_COLOR_FONT, TITLE_COLOR_BG);
      printf("[Y] Agregar articulo  [ENTER] Confirmar factura  [ESC] Cancelar");
      colordefault();

      key = getch();
      if (key == __UNDEFINED || key == ALPHA) {
         key = getch();
      }

      if (key == 'y' || key == 'Y') {
         system("cls");
         gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
         printf("Seleccione Articulo-Proveedor");

         int selectedAP = ShowTable(ArtProvFile, sizeof(ARTICULOPROVEEDOR), ShowArtProvBill, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
         if (selectedAP == -1) {
            continue;
         }

         ARTICULOPROVEEDOR ap;
         rewind(ArtProvFile);
         for (int index = 0; index <= selectedAP; index++) {
            fread(&ap, sizeof(ARTICULOPROVEEDOR), 1, ArtProvFile);
         }

         system("cls");
         gotoxy(FORM_LABEL_X, FORM_START_Y);
         printf("Art: %-5s  Prov: %-5s  Precio: %.2f", ap.Id_Art, ap.Id_Prov, ap.precio);

         gotoxy(FORM_LABEL_X, FORM_START_Y + 3);
         printf("Cantidad:");

         char buf[BUFFER] = {0};
         int cantidad = 0;
         CaptureInteger(buf, &cantidad, QUANTITY_BUFFER, FORM_LABEL_X + 12, FORM_START_Y + 3);

         if (cantidad <= 0) {
            continue;
         }

         if (detalleCount < 50) {
            memset(&detalles[detalleCount], 0, sizeof(DETALLEFACTURA));

            strcpy(detalles[detalleCount].numfactura, factura.numfactura);
            strcpy(detalles[detalleCount].IdArt, ap.Id_Art);
            strcpy(detalles[detalleCount].IdProv, ap.Id_Prov);

            detalles[detalleCount].cantidad = cantidad;
            detalles[detalleCount].precio = ap.precio;

            detalleCount++;
         }
      }

   } while (key != ENTER && key != ESC);

   if (key == ESC) {
      return;
   }

   if (detalleCount == 0) {
      system("cls");
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);
      printf("No se puede crear una factura sin articulos.");
      colordefault();
      getch();
      return;
   }

   fseek(BillFile, 0, SEEK_END);
   fwrite(&factura, sizeof(FACTURA), 1, BillFile);

   for (int index = 0; index < detalleCount; index++) {
      fseek(DetailFile, 0, SEEK_END);
      fwrite(&detalles[index], sizeof(DETALLEFACTURA), 1, DetailFile);
   }

   system("cls");

   gotoxy(FORM_LABEL_X, FORM_START_Y);
   setcolor(CT, CF);

   printf("Factura %s creada exitosamente. Items: %d", factura.numfactura, detalleCount);
   colordefault();

   getch();
}

/*
 * Funcion: CalctotalBill
 * Argumento: (char) *BillNum, (FILE) *DetailFile
 * Objetivo: Calcular el total de la factura de un cliente.
 * Retorna: El total de la factura de un cliente.
 */
float CalctotalBill(char *BillNum, FILE *DetailFile) {
   float total = 0.0;

   DETALLEFACTURA BillDetail;

   rewind(DetailFile);
   while (fread(&BillDetail, sizeof(DETALLEFACTURA), 1, DetailFile) == 1) {
      if (strcmp(BillDetail.numfactura, BillNum) == 0) {
         total += BillDetail.cantidad * BillDetail.precio;
      }
   }

   return total;
}

/*
 * Funcion: ConsultPendingBilling
 * Objetivo: Consultar los pagos pendientes de un cliente en especifico.
 * Argumentos: (FILE) *ClientFile, *BillFile, *DetailFile, *PaymentFile
 */
void ConsultPendingBilling(FILE *ClientFile, FILE *BillFile, FILE *DetailFile, FILE *PaymentFile) {
   system("cls");

   int selClient = ShowTable(ClientFile, sizeof(CLIENTE), ShowClient, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selClient == -1)
      return;

   CLIENTE client;
   rewind(ClientFile);
   for (int i = 0; i <= selClient; i++)
      fread(&client, sizeof(CLIENTE), 1, ClientFile);

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
   printf("Facturas pendientes de: %s %s", client.Nombre, client.Apellido);

   FACTURA bill;

   rewind(BillFile);
   int found = 0;

   while (fread(&bill, sizeof(FACTURA), 1, BillFile) == 1) {
      if (strcmp(bill.IdCliente, client.Id) == 0) {

         float total = CalctotalBill(bill.numfactura, DetailFile);
         float paid = CalcPayment(bill.numfactura, PaymentFile);
         float pending = total - paid;

         if (pending > 0.01f) {
            gotoxy(FORM_LABEL_X, FORM_START_Y + found);
            printf("Fact: %-5s | Fecha: %02d/%02d/%04d | Total: %.2f | Pagado: %.2f | Pendiente: %.2f",
                   bill.numfactura,
                   bill.Fecha.day, bill.Fecha.month, bill.Fecha.year,
                   total, paid, pending);
            found++;
         }
      }
   }

   if (found == 0) {
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      printf("No hay facturas pendientes.");
   }

   getch();
}

/*
 * Funcion: ConsultPayments
 * Argumentos: (FILE) *ClientFile, *BillFile, *PaymentFile
 * Objetivo: Consultar los pagos de un cliente en especifico
 */
void ConsultPayments(FILE *ClientFile, FILE *BillFile, FILE *PaymentFile) {
   system("cls");

   int selClient = ShowTable(ClientFile, sizeof(CLIENTE), ShowClient, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selClient == -1)
      return;

   CLIENTE client;
   rewind(ClientFile);
   for (int i = 0; i <= selClient; i++)
      fread(&client, sizeof(CLIENTE), 1, ClientFile);

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
   printf("Pagos de: %s %s", client.Nombre, client.Apellido);

   FACTURA bill;
   PAGO pago;
   int row = 0;

   rewind(BillFile);
   while (fread(&bill, sizeof(FACTURA), 1, BillFile) == 1) {

      if (strcmp(bill.IdCliente, client.Id) == 0) {

         rewind(PaymentFile);
         while (fread(&pago, sizeof(PAGO), 1, PaymentFile) == 1) {

            if (strcmp(pago.numfactura, bill.numfactura) == 0) {

               gotoxy(FORM_LABEL_X, FORM_START_Y + row);
               printf("Pago: %-5s | Factura: %-5s | Monto: %.2f",
                      pago.numpago, pago.numfactura, pago.monto);
               row++;
            }
         }
      }
   }

   if (row == 0) {
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      printf("No hay pagos registrados.");
   }

   getch();
}

/*
 * Funcion: ConsultBalance
 * Argumentos: (FILE) *ClientFile, *BillFile, *DetailFile, *PaymentFile
 * Objetivo: Consultar el balance de un cliente
 */
void ConsultBalance(FILE *ClientFile, FILE *BillFile, FILE *DetailFile, FILE *PaymentFile) {
   system("cls");

   int selClient = ShowTable(ClientFile, sizeof(CLIENTE), ShowClient, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selClient == -1)
      return;

   CLIENTE client;
   rewind(ClientFile);
   for (int i = 0; i <= selClient; i++)
      fread(&client, sizeof(CLIENTE), 1, ClientFile);

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y - 2);
   printf("Balance de: %s %s", client.Nombre, client.Apellido);

   FACTURA bill;
   int row = 0;

   float totalDebt = 0;
   float totalPaid = 0;

   rewind(BillFile);
   while (fread(&bill, sizeof(FACTURA), 1, BillFile) == 1) {

      if (strcmp(bill.IdCliente, client.Id) == 0) {

         float total = CalctotalBill(bill.numfactura, DetailFile);
         float paid = CalcPayment(bill.numfactura, PaymentFile);

         totalDebt += total;
         totalPaid += paid;

         gotoxy(FORM_LABEL_X, FORM_START_Y + row);
         printf("Fact: %-5s | Total: %.2f | Pagado: %.2f",
                bill.numfactura, total, paid);
         row++;
      }
   }

   float balance = totalDebt - totalPaid;

   gotoxy(FORM_LABEL_X, FORM_START_Y + row + 2);
   printf("TOTAL DEUDA: %.2f", totalDebt);

   gotoxy(FORM_LABEL_X, FORM_START_Y + row + 3);
   printf("TOTAL PAGADO: %.2f", totalPaid);

   gotoxy(FORM_LABEL_X, FORM_START_Y + row + 4);
   printf("BALANCE: %.2f", balance);

   getch();
}

/*
 * Funcion: CalcularPago
 * Objetivo: Calcular el pago hecho.
 * Argumentos: (char) *BillNum, (FILE) *PaymentFile
 * Retorna: El monto del pago hecho.
 */
float CalcPayment(char *BillNum, FILE *PaymentFile) {
   float paid = 0.0;
   PAGO payment;

   rewind(PaymentFile);
   while (fread(&payment, sizeof(PAGO), 1, PaymentFile) == 1) {
      if (strcmp(payment.numfactura, BillNum) == 0) {
         paid += payment.monto;
      }
   }

   return paid;
}

/*
 * Funcion: GeneratePaymentId
 * Objetivo: Generar el ID de un pago
 * Argumentos: (char) *newId, (FILE) *PaymentFile
 */
void GeneratePaymentId(char *newId, FILE *PaymentFile) {
   PAGO last;
   int LastNum = 0;

   rewind(PaymentFile);

   while (fread(&last, sizeof(PAGO), 1, PaymentFile) == 1) {
      LastNum = atoi(last.numpago);
   }

   sprintf(newId, "%04d", LastNum + 1);
}

/*
 * Funcion: ShowPendingBill
 * Argumentos: (void) *data, (FILE) *DetailFiles, (FILE) *PaymentFile
 * Objetivo: Mostrar las facturas pendientes de cierto cliente.
 */
void ShowPendingBill(void *data, FILE *DetailFiles, FILE *PaymentFile) {
   FACTURA *Bill = (FACTURA *)data;
   float total = CalctotalBill(Bill->numfactura, DetailFiles);
   float paid = CalcPayment(Bill->numfactura, PaymentFile);
   float pending = total - paid;

   printf("Factura: %-6s | Fecha: %02d/%02d/%04d | Total: %8.2f | Pagado: %8.2f | Pendiente: %8.2f",
          Bill->numfactura, Bill->Fecha.day, Bill->Fecha.month, Bill->Fecha.year, total, paid, pending);
}

/*
 * Funcion: PayFunc
 * Objetivo: Permitir al usuario pagar una factura pendiente
 * Argumentos: (FILE) Clientfile, BillFile, DetailFile, PaidFile+
 */
void PayFunc(FILE *ClientFile, FILE *BillFile, FILE *DetailFile, FILE *PaidFile) {
   system("cls");

   int selClient = ShowTable(ClientFile, sizeof(CLIENTE), ShowClient, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selClient == -1) {
      return;
   }

   CLIENTE client;
   rewind(ClientFile);
   for (int index = 0; index <= selClient; index++) {
      fread(&client, sizeof(CLIENTE), 1, ClientFile);
   }

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y - 2);

   printf("Facturas pendientes de clientes: %s %s", client.Nombre, client.Apellido);

   char *records = NULL;
   int count = 0;

   FACTURA temp;

   rewind(BillFile);
   while (fread(&temp, sizeof(FACTURA), 1, BillFile) == 1) {
      if (strcmp(temp.IdCliente, client.Id) == 0) {
         float total = CalctotalBill(temp.numfactura, DetailFile);
         float paid = CalcPayment(temp.numfactura, PaidFile);

         if (total - paid > 0.01f) {
            records = realloc(records, (count + 1) * sizeof(FACTURA));

            if (!records) {
               return;
            }

            memcpy(records + count * sizeof(FACTURA), &temp, sizeof(FACTURA));
            count++;
         }
      }
   }

   if (count == 0) {
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);

      printf("Este cliente no tiene facturas pendientes.");
      colordefault();

      getch();
      free(records);

      return;
   }

   int selected = 0;
   int key;
   do {
      for (int i = 0; i < count; i++) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + i);
         if (i == selected)
            setcolor(CTS, CFS);
         else
            setcolor(CT, CF);
         ShowPendingBill(records + i * sizeof(FACTURA), DetailFile, BillFile);
         colordefault();
      }

      gotoxy(FORM_LABEL_X, FORM_START_Y + count + 1);
      setcolor(TITLE_COLOR_FONT, TITLE_COLOR_BG);
      printf("ARRIBA/ABAJO: Navegar | ENTER: Seleccionar | ESC: Volver");
      colordefault();

      key = getch();
      if (key == __UNDEFINED || key == ALPHA)
         key = getch();

      if (key == UP)
         selected = (selected > 0) ? selected - 1 : count - 1;
      if (key == DOWN)
         selected = (selected < count - 1) ? selected + 1 : 0;
   } while (key != ENTER && key != ESC);

   if (key == ESC) {
      free(records);
      return;
   }

   FACTURA factura;
   memcpy(&factura, records + selected * sizeof(FACTURA), sizeof(FACTURA));
   free(records);

   float total = CalctotalBill(factura.numfactura, DetailFile);
   float pagado = CalcPayment(factura.numfactura, PaidFile);
   float pendiente = total - pagado;

   system("cls");
   gotoxy(FORM_LABEL_X, FORM_START_Y);
   printf("Factura: %s   Pendiente: %.2f", factura.numfactura, pendiente);

   char buf[ARTICLEPROVIDER_PRICE_FIELD_BUFFER] = "";
   float monto = 0.0;

   do {
      gotoxy(FORM_LABEL_X, FORM_START_Y + 3);
      printf("Monto a pagar (0 - %.2f): ", pendiente);
      CaptureFloat(buf, &monto, ARTICLEPROVIDER_PRICE_FIELD_BUFFER, FORM_LABEL_X + 30, FORM_START_Y + 3);

      if (monto <= 0 || monto > pendiente + 0.01f) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + 6);
         setcolor(WARNING_CT, WARNING_CF);

         printf("Monto inválido. Debe estar entre 0 y %.2f", pendiente);
         colordefault();

         getch();
         system("cls");
      }
   } while (monto <= 0 || monto > pendiente + 0.01f);

   PAGO pago;
   GeneratePaymentId(pago.numpago, PaidFile);
   strcpy(pago.numfactura, factura.numfactura);
   pago.monto = monto;

   fseek(PaidFile, 0, SEEK_END);
   fwrite(&pago, sizeof(PAGO), 1, PaidFile);
   fflush(PaidFile);

   gotoxy(FORM_LABEL_X, FORM_START_Y + 9);
   setcolor(CFS, CTS);
   printf("Pago registrado correctamente (N° %s - %.2f)", pago.numpago, monto);
   colordefault();
   getch();
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
       {"Id", article->Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Descripcion", article->Descripcion, FIELD_ALPHA, LENDESC - 1, validateAlphachar},
       {"Cantidad", &article->cantidad, FIELD_INT, QUANTITY_BUFFER, NULL},
       {"Reorden", &article->reorden, FIELD_INT, REORDER_BUFFER, NULL},
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
   int selected = ShowTable(file, sizeof(ARTICULO), ShowArticle, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   ARTICULO record;
   rewind(file);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(ARTICULO), 1, file);

   FormField fields[] = {
       {"Id", record.Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Descripcion", record.Descripcion, FIELD_ALPHA, LENDESC - 1, validateAlphachar},
       {"Cantidad", &record.cantidad, FIELD_INT, QUANTITY_BUFFER, NULL},
       {"Reorden", &record.reorden, FIELD_INT, REORDER_BUFFER, NULL},
   };

   if (ShowForm(fields, ARTICLE_FIELDS, "Modificar Articulo")) {
      fseek(file, (long)selected * sizeof(ARTICULO), SEEK_SET);
      fwrite(&record, sizeof(ARTICULO), 1, file);
   }
}

/*
 * Funcion: DeleteArticle
 * Objetivo: Verificar la integridad referencial y eliminar un articulo del archivo, SOLO EN CASO DE QUE SE MANTENGA LA INTEGRIDAD REFERENCIAL.
 * Argumentos: (FILE) *fileArticulos, (FILE) *fileArtProv
 */
void DeleteArticle(FILE *ArticleFile, FILE *fileArtProv) {
   system("cls");
   int selected = ShowTable(ArticleFile, sizeof(ARTICULO), ShowArticle, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   ARTICULO record;
   rewind(ArticleFile);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(ARTICULO), 1, ArticleFile);

   ARTICULOPROVEEDOR ap;
   rewind(fileArtProv);
   while (fread(&ap, sizeof(ARTICULOPROVEEDOR), 1, fileArtProv) == 1) {
      if (strcmp(ap.Id_Art, record.Id) == 0) {
         gotoxy(FORM_LABEL_X, FORM_START_Y);
         setcolor(WARNING_CT, WARNING_CF);
         printf("No se puede eliminar: articulo referenciado en Articulo-Proveedor.");
         colordefault();
         getch();
         return;
      }
   }

   if (!ConfirmDelete(record.Id))
      return;

   FILE *temp = fopen("temp.dat", "wb");
   if (!temp)
      return;
   rewind(ArticleFile);
   while (fread(&record, sizeof(ARTICULO), 1, ArticleFile) == 1) {
      if (strcmp(record.Id, record.Id) != 0)
         fwrite(&record, sizeof(ARTICULO), 1, temp);
   }
   fclose(temp);
   freopen("articulos.dat", "wb", ArticleFile);
   temp = fopen("temp.dat", "rb");
   while (fread(&record, sizeof(ARTICULO), 1, temp) == 1)
      fwrite(&record, sizeof(ARTICULO), 1, ArticleFile);
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
void GenerateProviderId(char *newId, FILE *ProviderFile) {
   PROVEEDOR last;
   int lastNum = 0;
   rewind(ProviderFile);
   while (fread(&last, sizeof(PROVEEDOR), 1, ProviderFile) == 1)
      ;
   if (last.Id[0] == 'P')
      lastNum = atoi(last.Id + 1);
   sprintf(newId, "P%04d", lastNum + 1);
}

/*
 * Funcion: GenerateClientId
 * Objetivo: Generar un Id de un cliente nuevo
 * Argumentos: (char) *newId, (FILE) *ClientFile
 */
void GenerateClientId(char *newId, FILE *ClientFile) {
   CLIENTE last;
   int lastNum = 0;
   rewind(ClientFile);
   while (fread(&last, sizeof(CLIENTE), 1, ClientFile) == 1) {
      if (last.Id[0] == 'C')
         lastNum = atoi(last.Id + 1);
   }
   sprintf(newId, "C%04d", lastNum + 1);
}

/*
 * Funcion: GenerateUserId
 * Objetivo: Generar el Id de un usuario nuevo
 * Argumentos: (char) *newId, (FILE) *UserFile
 */
void GenerateUserId(char *newId, FILE *UserFile) {
   USUARIO last;
   int lastNum = 0;
   rewind(UserFile);
   while (fread(&last, sizeof(USUARIO), 1, UserFile) == 1) {
      if (last.Id[0] == 'U')
         lastNum = atoi(last.Id + 1);
   }
   sprintf(newId, "U%04d", lastNum + 1);
}

/*
 * Funcion: GenerateBillId
 * Argumentos: (char) *newId, (FILE) *BillFile
 * Objetivo: Generar un ID a una factura nueva.
 */
void GenerateBillId(char *newId, FILE *BillFile) {
   FACTURA last;
   int lastNum = 0;

   rewind(BillFile);
   while (fread(&last, sizeof(FACTURA), 1, BillFile) == 1)
      ;

   if (last.numfactura[0] == 'F') {
      lastNum = atoi(last.numfactura + 1);
   }

   sprintf(newId, "F%04d", lastNum + 1);
}

/*
 * Funcion: GetKeyClient
 * Objetivo: Obtener el ID del cliente
 * Retorna: 1, si consigue el ID, 0 si no existe el ID.
 */
int GetKeyClient(void *data, void *key) {
   return strcmp(((CLIENTE *)data)->Id, (char *)key) == 0;
}

int CompareClient(void *inputA, void *inputB) {
   return strcmp(((CLIENTE *)inputA)->Id, ((CLIENTE *)inputB)->Id);
}

/*
 * Funcion: ShowClient
 * Objetivo: Mostrar informacion sobre los clientes en un formato tabla.
 * Argumentos: (void) *data
 */
void ShowClient(void *data) {
   CLIENTE *client = (CLIENTE *)data;
   printf("ID: %-5s | Nombre: %-20s | Apellido: %-15s | Nac: %02d/%02d/%04d", client->Id, client->Nombre, client->Apellido, client->Nacimiento.day, client->Nacimiento.month, client->Nacimiento.year);
}

/*
 * Funcion: ReadClient
 * Objetivo: Permitir al usuario ingresar un nuevo cliente al sistema.
 * Argumentos: (void) *data, (FILE) ClientFile
 */
void ReadClient(void *data, FILE *ClientFile) {
   CLIENTE *client = (CLIENTE *)data;
   GenerateClientId(client->Id, ClientFile);

   FormField fields[] = {
       {"Id", client->Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Nombre", client->Nombre, FIELD_ALPHA, LENNOMB - 1, validateAlphachar},
       {"Apellido", client->Apellido, FIELD_ALPHA, LENAPEL - 1, validateAlphachar},
       {"Nacimiento", &client->Nacimiento, FIELD_DATE, 10, NULL}};

   if (ShowForm(fields, 4, "Nuevo cliente")) {
      __DATE today;
      GetCurrentDate(&today);

      if (CalculateAge(client->Nacimiento, today) < 18) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + 10);
         setcolor(WARNING_CT, WARNING_CF);
         printf("El cliente debe tener 18 o m%cs a%cos", 160, 164);
         getch();
         return;
      }
      fseek(ClientFile, 0, SEEK_END);
      fwrite(client, sizeof(CLIENTE), 1, ClientFile);
      fflush(ClientFile);
   }
}

void ModifyClient(FILE *ClientFile) {
   system("cls");
   int selected = ShowTable(ClientFile, sizeof(CLIENTE), ShowClient, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   CLIENTE record;
   rewind(ClientFile);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(CLIENTE), 1, ClientFile);

   FormField fields[] = {
       {"Id", record.Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Nombre", record.Nombre, FIELD_ALPHA, LENNOMB - 1, validateAlphachar},
       {"Apellido", record.Apellido, FIELD_ALPHA, LENAPEL - 1, validateAlphachar},
       {"Nacimiento", &record.Nacimiento, FIELD_DATE, 10, NULL},
   };

   if (ShowForm(fields, 4, "Modificar Cliente")) {
      __DATE today;
      GetCurrentDate(&today);
      if (CalculateAge(record.Nacimiento, today) < 18) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + 10);
         setcolor(RED, BLACK);
         printf("Cliente debe tener 18 a%cos o m%cs", 164, 160);
         colordefault();
         getch();
         return;
      }
      fseek(ClientFile, (long)selected * sizeof(CLIENTE), SEEK_SET);
      fwrite(&record, sizeof(CLIENTE), 1, ClientFile);
      fflush(ClientFile);
   }
}

void DeleteClient(FILE *Clientfile, FILE *BillFile) {
   system("cls");
   int selected = ShowTable(Clientfile, sizeof(CLIENTE), ShowClient, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   CLIENTE record;
   rewind(Clientfile);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(CLIENTE), 1, Clientfile);

   FACTURA bill;
   int referenced = 0;
   rewind(BillFile);
   while (fread(&bill, sizeof(FACTURA), 1, BillFile) == 1) {
      if (strcmp(bill.IdCliente, record.Id) == 0) {
         referenced = 1;
         break;
      }
   }
   if (referenced) {
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);
      printf("No se puede eliminar: cliente referenciado en facturas.");
      colordefault();
      getch();
      return;
   }

   if (!ConfirmDelete(record.Id))
      return;

   FILE *temp = fopen("temp.dat", "wb");
   if (!temp)
      return;

   CLIENTE __temp;
   rewind(Clientfile);
   while (fread(&__temp, sizeof(CLIENTE), 1, Clientfile) == 1) {
      if (strcmp(__temp.Id, record.Id) != 0)
         fwrite(&__temp, sizeof(CLIENTE), 1, temp);
      fflush(Clientfile);
   }
   fclose(temp);

   freopen("clientes.dat", "wb", Clientfile);
   temp = fopen("temp.dat", "rb");
   while (fread(&__temp, sizeof(CLIENTE), 1, temp) == 1)
      fwrite(&__temp, sizeof(CLIENTE), 1, Clientfile);
   fflush(Clientfile);
   fclose(temp);
   remove("temp.dat");
}

/*
 * Funcion: ShowclientBill
 * Objetivo: Mostrar en formato tabla los clientes con factura
 * Argumentos: (void) *data
 */
void ShowclientBill(void *data) {
   CLIENTE *client = (CLIENTE *)data;
   printf("%-5s | %-20s | %-15s", client->Id, client->Nombre, client->Apellido);
}

/*
 * funcion: GetKeyUser
 * Objetivo: Obtener el Id y verificar si existe el usuario.
 * Retorna: 1 si existe, 0 si no existe.
 */
int GetKeyUser(void *Data, void *key) {
   return strcmp(((USUARIO *)Data)->Id, (char *)key) == 0;
}

/*
 * Funcion: CompareUser
 * Objetivo: Comparar entre usuarios
 * Argumento: (void) *inputA, (void) *inputB
 * Retorna: 1 si son iguales, 0 si no son iguales.
 */
int CompareUser(void *inputA, void *inputB) {
   return strcmp(((USUARIO *)inputA)->Id, ((USUARIO *)inputB)->Id);
}

/*
 * Funcion: ShowUser
 * Objetivo: Mostrar la informacion de los usuarios en formato tabla
 * Argumentos: (void) *data
 */
void ShowUser(void *data) {
   USUARIO *u = (USUARIO *)data;
   printf("ID: %-5s | Nombre: %-20s | Apellido: %-15s | Nac: %02d/%02d/%04d | Jefe: %-5s",
          u->Id, u->Nombre, u->Apellido, u->Nacimiento.day, u->Nacimiento.month, u->Nacimiento.year, u->Id_jefe);
}

/*
 * Funcion: ReadUser
 * Objetivo: Permitir que el usuario ingrese un "usuario" nuevo.
 * Argumentos: (void) *data, (FILE) *UserFile
 */
void readUser(void *data, FILE *UserFile) {
   USUARIO *u = (USUARIO *)data;
   GenerateUserId(u->Id, UserFile);

   FormField fields[] = {
       {"Id", u->Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Nombre", u->Nombre, FIELD_ALPHA, LENNOMB - 1, validateAlphachar},
       {"Apellido", u->Apellido, FIELD_ALPHA, LENAPEL - 1, validateAlphachar},
       {"Password", u->PassWord, FIELD_ALPHA, LENPWD - 1, validateAlphachar},
       {"Nacimiento", &u->Nacimiento, FIELD_DATE, 10, NULL},
       {"Id Jefe", u->Id_jefe, FIELD_ALPHA, MAXID - 1, validateAlphachar},
   };

   if (ShowForm(fields, 6, "Nuevo Usuario")) {
      __DATE today;
      GetCurrentDate(&today);
      if (CalculateAge(u->Nacimiento, today) < 18) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + 10);
         setcolor(RED, BLACK);
         printf("Usuario debe tener >= 18 años");
         colordefault();
         getch();
         return;
      }
      fseek(UserFile, 0, SEEK_END);
      fwrite(u, sizeof(USUARIO), 1, UserFile);
   }
}

/*
 * Funcion: ModifyUser
 * Objetivo: Modificar un usuario existente
 * Argumentos: (FILE) *UserFile
 */
void ModifyUser(FILE *UserFile) {
   system("cls");
   int selected = ShowTable(UserFile, sizeof(USUARIO), ShowUser, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   USUARIO record;
   rewind(UserFile);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(USUARIO), 1, UserFile);

   FormField fields[] = {
       {"Id", record.Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Nombre", record.Nombre, FIELD_ALPHA, LENNOMB - 1, validateAlphachar},
       {"Apellido", record.Apellido, FIELD_ALPHA, LENAPEL - 1, validateAlphachar},
       {"Password", record.PassWord, FIELD_ALPHA, LENPWD - 1, validateAlphachar},
       {"Nacimiento", &record.Nacimiento, FIELD_DATE, 10, NULL},
       {"Id Jefe", record.Id_jefe, FIELD_ALPHA, MAXID - 1, validateAlphachar},
   };

   if (ShowForm(fields, 6, "Modificar Usuario")) {
      __DATE today;
      GetCurrentDate(&today);
      if (CalculateAge(record.Nacimiento, today) < 18) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + 10);
         setcolor(RED, BLACK);
         printf("Usuario debe tener >= 18 años");
         colordefault();
         getch();
         return;
      }
      fseek(UserFile, (long)selected * sizeof(USUARIO), SEEK_SET);
      fwrite(&record, sizeof(USUARIO), 1, UserFile);
   }
}

/*
 * Funcion: DeleteUser
 * Objetivo: Eliminar un usuario, verificando que no este ligado a ningun dato.
 * Argumentos: (FILE) *UserFile, *BillFile
 */
void DeleteUser(FILE *UserFile, FILE *BillFile) {
   system("cls");
   int selected = ShowTable(UserFile, sizeof(USUARIO), ShowUser, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   USUARIO record;
   rewind(UserFile);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(USUARIO), 1, UserFile);

   FACTURA bill;
   int referenced = 0;
   rewind(BillFile);
   while (fread(&bill, sizeof(FACTURA), 1, BillFile) == 1) {
      if (strcmp(bill.IdUsuario, record.Id) == 0) {
         referenced = 1;
         break;
      }
   }

   if (referenced) {
      gotoxy(FORM_LABEL_X, FORM_START_Y);
      setcolor(WARNING_CT, WARNING_CF);
      printf("No se puede eliminar: usuario referenciado en facturas.");
      colordefault();
      getch();
      return;
   }

   if (!ConfirmDelete(record.Id))
      return;

   FILE *temp = fopen("temp.dat", "wb");
   if (!temp)
      return;

   USUARIO _temp;
   rewind(UserFile);
   while (fread(&_temp, sizeof(USUARIO), 1, UserFile) == 1) {
      if (strcmp(_temp.Id, record.Id) != 0)
         fwrite(&_temp, sizeof(USUARIO), 1, temp);
   }
   fclose(temp);

   freopen("usuarios.dat", "wb", UserFile);
   temp = fopen("temp.dat", "rb");
   while (fread(&_temp, sizeof(USUARIO), 1, temp) == 1) {
      fwrite(&_temp, sizeof(USUARIO), 1, UserFile);
   }

   fclose(temp);
   remove("temp.dat");
}

/*
 * Funcion: GetKeyProvider
 * Objetivo: Verificar si un proveedor existe
 * Argumentos: (void) *data, (void) *key
 * Retorna: 1 si existe, 0 si no existe.
 */
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
       {"Id", provider->Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Descripcion", provider->Descripcion, FIELD_ALPHA, LENDESC - 1, validateAlphachar},
       {"Direccion", provider->Direccion, FIELD_ALPHA, LENDIR - 1, validateAlphachar},
       {"Nacimiento", &provider->Nacimiento, FIELD_DATE, (DAY_BUFFER + MONTH_BUFFER + YEAR_BUFFER) - 1, NULL},
   };

   if (ShowForm(fields, PROVIDER_FIELDS, "Nuevo proveedor")) {
      __DATE today;
      GetCurrentDate(&today);
      if (CalculateAge(provider->Nacimiento, today) < 18) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + 10);
         setcolor(RED, BLACK);
         printf("Proveedor debe tener >= 18 años");
         colordefault();
         getch();
         return;
      }
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
void ModifyProvider(FILE *ProviderFile) {
   system("cls");
   int selected = ShowTable(ProviderFile, sizeof(PROVEEDOR), ShowProvider, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   PROVEEDOR record;
   rewind(ProviderFile);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(PROVEEDOR), 1, ProviderFile);

   FormField fields[] = {
       {"Id", record.Id, FIELD_READONLY, MAXID - 1, NULL},
       {"Descripcion", record.Descripcion, FIELD_ALPHA, LENDESC - 1, validateAlphachar},
       {"Direccion", record.Direccion, FIELD_ALPHA, LENDIR - 1, validateAlphachar},
       {"Nacimiento", &record.Nacimiento, FIELD_DATE, 10, NULL},
   };

   if (ShowForm(fields, PROVIDER_FIELDS, "Modificar Proveedor")) {
      __DATE today;
      GetCurrentDate(&today);
      if (CalculateAge(record.Nacimiento, today) < 18) {
         gotoxy(FORM_LABEL_X, FORM_START_Y + 10);
         setcolor(RED, BLACK);
         printf("Proveedor debe tener >= 18 años");
         colordefault();
         getch();
         return;
      }
      fseek(ProviderFile, (long)selected * sizeof(PROVEEDOR), SEEK_SET);
      fwrite(&record, sizeof(PROVEEDOR), 1, ProviderFile);
   }
}

/*
 * Funcion: DeleteProvider
 * Objetivo: Verificar integridad referencial y eliminar un proveedor del archivo.
 * Argumentos: (FILE) *fileProveedores, (FILE) *fileArtProv
 */
void DeleteProvider(FILE *FileProviders, FILE *fileArtProv) {
   system("cls");
   int selected = ShowTable(FileProviders, sizeof(PROVEEDOR), ShowProvider, NULL, NULL, FORM_LABEL_X, FORM_START_Y);
   if (selected == -1)
      return;

   PROVEEDOR record;
   rewind(FileProviders);
   for (int i = 0; i <= selected; i++)
      fread(&record, sizeof(PROVEEDOR), 1, FileProviders);

   ARTICULOPROVEEDOR ap;
   rewind(fileArtProv);
   while (fread(&ap, sizeof(ARTICULOPROVEEDOR), 1, fileArtProv) == 1) {
      if (strcmp(ap.Id_Prov, record.Id) == 0) {
         gotoxy(FORM_LABEL_X, FORM_START_Y);
         setcolor(WARNING_CT, WARNING_CF);
         printf("No se puede eliminar: proveedor referenciado en Articulo-Proveedor.");
         colordefault();
         getch();
         return;
      }
   }

   if (!ConfirmDelete(record.Id))
      return;

   FILE *temp = fopen("temp.dat", "wb");
   if (!temp)
      return;
   rewind(FileProviders);
   while (fread(&record, sizeof(PROVEEDOR), 1, FileProviders) == 1) {
      if (strcmp(record.Id, record.Id) != 0)
         fwrite(&record, sizeof(PROVEEDOR), 1, temp);
   }
   fclose(temp);
   freopen("proveedores.dat", "wb", FileProviders);
   temp = fopen("temp.dat", "rb");
   while (fread(&record, sizeof(PROVEEDOR), 1, temp) == 1)
      fwrite(&record, sizeof(PROVEEDOR), 1, FileProviders);
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
 * Funcion: GetKeyArtProvByProv
 * Objetivo: Filtrar registros ARTICULOPROVEEDOR por Id_Prov.
 * Retorna: 1 si coincide, 0 si no.
 */
int GetKeyArtProvByProv(void *data, void *key) {
   return strcmp(((ARTICULOPROVEEDOR *)data)->Id_Prov, (char *)key) == 0;
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
       {"ID Articulo", ArticleProvider->Id_Art, FIELD_ALPHA, MAXID - 1, validateAlphachar},
       {"ID Proveedor", ArticleProvider->Id_Prov, FIELD_ALPHA, MAXID - 1, validateAlphachar},
       {"Precio", &ArticleProvider->precio, FIELD_FLOAT, ARTICLEPROVIDER_PRICE_FIELD_BUFFER, NULL},
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
       {"ID Articulo", record.Id_Art, FIELD_READONLY, MAXID - 1, NULL},
       {"ID Proveedor", record.Id_Prov, FIELD_READONLY, MAXID - 1, NULL},
       {"Precio", &record.precio, FIELD_FLOAT, ARTICLEPROVIDER_PRICE_FIELD_BUFFER, NULL},
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
 * Funcion: ShowArtProvBill
 * Argumentos: (void) *data
 * Objetivo: mostrar en formato tabla un articulo-proveedor en la seleccion de los items que un cliente adquirio.
 */
void ShowArtProvBill(void *data) {
   ARTICULOPROVEEDOR *ArtProv = (ARTICULOPROVEEDOR *)data;
   printf("Art: %-5s  Prov: %-5s  Precio: %8.2f",
          ArtProv->Id_Art, ArtProv->Id_Prov, ArtProv->precio);
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

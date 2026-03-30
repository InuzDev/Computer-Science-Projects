#include <conio.c>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CT BLACK
#define CF CYAN
#define CTS BLACK
#define CFS YELLOW

#define CET BLACK
#define CEF GREEN

#define CT_ROW LIGHTGRAY
#define CF_ROW BLACK

#define CTS_ROW BLACK
#define CFS_ROW BLUE

#define SELECTED_TEXTCOLOR BLACK
#define SELECTED_BGCOLOR BLUE
#define TEXT_COLOR LIGHTGRAY
#define BG_COLOR BLACK

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ESC 27
#define ENTER 13
#define BACKSPACE 8
#define TAB 9

#define OPT_LENGTH 81
#define ROWS_VISIBLE 10

#define POSXINI 1
#define POSYINI 1
#define TABLE_XINIT 1
#define TABLE_YINIT 3
#define FORM_Y 4

#define LENIDAUTOR 11
#define LENNOMB 41
#define LENAPEL 36

#define DATA_FILE "autores.dat"

typedef struct {
   int dd, mm, yyyy;
} FECHA;

typedef struct {
   char idautor[LENIDAUTOR];
   char nombres[LENNOMB];
   char apellidos[LENAPEL];
   FECHA fechanacimiento;
} AUTOR;

typedef struct {
   int id, nombres, apellidos, fecha;
} AnchoColumnas;

struct Node {
   AUTOR data;
   struct Node *next;
   struct Node *prev;
};

struct Node *createNode(AUTOR newAutor);
void insertAtEnd(struct Node **head, AUTOR newAutor);
void cargarDesdeArchivo(struct Node **head);
void guardarEnArchivo(struct Node *head);
void liberarLista(struct Node **head);

void setcolor(int ct, int cf);
void colordefault();

void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y);
int validateAlphachar(char character);
void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char character));
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int pos_x, int pos_y);
int ValidDate(int day, int month, int year);
void CaptureFecha(FECHA *fecha, int pos_x, int pos_y);

void show_header(char menu[][OPT_LENGTH], int options, int Xpos, int Ypos, int isSelected);
void show_form(AUTOR *autor, int campo_sel, int es_nuevo);
int EditForm(AUTOR *autor, int es_nuevo);

int contarNodos(struct Node *head);
struct Node *obtenerNodoPorIndice(struct Node *head, int indice);
AnchoColumnas calc_col_widths(struct Node *head);
void show_table_header(int px, int py, AnchoColumnas anchos);
void show_table_row(struct Node *nodo, int py, int px, int seleccionado, AnchoColumnas anchos);
int movimiento_autores(struct Node *head, int cantidad, int py, int px, AnchoColumnas anchos);

void IngreseAutor(struct Node **head);
void ModificarAutor(struct Node **head);
void EliminarAutor(struct Node **head);
void ListarAutores(struct Node *head);

int main() {
   _setcursortype(0);
   char menu[][OPT_LENGTH] = {
       "INGRESAR", "MODIFICAR", "ELIMINAR", "LISTAR", "SALIR"};
   int opcion_sel = 0;
   int redibuja = 1;
   int tecla;

   struct Node *listaAutores = NULL;
   cargarDesdeArchivo(&listaAutores);

   do {
      if (redibuja) {
         system("cls");
         redibuja = 0;
      }
      show_header(menu, 5, POSXINI, POSYINI, opcion_sel);

      tecla = getch();
      if (tecla == 0 || tecla == 224)
         tecla = getch();

      if (tecla == LEFT && opcion_sel > 0)
         opcion_sel--;
      if (tecla == RIGHT && opcion_sel < 4)
         opcion_sel++;

      if (tecla == ENTER) {
         switch (opcion_sel) {
         case 0:
            IngreseAutor(&listaAutores);
            break;
         case 1:
            ModificarAutor(&listaAutores);
            break;
         case 2:
            EliminarAutor(&listaAutores);
            break;
         case 3:
            ListarAutores(listaAutores);
            break;
         case 4:
            tecla = ESC;
            break;
         }
         if (tecla != ESC)
            redibuja = 1;
      }
   } while (tecla != ESC);

   guardarEnArchivo(listaAutores);
   liberarLista(&listaAutores);

   _setcursortype(100);
   return 0;
}

/*
 * Funcion  : createNode
 * Objetivo : Crea un nuevo nodo para la lista doblemente enlazada.
 * Argumentos:
 *   newAutor - Estructura con todos los datos del autor que se va a guardar.
 * Retorna  : Puntero al nodo recién creado.
 */
struct Node *createNode(AUTOR newAutor) {
   struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

   if (newNode == NULL) {
      printf("Hubo un error alojando memoria.\n");
      return NULL;
   }

   newNode->data = newAutor;
   newNode->next = NULL;
   newNode->prev = NULL;
   return newNode;
}

/*
 * Funcion  : insertAtEnd
 * Objetivo : Agrega un nuevo autor al final de la lista.
 * Argumentos:
 *   head     - Puntero a la cabeza de la lista (se modifica si la lista estaba vacía).
 *   newAutor - Datos del autor que se va a insertar.
 */
void insertAtEnd(struct Node **head, AUTOR newAutor) {
   struct Node *newNode = createNode(newAutor);
   if (newNode == NULL)
      return;

   if (*head == NULL) {
      *head = newNode;
      return;
   }

   struct Node *temp = *head;
   while (temp->next != NULL) {
      temp = temp->next;
   }

   temp->next = newNode;
   newNode->prev = temp;
}

/*
 * Funcion  : cargarDesdeArchivo
 * Objetivo : Lee todos los autores guardados en el archivo y los carga en la lista.
 * Argumentos:
 *   head - Puntero a la cabeza de la lista (se llena con los datos del archivo).
 */
void cargarDesdeArchivo(struct Node **head) {
   FILE *file = fopen(DATA_FILE, "rb");
   if (!file)
      return;

   AUTOR tempAutor;
   while (fread(&tempAutor, sizeof(AUTOR), 1, file)) {
      insertAtEnd(head, tempAutor);
   }
   fclose(file);
}

/*
 * Funcion  : guardarEnArchivo
 * Objetivo : Guarda toda la lista de autores en el archivo binario.
 * Argumentos:
 *   head - Cabeza de la lista que se va a guardar.
 */
void guardarEnArchivo(struct Node *head) {
   FILE *file = fopen(DATA_FILE, "wb");
   if (!file)
      return;

   struct Node *temp = head;
   while (temp != NULL) {
      fwrite(&temp->data, sizeof(AUTOR), 1, file);
      temp = temp->next;
   }
   fclose(file);
}

/*
 * Funcion  : liberarLista
 * Objetivo : Libera toda la memoria de la lista para cerrar el programa correctamente.
 * Argumentos:
 *   head - Puntero a la cabeza de la lista.
 */
void liberarLista(struct Node **head) {
   struct Node *temp;
   while (*head != NULL) {
      temp = *head;
      *head = (*head)->next;
      free(temp);
   }
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
   setcolor(TEXT_COLOR, BG_COLOR);
   for (int index = 0; index < max_length; index++) {
      gotoxy(pos_x + index + 1, pos_y);
      printf("%c", field_str[index] ? field_str[index] : ' ');
   }
   colordefault();
   gotoxy(pos_x + cursor_position + 1, pos_y);
}

/*
 * Funcion  : validateAlphachar
 * Objetivo : Verifica si un carácter es letra, espacio o guion.
 * Argumentos:
 *   character - Carácter que se quiere validar.
 * Retorna  : 1 si es válido, 0 si no lo es.
 */
int validateAlphachar(char character) {
   return (isalpha((unsigned char)character) || character == ' ' || character == '-');
}

/*
 * Funcion  : captureAlphanumeric
 * Objetivo : Permite al usuario escribir texto (nombres o apellidos) con flechas y backspace.
 * Argumentos:
 *   result_str - Lugar donde se guarda el texto que escribe el usuario.
 *   max_length - Cantidad máxima de caracteres permitidos.
 *   Xpos, Ypos - Posición en pantalla del campo.
 *   validator  - Función que decide qué caracteres son válidos.
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
 * Funcion  : CaptureInteger
 * Objetivo : Permite al usuario escribir solo números (día, mes o año).
 * Argumentos:
 *   digit_str    - Buffer temporal donde se guarda el número mientras se escribe.
 *   result_value - Variable donde se guarda el número final como entero.
 *   max_digits   - Cantidad máxima de dígitos permitidos.
 *   pos_x, pos_y - Posición en pantalla del campo.
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
 * Funcion  : ValidDate
 * Objetivo : Verifica si la fecha ingresada es una fecha real y válida (usa time.h).
 * Argumentos:
 *   day, month, year - Día, mes y año que se quieren comprobar.
 * Retorna  : 1 si la fecha es válida, 0 si no lo es.
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
 * Funcion  : CaptureFecha
 * Objetivo : Pide al usuario que ingrese una fecha (día/mes/año) y la valida.
 * Argumentos:
 *   fecha  - Estructura donde se guardará la fecha ingresada.
 *   pos_x  - Columna donde empieza el campo de fecha en pantalla.
 *   pos_y  - Fila donde se muestra el campo de fecha en pantalla.
 */
void CaptureFecha(FECHA *fecha, int pos_x, int pos_y) {
   char buf_dia[3] = "";
   char buf_mes[3] = "";
   char buf_year[5] = "";
   int dia_temp = 0, mes_temp = 0, year_temp = 0;
   int valida;

   _setcursortype(100);

   do {
      valida = 1;

      if (fecha->dd > 0)
         sprintf(buf_dia, "%02d", fecha->dd);
      if (fecha->mm > 0)
         sprintf(buf_mes, "%02d", fecha->mm);
      if (fecha->yyyy > 0)
         sprintf(buf_year, "%04d", fecha->yyyy);

      gotoxy(pos_x + 1, pos_y);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("  /  /    ");
      colordefault();

      CaptureInteger(buf_dia, &dia_temp, 3, pos_x, pos_y);
      gotoxy(pos_x + 3, pos_y);
      printf("/");
      CaptureInteger(buf_mes, &mes_temp, 3, pos_x + 3, pos_y);
      gotoxy(pos_x + 6, pos_y);
      printf("/");
      CaptureInteger(buf_year, &year_temp, 5, pos_x + 6, pos_y);

      gotoxy(pos_x, pos_y + 1);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("%-40s", "");
      colordefault();

      if (!ValidDate(dia_temp, mes_temp, year_temp)) {
         valida = 0;
         gotoxy(pos_x, pos_y + 1);
         setcolor(RED, BLACK);
         printf("Fecha invalida. Verifique dia/mes/a%co.", 164);
         colordefault();
         getch();
      }
   } while (!valida);

   fecha->dd = dia_temp;
   fecha->mm = mes_temp;
   fecha->yyyy = year_temp;
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
void show_header(char menu[][OPT_LENGTH], int options, int Xpos, int Ypos, int isSelected) {
   gotoxy(Xpos, Ypos);
   setcolor(CT, CF);
   printf("AUTOR  ");

   int desplazamiento = (int)strlen("AUTOR  ") + Xpos;

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
 * Funcion  : show_form
 * Objetivo : Muestra el formulario completo para ingresar o modificar un autor.
 * Argumentos:
 *   autor      - Datos del autor que se están editando.
 *   campo_sel  - Campo que está seleccionado en este momento.
 *   es_nuevo   - 1 si es un autor nuevo, 0 si se está modificando uno existente.
 */
void show_form(AUTOR *autor, int campo_sel, int es_nuevo) {
   char *etiquetas[] = {
       "ID Autor:",
       "Nombres:",
       "Apellidos:",
       "Fecha Nacimiento:",
       "[ GUARDAR ]"};
   int total_campos = 5;

   gotoxy(POSXINI, POSYINI);
   setcolor(CET, CEF);
   if (es_nuevo)
      printf("INGRESANDO NUEVO AUTOR                          ");
   else
      printf("MODIFICANDO AUTOR (ID: %s)                      ", autor->idautor);
   colordefault();

   for (int index = 0; index < total_campos; index++) {
      gotoxy(3, FORM_Y + index);
      if (campo_sel == index)
         setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setcolor(TEXT_COLOR, BG_COLOR);

      printf("%-22s", etiquetas[index]);
      gotoxy(26, FORM_Y + index);

      switch (index) {
      case 0:
         printf("%s", autor->idautor);
         break;
      case 1:
         printf("%-40s", autor->nombres);
         break;
      case 2:
         printf("%-35s", autor->apellidos);
         break;
      case 3:
         if (autor->fechanacimiento.dd > 0)
            printf("%02d/%02d/%04d", autor->fechanacimiento.dd,
                   autor->fechanacimiento.mm, autor->fechanacimiento.yyyy);
         else
            printf("DD/MM/AAAA");
         break;
      case 4:
         if (campo_sel == 4)
            printf("<< PRESIONE ENTER PARA GUARDAR >>");
         break;
      }
      colordefault();
   }

   gotoxy(POSXINI, FORM_Y + total_campos + 1);
   setcolor(CET, CEF);
   printf("Flechas: Navegar | ENTER: Editar campo | ESC: Salir");
   colordefault();
}

/*
 * Funcion  : EditForm
 * Objetivo : Controla todo el formulario: navegar entre campos y editarlos.
 * Argumentos:
 *   autor    - Datos del autor que se están editando.
 *   es_nuevo - 1 si es nuevo, 0 si es modificación.
 * Retorna  : 1 si el usuario guardó los cambios, 0 si canceló.
 */
int EditForm(AUTOR *autor, int es_nuevo) {
   int campo_actual = 1;
   int tecla;
   int guardado = 0;
   int total_campos = 5;

   system("cls");
   do {
      show_form(autor, campo_actual, es_nuevo);

      tecla = getch();
      if (tecla == 0 || tecla == 224)
         tecla = getch();

      if (tecla == DOWN && campo_actual < total_campos - 1)
         campo_actual++;
      if (tecla == UP && campo_actual > 1)
         campo_actual--;

      if (tecla == ENTER) {
         switch (campo_actual) {
         case 1:
            captureAlphanumeric(autor->nombres, LENNOMB - 1, 25, FORM_Y + 1, validateAlphachar);
            break;
         case 2:
            captureAlphanumeric(autor->apellidos, LENAPEL - 1, 25, FORM_Y + 2, validateAlphachar);
            break;
         case 3:
            CaptureFecha(&autor->fechanacimiento, 25, FORM_Y + 3);
            break;
         case 4:
            guardado = 1;
            tecla = ESC;
            break;
         }
      }

      if (tecla == ESC && !guardado) {
         gotoxy(POSXINI, FORM_Y + total_campos + 3);
         setcolor(CTS, CFS);
         printf("Desea descartar los cambios? [S/N]: ");
         colordefault();
         char confirma;
         do {
            confirma = (char)toupper(getch());
         } while (confirma != 'S' && confirma != 'N');
         if (confirma == 'N') {
            tecla = 0;
            gotoxy(POSXINI, FORM_Y + total_campos + 3);
            printf("%-40s", "");
         }
      }
   } while (tecla != ESC);

   return guardado;
}

/*
 * Funcion  : contarNodos
 * Objetivo : Cuenta cuántos autores hay en la lista.
 * Argumentos:
 *   head - Cabeza de la lista.
 * Retorna  : Número total de autores.
 */
int contarNodos(struct Node *head) {
   int count = 0;
   struct Node *temp = head;
   while (temp) {
      count++;
      temp = temp->next;
   }
   return count;
}

/*
 * Funcion  : obtenerNodoPorIndice
 * Objetivo : Devuelve el autor que está en la posición indicada.
 * Argumentos:
 *   head   - Cabeza de la lista.
 *   indice - Número de posición (0 = primero).
 * Retorna  : Puntero al nodo en esa posición.
 */
struct Node *obtenerNodoPorIndice(struct Node *head, int indice) {
   struct Node *temp = head;
   int current = 0;
   while (temp && current < indice) {
      temp = temp->next;
      current++;
   }
   return temp;
}

/*
 * Funcion  : calc_col_widths
 * Objetivo : Calcula el ancho necesario para cada columna de la tabla.
 * Argumentos:
 *   head - Cabeza de la lista.
 * Retorna  : Estructura con los anchos más grandes de cada columna.
 */
AnchoColumnas calc_col_widths(struct Node *head) {
   AnchoColumnas anchos;
   anchos.id = (int)strlen("ID Autor");
   anchos.nombres = (int)strlen("Nombres");
   anchos.apellidos = (int)strlen("Apellidos");
   anchos.fecha = (int)strlen("Fec. Nac.");

   struct Node *temp = head;
   char buf[20];
   while (temp) {
      int longitud = (int)strlen(temp->data.idautor);
      if (longitud > anchos.id)
         anchos.id = longitud;

      longitud = (int)strlen(temp->data.nombres);
      if (longitud > anchos.nombres)
         anchos.nombres = longitud;

      longitud = (int)strlen(temp->data.apellidos);
      if (longitud > anchos.apellidos)
         anchos.apellidos = longitud;

      sprintf(buf, "%02d/%02d/%04d",
              temp->data.fechanacimiento.dd,
              temp->data.fechanacimiento.mm,
              temp->data.fechanacimiento.yyyy);
      longitud = (int)strlen(buf);
      if (longitud > anchos.fecha)
         anchos.fecha = longitud;

      temp = temp->next;
   }
   return anchos;
}

/*
 * Funcion  : show_table_header
 * Objetivo : Dibuja los títulos de las columnas de la tabla (alineados perfectamente).
 * Argumentos:
 *   px, py   - Posición donde empieza la tabla.
 *   anchos   - Anchos calculados para cada columna.
 */
void show_table_header(int px, int py, AnchoColumnas anchos) {
   gotoxy(px, py);
   setcolor(CET, CEF);
   printf("%-*s  %-*s  %-*s  %-*s",
          anchos.id, "ID Autor",
          anchos.nombres, "Nombres",
          anchos.apellidos, "Apellidos",
          anchos.fecha, "Fec. Nac.");
   colordefault();
}

/*
 * Funcion  : show_table_row
 * Objetivo : Dibuja una fila de la tabla (un autor) con la misma alineación que el encabezado.
 * Argumentos:
 *   nodo         - Autor que se va a mostrar.
 *   py, px       - Posición en pantalla.
 *   seleccionado - 1 si la fila está resaltada.
 *   anchos       - Anchos de las columnas.
 */
void show_table_row(struct Node *nodo, int py, int px, int seleccionado, AnchoColumnas anchos) {
   char buf_fecha[12];
   sprintf(buf_fecha, "%02d/%02d/%04d",
           nodo->data.fechanacimiento.dd,
           nodo->data.fechanacimiento.mm,
           nodo->data.fechanacimiento.yyyy);

   gotoxy(px, py);
   if (seleccionado)
      setcolor(CTS_ROW, CFS_ROW);
   else
      setcolor(CT_ROW, CF_ROW);

   printf("%-*s  %-*s  %-*s  %-*s",
          anchos.id, nodo->data.idautor,
          anchos.nombres, nodo->data.nombres,
          anchos.apellidos, nodo->data.apellidos,
          anchos.fecha, buf_fecha);

   colordefault();
}

/*
 * Funcion  : movimiento_autores
 * Objetivo : Permite navegar por la lista con flechas, seleccionar con ENTER o salir con ESC.
 * Argumentos:
 *   head      - Cabeza de la lista.
 *   cantidad  - Cantidad total de autores.
 *   py, px    - Posición de la tabla en pantalla.
 *   anchos    - Anchos de las columnas.
 * Retorna  : Índice del autor seleccionado o -1 si se canceló.
 */
int movimiento_autores(struct Node *head, int cantidad, int py, int px, AnchoColumnas anchos) {
   int opcion_sel = 0;
   int scroll = 0;
   int tecla;

   while (1) {
      if (opcion_sel >= cantidad)
         opcion_sel = cantidad - 1;
      if (opcion_sel < 0)
         opcion_sel = 0;
      if (scroll > opcion_sel)
         scroll = opcion_sel;
      if (scroll < opcion_sel - ROWS_VISIBLE + 1)
         scroll = opcion_sel - ROWS_VISIBLE + 1;
      if (scroll < 0)
         scroll = 0;

      gotoxy(POSXINI, POSYINI);
      setcolor(CT, CF);
      printf("(%d autores)   Flechas: navegar | ENTER: seleccionar | ESC: volver%5s", cantidad, "");
      colordefault();

      show_table_header(px, py, anchos);

      struct Node *temp = head;
      int current_idx = 0;
      while (temp && current_idx < scroll) {
         temp = temp->next;
         current_idx++;
      }

      for (int fila = 0; fila < ROWS_VISIBLE; fila++) {
         if (temp) {
            show_table_row(temp, py + 1 + fila, px, (current_idx == opcion_sel), anchos);
            temp = temp->next;
            current_idx++;
         } else {
            gotoxy(px, py + 1 + fila);
            setcolor(CT_ROW, CF_ROW);
            printf("%-80s", "");
            colordefault();
         }
      }

      tecla = getch();
      if (tecla == 0 || tecla == 224)
         tecla = getch();

      if (tecla == UP && opcion_sel > 0)
         opcion_sel--;
      else if (tecla == DOWN && opcion_sel < cantidad - 1)
         opcion_sel++;
      else if (tecla == ESC)
         return -1;
      else if (tecla == ENTER && cantidad > 0)
         return opcion_sel;
   }
}

/*
 * Funcion  : IngreseAutor
 * Objetivo : Crea un nuevo autor con ID automático y lo guarda.
 * Argumentos:
 *   head - Puntero a la cabeza de la lista.
 */
void IngreseAutor(struct Node **head) {
   AUTOR nuevo;
   memset(&nuevo, 0, sizeof(AUTOR));

   struct Node *temp = *head;
   int max_id = 0;
   while (temp) {
      int id_num = atoi(temp->data.idautor);
      if (id_num > max_id)
         max_id = id_num;
      temp = temp->next;
   }
   sprintf(nuevo.idautor, "%05d", max_id + 1);

   if (EditForm(&nuevo, 1)) {
      insertAtEnd(head, nuevo);
      guardarEnArchivo(*head);
      system("cls");
      gotoxy(POSXINI, POSYINI);
      setcolor(GREEN, BLACK);
      printf("Autor %s guardado exitosamente.", nuevo.idautor);
      colordefault();
      getch();
   }
}

/*
 * Funcion  : ModificarAutor
 * Objetivo : Permite elegir un autor de la lista y modificarlo.
 * Argumentos:
 *   head - Puntero a la cabeza de la lista.
 */
void ModificarAutor(struct Node **head) {
   int cantidad = contarNodos(*head);
   if (cantidad == 0) {
      gotoxy(POSXINI, 3);
      printf("No hay autores registrados.");
      getch();
      return;
   }

   AnchoColumnas anchos = calc_col_widths(*head);
   system("cls");
   int seleccionado = movimiento_autores(*head, cantidad, TABLE_YINIT, TABLE_XINIT, anchos);

   if (seleccionado >= 0) {
      struct Node *nodo = obtenerNodoPorIndice(*head, seleccionado);
      if (nodo && EditForm(&nodo->data, 0)) {
         guardarEnArchivo(*head);
         system("cls");
         gotoxy(POSXINI, POSYINI);
         setcolor(GREEN, BLACK);
         printf("Autor %s modificado correctamente.", nodo->data.idautor);
         colordefault();
         getch();
      }
   }
}

/*
 * Funcion  : EliminarAutor
 * Objetivo : Permite elegir un autor y eliminarlo después de confirmar.
 * Argumentos:
 *   head - Puntero a la cabeza de la lista.
 */
void EliminarAutor(struct Node **head) {
   int cantidad = contarNodos(*head);
   if (cantidad == 0) {
      gotoxy(POSXINI, 3);
      printf("No hay autores registrados.");
      getch();
      return;
   }

   AnchoColumnas anchos = calc_col_widths(*head);
   system("cls");
   int seleccionado = movimiento_autores(*head, cantidad, TABLE_YINIT, TABLE_XINIT, anchos);

   if (seleccionado >= 0) {
      struct Node *nodo = obtenerNodoPorIndice(*head, seleccionado);
      if (nodo) {
         system("cls");
         gotoxy(POSXINI, POSYINI);
         setcolor(RED, BLACK);
         printf("Esta seguro que desea eliminar al autor:");
         gotoxy(POSXINI, 2);
         printf("  ID: %-6s  Nombres: %s %s",
                nodo->data.idautor,
                nodo->data.nombres,
                nodo->data.apellidos);
         gotoxy(POSXINI, 4);
         printf("[S] Si, eliminar    [N] No, cancelar");
         colordefault();

         char confirma;
         do {
            confirma = (char)toupper(getch());
         } while (confirma != 'S' && confirma != 'N');

         if (confirma == 'S') {
            if (nodo->prev)
               nodo->prev->next = nodo->next;
            else
               *head = nodo->next;
            if (nodo->next)
               nodo->next->prev = nodo->prev;
            free(nodo);

            guardarEnArchivo(*head);

            gotoxy(POSXINI, 6);
            setcolor(GREEN, BLACK);
            printf("Autor eliminado correctamente.");
            colordefault();
         } else {
            gotoxy(POSXINI, 6);
            printf("Eliminacion cancelada.");
         }
         getch();
      }
   }
}

/*
 * Funcion  : ListarAutores
 * Objetivo : Muestra todos los autores en una tabla con scroll.
 * Argumentos:
 *   head - Cabeza de la lista.
 */
void ListarAutores(struct Node *head) {
   int cantidad = contarNodos(head);
   if (cantidad == 0) {
      system("cls");
      gotoxy(POSXINI, POSYINI);
      printf("No hay autores registrados.");
      getch();
      return;
   }

   AnchoColumnas anchos = calc_col_widths(head);
   system("cls");
   movimiento_autores(head, cantidad, TABLE_YINIT, TABLE_XINIT, anchos);
}

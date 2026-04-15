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

#define LENCOD 6
#define LENNOMBRE 31
#define LENDESC 26
#define LENNUM 4

#define OPT_LENGTH 81
#define ROWS_VISIBLE 10

#define POSXINI 1
#define POSYINI 1
#define TABLE_XINIT 1
#define TABLE_YINIT 3
#define FORM_Y 4

#define EDAD_MINIMA 18

#define DATA_FILE "jugadores.dat"

typedef struct {
   int dd, mm, yy;
} FECHA;

typedef struct {
   char codjugador[LENCOD];
   char nombres[LENNOMBRE];
   char apellidos[LENNOMBRE];
   char ciudadnacim[LENCOD];
   FECHA fecnacim;
   char numero[LENNUM];
   char codequipo[LENCOD];
} JUGADOR;

typedef struct {
   int cod, nombres, apellidos, ciudad, fecha, numero, equipo;
} AnchoColumnas;

void setcolor(int ct, int cf);
void colordefault();
void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y);
int validate_alpha_char(char character);
int validate_alphanumeric_char(char character);
int validate_digit_char(char character);
void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char character));
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int pos_x, int pos_y);
int es_bisiesto(int year);
int dias_en_mes(int mes, int year);
int fecha_valida(int dd, int mm, int yy);
int jugador_mayor_de_edad(FECHA fec);
void CaptureFecha(FECHA *fecha, int pos_x, int pos_y);
void show_header(char menu[][OPT_LENGTH], int options, int Xpos, int Ypos, int isSelected);
void show_form(JUGADOR *jug, int campo_sel, int es_nuevo);
int EditForm(JUGADOR *jug, int es_nuevo);
int getLastId();
void IngreseJugador();
void ModificarJugador();
void EliminarJugador();
void ListarJugadores();
AnchoColumnas calc_col_widths(JUGADOR *jugadores, int cantidad);
void show_table_header(int px, int py, AnchoColumnas anchos);
void show_table_row(JUGADOR *jug, int py, int px, int seleccionado, AnchoColumnas anchos);
int movimiento_jugadores(JUGADOR *jugadores, int cantidad, int py, int px, AnchoColumnas anchos);

int main() {
   _setcursortype(0);
   char menu[][OPT_LENGTH] = {
       "Ingresar", "Modificar", "Eliminar", "Listar", "Salir"};
   int opcion_sel = 0;
   int redibuja = 1;
   int tecla;

   do {
      if (redibuja) {
         system("cls");
         redibuja = 0;
      }
      show_header(menu, 5, 1, 1, opcion_sel);

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
            IngreseJugador();
            break;
         case 1:
            ModificarJugador();
            break;
         case 2:
            EliminarJugador();
            break;
         case 3:
            ListarJugadores();
            break;
         case 4:
            tecla = ESC;
            break;
         }
         if (tecla != ESC)
            redibuja = 1;
      }
   } while (tecla != ESC);

   _setcursortype(100);
   return 0;
}

/*
 * Funcion  : setcolor
 * Objetivo : Establece los colores de texto y fondo para la consola.
 * Argumentos: ct - color del texto
 *             cf - color del fondo
 */
void setcolor(int ct, int cf) {
   textbackground(cf);
   textcolor(ct);
}

/*
 * Funcion  : colordefault
 * Objetivo : Restaura los colores predeterminados de la consola.
 */
void colordefault() {
   setcolor(LIGHTGRAY, BLACK);
}

/*
 * Funcion  : show_header
 * Objetivo : Despliega el menu horizontal (pushdown) resaltando
 *            la opcion actualmente seleccionada.
 * Argumentos: menu       - arreglo de cadenas con las opciones del menu
 *             options    - numero de opciones
 *             Xpos       - columna inicial en pantalla
 *             Ypos       - fila inicial en pantalla
 *             isSelected - indice de la opcion resaltada
 */
void show_header(char menu[][OPT_LENGTH], int options,
                 int Xpos, int Ypos, int isSelected) {
   gotoxy(Xpos, Ypos);
   setcolor(CT, CF);
   printf("Jugadores  ");

   int desplazamiento = (int)strlen("Jugadores  ") + Xpos;

   for (int ind = 0; ind < options; ind++) {
      gotoxy(desplazamiento, Ypos);
      if (isSelected == ind)
         setcolor(CTS, CFS);
      else
         setcolor(CT, CF);
      printf("%s", menu[ind]);
      desplazamiento += (int)strlen(menu[ind]) + 2;
      setcolor(CT, CF);
      printf("  ");
   }
   colordefault();
}

/*
 * Funcion  : validate_alpha_char
 * Objetivo : Valida que un caracter sea letra, guion o espacio.
 * Argumentos: character - caracter a validar
 * Retorna  : 1 si es valido, 0 si no lo es
 */
int validate_alpha_char(char character) {
   return (isalpha((unsigned char)character) || character == ' ' || character == '-');
}

/*
 * Funcion  : validate_alphanumeric_char
 * Objetivo : Valida que un caracter sea letra, digito, guion o espacio.
 * Argumentos: character - caracter a validar
 * Retorna  : 1 si es valido, 0 si no lo es
 */
int validate_alphanumeric_char(char character) {
   return (isalpha((unsigned char)character) || isdigit((unsigned char)character) || character == ' ' || character == '-');
}

/*
 * Funcion  : validate_digit_char
 * Objetivo : Valida que un caracter sea un digito numerico.
 * Argumentos: character - caracter a validar
 * Retorna  : 1 si es valido, 0 si no lo es
 */
int validate_digit_char(char character) {
   return isdigit((unsigned char)character);
}

/*
 * Funcion  : show_field
 * Objetivo : Dibuja el contenido de un campo editable en pantalla
 *            y posiciona el cursor sobre el caracter activo.
 * Argumentos: field_str       - cadena con el contenido del campo
 *             cursor_position - posicion actual del cursor dentro del campo
 *             max_length      - longitud maxima del campo
 *             pos_x           - columna inicial en pantalla
 *             pos_y           - fila en pantalla
 */
void show_field(char *field_str, int cursor_position,
                int max_length, int pos_x, int pos_y) {
   setcolor(TEXT_COLOR, BG_COLOR);
   for (int ind = 0; ind < max_length; ind++) {
      gotoxy(pos_x + ind + 1, pos_y);
      printf("%c", field_str[ind] ? field_str[ind] : ' ');
   }
   colordefault();
   gotoxy(pos_x + cursor_position + 1, pos_y);
}

/*
 * Funcion  : captureAlphanumeric
 * Objetivo : Captura una cadena de texto usando el validador provisto.
 *            Soporta movimiento con flechas izquierda/derecha y borrado.
 *            Sale con ENTER, ESC o TAB.
 * Argumentos: result_str - buffer donde se almacena el texto capturado
 *             max_length - longitud maxima aceptada
 *             Xpos       - columna inicial del campo en pantalla
 *             Ypos       - fila del campo en pantalla
 *             validator  - puntero a funcion que valida cada caracter
 */
void captureAlphanumeric(char *result_str, int max_length,
                         int Xpos, int Ypos, int (*validator)(char character)) {
   int cursor_index = 0;
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
            }
         } else {
            if (cursor_index < max_length - 1) {
               memmove(result_str + cursor_index + 1,
                       result_str + cursor_index,
                       max_length - cursor_index - 1);
               result_str[cursor_index] = pressed_key;
               cursor_index++;
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC && pressed_key != TAB);
}

/*
 * Funcion  : CaptureInteger
 * Objetivo : Captura un valor entero positivo aceptando solo digitos.
 *            Sale con ENTER o ESC.
 * Argumentos: digit_str    - buffer de trabajo con el texto del numero
 *             result_value - puntero donde se almacena el entero resultante
 *             max_digits   - cantidad maxima de digitos
 *             pos_x        - columna inicial del campo en pantalla
 *             pos_y        - fila del campo en pantalla
 */
void CaptureInteger(char *digit_str, int *result_value,
                    int max_digits, int pos_x, int pos_y) {
   int cursor_index = 0;
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
            }
         } else {
            if (cursor_index < max_digits - 1) {
               memmove(digit_str + cursor_index + 1,
                       digit_str + cursor_index,
                       max_digits - cursor_index - 1);
               digit_str[cursor_index] = pressed_key;
               cursor_index++;
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC) {
      digit_str[cursor_index] = '\0';
      *result_value = atoi(digit_str);
   }
}

/*
 * Funcion  : es_bisiesto
 * Objetivo : Determina si un year es bisiesto.
 * Argumentos: year - year a evaluar
 * Retorna  : 1 si es bisiesto, 0 si no lo es
 */
int es_bisiesto(int year) {
   return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/*
 * Funcion  : dias_en_mes
 * Objetivo : Retorna la cantidad de dias que tiene un mes dado un year.
 * Argumentos: mes  - mes a evaluar (1-12)
 *             year - year de referencia para febrero bisiesto
 * Retorna  : cantidad de dias del mes
 */
int dias_en_mes(int mes, int year) {
   int tabla_dias[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   if (mes == 2 && es_bisiesto(year))
      return 29;
   return tabla_dias[mes];
}

/*
 * Funcion  : fecha_valida
 * Objetivo : Verifica que una fecha compuesta por dia, mes y year sea
 *            coherente con el calendario gregoriano.
 * Argumentos: dd  - dia
 *             mm  - mes
 *             yy  - year
 * Retorna  : 1 si la fecha es valida, 0 si no lo es
 */
int fecha_valida(int dd, int mm, int yy) {
   if (mm < 1 || mm > 12)
      return 0;
   if (yy < 1)
      return 0;
   if (dd < 1 || dd > dias_en_mes(mm, yy))
      return 0;
   return 1;
}

/*
 * Funcion  : jugador_mayor_de_edad
 * Objetivo : Comprueba que la fecha de nacimiento corresponda a una persona
 *            con al menos EDAD_MINIMA years cumplidos a la fecha actual.
 * Argumentos: fec - fecha de nacimiento del jugador
 * Retorna  : 1 si cumple la edad minima, 0 si no la cumple
 */
int jugador_mayor_de_edad(FECHA fec) {
   time_t ahora = time(NULL);
   struct tm *hoy = localtime(&ahora);
   int year_actual = hoy->tm_year + 1900;
   int mes_actual = hoy->tm_mon + 1;
   int dia_actual = hoy->tm_mday;

   int edad = year_actual - fec.yy;
   if (mes_actual < fec.mm || (mes_actual == fec.mm && dia_actual < fec.dd))
      edad--;

   return edad >= EDAD_MINIMA;
}

/*
 * Funcion  : CaptureFecha
 * Objetivo : Solicita al usuario que ingrese una fecha valida (dd/mm/aaaa)
 *            campo por campo. Valida coherencia del calendario y que el
 *            jugador sea mayor de EDAD_MINIMA years.
 * Argumentos: fecha - puntero a la estructura FECHA donde se guarda el resultado
 *             pos_x - columna inicial del campo en pantalla
 *             pos_y - fila del campo en pantalla
 */
void CaptureFecha(FECHA *fecha, int pos_x, int pos_y) {
   char buf_dia[3];
   char buf_mes[3];
   char buf_year[5];
   int dia_temp, mes_temp, year_temp;
   int valida;

   _setcursortype(100);

   do {
      valida = 1;

      memset(buf_dia, 0, sizeof(buf_dia));
      memset(buf_mes, 0, sizeof(buf_mes));
      memset(buf_year, 0, sizeof(buf_year));

      if (fecha->dd > 0)
         sprintf(buf_dia, "%02d", fecha->dd);
      if (fecha->mm > 0)
         sprintf(buf_mes, "%02d", fecha->mm);
      if (fecha->yy > 0)
         sprintf(buf_year, "%04d", fecha->yy);

      gotoxy(pos_x + 1, pos_y);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("  /  /    ");
      colordefault();

      CaptureInteger(buf_dia, &dia_temp, 3, pos_x, pos_y);
      gotoxy(pos_x + 3, pos_y);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("/");
      colordefault();
      CaptureInteger(buf_mes, &mes_temp, 3, pos_x + 3, pos_y);
      gotoxy(pos_x + 6, pos_y);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("/");
      colordefault();
      CaptureInteger(buf_year, &year_temp, 5, pos_x + 6, pos_y);

      gotoxy(pos_x, pos_y + 1);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("%-40s", "");
      colordefault();

      if (!fecha_valida(dia_temp, mes_temp, year_temp)) {
         valida = 0;
         gotoxy(pos_x, pos_y + 1);
         setcolor(RED, BLACK);
         printf("Fecha invalida. Verifique dia/mes/a%co.", 164);
         colordefault();
         getch();
      } else {
         FECHA candidata;
         candidata.dd = dia_temp;
         candidata.mm = mes_temp;
         candidata.yy = year_temp;
         if (!jugador_mayor_de_edad(candidata)) {
            valida = 0;
            gotoxy(pos_x, pos_y + 1);
            setcolor(RED, BLACK);
            printf("El jugador debe tener al menos %d a%cos.", EDAD_MINIMA, 164);
            colordefault();
            getch();
            gotoxy(pos_x, pos_y + 1);
            setcolor(TEXT_COLOR, BG_COLOR);
            printf("%-40s", "");
            colordefault();
         }
      }
   } while (!valida);

   fecha->dd = dia_temp;
   fecha->mm = mes_temp;
   fecha->yy = year_temp;
}

/*
 * Funcion  : getLastId
 * Objetivo : Recorre el archivo binario y obtiene el ID maximo registrado.
 * Retorna  : El ID maximo encontrado mas uno, o 1 si el archivo no existe
 */
int getLastId() {
   FILE *archivo = fopen(DATA_FILE, "rb");
   if (!archivo)
      return 1;

   JUGADOR temp;
   int id_max = 0;
   while (fread(&temp, sizeof(JUGADOR), 1, archivo)) {
      int actual = atoi(temp.codjugador);
      if (actual > id_max)
         id_max = actual;
   }
   fclose(archivo);
   return id_max + 1;
}

/*
 * Funcion  : show_form
 * Objetivo : Dibuja el formulario completo de un jugador, resaltando
 *            el campo actualmente seleccionado.
 * Argumentos: jug       - puntero al jugador cuyos datos se muestran
 *             campo_sel - indice del campo resaltado
 *             es_nuevo  - 1 si es ingreso nuevo, 0 si es modificacion
 */
void show_form(JUGADOR *jug, int campo_sel, int es_nuevo) {
   char *etiquetas[] = {
       "Cod. Jugador:",
       "Nombres:",
       "Apellidos:",
       "Ciudad Nacim.:",
       "Fecha Nacim.:",
       "Numero:",
       "Cod. Equipo:",
       "[ GUARDAR ]"};
   int total_campos = 8;

   gotoxy(1, 1);
   setcolor(CET, CEF);
   if (es_nuevo)
      printf("INGRESANDO NUEVO JUGADOR                        ");
   else
      printf("MODIFICANDO JUGADOR (ID: %s)                    ", jug->codjugador);
   colordefault();

   for (int ind = 0; ind < total_campos; ind++) {
      gotoxy(3, FORM_Y + ind);
      if (campo_sel == ind)
         setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setcolor(TEXT_COLOR, BG_COLOR);

      printf("%-22s", etiquetas[ind]);
      gotoxy(26, FORM_Y + ind);

      switch (ind) {
      case 0:
         printf("%s", jug->codjugador);
         break;
      case 1:
         printf("%-30s", jug->nombres);
         break;
      case 2:
         printf("%-30s", jug->apellidos);
         break;
      case 3:
         printf("%-5s ", jug->ciudadnacim);
         break;
      case 4:
         if (jug->fecnacim.dd > 0)
            printf("%02d/%02d/%04d", jug->fecnacim.dd,
                   jug->fecnacim.mm, jug->fecnacim.yy);
         else
            printf("DD/MM/AAAA");
         break;
      case 5:
         printf("%-3s ", jug->numero);
         break;
      case 6:
         printf("%-5s ", jug->codequipo);
         break;
      case 7:
         if (campo_sel == 7)
            printf("<< PRESIONE ENTER PARA GUARDAR >>");
         break;
      }
      colordefault();
   }

   gotoxy(1, FORM_Y + total_campos + 1);
   setcolor(CET, CEF);
   printf("Flechas: Navegar | ENTER: Editar campo | ESC: Salir");
   colordefault();
}

/*
 * Funcion  : EditForm
 * Objetivo : Gestiona la navegacion y edicion de todos los campos
 *            de un JUGADOR mediante el formulario en pantalla.
 *            Al presionar ESC solicita confirmacion antes de descartar.
 * Argumentos: jug      - puntero al jugador a editar
 *             es_nuevo - 1 si es ingreso nuevo, 0 si es modificacion
 * Retorna  : 1 si el usuario guardo, 0 si cancelo
 */
int EditForm(JUGADOR *jug, int es_nuevo) {
   int campo_actual = 1;
   int tecla;
   int guardado = 0;
   int total_campos = 8;

   system("cls");
   do {
      show_form(jug, campo_actual, es_nuevo);

      tecla = getch();
      if (tecla == 0 || tecla == 224)
         tecla = getch();

      if (tecla == DOWN && campo_actual < total_campos - 1)
         campo_actual++;
      if (tecla == UP && campo_actual > 0)
         campo_actual--;

      if (tecla == ENTER) {
         switch (campo_actual) {
         case 1:
            captureAlphanumeric(jug->nombres, LENNOMBRE - 1,
                                25, FORM_Y + 1,
                                validate_alpha_char);
            break;
         case 2:
            captureAlphanumeric(jug->apellidos, LENNOMBRE - 1,
                                25, FORM_Y + 2,
                                validate_alpha_char);
            break;
         case 3:
            captureAlphanumeric(jug->ciudadnacim, LENCOD - 1,
                                25, FORM_Y + 3,
                                validate_alphanumeric_char);
            break;
         case 4:
            CaptureFecha(&jug->fecnacim, 25, FORM_Y + 4);
            break;
         case 5:
            captureAlphanumeric(jug->numero, LENNUM - 1,
                                25, FORM_Y + 5,
                                validate_digit_char);
            break;
         case 6:
            captureAlphanumeric(jug->codequipo, LENCOD - 1,
                                25, FORM_Y + 6,
                                validate_alphanumeric_char);
            break;
         case 7:
            guardado = 1;
            tecla = ESC;
            break;
         }
      }

      if (tecla == ESC && !guardado) {
         gotoxy(1, FORM_Y + total_campos + 3);
         setcolor(CTS, CFS);
         printf("Desea descartar los cambios? [S/N]: ");
         colordefault();
         char confirma;
         do {
            confirma = (char)toupper(getch());
         } while (confirma != 'S' && confirma != 'N');
         if (confirma == 'N') {
            tecla = 0;
            gotoxy(1, FORM_Y + total_campos + 3);
            printf("%-40s", "");
         }
      }

   } while (tecla != ESC);

   return guardado;
}

/*
 * Funcion  : IngreseJugador
 * Objetivo : Crea un nuevo JUGADOR con ID autogenerado y lo persiste
 *            al final del archivo binario.
 */
void IngreseJugador() {
   JUGADOR nuevo;
   memset(&nuevo, 0, sizeof(JUGADOR));

   int siguiente_id = getLastId();
   sprintf(nuevo.codjugador, "%05d", siguiente_id);

   if (EditForm(&nuevo, 1)) {
      FILE *archivo = fopen(DATA_FILE, "ab");
      if (archivo) {
         fwrite(&nuevo, sizeof(JUGADOR), 1, archivo);
         fclose(archivo);
         system("cls");
         gotoxy(1, 1);
         setcolor(GREEN, BLACK);
         printf("Jugador %s guardado exitosamente.", nuevo.codjugador);
         colordefault();
         getch();
      }
   }
}

/*
 * Funcion  : ModificarJugador
 * Objetivo : Carga todos los jugadores, permite seleccionar uno mediante
 *            scroll, edita sus campos y reescribe el registro en su
 *            posicion original dentro del archivo binario.
 */
void ModificarJugador() {
   FILE *archivo = fopen(DATA_FILE, "rb+");
   if (!archivo) {
      gotoxy(1, 3);
      printf("No hay jugadores registrados.");
      getch();
      return;
   }

   fseek(archivo, 0, SEEK_END);
   int cantidad = (int)(ftell(archivo) / sizeof(JUGADOR));
   rewind(archivo);

   if (cantidad == 0) {
      fclose(archivo);
      gotoxy(1, 3);
      printf("No hay jugadores registrados.");
      getch();
      return;
   }

   JUGADOR *lista = (JUGADOR *)malloc(cantidad * sizeof(JUGADOR));
   fread(lista, sizeof(JUGADOR), cantidad, archivo);

   system("cls");
   AnchoColumnas anchos = calc_col_widths(lista, cantidad);
   int seleccionado = movimiento_jugadores(lista, cantidad,
                                           TABLE_YINIT, TABLE_XINIT,
                                           anchos);

   if (seleccionado >= 0) {
      JUGADOR editando = lista[seleccionado];
      if (EditForm(&editando, 0)) {
         fseek(archivo, (long)(seleccionado * sizeof(JUGADOR)), SEEK_SET);
         fwrite(&editando, sizeof(JUGADOR), 1, archivo);
         system("cls");
         gotoxy(1, 1);
         setcolor(GREEN, BLACK);
         printf("Jugador %s modificado correctamente.", editando.codjugador);
         colordefault();
         getch();
      }
   }

   free(lista);
   fclose(archivo);
}

/*
 * Funcion  : EliminarJugador
 * Objetivo : Selecciona un jugador mediante scroll, solicita confirmacion
 *            y reescribe el archivo binario sin ese registro.
 */
void EliminarJugador() {
   FILE *archivo = fopen(DATA_FILE, "rb");
   if (!archivo) {
      gotoxy(1, 3);
      printf("No hay jugadores registrados.");
      getch();
      return;
   }

   fseek(archivo, 0, SEEK_END);
   int cantidad = (int)(ftell(archivo) / sizeof(JUGADOR));
   rewind(archivo);

   if (cantidad == 0) {
      fclose(archivo);
      gotoxy(1, 3);
      printf("No hay jugadores registrados.");
      getch();
      return;
   }

   JUGADOR *lista = (JUGADOR *)malloc(cantidad * sizeof(JUGADOR));
   fread(lista, sizeof(JUGADOR), cantidad, archivo);
   fclose(archivo);

   system("cls");
   AnchoColumnas anchos = calc_col_widths(lista, cantidad);
   int seleccionado = movimiento_jugadores(lista, cantidad,
                                           TABLE_YINIT, TABLE_XINIT,
                                           anchos);

   if (seleccionado >= 0) {
      system("cls");
      gotoxy(1, 1);
      setcolor(RED, BLACK);
      printf("Esta seguro que desea eliminar al jugador:");
      gotoxy(1, 2);
      printf("  ID: %-6s  Nombres: %s %s",
             lista[seleccionado].codjugador,
             lista[seleccionado].nombres,
             lista[seleccionado].apellidos);
      gotoxy(1, 4);
      printf("[S] Si, eliminar    [N] No, cancelar");
      colordefault();

      char confirma;
      do {
         confirma = (char)toupper(getch());
      } while (confirma != 'S' && confirma != 'N');

      if (confirma == 'S') {
         FILE *temp = fopen(DATA_FILE, "wb");
         if (temp) {
            for (int ind = 0; ind < cantidad; ind++) {
               if (ind != seleccionado)
                  fwrite(&lista[ind], sizeof(JUGADOR), 1, temp);
            }
            fclose(temp);
            gotoxy(1, 6);
            setcolor(GREEN, BLACK);
            printf("Jugador eliminado correctamente.");
            colordefault();
         }
      } else {
         gotoxy(1, 6);
         printf("Eliminacion cancelada.");
      }
      getch();
   }

   free(lista);
}

/*
 * Funcion  : ListarJugadores
 * Objetivo : Carga todos los jugadores del archivo y lanza la vista
 *            interactiva de tabla. Solo es para consulta.
 */
void ListarJugadores() {
   FILE *archivo = fopen(DATA_FILE, "rb");
   if (!archivo) {
      system("cls");
      gotoxy(1, 1);
      printf("No hay jugadores registrados.");
      getch();
      return;
   }

   fseek(archivo, 0, SEEK_END);
   int cantidad = (int)(ftell(archivo) / sizeof(JUGADOR));
   rewind(archivo);

   if (cantidad == 0) {
      fclose(archivo);
      system("cls");
      gotoxy(1, 1);
      printf("No hay jugadores registrados.");
      getch();
      return;
   }

   JUGADOR *lista = (JUGADOR *)malloc(cantidad * sizeof(JUGADOR));
   fread(lista, sizeof(JUGADOR), cantidad, archivo);
   fclose(archivo);

   system("cls");
   AnchoColumnas anchos = calc_col_widths(lista, cantidad);
   movimiento_jugadores(lista, cantidad,
                        TABLE_YINIT, TABLE_XINIT,
                        anchos);

   free(lista);
}

/*
 * Funcion  : calc_col_widths
 * Objetivo : Calcula el ancho maximo de cada columna comparando el largo
 *            de cada dato contra el largo del encabezado correspondiente.
 * Argumentos: jugadores - arreglo de jugadores
 *             cantidad  - cantidad de jugadores en el arreglo
 * Retorna  : estructura AnchoColumnas con el ancho maximo de cada columna
 */
AnchoColumnas calc_col_widths(JUGADOR *jugadores, int cantidad) {
   AnchoColumnas anchos;
   anchos.cod = (int)strlen("Codigo");
   anchos.nombres = (int)strlen("Nombres");
   anchos.apellidos = (int)strlen("Apellidos");
   anchos.ciudad = (int)strlen("Ciudad");
   anchos.fecha = (int)strlen("Fec.Nac");
   anchos.numero = (int)strlen("Num");
   anchos.equipo = (int)strlen("Equipo");

   char buf[20];
   for (int ind = 0; ind < cantidad; ind++) {
      int longitud;

      longitud = (int)strlen(jugadores[ind].codjugador);
      if (longitud > anchos.cod)
         anchos.cod = longitud;

      longitud = (int)strlen(jugadores[ind].nombres);
      if (longitud > anchos.nombres)
         anchos.nombres = longitud;

      longitud = (int)strlen(jugadores[ind].apellidos);
      if (longitud > anchos.apellidos)
         anchos.apellidos = longitud;

      longitud = (int)strlen(jugadores[ind].ciudadnacim);
      if (longitud > anchos.ciudad)
         anchos.ciudad = longitud;

      sprintf(buf, "%02d/%02d/%04d",
              jugadores[ind].fecnacim.dd,
              jugadores[ind].fecnacim.mm,
              jugadores[ind].fecnacim.yy);
      longitud = (int)strlen(buf);
      if (longitud > anchos.fecha)
         anchos.fecha = longitud;

      longitud = (int)strlen(jugadores[ind].numero);
      if (longitud > anchos.numero)
         anchos.numero = longitud;

      longitud = (int)strlen(jugadores[ind].codequipo);
      if (longitud > anchos.equipo)
         anchos.equipo = longitud;
   }
   return anchos;
}

/*
 * Funcion  : show_table_header
 * Objetivo : Dibuja el encabezado de columnas de la tabla con anchos dinamicos.
 * Argumentos: px     - columna inicial en pantalla
 *             py     - fila en pantalla
 *             anchos - anchos de columna calculados por calc_col_widths
 */
void show_table_header(int px, int py, AnchoColumnas anchos) {
   gotoxy(px, py);
   setcolor(CET, CEF);
   printf("%-*s %-*s %-*s %-*s %-*s %-*s %-*s",
          anchos.cod, "Codigo",
          anchos.nombres, "Nombres",
          anchos.apellidos, "Apellidos",
          anchos.ciudad, "Ciudad",
          anchos.fecha, "Fec.Nac",
          anchos.numero, "Num",
          anchos.equipo, "Equipo");
   colordefault();
}

/*
 * Funcion  : show_table_row
 * Objetivo : Dibuja una fila de jugador con anchos dinamicos,
 *            resaltandola si esta seleccionada.
 * Argumentos: jug         - puntero al jugador a mostrar
 *             py          - fila en pantalla
 *             px          - columna inicial en pantalla
 *             seleccionado- 1 si la fila esta seleccionada, 0 si no
 *             anchos      - anchos de columna calculados por calc_col_widths
 */
void show_table_row(JUGADOR *jug, int py, int px,
                    int seleccionado, AnchoColumnas anchos) {
   char buf_fecha[12];
   sprintf(buf_fecha, "%02d/%02d/%04d",
           jug->fecnacim.dd, jug->fecnacim.mm, jug->fecnacim.yy);

   gotoxy(px, py);
   if (seleccionado)
      setcolor(CTS_ROW, CFS_ROW);
   else
      setcolor(CT_ROW, CF_ROW);

   printf("%-*s %-*s %-*s %-*s %-*s %-*s %-*s",
          anchos.cod, jug->codjugador,
          anchos.nombres, jug->nombres,
          anchos.apellidos, jug->apellidos,
          anchos.ciudad, jug->ciudadnacim,
          anchos.fecha, buf_fecha,
          anchos.numero, jug->numero,
          anchos.equipo, jug->codequipo);

   colordefault();
}

/*
 * Funcion  : movimiento_jugadores
 * Objetivo : Presenta la lista con scroll (ROWS_VISIBLE filas a la vez),
 *            permite navegar con flechas arriba/abajo y seleccionar
 *            con ENTER. ESC cancela y retorna -1.
 * Argumentos: jugadores - arreglo de jugadores cargados en memoria
 *             cantidad  - cantidad total de jugadores
 *             py        - fila donde inicia la tabla en pantalla
 *             px        - columna donde inicia la tabla en pantalla
 *             anchos    - anchos de columna calculados por calc_col_widths
 * Retorna  : indice del jugador seleccionado en el arreglo original,
 *            o -1 si el usuario salio con ESC
 */
int movimiento_jugadores(JUGADOR *jugadores, int cantidad,
                         int py, int px, AnchoColumnas anchos) {
   int opcion_sel = 0;
   int scroll = 0;
   int tecla_especial;

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

      gotoxy(1, 1);
      setcolor(CT, CF);
      printf("(%d registros)   Flechas: navegar | ENTER: seleccionar | ESC: volver%5s",
             cantidad, "");
      colordefault();

      gotoxy(1, 2);
      setcolor(CT, CF);
      printf("%-80s", "");
      colordefault();

      show_table_header(px, py, anchos);

      for (int fila = 0; fila < ROWS_VISIBLE; fila++) {
         int abs_idx = scroll + fila;
         if (abs_idx < cantidad) {
            show_table_row(&jugadores[abs_idx],
                           py + 1 + fila, px,
                           (abs_idx == opcion_sel),
                           anchos);
         } else {
            gotoxy(px, py + 1 + fila);
            setcolor(CT_ROW, CF_ROW);
            printf("%-80s", "");
            colordefault();
         }
      }

      _setcursortype(0);
      tecla_especial = 0;
      int tecla = getch();
      if (tecla == 0 || tecla == 224) {
         tecla = getch();
         tecla_especial = 1;
      }

      if (tecla_especial) {
         if (tecla == UP && opcion_sel > 0) {
            opcion_sel--;
            if (opcion_sel < scroll)
               scroll = opcion_sel;
         } else if (tecla == DOWN && opcion_sel < cantidad - 1) {
            opcion_sel++;
            if (opcion_sel >= scroll + ROWS_VISIBLE)
               scroll = opcion_sel - ROWS_VISIBLE + 1;
         }
      } else {
         if (tecla == ESC) {
            return -1;
         } else if (tecla == ENTER && cantidad > 0) {
            return opcion_sel;
         }
      }
   }
}

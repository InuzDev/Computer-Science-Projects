#include <conio.c>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define MAXID 6
#define LENTHSTR 41
#define MAXPRICE 12
#define MAXUNITS 6

#define OPT_LENGTH 81

#define POSXINI 1
#define POSYINI 1
#define TABLE_XINIT 1
#define TABLE_YINIT 3
#define FORM_Y 4
#define ROWS_VISIBLE 10

#define DATA_FILE "productos.dat"

typedef struct {
   char ProductId[MAXID];
   char ProductName[LENTHSTR];
   char SupplierId[MAXID];
   char CategoryId[MAXID];
   char QuantityPerUnit[LENTHSTR];
   float UnitPrice;
   int UnitsInStock;
   int ReorderLevel;
   char Discontinued;
} PRODUCT;

typedef enum { VIGENTES = 0,
               DESCONTINUADOS,
               AMBOS } FiltroLista;

void setcolor(int ct, int cf);
void colordefault(void);
void show_field(char *field_str, int cursor_position, int max_length,
                int pos_x, int pos_y);
int validate_alpha_char(char c);
int validate_alphanumeric_char(char c);
int validate_price_char(char c);
void captureAlphanumeric(char *result_str, int max_length,
                         int Xpos, int Ypos, int (*validator)(char));
void CaptureInteger(char *digit_str, int *result_value,
                    int max_digits, int pos_x, int pos_y);
void CaptureFloat(char *digit_str, float *result_value,
                  int max_digits, int pos_x, int pos_y);
void CaptureDiscontinued(char *result, int pos_x, int pos_y);
void show_header(char menu[][OPT_LENGTH], int options,
                 int Xpos, int Ypos, int isSelected);
void show_form(PRODUCT *prod, int isSelected, int isNew);
int EditForm(PRODUCT *prod, int isNew);
int getLastId(void);
void IngreseProducto(void);
void ModificarProducto(void);
void EliminarProducto(void);
void ListarProductos(void);

typedef struct {
   int id, name, supplier, category, qty, price, stock, reorder;
} ColWidths;

ColWidths calc_col_widths(PRODUCT *prods, int n);
void show_table_header(int px, int py, ColWidths w);
void show_table_row(PRODUCT *prod, int py, int px, int isSelected, ColWidths w);
int movimiento_productos(PRODUCT *prods, int n,
                         int py, int px,
                         FiltroLista filtro, char *patron,
                         ColWidths w);

int main(void) {
   _setcursortype(0);
   char menu[][OPT_LENGTH] = {
       "Ingresar", "Modificar", "Eliminar", "Listar", "Salir"};
   int isSelected = 0;
   int full_redraw = 1;
   int key;

   do {
      if (full_redraw) {
         system("cls");
         full_redraw = 0;
      }
      show_header(menu, 5, 1, 1, isSelected);

      key = getch();
      if (key == 0 || key == 224)
         key = getch();

      if (key == LEFT && isSelected > 0)
         isSelected--;
      if (key == RIGHT && isSelected < 4)
         isSelected++;

      if (key == ENTER) {
         switch (isSelected) {
         case 0:
            IngreseProducto();
            break;
         case 1:
            ModificarProducto();
            break;
         case 2:
            EliminarProducto();
            break;
         case 3:
            ListarProductos();
            break;
         case 4:
            key = ESC;
            break;
         }
         if (key != ESC)
            full_redraw = 1;
      }
   } while (key != ESC);

   _setcursortype(100);
   return 0;
}

/*
 * Funcion  : show_header
 * Objetivo : Despliega el menu horizontal (pushdown) resaltando
 *            la opcion actualmente seleccionada.
 * Argumentos: menu      - arreglo de cadenas con las opciones del menu
 *             options   - numero de opciones
 *             Xpos      - columna inicial en pantalla
 *             Ypos      - fila inicial en pantalla
 *             isSelected - indice de la opcion resaltada
 */
void show_header(char menu[][OPT_LENGTH], int options,
                 int Xpos, int Ypos, int isSelected) {
   gotoxy(Xpos, Ypos);
   setcolor(CT, CF);
   printf("Productos  ");

   int col_offset = (int)strlen("Productos  ") + Xpos;

   for (int i = 0; i < options; i++) {
      gotoxy(col_offset, Ypos);
      if (isSelected == i)
         setcolor(CTS, CFS);
      else
         setcolor(CT, CF);
      printf("%s", menu[i]);
      col_offset += (int)strlen(menu[i]) + 2;
      setcolor(CT, CF);
      printf("  ");
   }
   colordefault();
}

void setcolor(int ct, int cf) {
   textbackground(cf);
   textcolor(ct);
}

void colordefault(void) {
   setcolor(LIGHTGRAY, BLACK);
}

/*
 * Funcion  : validate_alpha_char
 * Objetivo : Valida que un caracter sea letra, guion o espacio.
 * Argumentos: c - caracter a validar
 * Retorna  : 1 si es valido, 0 si no lo es
 */
int validate_alpha_char(char c) {
   return (isalpha((unsigned char)c) || c == ' ' || c == '-');
}

/*
 * Funcion  : validate_alphanumeric_char
 * Objetivo : Valida que un caracter sea letra, digito, guion o espacio.
 * Argumentos: c - caracter a validar
 * Retorna  : 1 si es valido, 0 si no lo es
 */
int validate_alphanumeric_char(char c) {
   return (isalpha((unsigned char)c) || isdigit((unsigned char)c) || c == ' ' || c == '-');
}

/*
 * Funcion  : validate_price_char
 * Objetivo : Valida que un caracter sea digito o punto decimal.
 * Argumentos: c - caracter a validar
 * Retorna  : 1 si es valido, 0 si no lo es
 */
int validate_price_char(char c) {
   return (isdigit((unsigned char)c) || c == '.');
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
   for (int i = 0; i < max_length; i++) {
      gotoxy(pos_x + i + 1, pos_y);
      printf("%c", field_str[i] ? field_str[i] : ' ');
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
                         int Xpos, int Ypos, int (*validator)(char)) {
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
 * Funcion  : CaptureFloat
 * Objetivo : Captura un valor flotante aceptando digitos y un punto decimal.
 *            Sale con ENTER o ESC.
 * Argumentos: digit_str    - buffer de trabajo con el texto del numero
 *             result_value - puntero donde se almacena el flotante resultante
 *             max_digits   - cantidad maxima de caracteres
 *             pos_x        - columna inicial del campo en pantalla
 *             pos_y        - fila del campo en pantalla
 */
void CaptureFloat(char *digit_str, float *result_value,
                  int max_digits, int pos_x, int pos_y) {
   int cursor_index = 0;
   char pressed_key;
   int dot_count = 0;

   for (int i = 0; digit_str[i]; i++)
      if (digit_str[i] == '.')
         dot_count++;

   _setcursortype(100);

   do {
      show_field(digit_str, cursor_index, max_digits, pos_x, pos_y);
      fflush(stdin);
      do {
         pressed_key = getch();
      } while (!validate_price_char(pressed_key) &&
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
               if (digit_str[cursor_index - 1] == '.')
                  dot_count--;
               cursor_index--;
               memmove(digit_str + cursor_index,
                       digit_str + cursor_index + 1,
                       max_digits - cursor_index);
            }
         } else {
            if (pressed_key == '.' && dot_count > 0) {
            } else {
               if (cursor_index < max_digits - 1) {
                  if (pressed_key == '.')
                     dot_count++;
                  memmove(digit_str + cursor_index + 1,
                          digit_str + cursor_index,
                          max_digits - cursor_index - 1);
                  digit_str[cursor_index] = pressed_key;
                  cursor_index++;
               }
            }
         }
      }
   } while (pressed_key != ENTER && pressed_key != ESC);

   if (pressed_key != ESC) {
      digit_str[cursor_index] = '\0';
      *result_value = (float)atof(digit_str);
   }
}

/*
 * Funcion  : CaptureDiscontinued
 * Objetivo : Captura el campo Discontinued aceptando solo 'S' o 'N'.
 *            Sale con ENTER o ESC sin modificar el valor si no se digito nada.
 * Argumentos: result - puntero al caracter donde se almacena el valor
 *             pos_x  - columna del campo en pantalla
 *             pos_y  - fila del campo en pantalla
 */
void CaptureDiscontinued(char *result, int pos_x, int pos_y) {
   char key;
   _setcursortype(100);

   do {
      gotoxy(pos_x, pos_y);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("[%c]", *result ? *result : ' ');
      colordefault();
      gotoxy(pos_x + 1, pos_y);

      key = (char)toupper(getch());
   } while (key != 'S' && key != 'N' && key != ENTER && key != ESC);

   if (key == 'S' || key == 'N')
      *result = key;
}

/*
 * Funcion  : getLastId
 * Objetivo : Recorre el archivo binario y obtiene el ID maximo registrado.
 * Retorna  : El ID maximo encontrado mas uno, o 1 si el archivo no existe
 */
int getLastId(void) {
   FILE *file = fopen(DATA_FILE, "rb");
   if (!file)
      return 1;

   PRODUCT temp;
   int max_id = 0;
   while (fread(&temp, sizeof(PRODUCT), 1, file)) {
      int curr = atoi(temp.ProductId);
      if (curr > max_id)
         max_id = curr;
   }
   fclose(file);
   return max_id + 1;
}

/*
 * Funcion  : show_form
 * Objetivo : Dibuja el formulario completo de un producto resaltando
 *            el campo actualmente seleccionado.
 * Argumentos: prod       - puntero al producto cuyos datos se muestran
 *             isSelected - indice del campo resaltado
 *             isNew      - 1 si es ingreso nuevo, 0 si es modificacion
 */
void show_form(PRODUCT *prod, int isSelected, int isNew) {
   char *labels[] = {
       "ID:",
       "Nombre:",
       "Proveedor ID:",
       "Categoria ID:",
       "Cantidad/Unidad:",
       "Precio Unitario:",
       "Unidades Stock:",
       "Nivel Reorden:",
       "Descontinuado (S/N):",
       "[ GUARDAR ]"};
   int num_fields = 10;

   gotoxy(1, 1);
   setcolor(CET, CEF);
   if (isNew)
      printf("INGRESANDO NUEVO PRODUCTO");
   else
      printf("MODIFICANDO PRODUCTO (ID: %s)", prod->ProductId);
   colordefault();

   for (int i = 0; i < num_fields; i++) {
      gotoxy(3, FORM_Y + i);
      if (isSelected == i)
         setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setcolor(TEXT_COLOR, BG_COLOR);

      printf("%-22s", labels[i]);
      gotoxy(26, FORM_Y + i);

      switch (i) {
      case 0:
         printf("%s", prod->ProductId);
         break;
      case 1:
         printf("%s", prod->ProductName);
         break;
      case 2:
         printf("%s", prod->SupplierId);
         break;
      case 3:
         printf("%s", prod->CategoryId);
         break;
      case 4:
         printf("%s", prod->QuantityPerUnit);
         break;
      case 5:
         printf("%.2f", prod->UnitPrice);
         break;
      case 6:
         printf("%d", prod->UnitsInStock);
         break;
      case 7:
         printf("%d", prod->ReorderLevel);
         break;
      case 8:
         printf("%c", prod->Discontinued ? prod->Discontinued : ' ');
         break;
      case 9:
         if (isSelected == 9)
            printf("<< PRESIONE ENTER PARA GUARDAR >> ");
         break;
      }
      colordefault();
   }

   gotoxy(1, FORM_Y + num_fields + 1);
   setcolor(CET, CEF);
   printf("Flechas: Navegar | ENTER: Editar campo | ESC: Cancelar");
   colordefault();
}

/*
 * Funcion  : EditForm
 * Objetivo : Gestiona la navegacion y edicion de todos los campos
 *            de un PRODUCT mediante el formulario en pantalla.
 * Argumentos: prod  - puntero al producto a editar
 *             isNew - 1 si es ingreso nuevo, 0 si es modificacion
 * Retorna  : 1 si el usuario guardo, 0 si cancelo con ESC
 */
int EditForm(PRODUCT *prod, int isNew) {
   int field = 1;
   int key;
   int saved = 0;
   int num_fields = 10;
   char temp_str[MAXPRICE + 1];
   int temp_int;
   float temp_float;

   system("cls");
   do {
      show_form(prod, field, isNew);

      key = getch();
      if (key == 0 || key == 224)
         key = getch();

      if (key == DOWN && field < num_fields - 1)
         field++;
      if (key == UP && field > 0)
         field--;

      if (key == ENTER) {
         switch (field) {
         case 1:
            captureAlphanumeric(prod->ProductName, LENTHSTR - 1,
                                25, FORM_Y + 1,
                                validate_alpha_char);
            break;
         case 2:
            captureAlphanumeric(prod->SupplierId, MAXID - 1,
                                25, FORM_Y + 2,
                                validate_alphanumeric_char);
            break;
         case 3:
            captureAlphanumeric(prod->CategoryId, MAXID - 1,
                                25, FORM_Y + 3,
                                validate_alphanumeric_char);
            break;
         case 4:
            captureAlphanumeric(prod->QuantityPerUnit, LENTHSTR - 1,
                                25, FORM_Y + 4,
                                validate_alphanumeric_char);
            break;
         case 5:
            sprintf(temp_str, "%.2f", prod->UnitPrice);
            CaptureFloat(temp_str, &temp_float,
                         MAXPRICE - 1, 25, FORM_Y + 5);
            prod->UnitPrice = temp_float;
            break;
         case 6:
            sprintf(temp_str, "%d", prod->UnitsInStock);
            CaptureInteger(temp_str, &temp_int,
                           MAXUNITS - 1, 25, FORM_Y + 6);
            prod->UnitsInStock = temp_int;
            break;
         case 7:
            sprintf(temp_str, "%d", prod->ReorderLevel);
            CaptureInteger(temp_str, &temp_int,
                           MAXUNITS - 1, 25, FORM_Y + 7);
            prod->ReorderLevel = temp_int;
            break;
         case 8:
            CaptureDiscontinued(&prod->Discontinued, 25, FORM_Y + 8);
            break;
         case 9:
            saved = 1;
            key = ESC;
            break;
         }
      }
   } while (key != ESC);

   return saved;
}

/*
 * Funcion  : IngreseProducto
 * Objetivo : Crea un nuevo PRODUCT con ID autogenerado y lo persiste
 *            al final del archivo binario.
 */
void IngreseProducto(void) {
   PRODUCT nuevo;
   memset(&nuevo, 0, sizeof(PRODUCT));

   int next_id = getLastId();
   sprintf(nuevo.ProductId, "%05d", next_id);
   nuevo.Discontinued = 'N';

   if (EditForm(&nuevo, 1)) {
      FILE *file = fopen(DATA_FILE, "ab");
      if (file) {
         fwrite(&nuevo, sizeof(PRODUCT), 1, file);
         fclose(file);
         system("cls");
         gotoxy(1, 1);
         setcolor(GREEN, BLACK);
         printf("Producto %s guardado exitosamente.", nuevo.ProductId);
         colordefault();
         getch();
      }
   }
}

/*
 * Funcion  : ModificarProducto
 * Objetivo : Carga todos los productos, permite seleccionar uno mediante
 *            scroll, edita sus campos y reescribe el registro en su
 *            posicion original dentro del archivo binario.
 */
void ModificarProducto(void) {
   FILE *file = fopen(DATA_FILE, "rb+");
   if (!file) {
      gotoxy(1, 3);
      printf("No hay productos registrados.");
      getch();
      return;
   }

   fseek(file, 0, SEEK_END);
   int count = (int)(ftell(file) / sizeof(PRODUCT));
   rewind(file);

   if (count == 0) {
      fclose(file);
      gotoxy(1, 3);
      printf("No hay productos registrados.");
      getch();
      return;
   }

   PRODUCT *lista = (PRODUCT *)malloc(count * sizeof(PRODUCT));
   fread(lista, sizeof(PRODUCT), count, file);

   system("cls");
   char patron[LENTHSTR] = {0};
   ColWidths w = calc_col_widths(lista, count);
   int selected = movimiento_productos(lista, count, TABLE_YINIT, TABLE_XINIT, AMBOS, patron, w);

   if (selected >= 0) {
      PRODUCT editando = lista[selected];
      if (EditForm(&editando, 0)) {
         fseek(file, (long)(selected * sizeof(PRODUCT)), SEEK_SET);
         fwrite(&editando, sizeof(PRODUCT), 1, file);
         system("cls");
         gotoxy(1, 1);
         setcolor(GREEN, BLACK);
         printf("Producto %s modificado.", editando.ProductId);
         colordefault();
         getch();
      }
   }

   free(lista);
   fclose(file);
}

/*
 * Funcion  : EliminarProducto
 * Objetivo : Selecciona un producto mediante scroll, solicita confirmacion
 *            y reescribe el archivo binario sin ese registro.
 */
void EliminarProducto(void) {
   FILE *file = fopen(DATA_FILE, "rb");
   if (!file) {
      gotoxy(1, 3);
      printf("No hay productos registrados.");
      getch();
      return;
   }

   fseek(file, 0, SEEK_END);
   int count = (int)(ftell(file) / sizeof(PRODUCT));
   rewind(file);

   if (count == 0) {
      fclose(file);
      gotoxy(1, 3);
      printf("No hay productos registrados.");
      getch();
      return;
   }

   PRODUCT *lista = (PRODUCT *)malloc(count * sizeof(PRODUCT));
   fread(lista, sizeof(PRODUCT), count, file);
   fclose(file);

   system("cls");
   char patron[LENTHSTR] = {0};
   ColWidths w = calc_col_widths(lista, count);
   int selected = movimiento_productos(lista, count,
                                       TABLE_YINIT, TABLE_XINIT,
                                       AMBOS, patron, w);

   if (selected >= 0) {
      system("cls");
      gotoxy(1, 1);
      setcolor(RED, BLACK);
      printf("Esta seguro que desea eliminar el producto:");
      gotoxy(1, 2);
      printf("  ID: %-6s  Nombre: %s", lista[selected].ProductId,
             lista[selected].ProductName);
      gotoxy(1, 4);
      printf("[S] Si, eliminar    [N] No, cancelar");
      colordefault();

      char confirm;
      do {
         confirm = (char)toupper(getch());
      } while (confirm != 'S' && confirm != 'N');

      if (confirm == 'S') {
         FILE *tmp = fopen(DATA_FILE, "wb");
         if (tmp) {
            for (int i = 0; i < count; i++) {
               if (i != selected)
                  fwrite(&lista[i], sizeof(PRODUCT), 1, tmp);
            }
            fclose(tmp);
            gotoxy(1, 6);
            setcolor(GREEN, BLACK);
            printf("Producto eliminado correctamente.");
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
 * Funcion  : calc_col_widths
 * Objetivo : Calcula el ancho maximo de cada columna comparando el largo
 *            de cada dato contra el largo del encabezado correspondiente.
 * Argumentos: prods - arreglo de productos
 *             n     - cantidad de productos en el arreglo
 * Retorna  : estructura ColWidths con el ancho maximo de cada columna
 */
ColWidths calc_col_widths(PRODUCT *prods, int n) {
   ColWidths width;
   width.id = (int)strlen("ID");
   width.name = (int)strlen("Nombre");
   width.supplier = (int)strlen("Prov");
   width.category = (int)strlen("Cat");
   width.qty = (int)strlen("Cant/Unidad");
   width.price = (int)strlen("Precio");
   width.stock = (int)strlen("Stock");
   width.reorder = (int)strlen("Reord.");

   char buf[20];
   for (int ind = 0; ind < n; ind++) {
      int len;

      len = (int)strlen(prods[ind].ProductId);
      if (len > width.id)
         width.id = len;

      len = (int)strlen(prods[ind].ProductName);
      if (len > width.name)
         width.name = len;

      len = (int)strlen(prods[ind].SupplierId);
      if (len > width.supplier)
         width.supplier = len;

      len = (int)strlen(prods[ind].CategoryId);
      if (len > width.category)
         width.category = len;

      len = (int)strlen(prods[ind].QuantityPerUnit);
      if (len > width.qty)
         width.qty = len;

      sprintf(buf, "%.2f", prods[ind].UnitPrice);
      len = (int)strlen(buf);
      if (len > width.price)
         width.price = len;

      sprintf(buf, "%d", prods[ind].UnitsInStock);
      len = (int)strlen(buf);
      if (len > width.stock)
         width.stock = len;

      sprintf(buf, "%d", prods[ind].ReorderLevel);
      len = (int)strlen(buf);
      if (len > width.reorder)
         width.reorder = len;
   }
   return width;
}

/*
 * Funcion  : show_table_header
 * Objetivo : Dibuja el encabezado de columnas de la tabla con anchos dinamicos.
 * Argumentos: px - columna inicial en pantalla
 *             py - fila en pantalla
 *             w  - anchos de columna calculados por calc_col_widths
 */
void show_table_header(int px, int py, ColWidths w) {
   gotoxy(px, py);
   setcolor(CET, CEF);
   printf("%-*s %-*s %-*s %-*s %-*s %*s %*s %*s D",
          w.id, "ID",
          w.name, "Nombre",
          w.supplier, "Prov",
          w.category, "Cat",
          w.qty, "Cant/Unidad",
          w.price, "Precio",
          w.stock, "Stock",
          w.reorder, "Reord.");
   colordefault();
}

/*
 * Funcion  : show_table_row
 * Objetivo : Dibuja una fila de producto con anchos dinamicos,
 *            resaltandola si esta seleccionada.
 * Argumentos: prod       - puntero al producto a mostrar
 *             py         - fila en pantalla
 *             px         - columna inicial en pantalla
 *             isSelected - 1 si la fila esta seleccionada, 0 si no
 *             w          - anchos de columna calculados por calc_col_widths
 */
void show_table_row(PRODUCT *prod, int py, int px, int isSelected, ColWidths w) {
   gotoxy(px, py);
   if (isSelected)
      setcolor(CTS_ROW, CFS_ROW);
   else
      setcolor(CT_ROW, CF_ROW);

   printf("%-*s %-*s %-*s %-*s %-*s %*.2f %*d %*d %c",
          w.id, prod->ProductId,
          w.name, prod->ProductName,
          w.supplier, prod->SupplierId,
          w.category, prod->CategoryId,
          w.qty, prod->QuantityPerUnit,
          w.price, prod->UnitPrice,
          w.stock, prod->UnitsInStock,
          w.reorder, prod->ReorderLevel,
          prod->Discontinued);

   colordefault();
}

/*
 * Funcion  : movimiento_productos
 * Objetivo : Presenta la lista con scroll (ROWS_VISIBLE filas a la vez),
 *            permite navegar con flechas, filtrar con TAB y seleccionar
 *            con ENTER. F2/F3/F4 cambian la vista entre vigentes,
 *            descontinuados y ambos.
 * Argumentos: prods   - arreglo de productos cargados en memoria
 *             n       - cantidad total de productos
 *             py      - fila donde inicia la tabla en pantalla
 *             px      - columna donde inicia la tabla en pantalla
 *             filtro  - vista inicial (VIGENTES, DESCONTINUADOS o AMBOS)
 *             patron  - buffer del patron de filtro activo
 *             w       - anchos de columna calculados por calc_col_widths
 * Retorna  : indice del producto seleccionado en el arreglo original,
 *            o -1 si el usuario salio con ESC
 */
int movimiento_productos(PRODUCT *prods, int n,
                         int py, int px,
                         FiltroLista filtro, char *patron,
                         ColWidths w) {

   int *indices = (int *)malloc(n * sizeof(int));
   int filtered = 0;
   char patron_lower[LENTHSTR];
   char buf[LENTHSTR];
   int opcsel = 0;
   int scroll = 0;
   int special;
   int modo_filtro = 0;

   memset(patron_lower, 0, sizeof(patron_lower));

   while (1) {
      filtered = 0;
      int plen = (int)strlen(patron);
      for (int i = 0; i < plen; i++)
         patron_lower[i] = (char)tolower((unsigned char)patron[i]);
      patron_lower[plen] = '\0';

      for (int i = 0; i < n; i++) {
         if (filtro == VIGENTES && prods[i].Discontinued == 'S')
            continue;
         if (filtro == DESCONTINUADOS && prods[i].Discontinued != 'S')
            continue;
         if (plen > 0) {
            strncpy(buf, prods[i].ProductName, LENTHSTR - 1);
            buf[LENTHSTR - 1] = '\0';
            for (int j = 0; buf[j]; j++)
               buf[j] = (char)tolower((unsigned char)buf[j]);
            if (!strstr(buf, patron_lower))
               continue;
         }
         indices[filtered++] = i;
      }

      if (filtered == 0) {
         opcsel = 0;
         scroll = 0;
      } else {
         if (opcsel >= filtered)
            opcsel = filtered - 1;
         if (opcsel < 0)
            opcsel = 0;
         if (scroll > opcsel)
            scroll = opcsel;
         if (scroll < opcsel - ROWS_VISIBLE + 1)
            scroll = opcsel - ROWS_VISIBLE + 1;
         if (scroll < 0)
            scroll = 0;
      }

      gotoxy(1, 1);
      setcolor(CT, CF);
      printf("Filtro: [%-30s]  Vista: [%-14s]  (%d registros)   ",
             patron,
             filtro == VIGENTES ? "Vigentes" : filtro == DESCONTINUADOS ? "Descontinuados"
                                                                        : "Ambos",
             filtered);
      colordefault();

      gotoxy(1, 2);
      setcolor(CT, CF);
      if (modo_filtro)
         printf("MODO FILTRO: escriba y ENTER/TAB para volver | ESC: limpiar filtro%10s", "");
      else
         printf("TAB: filtrar | F2 Vig. F3 Desc. F4 Ambos | Flechas: navegar | ENTER: sel. | ESC: volver");
      colordefault();

      show_table_header(px, py, w);

      for (int row = 0; row < ROWS_VISIBLE; row++) {
         int abs_idx = scroll + row;
         if (abs_idx < filtered) {
            show_table_row(&prods[indices[abs_idx]],
                           py + 1 + row, px,
                           (abs_idx == opcsel), w);
         } else {
            gotoxy(px, py + 1 + row);
            setcolor(CT_ROW, CF_ROW);
            printf("%-80s", "");
            colordefault();
         }
      }

      if (modo_filtro) {
         _setcursortype(100);
         captureAlphanumeric(patron, LENTHSTR - 1, 8, 1,
                             validate_alphanumeric_char);
         modo_filtro = 0;
         opcsel = 0;
         scroll = 0;
      } else {
         _setcursortype(0);
         special = 0;
         int ikey = getch();
         if (ikey == 0 || ikey == 224) {
            ikey = getch();
            special = 1;
         }

         if (special) {
            if (ikey == UP && opcsel > 0) {
               opcsel--;
               if (opcsel < scroll)
                  scroll = opcsel;
            } else if (ikey == DOWN && opcsel < filtered - 1) {
               opcsel++;
               if (opcsel >= scroll + ROWS_VISIBLE)
                  scroll = opcsel - ROWS_VISIBLE + 1;
            } else if (ikey == 60) {
               filtro = VIGENTES;
               opcsel = 0;
               scroll = 0;
               patron[0] = '\0';
            } else if (ikey == 61) {
               filtro = DESCONTINUADOS;
               opcsel = 0;
               scroll = 0;
               patron[0] = '\0';
            } else if (ikey == 62) {
               filtro = AMBOS;
               opcsel = 0;
               scroll = 0;
               patron[0] = '\0';
            }
         } else {
            if (ikey == TAB) {
               modo_filtro = 1;
            } else if (ikey == ESC) {
               if (strlen(patron) > 0) {
                  patron[0] = '\0';
                  opcsel = 0;
                  scroll = 0;
               } else {
                  free(indices);
                  return -1;
               }
            } else if (ikey == ENTER && filtered > 0) {
               int result = indices[opcsel];
               free(indices);
               return result;
            }
         }
      }
   }
}
/*
 * Funcion  : ListarProductos
 * Objetivo : Carga todos los productos del archivo y lanza la vista
 *            interactiva de tabla. Solo es para consulta.
 */
void ListarProductos(void) {
   FILE *file = fopen(DATA_FILE, "rb");
   if (!file) {
      system("cls");
      gotoxy(1, 1);
      printf("No hay productos registrados.");
      getch();
      return;
   }

   fseek(file, 0, SEEK_END);
   int count = (int)(ftell(file) / sizeof(PRODUCT));
   rewind(file);

   if (count == 0) {
      fclose(file);
      system("cls");
      gotoxy(1, 1);
      printf("No hay productos registrados.");
      getch();
      return;
   }

   PRODUCT *lista = (PRODUCT *)malloc(count * sizeof(PRODUCT));
   fread(lista, sizeof(PRODUCT), count, file);
   fclose(file);

   system("cls");
   char patron[LENTHSTR] = {0};
   ColWidths w = calc_col_widths(lista, count);
   movimiento_productos(lista, count,
                        TABLE_YINIT, TABLE_XINIT,
                        AMBOS, patron, w);

   free(lista);
}

/*
 * Charles David ; crud.c
 *
 * Aplicacion CRUD (Crear, Leer, Actualizar, Eliminar) para la clase de
 * Fundamentos de Programacion. Permite gestionar productos almacenados
 * en un archivo binario.
 */

#include <conio.c>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SELECTED_TEXTCOLOR BLACK
#define SELECTED_BGCOLOR BLUE
#define TEXT_COLOR LIGHTGRAY
#define BG_COLOR BLACK

#define TABLE_TEXTCOLOR BLACK
#define TABLE_BGCOLOR BLUE
#define TABLE_CURSOR_TEXTCOLOR BLACK
#define TABLE_CURSOR_BGCOLOR YELLOW
#define TABLE_SELECTED_BGCOLOR GREEN

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ESC 27
#define ENTER 13
#define BACKSPACE 8

#define ENTER_INFO 0
#define LIST 1
#define EXIT 2
#define OPTIONS 3
#define OPT_LENGTH 81

#define MAXID 6
#define LENTHSTR 41

#define INPUT_X 20
#define X_INIT 1
#define Y_INIT 1
#define TABLE_XINIT 1
#define TABLE_YINIT 3
#define FORM_Y 5

#define MAX_INTFLOAT 6
#define MAX_DECIMFLOAT 2

typedef struct {
   int dd, mm, yyyy;
} __DATE;
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

void DeleteProduct(PRODUCT *prdcts, int index, int real_index);
void FileInput(void);
void FileModify(PRODUCT *prdct, int index);
void generate_id(char *new_id);
void show_form(PRODUCT *p, int selected);
void overwrite_product(PRODUCT *prdct, int index);
void get_col_widths(PRODUCT *prdcts, int count, int *widths);
void CaptureInteger(char *digit_str, int *result_value, int max_digits, int Xpos, int Ypos);
void CaptureFloat(char *decimal_str, int *result_value, int max_digits, int pos_x, int Ypos);
void show_field(char *field_str, int cursor_position, int max_length, int Xpos, int Ypos);
void show_header(char menu[][OPT_LENGTH], int Options, int Xpos, int Ypos, int isSelected);
void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos, int (*validator)(char));
void showopcxy_productos(PRODUCT *prdcts, int num, int Xpos, int Ypos, int opcsel);
void setcolor(int text_color, int background_color);
void colordefault(void);

float captureFloat(int max_digits, int max_decimals, int Xpos, int Ypos);

int show_action_menu(int Xpos, int Ypos);
int show_filter_menu(void);
int ProductMovement(PRODUCT **prdcts, int **file_indices, int *count, int Xpos, int Ypos, int OptionSelection, char filter);
int header_movement(char menu[][OPT_LENGTH], int Options, int Xpos, int Ypos, int isSelected);
int getPrdcts(PRODUCT **Prdcts);
int getPrdctsFiltered(PRODUCT **prdcts, int **file_indices, char filter);
int validate_alpha_char(char _char);
int validate_alphanumeric_char(char _char);

int main() {
   system("cls");

   PRODUCT *prdcts = NULL;
   int *file_indices = NULL;
   int count = 0;

   char menu[][OPT_LENGTH] = {"Ingresar", "Listar", "Salir"};
   int isSelected = 0;

   count = getPrdctsFiltered(&prdcts, &file_indices, 'A');
   show_header(menu, OPTIONS, X_INIT, Y_INIT, isSelected);
   if (count > 0)
      showopcxy_productos(prdcts, count, TABLE_XINIT, TABLE_YINIT, -1);

   do {
      isSelected = header_movement(menu, OPTIONS, X_INIT, Y_INIT, isSelected);

      if (isSelected == ENTER_INFO) {
         system("cls");
         FileInput();

         free(prdcts);
         prdcts = NULL;
         free(file_indices);
         file_indices = NULL;
         count = getPrdctsFiltered(&prdcts, &file_indices, 'A');
         system("cls");
         show_header(menu, OPTIONS, X_INIT, Y_INIT, isSelected);
         if (count > 0)
            showopcxy_productos(prdcts, count, TABLE_XINIT, TABLE_YINIT, -1);

      } else if (isSelected == LIST) {
         char filter = show_filter_menu();
         free(prdcts);
         prdcts = NULL;
         free(file_indices);
         file_indices = NULL;
         count = getPrdctsFiltered(&prdcts, &file_indices, filter);

         system("cls");
         show_header(menu, OPTIONS, X_INIT, Y_INIT, isSelected);
         if (count > 0) {
            showopcxy_productos(prdcts, count, TABLE_XINIT, TABLE_YINIT, 0);
            ProductMovement(&prdcts, &file_indices, &count,
                            TABLE_XINIT, TABLE_YINIT, 0, filter);
         }

         free(prdcts);
         prdcts = NULL;
         free(file_indices);
         file_indices = NULL;
         count = getPrdctsFiltered(&prdcts, &file_indices, 'A');
         system("cls");
         show_header(menu, OPTIONS, X_INIT, Y_INIT, isSelected);
         if (count > 0)
            showopcxy_productos(prdcts, count, TABLE_XINIT, TABLE_YINIT, -1);

      } else if (isSelected == EXIT) {
         free(prdcts);
         free(file_indices);
         system("cls");
         return 0;
      }

   } while (1);

   return 0;
}

/*
 * Funcion: getPrdcts
 * Argumentos:
 *   PRODUCT **prdcts: puntero doble donde se almacenara el arreglo dinamico
 * Objetivo:
 *   Leer todos los registros del archivo binario y cargarlos en memoria.
 * Retorno: Cantidad de productos leidos.
 */
int getPrdcts(PRODUCT **prdcts) {
   FILE *file = fopen("products.dat", "rb");
   if (file == NULL)
      return 0;

   int count = 0;
   PRODUCT temp, *ptr = NULL;

   while (fread(&temp, sizeof(PRODUCT), 1, file)) {
      ptr = realloc(*prdcts, (count + 1) * sizeof(PRODUCT));
      if (ptr == NULL)
         break;
      *prdcts = ptr;
      (*prdcts)[count++] = temp;
   }

   fclose(file);
   return count;
}

/*
 * Funcion: getPrdctsFiltered
 * Argumentos:
 *   PRODUCT **prdcts: puntero doble donde se almacena el arreglo filtrado
 *   int **file_indices: puntero doble donde se guardan los indices reales
 *                       de cada registro en el archivo (para overwrite_product)
 *   char filter: 'N' = solo vigentes, 'S' = solo descontinuados, 'A' = ambos
 * Objetivo:
 *   Leer el archivo y cargar solo los productos que coincidan con el filtro.
 *   Guardar en file_indices la posicion real de cada registro en el archivo
 *   para que DeleteProduct y FileModify puedan sobreescribir el registro correcto.
 * Retorno: Cantidad de productos que pasaron el filtro.
 */
int getPrdctsFiltered(PRODUCT **prdcts, int **file_indices, char filter) {
   FILE *file = fopen("products.dat", "rb");
   if (file == NULL)
      return 0;

   int count = 0;
   int file_pos = 0;
   PRODUCT temp, *ptr = NULL;
   int *iptr = NULL;

   while (fread(&temp, sizeof(PRODUCT), 1, file)) {
      if (filter == 'A' || temp.Discontinued == filter) {
         ptr = realloc(*prdcts, (count + 1) * sizeof(PRODUCT));
         iptr = realloc(*file_indices, (count + 1) * sizeof(int));

         if (ptr == NULL || iptr == NULL)
            break;

         *prdcts = ptr;
         *file_indices = iptr;

         (*prdcts)[count] = temp;
         (*file_indices)[count] = file_pos;
         count++;
      }
      file_pos++;
   }

   fclose(file);
   return count;
}

/*
 * Funcion: show_filter_menu
 * Objetivo:
 *   Mostrar un submenu que permite al usuario elegir que productos
 *   quiere ver: vigentes, descontinuados o ambos.
 * Retorno: 'N' para vigentes, 'S' para descontinuados, 'A' para ambos.
 */
int show_filter_menu(void) {
   char *options[] = {"Vigentes", "Descontinuados", "Ambos"};
   int selected = 0;
   char key;

   system("cls");

   do {
      gotoxy(1, 1);
      setcolor(TEXT_COLOR, BG_COLOR);
      printf("Mostrar productos: ");

      for (int i = 0; i < 3; i++) {
         gotoxy(1 + i * 18, 2);
         if (selected == i)
            setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
         else
            setcolor(TEXT_COLOR, BG_COLOR);
         printf("%-16s", options[i]);
      }
      colordefault();

      do {
         key = getch();
         if (key == 0 || key == 224)
            key = getch();
      } while (key != ENTER && key != LEFT && key != RIGHT && key != ESC);

      if (key == LEFT && selected > 0)
         selected--;
      if (key == RIGHT && selected < 2)
         selected++;

   } while (key != ENTER && key != ESC);

   if (selected == 0)
      return 'N';
   if (selected == 1)
      return 'S';
   return 'A';
}

/*
 * Funcion: overwrite_product
 * Argumentos:
 *   PRODUCT *prdct: puntero al producto con los datos actualizados
 *   int index: posicion del registro en el archivo (basado en 0)
 * Objetivo:
 *   Sobreescribir un registro especifico en el archivo binario sin
 *   afectar los demas registros, usando fseek para posicionarse.
 */
void overwrite_product(PRODUCT *prdct, int index) {
   FILE *file = fopen("products.dat", "r+b");
   if (file == NULL)
      return;

   fseek(file, index * sizeof(PRODUCT), SEEK_SET);
   fwrite(prdct, sizeof(PRODUCT), 1, file);
   fclose(file);
}

/*
 * Funcion: generate_id
 * Argumentos:
 *   char *new_id: buffer donde se escribira el nuevo ID generado
 * Objetivo:
 *   Generar automaticamente el ID del siguiente producto leyendo el
 *   ultimo registro del archivo y sumando 1 al numero del ID.
 *   Si el archivo no existe, comienza desde P001.
 */
void generate_id(char *new_id) {
   PRODUCT last;
   int last_num = 0;

   FILE *file = fopen("products.dat", "rb");
   if (file != NULL) {
      fseek(file, -(long)sizeof(PRODUCT), SEEK_END);
      fread(&last, sizeof(PRODUCT), 1, file);
      fclose(file);
      last_num = atoi(last.ProductId + 1);
   }

   sprintf(new_id, "P%03d", last_num + 1);
}

/*
 * Funcion: show_form
 * Argumentos:
 *   PRODUCT *prdcts: puntero al producto cuyos datos se muestran
 *   int isSelected: indice del campo actualmente resaltado (0-8)
 * Objetivo:
 *   Mostrar el formulario de ingreso/edicion con todos los campos
 *   del producto. El campo seleccionado se resalta con color diferente.
 *   El campo "[ Guardar ]" es el ultimo (indice 8).
 */
void show_form(PRODUCT *prdcts, int isSelected) {
   char *labels[] = {"ID", "Nombre", "Edad", "Antiguedad", "Salario", "Jefe"};

   gotoxy(1, FORM_Y);
   setcolor(TEXT_COLOR, BG_COLOR);
   printf("Id: %-6s", prdcts->ProductId);

   char precio[20], stock[10], reord[10];
   sprintf(precio, "%.2f", prdcts->UnitPrice);
   sprintf(stock, "%d", prdcts->UnitsInStock);
   sprintf(reord, "%d", prdcts->ReorderLevel);

   char *values[] = {
       prdcts->ProductName,
       prdcts->SupplierId,
       prdcts->CategoryId,
       prdcts->QuantityPerUnit,
       precio,
       stock,
       reord,
       prdcts->Discontinued == 'N' ? "No" : "Si",
       ""};

   for (int i = 0; i < 9; i++) {
      gotoxy(1, FORM_Y + i + 1);

      if (isSelected == i)
         setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setcolor(TEXT_COLOR, BG_COLOR);

      printf("%-16s", labels[i]);
      colordefault();
      printf("%-41s", values[i]);
   }

   gotoxy(1, FORM_Y + 11);
   setcolor(TEXT_COLOR, BG_COLOR);
   printf("Flechas: Navegar | ENTER: Editar/Confirmar | ESC: Cancelar");
   colordefault();
}

/*
 * Funcion: FileInput
 * Objetivo:
 *   Mostrar el formulario vacio para ingresar un nuevo producto.
 *   El usuario navega con las flechas, edita con ENTER, y guarda
 *   seleccionando "[ Guardar ]". ESC cancela sin guardar.
 */
void FileInput(void) {
   PRODUCT prdcts;
   memset(&prdcts, 0, sizeof(PRODUCT));
   prdcts.Discontinued = 'N';

   generate_id(prdcts.ProductId);

   int field = 0;
   char key;

   do {
      show_form(&prdcts, field);

      do {
         key = getch();
         if (key == 0 || key == 224)
            key = getch();
      } while (key != UP && key != DOWN && key != ENTER && key != ESC);

      if (key == DOWN && field < 8)
         field++;
      if (key == UP && field > 0)
         field--;

      if (key == ENTER) {
         char temp[10] = {0};

         switch (field) {
         case 0:
            captureAlphanumeric(prdcts.ProductName, LENTHSTR,
                                INPUT_X, FORM_Y + 1, validate_alpha_char);
            break;
         case 1:
            captureAlphanumeric(prdcts.SupplierId, MAXID,
                                INPUT_X, FORM_Y + 2, validate_alphanumeric_char);
            break;
         case 2:
            captureAlphanumeric(prdcts.CategoryId, MAXID,
                                INPUT_X, FORM_Y + 3, validate_alphanumeric_char);
            break;
         case 3:
            captureAlphanumeric(prdcts.QuantityPerUnit, LENTHSTR,
                                INPUT_X, FORM_Y + 4, validate_alphanumeric_char);
            break;
         case 4:
            prdcts.UnitPrice = captureFloat(MAX_INTFLOAT, MAX_DECIMFLOAT,
                                            INPUT_X, FORM_Y + 5);
            break;
         case 5:
            CaptureInteger(temp, &prdcts.UnitsInStock, 5, INPUT_X, FORM_Y + 6);
            break;
         case 6:
            CaptureInteger(temp, &prdcts.ReorderLevel, 5, INPUT_X, FORM_Y + 7);
            break;
         case 7:
            prdcts.Discontinued = (prdcts.Discontinued == 'N') ? 'S' : 'N';
            break;
         case 8: {
            FILE *file = fopen("products.dat", "ab");
            if (file != NULL) {
               fwrite(&prdcts, sizeof(PRODUCT), 1, file);
               fclose(file);
            }
         }
            return;
         }
      }

   } while (key != ESC);
}

/*
 * Funcion: FileModify
 * Argumentos:
 *   PRODUCT *prdct: puntero al producto existente a modificar
 *   int index: posicion del registro en el archivo
 * Objetivo:
 *   Mostrar el formulario pre-llenado con los datos del producto
 *   seleccionado. El usuario puede editar cualquier campo excepto
 *   el ID. Al seleccionar "[ Guardar ]" se sobreescribe el registro.
 */
void FileModify(PRODUCT *prdct, int index) {
   PRODUCT edited = *prdct;
   int field = 0;
   char key;

   do {
      show_form(&edited, field);

      do {
         key = getch();
         if (key == 0 || key == 224)
            key = getch();
      } while (key != UP && key != DOWN && key != ENTER && key != ESC);

      if (key == DOWN && field < 8)
         field++;
      if (key == UP && field > 0)
         field--;

      if (key == ENTER) {
         char temp[10] = {0};

         switch (field) {
         case 0:
            captureAlphanumeric(edited.ProductName, LENTHSTR,
                                INPUT_X, FORM_Y + 1, validate_alpha_char);
            break;
         case 1:
            captureAlphanumeric(edited.SupplierId, MAXID,
                                INPUT_X, FORM_Y + 2, validate_alphanumeric_char);
            break;
         case 2:
            captureAlphanumeric(edited.CategoryId, MAXID,
                                INPUT_X, FORM_Y + 3, validate_alphanumeric_char);
            break;
         case 3:
            captureAlphanumeric(edited.QuantityPerUnit, LENTHSTR,
                                INPUT_X, FORM_Y + 4, validate_alphanumeric_char);
            break;
         case 4:
            edited.UnitPrice = captureFloat(MAX_INTFLOAT, MAX_DECIMFLOAT,
                                            INPUT_X, FORM_Y + 5);
            break;
         case 5:
            CaptureInteger(temp, &edited.UnitsInStock, 5, INPUT_X, FORM_Y + 6);
            break;
         case 6:
            CaptureInteger(temp, &edited.ReorderLevel, 5, INPUT_X, FORM_Y + 7);
            break;
         case 7:
            edited.Discontinued = (edited.Discontinued == 'N') ? 'S' : 'N';
            break;
         case 8:
            overwrite_product(&edited, index);
            return;
         }
      }

   } while (key != ESC);
}

/*
 * Funcion: DeleteProduct
 * Argumentos:
 *   PRODUCT *prdcts: arreglo filtrado en memoria
 *   int index: indice en el arreglo filtrado (para mostrar el nombre)
 *   int real_index: posicion real del registro en el archivo binario
 * Objetivo:
 *   Pedir confirmacion al usuario y, si confirma, marcar el producto
 *   como descontinuado sobreescribiendo el registro correcto en el archivo.
 *   Usa real_index para garantizar que se modifica el registro correcto
 *   independientemente del filtro que este activo.
 */
void DeleteProduct(PRODUCT *prdcts, int index, int real_index) {
   gotoxy(1, FORM_Y);
   setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
   printf("Seguro que desea eliminar \"%s\"? [S/N]: ",
          prdcts[index].ProductName);
   colordefault();

   char key;
   do {
      key = getch();
      key = toupper(key);
   } while (key != 'S' && key != 'N');

   if (key == 'S') {
      prdcts[index].Discontinued = 'S';
      overwrite_product(&prdcts[index], real_index);
   }
}

/*
 * Funcion: show_action_menu
 * Argumentos:
 *   int px: posicion horizontal del submenu
 *   int py: posicion vertical del submenu
 * Objetivo:
 *   Mostrar el submenu "Modificar | Eliminar" cuando el usuario
 *   presiona ENTER sobre un producto en la tabla.
 * Retorno: 0 para Modificar, 1 para Eliminar, -1 si presiona ESC.
 */
int show_action_menu(int px, int py) {
   char *options[] = {"Modificar", "Eliminar"};
   int selected = 0;
   char key;

   do {
      for (int i = 0; i < 2; i++) {
         gotoxy(px + i * 12, py);
         if (selected == i)
            setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
         else
            setcolor(TEXT_COLOR, BG_COLOR);
         printf("%-10s", options[i]);
      }
      colordefault();

      do {
         key = getch();
         if (key == 0 || key == 224)
            key = getch();
      } while (key != ENTER && key != ESC && key != LEFT && key != RIGHT);

      if (key == LEFT && selected > 0)
         selected--;
      if (key == RIGHT && selected < 1)
         selected++;

   } while (key != ENTER && key != ESC);

   return (key == ESC) ? -1 : selected;
}

/*
 * Funcion: ProductMovement
 * Argumentos:
 *   PRODUCT **prdcts: puntero doble al arreglo filtrado en memoria
 *   int **file_indices: puntero doble al arreglo de indices reales en archivo
 *   int *count: puntero al contador de productos visibles
 *   int Xpos: columna inicial de la tabla
 *   int Ypos: fila inicial de la tabla
 *   int OptionSelection: fila inicialmente seleccionada
 * Objetivo:
 *   Permitir navegar la tabla con flechas, seleccionar con ENTER
 *   y elegir entre Modificar o Eliminar. Usa file_indices para
 *   garantizar que overwrite_product sobreescriba el registro correcto
 *   en el archivo independientemente del filtro activo.
 * Retorno: Indice de la ultima fila seleccionada.
 */
int ProductMovement(PRODUCT **prdcts, int **file_indices, int *count,
                    int Xpos, int Ypos, int OptionSelection, char filter) {
   char key;
   _setcursortype(0);

   do {
      showopcxy_productos(*prdcts, *count, Xpos, Ypos, OptionSelection);

      do {
         key = getch();
         if (key == 0 || key == 224)
            key = getch();
      } while (key != ESC && key != ENTER && key != UP && key != DOWN);

      if (key == UP) {
         OptionSelection = (OptionSelection > 0)
                               ? OptionSelection - 1
                               : *count - 1;
      }

      if (key == DOWN) {
         OptionSelection = (OptionSelection < *count - 1)
                               ? OptionSelection + 1
                               : 0;
      }

      if (key == ENTER) {
         int real_index = (*file_indices)[OptionSelection];

         int action = show_action_menu(Xpos, Ypos + *count + 2);

         if (action == 0) {
            system("cls");
            FileModify(&(*prdcts)[OptionSelection], real_index);

         } else if (action == 1) {
            DeleteProduct(*prdcts, OptionSelection, real_index);
         }

         free(*prdcts);
         *prdcts = NULL;
         free(*file_indices);
         *file_indices = NULL;
         *count = getPrdctsFiltered(prdcts, file_indices, filter);

         if (OptionSelection >= *count)
            OptionSelection = *count - 1;
         if (OptionSelection < 0)
            OptionSelection = 0;

         system("cls");
         if (*count > 0)
            showopcxy_productos(*prdcts, *count, Xpos, Ypos, OptionSelection);
      }

   } while (key != ESC);

   _setcursortype(100);
   return OptionSelection;
}

/*
 * Funcion: get_col_widths
 * Argumentos:
 *   PRODUCT *prdcts: arreglo de productos
 *   int count: cantidad de productos
 *   int *widths: arreglo de 8 enteros donde se guardan los anchos calculados
 * Objetivo:
 *   Calcular el ancho maximo de cada columna comparando la longitud
 *   del encabezado con la longitud del valor mas largo en esa columna.
 *   Esto asegura que la tabla se ajuste dinamicamente a los datos.
 */
void get_col_widths(PRODUCT *prdcts, int count, int *widths) {
   widths[0] = strlen("Id");
   widths[1] = strlen("Nombre");
   widths[2] = strlen("SupId");
   widths[3] = strlen("CatId");
   widths[4] = strlen("Cantidad");
   widths[5] = strlen("Precio");
   widths[6] = strlen("Stock");
   widths[7] = strlen("Reorden");

   for (int i = 0; i < count; i++) {
      char buf[20];

      if (strlen(prdcts[i].ProductId) > widths[0])
         widths[0] = strlen(prdcts[i].ProductId);
      if (strlen(prdcts[i].ProductName) > widths[1])
         widths[1] = strlen(prdcts[i].ProductName);
      if (strlen(prdcts[i].SupplierId) > widths[2])
         widths[2] = strlen(prdcts[i].SupplierId);
      if (strlen(prdcts[i].CategoryId) > widths[3])
         widths[3] = strlen(prdcts[i].CategoryId);
      if (strlen(prdcts[i].QuantityPerUnit) > widths[4])
         widths[4] = strlen(prdcts[i].QuantityPerUnit);

      sprintf(buf, "%.2f", prdcts[i].UnitPrice);
      if (strlen(buf) > widths[5])
         widths[5] = strlen(buf);

      sprintf(buf, "%d", prdcts[i].UnitsInStock);
      if (strlen(buf) > widths[6])
         widths[6] = strlen(buf);

      sprintf(buf, "%d", prdcts[i].ReorderLevel);
      if (strlen(buf) > widths[7])
         widths[7] = strlen(buf);
   }
}

/*
 * Funcion: showopcxy_productos
 * Argumentos:
 *   PRODUCT *prdcts: arreglo de productos a mostrar
 *   int num: cantidad de productos
 *   int px: columna inicial de la tabla
 *   int py: fila inicial de la tabla
 *   int opcsel: indice de la fila resaltada (-1 para ninguna)
 * Objetivo:
 *   Dibujar la tabla completa con encabezados y filas. El ancho de
 *   cada columna se calcula dinamicamente con get_col_widths.
 *   La fila seleccionada se resalta con color diferente.
 */
void showopcxy_productos(PRODUCT *prdcts, int num, int px, int py, int opcsel) {
   int w[8];
   get_col_widths(prdcts, num, w);

   gotoxy(px, py);
   setcolor(TABLE_TEXTCOLOR, TABLE_BGCOLOR);
   printf("%-*s %-*s %-*s %-*s %-*s %-*s %-*s %-*s",
          w[0], "Id",
          w[1], "Nombre",
          w[2], "SupId",
          w[3], "CatId",
          w[4], "Cantidad",
          w[5], "Precio",
          w[6], "Stock",
          w[7], "Reord");

   for (int i = 0; i < num; i++) {
      gotoxy(px, py + i + 1);

      if (opcsel == i)
         setcolor(TABLE_CURSOR_TEXTCOLOR, TABLE_CURSOR_BGCOLOR);
      else
         setcolor(TABLE_TEXTCOLOR, TABLE_BGCOLOR);

      char precio[20], stock[10], reord[10];
      sprintf(precio, "%.2f", prdcts[i].UnitPrice);
      sprintf(stock, "%d", prdcts[i].UnitsInStock);
      sprintf(reord, "%d", prdcts[i].ReorderLevel);

      printf("%-*s %-*s %-*s %-*s %-*s %-*s %-*s %-*s",
             w[0], prdcts[i].ProductId,
             w[1], prdcts[i].ProductName,
             w[2], prdcts[i].SupplierId,
             w[3], prdcts[i].CategoryId,
             w[4], prdcts[i].QuantityPerUnit,
             w[5], precio,
             w[6], stock,
             w[7], reord);
   }

   colordefault();
}

/*
 * Funcion: show_header
 * Argumentos:
 *   char menu[][OPT_LENGTH]: arreglo con los nombres de las opciones
 *   int Options: cantidad de opciones
 *   int Xpos: columna donde comienza el menu
 *   int Ypos: fila donde se muestra el menu
 *   int isSelected: indice de la opcion actualmente resaltada
 * Objetivo:
 *   Dibujar el menu superior estilo vim con "CRUD" a la izquierda
 *   y las opciones a su derecha. La opcion activa se resalta.
 */
void show_header(char menu[][OPT_LENGTH], int Options, int Xpos, int Ypos, int isSelected) {
   gotoxy(Xpos, Ypos);
   setcolor(TEXT_COLOR, BG_COLOR);
   printf("CRUD");

   int col_offset = strlen("CRUD") + 2;

   for (int index = 0; index < Options; index++) {
      gotoxy(Xpos + col_offset, Ypos);

      if (isSelected == index)
         setcolor(SELECTED_TEXTCOLOR, SELECTED_BGCOLOR);
      else
         setcolor(TEXT_COLOR, BG_COLOR);

      printf("%s ", menu[index]);
      col_offset += strlen(menu[index]) + 1;
   }

   colordefault();
}

/*
 * Funcion: header_movement
 * Argumentos:
 *   char menu[][OPT_LENGTH]: arreglo con los nombres de las opciones
 *   int Options: cantidad de opciones
 *   int Xpos: columna del menu
 *   int Ypos: fila del menu
 *   int isSelected: opcion inicialmente seleccionada
 * Objetivo:
 *   Manejar la navegacion del menu superior con las flechas izquierda
 *   y derecha. Confirma con ENTER o sale con ESC hacia "Salir".
 * Retorno: Indice de la opcion seleccionada.
 */
int header_movement(char menu[][OPT_LENGTH], int Options, int Xpos, int Ypos, int isSelected) {
   char key;
   _setcursortype(0);

   do {
      show_header(menu, Options, Xpos, Ypos, isSelected);

      do {
         key = getch();
         if (key == 0 || key == 224)
            key = getch();
      } while (key != ESC && key != ENTER && key != LEFT && key != RIGHT);

      if (key == LEFT) {
         isSelected = (isSelected > 0) ? isSelected - 1 : Options - 1;
      } else if (key == RIGHT) {
         isSelected = (isSelected < Options - 1) ? isSelected + 1 : 0;
      } else if (key == ESC) {
         isSelected = EXIT;
      }

   } while (key != ESC && key != ENTER);

   _setcursortype(100);
   return isSelected;
}

/*
 * Funcion: captureFloat
 * Argumentos:
 *   int max_digits: digitos maximos antes del punto decimal
 *   int max_decimals: digitos maximos despues del punto decimal
 *   int Xpos: columna donde inicia el campo
 *   int Ypos: fila donde se muestra el campo
 * Objetivo:
 *   Capturar un numero flotante mostrando dos campos separados
 *   por un punto: parte entera y parte decimal.
 * Retorno: El valor flotante capturado.
 */
float captureFloat(int max_digits, int max_decimals, int Xpos, int Ypos) {
   char int_str[max_digits + 1];
   char dec_str[max_decimals + 1];
   int int_value = 0, dec_value = 0;

   memset(int_str, ' ', max_digits);
   int_str[max_digits] = '\0';
   memset(dec_str, ' ', max_decimals);
   dec_str[max_decimals] = '\0';

   CaptureInteger(int_str, &int_value, max_digits, Xpos, Ypos);

   gotoxy(Xpos + max_digits + 1, Ypos);
   printf(".");

   CaptureFloat(dec_str, &dec_value, max_decimals, Xpos + max_digits + 2, Ypos);

   return int_value + (dec_value / pow(10.0, strlen(dec_str)));
}

/*
 * Funcion: CaptureInteger
 * Argumentos:
 *   char *digit_str: buffer temporal donde se almacena el numero como texto
 *   int *result_value: puntero donde se guardara el valor entero convertido
 *   int max_digits: cantidad maxima de digitos permitidos
 *   int pos_x: columna donde se muestra el campo
 *   int pos_y: fila donde se muestra el campo
 * Objetivo:
 *   Capturar solo digitos numericos (0-9) del usuario con soporte
 *   para moverse con flechas y borrar con backspace.
 *   Al confirmar con ENTER convierte el texto a entero con atoi.
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
 * Funcion: CaptureFloat
 * Argumentos: (iguales a CaptureInteger)
 * Objetivo:
 *   Alias de CaptureInteger usado especificamente para capturar
 *   la parte decimal de un numero flotante.
 */
void CaptureFloat(char *decimal_str, int *result_value, int max_digits, int pos_x, int pos_y) {
   CaptureInteger(decimal_str, result_value, max_digits, pos_x, pos_y);
}

/*
 * Funcion: captureAlphanumeric
 * Argumentos:
 *   char *result_str: buffer donde se almacenara el texto capturado
 *   int max_length: longitud maxima permitida del campo
 *   int Xpos: columna donde se muestra el campo
 *   int Ypos: fila donde se muestra el campo
 *   int (*validator)(char): puntero a funcion validadora del caracter
 * Objetivo:
 *   Capturar texto del usuario usando la funcion validadora para
 *   filtrar que caracteres son aceptados. Soporta flechas, backspace,
 *   ENTER para confirmar y ESC para cancelar.
 */
void captureAlphanumeric(char *result_str, int max_length, int Xpos, int Ypos,
                         int (*validator)(char)) {
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
               pressed_key != LEFT &&
               pressed_key != BACKSPACE &&
               pressed_key != RIGHT);

      if (pressed_key == RIGHT) {
         if (cursor_index < max_length - 1)
            cursor_index++;
      } else if (pressed_key == LEFT) {
         if (cursor_index > 0)
            cursor_index--;
      } else if (pressed_key != ENTER && pressed_key != ESC) {
         if (pressed_key == BACKSPACE) {
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
 * Funcion: show_field
 * Argumentos:
 *   char *field_str: contenido actual del campo
 *   int cursor_position: posicion del cursor dentro del campo (0 a max_length-1)
 *   int max_length: ancho visual del campo
 *   int pos_x: columna donde inicia el campo
 *   int pos_y: fila donde se muestra el campo
 * Objetivo:
 *   Dibujar el campo de entrada mostrando su contenido actual y
 *   posicionar el cursor del sistema en la posicion correcta para
 *   que el usuario vea donde esta escribiendo.
 */
void show_field(char *field_str, int cursor_position, int max_length, int pos_x, int pos_y) {
   setcolor(TEXT_COLOR, BG_COLOR);

   for (int index = 0; index < max_length; index++) {
      gotoxy(pos_x + index + 1, pos_y);
      printf("%c", *(field_str + index) ? *(field_str + index) : ' ');
   }

   colordefault();
   gotoxy(pos_x + cursor_position + 1, pos_y);
}

/*
 * Funcion: validate_alpha_char
 * Argumentos:
 *   char _char: caracter a validar
 * Objetivo:
 *   Validar campos que solo aceptan letras, espacios y guion.
 *   Usado para: ProductName.
 * Retorno: 1 si el caracter es valido, 0 si no lo es.
 */
int validate_alpha_char(char _char) {
   return (isalpha(_char) || _char == ' ' || _char == '-');
}

/*
 * Funcion: validate_alphanumeric_char
 * Argumentos:
 *   char _char: caracter a validar
 * Objetivo:
 *   Validar campos que aceptan letras, numeros, espacios y guion.
 *   Usado para: SupplierId, CategoryId, QuantityPerUnit.
 * Retorno: 1 si el caracter es valido, 0 si no lo es.
 */
int validate_alphanumeric_char(char _char) {
   return (isalpha(_char) || isdigit(_char) || _char == ' ' || _char == '-');
}

/*
 * Funcion: setcolor
 * Argumentos:
 *   int text_color: color del texto (constantes de conio.c)
 *   int background_color: color del fondo
 * Objetivo:
 *   Cambiar los colores de texto y fondo para la siguiente salida.
 */
void setcolor(int text_color, int background_color) {
   textcolor(text_color);
   textbackground(background_color);
}

/*
 * Funcion: colordefault
 * Objetivo:
 *   Restaurar los colores por defecto de la consola
 *   (texto gris claro sobre fondo negro).
 */
void colordefault(void) {
   setcolor(LIGHTGRAY, BLACK);
}

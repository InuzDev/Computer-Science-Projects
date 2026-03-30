#include <conio.c>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHA 224
#define __UNDEFINED 0

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

#define MAXID 6
#define LENNOMB 21
#define LENAPEL 16
#define LENDESC 26
#define LENDIR 31
#define LENPWD 21

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
   char *header;
   void (*getfield)(void *data, char *buf);
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

void Cleanup(GenericList *list, FILE *file);

void setcolor(int ct, int cf);
void colordefault();

void show_header(char menu[][OPT_LENGTH], int options, int xpos, int ypos, int isSelected);
int show_dropdown(char items[][OPT_LENGTH], int count, int xpos, int ypos);

int get_option_xpos(char menu[][OPT_LENGTH], int targetIndex, int startX);

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
   char process_dropdown[PROCESS_OPTS][OPT_LENGTH] = {"Precio Art.", "Facturar", "Pagar"};
   char consult_dropdown[CONSULT_OPTS][OPT_LENGTH] = {"Fact. Pend", "Pagos", "Balance"};

   int SelectedOption = 0;
   int subOption;
   int key;

   show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);

   do {
      key = getch();
      if (key == __UNDEFINED || key == ALPHA) {
         key = getch();
      }

      if (key == LEFT && SelectedOption > 0) {
         SelectedOption--;
         show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
      }

      if (key == RIGHT && SelectedOption < OPTS - 1) {
         SelectedOption++;
         show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
      }

      if (key == ENTER) {
         switch (SelectedOption) {
         case 0:
            subOption = show_dropdown(crud_dropdown, CRUD_OPTS, get_option_xpos(menu, 0, INIT_X_POS), INIT_Y_POS + 1);
            system("cls");
            show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
            if (subOption == 0) { /* Cliente */
            }
            if (subOption == 1) { /* Usuario */
            }
            if (subOption == 2) { /* Proveedor */
            }
            if (subOption == 3) { /* Articulo */
            }
            break;

         case 1:
            subOption = show_dropdown(process_dropdown, PROCESS_OPTS, get_option_xpos(menu, 1, INIT_X_POS), INIT_Y_POS + 1);
            system("cls");
            show_header(menu, OPTS, INIT_X_POS, INIT_Y_POS, SelectedOption);
            if (subOption == 0) { /* Precio Articulo */
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

      if (key == UP && isSelected > 0) {
         isSelected--;
      } else if (key == DOWN && isSelected < count - 1) {
         isSelected++;
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
   setcolor(CT, CF);
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

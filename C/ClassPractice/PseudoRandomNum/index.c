#include <stdio.h>
#include <stdlib.h>
#include <conio.c>
#include <time.h>

#define CANTNUM 10
#define LIMMIN 1
#define LIMMAX 50

#define CT BLUE
#define CF LIGHTGRAY

#define KEY_PREFIX_1 0
#define KEY_PREFIX_2 -32

#define CURSOR_100 100
#define CURSOR_0 0

#define CTS BLUE
#define CFS YELLOW

#define CTM YELLOW
#define CFM GREEN

#define NOMARK -1

#define POSXINI 10
#define POSYINI 5

#define ENTER 13
#define ESC 27

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

int randrange(int, int);
void genlist(int[], int, int, int);
void showlist(int[], int, int, int, int, int);
void setcolor(int, int);
int order(int[], int);
void movelist(int[], int, int, int);
void colordefault(void);

int main()
{
   srand(time(NULL));

   int list[CANTNUM];
   genlist(list, CANTNUM, LIMMIN, LIMMAX);
   movelist(list, CANTNUM, POSXINI, POSYINI);

   return 0;
}

/**
 * Funcion: randrange
 * Objetivo: Generar los valores aleatorios para la lista
 * Argumentos: (int) liminf, limsup
 * retorna: numeros aleatorios a base de la semilla utilizando la funcion rand();
 */
int randrange(int liminf, int limsup)
{
   return rand() % (limsup - liminf + 1) + liminf;
}

/**
 * Funcion: genlist
 * Objetivo: Ingresar los valores en el arreglo de la lista.
 * Argumentos: (int) list[], cantnum, limmin, limmax
 */
void genlist(int list[], int cantnum, int limmin, int limmax)
{
   for (int index = 0; index < CANTNUM; index++)
   {
      list[index] = randrange(limmin, limmax);
   }
}

/**
 * Funcion: showlist
 * Objetivo: imprimir la lista como interfaz de usuario
 * Argumentos: (int) list[], cantnum, mark, select, posx, posy
 */
void showlist(int list[], int cantnum, int mark, int select, int posx, int posy)
{
   for (int index = 0; index < cantnum; index++)
   {
      if (index == select && index == mark)
      {
         setcolor(CTM, CFM);
      }
      else if (index == select)
      {
         setcolor(CTS, CFS);
      }
      else if (index == mark)
      {
         setcolor(CTM, CFM);
      }
      else
      {
         setcolor(CT, CF);
      }

      gotoxy(posx, posy + index);
      printf("%2d", list[index]);
   }
   colordefault();
}

/**
 * Funcion moveList
 * Objetivo: Darle la "habilidad" al usuario de moverse a traves de la
 *           interfaz de usuario y poder organizar la lista.
 * Argumentos: (int) list[], cantnum, posx, posy
 */
void movelist(int list[], int cantnum, int posx, int posy)
{
   int possel = 0, posmark = NOMARK, temp;
   int newpos = 0;
   char key = 0;

   _setcursortype(CURSOR_0);
   showlist(list, cantnum, posmark, possel, posx, posy);

   do
   {
      key = getch();
      if (key == KEY_PREFIX_1 || key == KEY_PREFIX_2)
         key = getch();

      if (key == UP)
      {
         if (posmark == NOMARK)
         {
            possel = (possel == 0) ? cantnum - 1 : possel - 1;
         }
         else
         {
            newpos = (posmark == 0) ? cantnum - 1 : posmark - 1;
            temp = list[posmark];

            list[posmark] = list[newpos];
            list[newpos] = temp;

            posmark = newpos;
            possel = posmark;
         }
      }

      if (key == DOWN)
      {
         if (posmark == NOMARK)
         {
            possel = (possel == cantnum - 1) ? 0 : possel + 1;
         }
         else
         {
            newpos = (posmark == cantnum - 1) ? 0 : posmark + 1;
            temp = list[posmark];

            list[posmark] = list[newpos];
            list[newpos] = temp;

            posmark = newpos;
            possel = posmark;
         }
      }

      if (key == ENTER)
      {
         if (posmark == NOMARK)
         {
            posmark = possel;
         }
         else
         {
            temp = list[posmark];

            list[posmark] = list[posmark];
            list[posmark] = temp;

            posmark = NOMARK;

            if (order(list, cantnum))
            {
               setcolor(CT, CF);
               showlist(list, cantnum, posmark, possel, posx, posy);
               gotoxy(posx, posy + cantnum + 2);
               printf("Felicidades! Has ordenado la lista!");

               key = ESC;
            }
         }
      }

      showlist(list, cantnum, posmark, possel, posx, posy);

   } while (key != ESC);

   _setcursortype(CURSOR_100);
}

/**
 * Funcion: order
 * Objetivo: Detectar si la lista a sido ordenada
 * Argumentos: (int) list[], cantnum
 * retorna: 1, si no ha sido ordenada, 0 si ha sido ordenada.
 */
int order(int list[], int cantnum)
{
   for (int index = 0; index < CANTNUM - 1; index++)
   {
      if (list[index] > list[index + 1])
      {
         return 0;
      }
   }
   return 1;
}

/**
 * Funcion: setcolor
 * Objetivo: Ponerle un color distinto al numero seleccionado de la lista.
 * Argumentos: (int) ct, cf
 */
void setcolor(int ct, int cf)
{
   textcolor(ct);
   textbackground(cf);
}

/**
 * Funcion: colordefault
 * Objetivo: Definir un color por defecto para todos los elementos que no sean parte de la lista.
 */
void colordefault(void)
{
   setcolor(LIGHTGRAY, BLACK);
}
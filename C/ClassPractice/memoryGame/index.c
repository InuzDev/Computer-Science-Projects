#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.c>

#define DIM 6
#define VALINI 0
#define ESP 3
#define MAXCHRS 27

#define CT BLUE
#define CF LIGHTGRAY

#define CTS BLUE
#define CFS YELLOW

#define CTM YELLOW
#define CFM GREEN

#define POSXINI 10
#define POSYINI 5

#define ENTER 13
#define ESC 27

#define ARRIBA 72
#define ABAJO 80
#define DERECHA 77
#define IZQUIERDA 75

void genmat(int cantfil, int cantcol, int mat[cantfil][cantcol], char figuras[]);
void showmatxy(int cantfil, int cantcol, int mat[cantfil][cantcol], int filsel, int colsel, int posx, int posy);
void movematxy(int cantfil, int cantcol, int mat[cantfil][cantcol], int posx, int posy);
void setcolor(int, int);
void colordefault(void);
int randrange(int, int);

int main()
{

   srand(time(NULL));

   char figuras[MAXCHRS] = {64, 128, 142, 143, 146, 152, 153, 154, 155, 156, 157, 158, 159, 224, 225, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238};
   int mat[DIM][DIM];

   /* for ( int val = 1; val <= 255; val++ )
       printf("%d - %c\n",val,val);

    printf("%s",figuras);
    getch();*/

   genmat(DIM, DIM, mat, figuras);
   system("cls");
   printf("Valores capturados:\n");
   movematxy(DIM, DIM, mat, POSXINI, POSYINI);
   return 0;
}

void initmat(int cantfil, int cantcol, int mat[cantfil][cantcol], int valini)
{
   for (int indfil = 0; indfil < cantfil; indfil++)
      for (int indcol = 0; indcol < cantcol; indcol++)
         mat[indfil][indcol] = valini;

   return;
}

void genmat(int cantfil, int cantcol, int mat[cantfil][cantcol], char figuras[])
{
   int indfiguras, indfilasign, indcolasign, cantasign = 0;
   char chr;

   initmat(cantfil, cantcol, mat, VALINI);

   for (int indfil = 0; indfil < cantfil; indfil++)
      for (int indcol = 0; indcol < cantcol; indcol++)
      {
         do
         {
            indfiguras = randrange(0, MAXCHRS - 1);

         } while (figuras[indfiguras] == VALINI);

         chr = figuras[indfiguras];
         if (mat[indfil][indcol] == VALINI)
         {
            mat[indfil][indcol] = chr;
            do
            {
               indfilasign = randrange(0, cantfil);
               indcolasign = randrange(0, cantcol);
            } while (mat[indfilasign][indcolasign] != VALINI);
            mat[indfilasign][indcolasign] = chr;
            figuras[indfiguras] = VALINI;
            cantasign++;
            if (cantasign == (cantfil * cantcol / 2))
               break;
         }
      }

   return;
}

void showmatxy(int cantfil, int cantcol, int mat[cantfil][cantcol], int filsel, int colsel, int posx, int posy)
{
   for (int indfil = 0; indfil < cantfil; indfil++)
   {
      for (int indcol = 0; indcol < cantcol; indcol++)
      {
         gotoxy(posx + indcol * ESP, posy + indfil);
         setcolor(CT, CF);
         if (indfil == filsel && indcol == colsel)
            setcolor(CTS, CFS);

         printf(" %c ", mat[indfil][indcol]);
      }
   }
   colordefault();
   return;
}

void movematxy(int cantfil, int cantcol, int mat[cantfil][cantcol], int posx, int posy)
{
   int filsel = 0, colsel = 0;
   int tecla;

   _setcursortype(0);
   do
   {
      showmatxy(cantfil, cantcol, mat, filsel, colsel, posx, posy);
      do
      {
         tecla = getch();
      } while (tecla != ENTER && tecla != ESC && tecla != ARRIBA && tecla != ABAJO &&
               tecla != DERECHA && tecla != IZQUIERDA);

      if (tecla == ARRIBA)
      {
         filsel--;
         if (filsel < 0)
            filsel = cantfil - 1;
      }
      if (tecla == DERECHA)
      {
         colsel++;
         if (colsel == cantcol)
            colsel = 0;
      }
      if (tecla == ABAJO)
      {
         filsel++;
         if (filsel == cantfil)
            filsel = 0;
      }
      if (tecla == IZQUIERDA)
      {
         colsel--;
         if (colsel < 0)
            colsel = cantcol - 1;
      }

   } while (tecla != ESC);
   _setcursortype(100);
}

int randrange(int liminf, int limsup)
{
   return rand() % (limsup - liminf + 1) + liminf;
}

void setcolor(int ct, int cf)
{
   textcolor(ct);
   textbackground(cf);
}

void colordefault(void)
{
   setcolor(LIGHTGRAY, BLACK);
}

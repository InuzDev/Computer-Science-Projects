/**
 * Objective: A modular program that ask the dimension of a square matrix, maximum being 15
 *            and minimum is 3, Generate the matrix with random numbers between two values indicated by the user
 *            and print an expanded matrix, indicating the sumatory and average of the values in each column and row,
 *            total sumatory of the values from the matrix, average of all values, minimum and maximum value in the matrix
 *
 * ! Important note: ! do NOT modify the original matrix, or making a new one. JUST print the indicated values
 *
 * Programa para practicar matrices
 *
 * InuDev ; 16 - 11 - 2025
 *
 * Archivo: main.c
 */
#include <conio.c>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_DIM 3
#define MAX_DIM 15

#define MAX_VALUE 9999

#define POSXINI 10
#define POSYINI 5

#define CT BLUE
#define CF LIGHTGRAY

#define CTS BLUE
#define CFS YELLOW

#define CTM YELLOW
#define CFM GREEN

#define ESP 6

#define ENTER 13
#define ESC 27

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

#define NOMARK -1

#define KEY_PREFIX_1 0
#define KEY_PREFIX_2 224

#define CURSOR_0 0
#define CURSOR_100 100

void genMatrix(int MinRange, int MaxRange, int dimensions, int Matrix[dimensions][dimensions]);
void showMatrix(int dimensions, int Matrix[dimensions][dimensions], int RowMark, int ColMark, int RowSel, int ColSel, int posX, int posY);
void drawCell(int dimensions, int Matrix[dimensions][dimensions], int RowMark, int ColMark, int RowSel, int ColSel, int row, int col, int posX, int posY);
void moveMatrix(int dimensions, int Matrix[dimensions][dimensions], int posX, int posY);
int maxMatrix(int dimensions, int Matrix[dimensions][dimensions]);

void colordefault(void);
void setcolor(int, int);

int randrange(int, int);

int main()
{
   srand(time(NULL));
   int dimensions = 0, MinRange = -1, MaxRange = -1;

   while (1)
   {
      printf("Introduzca las dimensiones de la matriz: ");
      scanf("%d", &dimensions);
      if (dimensions > MAX_DIM || dimensions < MIN_DIM)
      {
         printf("Valor introducido invalido. Por, favor. Introduzca un valor entre [%d, %d]\n\n", MIN_DIM, MAX_DIM);
         continue;
      }
      break;
   }

   int Matrix[dimensions][dimensions];

   while (1)
   {
      printf("Ingresa el rango inferior para generar los numeros aleatorios de la matriz: ");
      scanf("%d", &MinRange);
      if (MinRange < 0)
      {
         printf("Por, favor. Ingresar un valor mayor o igual a 0\n\n");
         continue;
      }
      break;
   }

   while (1)
   {
      printf("Ingresa el rango superior para generar los numeros aleatorios de la matriz: ");
      scanf("%d", &MaxRange);
      if (MaxRange < MinRange || MaxRange > MAX_VALUE)
      {
         printf("Por, favor. Ingresar un valor mayor al rango minimo.\n\n");
         continue;
      }
      break;
   }

   system("cls");

   genMatrix(MinRange, MaxRange, dimensions, Matrix);
   moveMatrix(dimensions, Matrix, POSXINI, POSYINI);

   int maximum = maxMatrix(dimensions, Matrix);

   return 0;
}

/**
 * Funcion: randrange
 * Objetivo: Generar los valores aleatorios para la matriz
 * Argumentos: MinRange, MaxRange
 * Retorna: valores aleatorios para la matrix a base de la semilla que utiliza el tiempo. (Cambia cada segundo)
 */
int randrange(int MinRange, int MaxRange)
{
   return rand() % (MaxRange - MinRange + 1) + MinRange;
}

/**
 * Funcion: maxMatrix
 * Objetivo: Encontrar el maximo valor en una matriz
 * Argumentos: (int) dimensions, posX, posY Matriz[dimensions][dimensions]
 * retorna: El maximo valor de la matriz.
 */
int maxMatrix(int dimensions, int Matrix[dimensions][dimensions])
{
   int maxVal = Matrix[0][0];

   for (int rowIndex = 0; rowIndex < dimensions; rowIndex++)
   {
      for (int colIndex = 0; colIndex < dimensions; colIndex++)
      {
         if (maxVal < Matrix[rowIndex][colIndex])
         {
            maxVal = Matrix[rowIndex][colIndex];
         }
      }
   }

   return maxVal;
}

/**
 * Funcion: GenMatrix
 * Objetivo: Generar la matriz con los valores aleatorios utilizando la funcion randrange
 * Argumentos: (int) MinRange, MaxRange, dimensions, Matrix[dimensions][dimensions]
 */
void genMatrix(int MinRange, int MaxRange, int dimensions, int Matrix[dimensions][dimensions])
{
   for (int rowIndex = 0; rowIndex < dimensions; rowIndex++)
   {
      for (int colIndex = 0; colIndex < dimensions; colIndex++)
      {
         Matrix[rowIndex][colIndex] = randrange(MinRange, MaxRange);
      }
   }
}

/**
 * Funcion drawCell
 * Objetivo: Dibujar la tabla cada vez que se mueve el usuario evitando bugs de grafica.
 * Argumentos: (int) dimensions, Matrix[dimensions][dimensions] RowMark ColMark, RowSel, ColSel, row, col, posX, posY
 */
void drawCell(int dimensions, int Matrix[dimensions][dimensions], int RowMark, int ColMark, int RowSel, int ColSel, int row, int col, int posX, int posY)
{
   int isSelected = (row == RowSel && col == ColSel);
   int isMarked = (row == RowMark || col == ColMark);

   gotoxy(posX + col * ESP, posY + row);
   if (isSelected)
      setcolor(CTS, CFS);
   else if (isMarked)
      setcolor(CTM, CFM);
   else
      setcolor(CT, CF);

   printf("%4d", Matrix[row][col]);
   colordefault();
}

/**
 * funcion: ShowMatrix
 * Objetivo: Mostrar la matriz
 * Argumentos: (int) dimension,s Matrix[dimensions][dimensions], RowSel, posX, ColSel, posY
 */
void showMatrix(int dimensions, int Matrix[dimensions][dimensions], int RowMark, int ColMark, int RowSel, int ColSel, int posX, int posY)
{
   int rowSum[dimensions];
   float rowAvg[dimensions];
   int colSum[dimensions];
   float colAvg[dimensions];
   int totalSum = 0;
   int minVal = Matrix[0][0];
   int maxVal = Matrix[0][0];

   for (int col = 0; col < dimensions; col++)
      colSum[col] = 0;

   for (int row = 0; row < dimensions; row++)
   {
      int rowTotal = 0;
      for (int col = 0; col < dimensions; col++)
      {
         int cellValue = Matrix[row][col];
         rowTotal += cellValue;
         colSum[col] += cellValue;
         if (cellValue < minVal)
            minVal = cellValue;
         if (cellValue > maxVal)
            maxVal = cellValue;
      }
      rowSum[row] = rowTotal;
      rowAvg[row] = (float)rowTotal / (float)dimensions;
      totalSum += rowTotal;
   }

   for (int col = 0; col < dimensions; col++)
      colAvg[col] = (float)colSum[col] / (float)dimensions;

   float overallAvg = (float)totalSum / (float)(dimensions * dimensions);

   /* Column headers */
   gotoxy(posX - ESP, posY - 2);
   printf("     ");
   for (int col = 0; col < dimensions; col++)
      printf("%*d", ESP, col + 1);

   printf("%*s%*s", ESP, "", ESP, "");

   for (int row = 0; row < dimensions; row++)
   {
      gotoxy(posX - ESP, posY + row);
      printf("R%02d", row + 1);

      for (int col = 0; col < dimensions; col++)
      {
         drawCell(dimensions, Matrix, RowMark, ColMark, RowSel, ColSel, row, col, posX, posY);
      }

      gotoxy(posX + dimensions * ESP + 2, posY + row);
      setcolor(CTM, CFM);
      printf(" %6d %6.2f", rowSum[row], rowAvg[row]);
      colordefault();
   }

   gotoxy(posX - ESP, posY + dimensions + 0);
   printf("SUM");
   for (int col = 0; col < dimensions; col++)
   {
      gotoxy(posX + col * ESP, posY + dimensions + 0);
      setcolor(CTM, CFM);
      printf("%6d", colSum[col]);
      colordefault();
   }

   gotoxy(posX - ESP, posY + dimensions + 1);
   printf("AVG");
   for (int col = 0; col < dimensions; col++)
   {
      gotoxy(posX + col * ESP, posY + dimensions + 1);
      setcolor(CTM, CFM);
      printf("%6.2f", colAvg[col]);
      colordefault();
   }

   gotoxy(posX + dimensions * ESP + 2, posY + dimensions + 0);
   setcolor(CTS, CFS);
   printf("Total:%8d", totalSum);
   gotoxy(posX + dimensions * ESP + 2, posY + dimensions + 1);
   printf("Overall Avg:%6.2f", overallAvg);
   gotoxy(posX + dimensions * ESP + 2, posY + dimensions + 2);
   printf("Min:%6d  Max:%6d", minVal, maxVal);
   colordefault();
}

/**
 * Funcion: MoveMatrix
 * Objetivo: Permitir al usaurio moverse en la matriz
 * Argumentos: (int) dimensions, Matrix[dimensions][dimensions], posX, posY
 */
void moveMatrix(int dimensions, int Matrix[dimensions][dimensions], int posX, int posY)
{
   int RowSel = 0;
   int ColSel = 0;
   int RowMark = NOMARK;
   int ColMark = NOMARK;
   int newRow = 0;
   int newCol = 0;
   int swapTemp = 0;
   int inputKey = 0;

   _setcursortype(CURSOR_0);

   showMatrix(dimensions, Matrix, RowMark, ColMark, RowSel, ColSel, posX, posY);

   int prevRow = RowSel, prevCol = ColSel;

   do
   {
      inputKey = getch();
      if (inputKey == KEY_PREFIX_1 || inputKey == KEY_PREFIX_2 || inputKey == 224)
         inputKey = getch();

      int fullRedraw = 0;

      if (inputKey == UP)
      {
         if (RowMark == NOMARK)
         {
            RowSel = (RowSel == 0) ? dimensions - 1 : RowSel - 1;
         }
         else
         {
            newRow = (RowMark == 0) ? dimensions - 1 : RowMark - 1;
            for (int colIndex = 0; colIndex < dimensions; colIndex++)
            {
               swapTemp = Matrix[RowMark][colIndex];
               Matrix[RowMark][colIndex] = Matrix[newRow][colIndex];
               Matrix[newRow][colIndex] = swapTemp;
            }
            RowMark = newRow;
            RowSel = RowMark;
            fullRedraw = 1;
         }
      }
      else if (inputKey == DOWN)
      {
         if (RowMark == NOMARK)
         {
            RowSel = (RowSel == dimensions - 1) ? 0 : RowSel + 1;
         }
         else
         {
            newRow = (RowMark == dimensions - 1) ? 0 : RowMark + 1;
            for (int colIndex = 0; colIndex < dimensions; colIndex++)
            {
               swapTemp = Matrix[RowMark][colIndex];
               Matrix[RowMark][colIndex] = Matrix[newRow][colIndex];
               Matrix[newRow][colIndex] = swapTemp;
            }
            RowMark = newRow;
            RowSel = RowMark;
            fullRedraw = 1;
         }
      }
      else if (inputKey == LEFT)
      {
         if (ColMark == NOMARK)
         {
            ColSel = (ColSel == 0) ? dimensions - 1 : ColSel - 1;
         }
         else
         {
            newCol = (ColMark == 0) ? dimensions - 1 : ColMark - 1;
            for (int rowIndex = 0; rowIndex < dimensions; rowIndex++)
            {
               swapTemp = Matrix[rowIndex][ColMark];
               Matrix[rowIndex][ColMark] = Matrix[rowIndex][newCol];
               Matrix[rowIndex][newCol] = swapTemp;
            }
            ColMark = newCol;
            ColSel = ColMark;
            fullRedraw = 1;
         }
      }
      else if (inputKey == RIGHT)
      {
         if (ColMark == NOMARK)
         {
            ColSel = (ColSel == dimensions - 1) ? 0 : ColSel + 1;
         }
         else
         {
            newCol = (ColMark == dimensions - 1) ? 0 : ColMark + 1;
            for (int rowIndex = 0; rowIndex < dimensions; rowIndex++)
            {
               swapTemp = Matrix[rowIndex][ColMark];
               Matrix[rowIndex][ColMark] = Matrix[rowIndex][newCol];
               Matrix[rowIndex][newCol] = swapTemp;
            }
            ColMark = newCol;
            ColSel = ColMark;
            fullRedraw = 1;
         }
      }
      else if (inputKey == ENTER)
      {
         int newValue = Matrix[RowSel][ColSel];
         gotoxy(posX, posY + dimensions + 4);
         printf("Ingrese nuevo valor para R%dC%d: ", RowSel + 1, ColSel + 1);
         fflush(stdout);
         if (scanf("%d", &newValue) == 1)
         {
            Matrix[RowSel][ColSel] = newValue;
            fullRedraw = 1;
         }
         gotoxy(posX, posY + dimensions + 4);
         printf("%*s", 60, "");
         gotoxy(posX, posY + dimensions + 4);
      }

      if (fullRedraw)
      {
         showMatrix(dimensions, Matrix, RowMark, ColMark, RowSel, ColSel, posX, posY);
         prevRow = RowSel;
         prevCol = ColSel;
      }
      else
      {
         if (prevRow != RowSel || prevCol != ColSel)
         {
            drawCell(dimensions, Matrix, RowMark, ColMark, RowSel, ColSel, prevRow, prevCol, posX, posY);
            drawCell(dimensions, Matrix, RowMark, ColMark, RowSel, ColSel, RowSel, ColSel, posX, posY);
            prevRow = RowSel;
            prevCol = ColSel;
         }
      }

   } while (inputKey != ESC);

   _setcursortype(CURSOR_100);
}

/**
 * Funcion: setcolor
 * Argumentos: (int) ct, cf
 */
void setcolor(int ct, int cf)
{
   textcolor(ct);
   textbackground(cf);
}

/**
 * Funcion: colordefault
 * Objetivo: poner el color que esta por defecto en la terminal
 */
void colordefault(void)
{
   setcolor(LIGHTGRAY, BLACK);
}

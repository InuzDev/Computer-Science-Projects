/**
 * index.c - Carlos David Jorge Taveras
 *
 * Sudoku - Trabajo final de Introduccion a la algoritmia
 *
 * 11/28/2025 - 12/2/2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.c>
#include <unistd.h>

#define DIMENSION 9
#define GENERATION_DIMENSION 3
#define START_POSITION 2

#define DEFAULT_PLAYER_VIEW " . "

#define MAX_NUM_SUDOKU 9
#define MIN_NUM_SUDOKU 1

#define DIFFICULTY_EASY 1
#define DIFFICULTY_MEDIUM 2
#define DIFFICULTY_HARD 3

#define COLOR2 MAGENTA
#define COLOR1 LIGHTGRAY
#define COLOR3 BLACK
#define COLOR4 CYAN
#define COLOR5 GREEN
#define COLOR6 YELLOW
#define COLOR7 BLUE
#define COLOR8 RED
#define COLOR9 LIGHTMAGENTA
#define COLOR10 LIGHTCYAN

#define DIFFICULT_MAX_ERROR_EASY 10
#define DIFFICULT_MAX_ERROR_MEDIUM 5
#define DIFFICULT_MAX_ERROR_HARD 3

#define MIN_RANDRANGE 1
#define MAX_RANDRANGE 9

#define ENTER 13
#define ESC 27

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

#define CURSOR_0 0
#define CURSOR_100 100

int randNumGen(int MinRange, int MaxRange);
void drawStatus(int leftX, int topY, int Errors, int MaxErrors, int selectionMode);
void showTransientMsg(int leftX, int topY, int msgId);
void genSubMatrix(int Sudoku[GENERATION_DIMENSION][GENERATION_DIMENSION]);
void placeSubMatrix(int Sudoku[DIMENSION][DIMENSION], int SubMatrix[GENERATION_DIMENSION][GENERATION_DIMENSION], int startRow, int startCol);
void buildPlayerGrid(int PlayerGrid[DIMENSION][DIMENSION], int Sudoku[DIMENSION][DIMENSION], int GridOverlay[DIMENSION][DIMENSION]);
void FillSudokuWithSubMatrix(int Sudoku[DIMENSION][DIMENSION]);
void generateBlockMask(int SubMatrix[DIMENSION][DIMENSION]);
void drawCellStyle(int row, int col, int cursorRow, int cursorCol, int isGiven, int displayValue);
void PlayerMovement(int sudoku[DIMENSION][DIMENSION], int PlayerMatrix[DIMENSION][DIMENSION], int SubMatrix[DIMENSION][DIMENSION], int MaxErrors);
void ShowPlayerMatrix(int PlayerMatrix[DIMENSION][DIMENSION], int cursorRow, int cursorCol, int selectionMode, int candidate, int givens[DIMENSION][DIMENSION]);
void drawCell(int PlayerMatrix[DIMENSION][DIMENSION], int row, int col, int cursorRow, int cursorCol, int selectionMode, int candidate, int givens[DIMENSION][DIMENSION]);
void drawCandidateCell(int PlayerMatrix[DIMENSION][DIMENSION], int row, int Col, int cursorRow, int cursorCol, int selectionMode, int candidate, int givens[DIMENSION][DIMENSION]);

int isValidSudoku(int Sudoku[DIMENSION][DIMENSION]);
int IsCellSafe(int grid[DIMENSION][DIMENSION], int row, int col, int num);
int CheckSudokuSolutionStatus(int grid[DIMENSION][DIMENSION], int row, int col);

void colordefault(void);
void setcolor(int ct, int cf);

int main()
{
   srand(time(NULL));
   int Sudoku[DIMENSION][DIMENSION] = {0};
   int CheckerMatrix[DIMENSION][DIMENSION] = {0};
   int PlayerMatrix[DIMENSION][DIMENSION];
   int Difficulty = 0;
   int MaxErrors = 0;

   FillSudokuWithSubMatrix(Sudoku);

   while (Difficulty == 0 || Difficulty > DIFFICULTY_HARD)
   {
      printf("Ingrese el nivel de dificultad:\n\n \tFacil: 1\n \tIntermedio: 2\n \tDificil: 3\n");
      scanf("%d", &Difficulty);
      if (Difficulty == 0 || Difficulty > DIFFICULTY_HARD)
      {
         printf("Por, favor. Ingrese un valid valido.\n");
      }
   }

   system("cls");

   generateBlockMask(CheckerMatrix);
   buildPlayerGrid(PlayerMatrix, Sudoku, CheckerMatrix);

   if (Difficulty == DIFFICULTY_EASY)
   {
      MaxErrors = DIFFICULT_MAX_ERROR_EASY;
   }
   else if (Difficulty == DIFFICULTY_MEDIUM)
   {
      MaxErrors = DIFFICULT_MAX_ERROR_MEDIUM;
   }
   else if (Difficulty == DIFFICULTY_HARD)
   {
      MaxErrors = DIFFICULT_MAX_ERROR_HARD;
   }

   PlayerMovement(Sudoku, PlayerMatrix, CheckerMatrix, MaxErrors);

   return 0;
}

/**
 * Funcion: randNumGen
 * Argumentos: (int) MinRange, MaxRange
 * Objetivo: Obtener un numero aleatorio uniforme en el rango [MinRange, MaxRange]
 * Retorna: Un entero aleatorio dentro del rango especificado.
 */
int randNumGen(int MinRange, int MaxRange)
{
   int range = MaxRange - MinRange + 1;
   int limit = RAND_MAX - (RAND_MAX % range);
   int row;
   do
   {
      row = rand();
   } while (row >= limit);
   return MinRange + (row % range);
}

/**
 * Funcion: genSubMatrix
 * Argumentos: (int[][]) Sudoku
 * Objetivo: Rellenar una submatriz 3x3 con numeros aleatorios entre 1 y 9.
 */
void genSubMatrix(int Sudoku[GENERATION_DIMENSION][GENERATION_DIMENSION])
{
   for (int Xindex = 0; Xindex < GENERATION_DIMENSION; Xindex++)
   {
      for (int Yindex = 0; Yindex < GENERATION_DIMENSION; Yindex++)
      {
         Sudoku[Xindex][Yindex] = randNumGen(MIN_RANDRANGE, MAX_RANDRANGE);
      }
   }
}

/**
 * Funcion: placeSubMatrix
 * Argumentos: (int[][]) Sudoku
 *             (int) startRow, startCol
 * Objetivo: Copiar los contenidos de una submatriz 3x3 dentro de la matriz 9x9
 *           en la posicion indicada.
 */
void placeSubMatrix(int Sudoku[DIMENSION][DIMENSION], int SubMatrix[GENERATION_DIMENSION][GENERATION_DIMENSION], int startRow, int startCol)
{
   for (int Xindex = 0; Xindex < GENERATION_DIMENSION; Xindex++)
   {
      for (int Yindex = 0; Yindex < GENERATION_DIMENSION; Yindex++)
      {
         Sudoku[startRow + Xindex][startCol + Yindex] = SubMatrix[Xindex][Yindex];
      }
   }
}

/**
 * Funcion: FillSudokuWithSubMatrix
 * Argumentos: (int[][]) Sudoku
 * Objetivo: Generar un Sudoku completo valido. Implementacion actual usa
 *           un generador por backtracking que rellena la matriz con numeros 1..9.
 */
void FillSudokuWithSubMatrix(int Sudoku[DIMENSION][DIMENSION])
{
   for (int Xindex = 0; Xindex < DIMENSION; Xindex++)
      for (int Yindex = 0; Yindex < DIMENSION; Yindex++)
         Sudoku[Xindex][Yindex] = 0;

   if (!CheckSudokuSolutionStatus(Sudoku, 0, 0))
   {
      for (int Xindex = 0; Xindex < DIMENSION; Xindex++)
         for (int Yindex = 0; Yindex < DIMENSION; Yindex++)
            Sudoku[Xindex][Yindex] = 0;
   }
}

/**
 * Funcion: buildPlayerGrid
 * Argumentos: (int[][]) PlayerMatrix, Sudoku, GridOverlay
 * Objetivo: Construir la vista del jugador copiando desde la solucion solo las
 *           celdas indicadas por la mascara; las demas se inicializan a 0.
 */
void buildPlayerGrid(int PlayerMatrix[DIMENSION][DIMENSION], int Sudoku[DIMENSION][DIMENSION], int GridOverlay[DIMENSION][DIMENSION])
{
   for (int Xindex = 0; Xindex < DIMENSION; Xindex++)
   {
      for (int Yindex = 0; Yindex < DIMENSION; Yindex++)
      {
         if (GridOverlay[Xindex][Yindex] == 1)
            PlayerMatrix[Xindex][Yindex] = Sudoku[Xindex][Yindex];
         else
            PlayerMatrix[Xindex][Yindex] = 0;
      }
   }
}

/**
 * Funcion: generateBlockMask
 * Argumentos: (int[][]) SubMatrix
 * Objetivo: Generar una mascara de pistas por bloques 3x3 seleccionando entre
 *           1 y 4 celdas por bloque para ser reveladas (marcadas con 1).
 */
void generateBlockMask(int SubMatrix[DIMENSION][DIMENSION])
{
   for (int Xindex = 0; Xindex < DIMENSION; Xindex += 3)
   {
      for (int Yindex = 0; Yindex < DIMENSION; Yindex += 3)
      {
         int clues = 1 + rand() % 4;
         int filled = 0;
         while (filled < clues)
         {
            int _Random1 = randNumGen(0, 2);
            int _Random2 = randNumGen(0, 2);
            int globalRow = Xindex + _Random1;
            int globalCol = Yindex + _Random2;

            if (SubMatrix[globalRow][globalCol] == 0)
            {
               SubMatrix[globalRow][globalCol] = 1;
               filled++;
            }
         }
      }
   }
}

/**
 * Funcion: drawCellStyle
 * Argumentos: (int) row, col, cursorRow, cursorCol, isGiven, displayValue
 * Objetivo: Centralizar la logica de posicionamiento y color para dibujar una
 *           celda en pantalla aplica color por bloque 3x3, resalta cursor y
 *           aplica estilo para pistas.
 */
void drawCellStyle(int row, int col, int cursorRow, int cursorCol, int isGiven, int displayValue)
{
   const int cellWidth = 3;
   const int LEFT_MARGIN = 1;
   const int TOP_MARGIN = 1;

   int x = LEFT_MARGIN + col * cellWidth;
   int y = TOP_MARGIN + row;

   int blockRow = row / 3;
   int blockCol = col / 3;

   int blockIndex = blockRow * 3 + blockCol;
   int blockBg[DIMENSION] = {COLOR1, COLOR2, COLOR4, COLOR5, COLOR6, COLOR7, COLOR8, COLOR9, COLOR10};
   int bgColor = blockBg[blockIndex % 9];

   if (row == cursorRow && col == cursorCol)
      setcolor(COLOR1, COLOR3);
   else
      setcolor(COLOR3, bgColor);

   gotoxy(x, y);

   if (displayValue == 0)
      printf(DEFAULT_PLAYER_VIEW);
   else
      printf(" %d ", displayValue);

   colordefault();
}

/**
 * Funcion: drawCandidateCell
 * Argumentos: (int[][]) PlayerMatrix, givens
 *             (int) row, Col, cursorRow, cursorCol, selectionMode, candidate
 * Objetivo: Mostrar el valor candidato o el valor actual de la celda y delegar
 *           el dibujo al helper de estilo, respetando las pistas.
 */
void drawCandidateCell(int PlayerMatrix[DIMENSION][DIMENSION], int Row, int Col, int cursorRow, int cursorCol, int selectionMode, int candidate, int givens[DIMENSION][DIMENSION])
{
   int displayValue;
   if (selectionMode && Row == cursorRow && Col == cursorCol)
      displayValue = candidate;
   else
      displayValue = PlayerMatrix[Row][Col];

   int isGiven = (givens[Row][Col] != 0);
   drawCellStyle(Row, Col, cursorRow, cursorCol, isGiven, displayValue);
}

/**
 * Funcion: drawStatus
 * Argumentos: (int) leftX, topY , Errors, MaxErrors, selectionMode
 * Objetivo: Dibujar la linea de estado con controles y el contador de fallas.
 */
void drawStatus(int leftX, int topY, int Errors, int MaxErrors, int selectionMode)
{
   gotoxy(leftX, topY);
   printf("Fallas: %d/%d   ", Errors, MaxErrors);

   gotoxy(leftX, topY + 1);
   if (!selectionMode)
      printf("Mover: Flechas  |  ENTER: Seleccionar valor  |  ESC: Salir    ");
   else
      printf("Seleccion: UP/DOWN para cambiar, ENTER confirmar, ESC cancelar   ");
}

/**
 * Funcion: drawCell
 * Argumentos: (int[][]) PlayerMatrix, givens
 *             (int) row, col, cursorRow, cursorCol , selectionMode, candidate
 * Objetivo: Preparar el valor a mostrar y delegar el dibujo al helper de estilo.
 */
void drawCell(int PlayerMatrix[DIMENSION][DIMENSION], int row, int col, int cursorRow, int cursorCol, int selectionMode, int candidate, int givens[DIMENSION][DIMENSION])
{
   int displayValue = PlayerMatrix[row][col];
   if (selectionMode && row == cursorRow && col == cursorCol)
      displayValue = candidate;

   int isGiven = (givens[row][col] != 0);
   drawCellStyle(row, col, cursorRow, cursorCol, isGiven, displayValue);
}

/**
 * Funcion: showTransientMsg
 * Argumentos: (int) leftX, topY, msgId
 * Objetivo: Mostrar mensajes transitorios de estado (errores, confirmaciones,
 *           prompts) en una linea dedicada.
 */
void showTransientMsg(int leftX, int topY, int msgId)
{
   gotoxy(leftX, topY);
   switch (msgId)
   {
   case 0:
      printf("%-60s", "\0");
      break;
   case 1:
      printf("%-60s", "No puedes cambiar una pista");
      break;
   case 2:
      printf("%-60s", "Entrada invalida");
      break;
   case 3:
      printf("%-60s", "Seleccion cancelada");
      break;
   case 4:
      printf("%-60s", "Seguro que desea salir?");
   default:
      printf("%-60s", "\0");
      break;
   }
   fflush(stdout);
}

/**
 * Funcion: PlayerMovement
 * Argumentos: (int[][]) sudoku, PlayerMatrix, SubMatrix
 *             (int) MaxErrors - numero maximo de errores antes de perder
 * Objetivo: Gestionar el bucle principal de interaccion: movimiento del cursor,
 *           seleccion de numeros, validaciones, conteo de errores y dibujo.
 */
void PlayerMovement(int sudoku[DIMENSION][DIMENSION], int PlayerMatrix[DIMENSION][DIMENSION], int SubMatrix[DIMENSION][DIMENSION], int MaxErrors)
{
   int cursorRow = 0, cursorCol = 0;
   int Errors = 0;
   int selectionMode = 0;
   int candidate = 0;
   int prefix = 0;
   int key;

   const int LEFT_MARGIN = 1;
   const int TOP_MARGIN = 1;

   _setcursortype(CURSOR_0);

   ShowPlayerMatrix(PlayerMatrix, cursorRow, cursorCol, selectionMode, candidate, SubMatrix);
   drawStatus(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 1, Errors, MaxErrors, selectionMode);
   showTransientMsg(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 3, 0);

   int prevRow = cursorRow, prevCol = cursorCol;

   do
   {
      key = getch();
      if (key == 0 || key == 224)
      {
         prefix = key;
         key = getch();
      }
      else
      {
         prefix = 0;
      }

      int fullRedraw = 0;

      if (!selectionMode)
      {
         if (prefix && key == UP)
            cursorRow = (cursorRow + DIMENSION - 1) % DIMENSION;
         else if (prefix && key == DOWN)
            cursorRow = (cursorRow + 1) % DIMENSION;
         else if (prefix && key == LEFT)
            cursorCol = (cursorCol + DIMENSION - 1) % DIMENSION;
         else if (prefix && key == RIGHT)
            cursorCol = (cursorCol + 1) % DIMENSION;
         else if (key == ENTER)
         {

            if (SubMatrix[cursorRow][cursorCol] != 0)
            {
               showTransientMsg(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 3, 1);

               sleep(1);
               showTransientMsg(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 3, 0);
            }
            else
            {
               selectionMode = 1;

               candidate = PlayerMatrix[cursorRow][cursorCol];
               if (candidate < 1 || candidate > 9)
                  candidate = 1;
            }
         }
      }
      else
      {

         if (prefix && key == UP)
         {
            candidate = (candidate == 9) ? 1 : candidate + 1;
         }
         else if (prefix && key == DOWN)
         {
            candidate = (candidate == 1) ? 9 : candidate - 1;
         }
         else if (key == ENTER)
         {

            int prev = PlayerMatrix[cursorRow][cursorCol];
            if (prev != 0 && prev != candidate)
            {
               Errors++;
               if (Errors >= MaxErrors)
               {
                  system("cls");
                  printf("Has perdido! N%cmero de fallas: (%d)\n", 163, MaxErrors);
                  _setcursortype(CURSOR_100);
                  return;
               }
            }

            PlayerMatrix[cursorRow][cursorCol] = candidate;
            selectionMode = 0;
            candidate = 0;
         }
         else if (key == ESC)
         {

            selectionMode = 0;
            candidate = 0;
            showTransientMsg(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 3, 2);
            sleep(1);
            showTransientMsg(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 3, 0);
         }
         else if (prefix && key == LEFT)
         {

            cursorCol = (cursorCol + DIMENSION - 1) % DIMENSION;

            if (SubMatrix[cursorRow][cursorCol] != 0)
               candidate = 0;
            else
            {
               candidate = PlayerMatrix[cursorRow][cursorCol];
               if (candidate < MIN_RANDRANGE || candidate > MAX_RANDRANGE)
                  candidate = 1;
            }
         }
         else if (prefix && key == RIGHT)
         {
            cursorCol = (cursorCol + 1) % DIMENSION;
            if (SubMatrix[cursorRow][cursorCol] != 0)
               candidate = 0;
            else
            {
               candidate = PlayerMatrix[cursorRow][cursorCol];
               if (candidate < MIN_RANDRANGE || candidate > MAX_RANDRANGE)
                  candidate = 1;
            }
         }
         else if (prefix && key == UP && key == DOWN)
         {
         }
      }

      if (prevRow != cursorRow || prevCol != cursorCol)
      {

         drawCell(PlayerMatrix, prevRow, prevCol, cursorRow, cursorCol, selectionMode, candidate, SubMatrix);

         if (selectionMode)
            drawCandidateCell(PlayerMatrix, cursorRow, cursorCol, cursorRow, cursorCol, selectionMode, candidate, SubMatrix);
         else
            drawCell(PlayerMatrix, cursorRow, cursorCol, cursorRow, cursorCol, selectionMode, candidate, SubMatrix);

         prevRow = cursorRow;
         prevCol = cursorCol;
      }
      else
      {

         if (selectionMode)
         {
            drawCandidateCell(PlayerMatrix, cursorRow, cursorCol, cursorRow, cursorCol, selectionMode, candidate, SubMatrix);
         }
         else
         {

            drawCell(PlayerMatrix, cursorRow, cursorCol, cursorRow, cursorCol, selectionMode, candidate, SubMatrix);
         }
      }

      drawStatus(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 1, Errors, MaxErrors, selectionMode);

      if (!selectionMode)
      {
         int complete = 1;
         for (int Xindex = 0; Xindex < DIMENSION && complete; Xindex++)
            for (int Yindex = 0; Yindex < DIMENSION && complete; Yindex++)
               if (PlayerMatrix[Xindex][Yindex] == 0)
                  complete = 0;

         if (complete)
         {
            system("cls");
            printf("Felicidades, completaste el Sudoku!\n");
            _setcursortype(CURSOR_100);
            return;
         }
      }

      if (key == ESC)
      {
         showTransientMsg(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 3, 4);

         sleep(1);
         key = getch();
         showTransientMsg(LEFT_MARGIN, TOP_MARGIN + DIMENSION + 3, 0);
      }

   } while (!(prefix == 0 && key == ESC));

   _setcursortype(CURSOR_100);
}

/**
 * Funcion: (void) ShowPlayerMatrix
 * Argumentos: (int[][]) Sudoku
 * Objetivo: Dibujar en pantalla el estado actual del Sudoku, aplicando colores por bloques 3x3, resaltando la posición del cursor y mostrando valores o candidatos según el modo de selección.
 */
void ShowPlayerMatrix(int PlayerMatrix[DIMENSION][DIMENSION], int cursorRow, int cursorCol, int selectionMode, int candidate, int givens[DIMENSION][DIMENSION])
{
   for (int Xindex = 0; Xindex < DIMENSION; Xindex++)
   {
      for (int Yindex = 0; Yindex < DIMENSION; Yindex++)
      {
         int displayValue = PlayerMatrix[Xindex][Yindex];
         if (selectionMode && Xindex == cursorRow && Yindex == cursorCol)
            displayValue = candidate;

         int isGiven = (givens[Xindex][Yindex] != 0);
         drawCellStyle(Xindex, Yindex, cursorRow, cursorCol, isGiven, displayValue);
      }
   }

   gotoxy(1, 1 + DIMENSION + (DIMENSION / 3));
}

/**
 * Funcion: isValidSudoku
 * Argumentos: (int[][]) Sudoku
 * Objetivo: Verificar que la matriz 9x9 generada sea valida y no haya numeros repetidos.
 * Retorna: 0 si no es valida, 1 si es valida.
 */
int isValidSudoku(int Sudoku[DIMENSION][DIMENSION])
{
   int seen[DIMENSION + 1];

   for (int Xindex = 0; Xindex < DIMENSION; Xindex++)
   {
      for (int Index = 1; Index <= DIMENSION; Index++)
         seen[Index] = 0;
      for (int columnIndex = 0; columnIndex < DIMENSION; columnIndex++)
      {
         int sudokuCellValue = Sudoku[Xindex][columnIndex];
         if (sudokuCellValue < MIN_NUM_SUDOKU || sudokuCellValue > MAX_NUM_SUDOKU)
            return 0;
         if (seen[sudokuCellValue])
            return 0;
         seen[sudokuCellValue] = 1;
      }
   }

   for (int Yindex = 0; Yindex < DIMENSION; Yindex++)
   {
      for (int Index = 1; Index <= DIMENSION; Index++)
         seen[Index] = 0;
      for (int Rows = 0; Rows < DIMENSION; Rows++)
      {
         int isValidCell = Sudoku[Rows][Yindex];
         if (isValidCell < MIN_NUM_SUDOKU || isValidCell > MAX_NUM_SUDOKU)
            return 0;
         if (seen[isValidCell])
            return 0;
         seen[isValidCell] = MIN_NUM_SUDOKU;
      }
   }

   for (int RowIndex = 0; RowIndex < GENERATION_DIMENSION; RowIndex++)
   {
      for (int Yindex = 0; Yindex < GENERATION_DIMENSION; Yindex++)
      {
         for (int Index = 1; Index <= DIMENSION; Index++)
            seen[Index] = 0;
         for (int Rows = RowIndex * 3; Rows < RowIndex * 3 + 3; Rows++)
         {
            for (int Cols = Yindex * 3; Cols < Yindex * 3 + 3; Cols++)
            {
               int selectedCell = Sudoku[Rows][Cols];
               if (selectedCell < MIN_NUM_SUDOKU || selectedCell > MAX_NUM_SUDOKU)
                  return 0;
               if (seen[selectedCell])
                  return 0;
               seen[selectedCell] = 1;
            }
         }
      }
   }

   return 1;
}

/**
 * Funcion: IsCellSafe
 * Objetivo: Verifica que cada celda en especifico sea valida
 * Argumentos: (int[][]) grid
 *             (int) row, col, num
 */
int IsCellSafe(int grid[DIMENSION][DIMENSION], int row, int col, int num)
{
   for (int Yindex = 0; Yindex < DIMENSION; Yindex++)
      if (grid[row][Yindex] == num)
         return 0;
   for (int Xindex = 0; Xindex < DIMENSION; Xindex++)
      if (grid[Xindex][col] == num)
         return 0;

   int startRow = (row / 3) * 3;
   int startCol = (col / 3) * 3;
   for (int Xindex = startRow; Xindex < startRow + 3; Xindex++)
      for (int Yindex = startCol; Yindex < startCol + 3; Yindex++)
         if (grid[Xindex][Yindex] == num)
            return 0;

   return 1;
}

/**
 * Funcion: CheckSudokuSolutionStatus
 * Objetivo: Verificar que el sudoku sea valido y tenga solucion
 * Argumentos: (int[][]) grid
 *             (int) row, col
 */
int CheckSudokuSolutionStatus(int grid[DIMENSION][DIMENSION], int row, int col)
{
   if (row == DIMENSION)
      return 1;

   int nextRow = (col == DIMENSION - 1) ? row + 1 : row;
   int nextCol = (col == DIMENSION - 1) ? 0 : col + 1;

   if (grid[row][col] != 0)
      return CheckSudokuSolutionStatus(grid, nextRow, nextCol);

   for (int num = 1; num <= 9; num++)
   {
      if (IsCellSafe(grid, row, col, num))
      {
         grid[row][col] = num;
         if (CheckSudokuSolutionStatus(grid, nextRow, nextCol))
            return 1;
         grid[row][col] = 0;
      }
   }

   return 0;
}

/**
 * Funcion: (void) setcolor
 * Argumentos: (int) ct, cf
 * Objetivo: Poner un color especifico dado por otra funcion
 */
void setcolor(int ct, int cf)
{
   textcolor(ct);
   textbackground(cf);
}

/**
 * Funcion: (void) colordefault
 * Objetivo: poner el color que esta por defecto en la terminal
 */
void colordefault(void)
{
   setcolor(COLOR1, COLOR3);
}
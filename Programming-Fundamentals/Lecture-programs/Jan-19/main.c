/*
 * main.c ; Carlos David Jorge Taveras
 *
 * saddle point ; Class Exercise
 *
 * expected compiled file : saddle-point.exe
 */

#include <conio.c>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define LIM_INF 1
#define LIM_SUP(X) (2 * ((X) * (X)))
#define MAX_DIGIT(X) ((X) > 0 ? ((int)log10(X) + 1) : 1)

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

#define ENTER 13
#define ESC 27

#define CT YELLOW
#define CF BLUE
#define CURSOR_CT RED
#define CURSOR_CF GREEN
#define SELECTED_CT GREEN
#define SELECTED_CF YELLOW

#define MIN_DIM 2

#define INIT_SCREENX 5
#define INIT_SCREENY 3
#define INIT_VALUE 0
#define INIT_POSX 0
#define INIT_POSY 0

#define INFO_WIDTH 20

#define CURSOR_0 0
#define CURSOR_100 100

int Exist(int Dimension, int Matrix[Dimension][Dimension], int Value);
int CheckChairPoint(int x_line, int y_line, int Matrix[x_line][y_line], int UserXPos, int UserYPos);
int GetMaxValue(int Line, int Column, int Matrix[Line][Column]);
int GetMaxValue_Column(int Dimension, int Matrix[Dimension][Dimension], int UserYPos);
int GetMinValue_Line(int Dimension, int Matrix[Dimension][Dimension], int UserXPos);
int randrange(int limInf, int limSup);
float GetAvgValue_Line(int Dimension, int Matrix[Dimension][Dimension], int UserXPos);
float GetAvgValue_Column(int Dimension, int Matrix[Dimension][Dimension], int UserYPos);
void setcolor(int cf, int ct);
void colordefault(void);
void Matrix_Initializer(int Dimension, int Matrix[Dimension][Dimension], int Init_value);
void GenerateMatrix(int Dimension, int Matrix[Dimension][Dimension]);
void ShowInfoPanel(int Dimension, int Matrix[Dimension][Dimension], int screenXpos, int screenYpos, int oldX, int oldY, int cursorX, int cursorY);
void ShowMatrix(int Dimension, int Matrix[Dimension][Dimension], int screenXpos, int screenYpos, int cursorXpos, int cursorYpos);
void Movement_engine(int Dimension, int Matrix[Dimension][Dimension], int XPos, int YPos);

int main() {
   system("cls");
   srand(time(NULL));

   int Dimension = 0;
   int DEBUGGINGMATRIX_EXAMPLE[3][3] = {{25, 200, 300}, {50, 100, 200}, {30, 500, 350}};

   do {
      printf("Ingrese las dimensiones de la matriz: ");
      scanf("%d", &Dimension);
      if (Dimension < MIN_DIM) {
         printf("Ingrese un valor mayor o igual a 2.\n\n");
      }
   } while (Dimension < MIN_DIM);

   int CreateMatrix[Dimension][Dimension];

   gotoxy(5, 2 * Dimension + 5); // Here we need to change from literal constant.
   printf("Presione [ESC] para salir del programa.");

   GenerateMatrix(Dimension, CreateMatrix);
   Movement_engine(Dimension, DEBUGGINGMATRIX_EXAMPLE, INIT_SCREENX, INIT_SCREENY);
   // Movement_engine(Dimension, CreateMatrix, INIT_SCREENX, INIT_SCREENY);
   colordefault();
   system("cls");

   return 0;
}

/**
 * Funcion: (int) randrange
 * Argumentos: (int) LimInf - Limite inferior de la generación del numero aleatorio, LimSup - Limite superior de la generación del numero aleatorio
 * Objetivo: Generar valores aleatoreos a parte de un limite inferior y superior.
 * Retorno: (int) Numero aleatorio entre "limInf" y "LimSup".
 */
int randrange(int limInf, int limSup) {
   return rand() % (limSup - limInf + 1) + limInf;
}

/**
 * Funcion: setcolor
 * Argumentos: (int) ct -> Color texto, cf -> Color de fondo
 * Objetivo: Poner un color especifico en el texto y en el fondo.
 */
void setcolor(int ct, int cf) {
   textcolor(ct);
   textbackground(cf);
}

/**
 * Funcion: colordefault
 * Objetivo: Poner la tematica por defecto de la terminal.
 */
void colordefault(void) {
   setcolor(LIGHTGRAY, BLACK);
}

/*
 * Funcion: Exist
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[Dimension][Dimension] - Matriz ingresada en la funcion, Value - Valor de la celda a examinar
 * Objetivo: Verificar si el numero existe en la matriz, en tal caso de que exista, retorna 1, en caso de que no retorna 0.
 * Retorna: 1 si el numero existe, 0 si no existe.
 */
int Exist(int Dimension, int Matrix[Dimension][Dimension], int Value) {
   for (int x_index = 0; x_index < Dimension; x_index++) {
      for (int y_index = 0; y_index < Dimension; y_index++) {
         if (Matrix[x_index][y_index] == Value) {
            return 1;
         }
      }
   }
   return 0;
}

/**
 * Funcion: Matrix_Initializer
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[Dimension][Dimension] - Matriz dada, Init_value - Valor inicial
 * Objetivo: Inicializar la matriz con un valor, en este caso 0.
 */
void Matrix_Initializer(int Dimension, int Matrix[Dimension][Dimension], int Init_value) {
   for (int XIndex = 0; XIndex < Dimension; XIndex++) {
      for (int YIndex = 0; YIndex < Dimension; YIndex++) {
         Matrix[XIndex][YIndex] = Init_value;
      }
   }
}

/**
 * Funcion: GenerateMatrix
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[Dimension][Dimension] - La matriz a generar
 * Objetivo: Generar una matriz a base de los parametros dados, esta matriz tendra numeros aleatoreos.
 */
void GenerateMatrix(int Dimension, int Matrix[Dimension][Dimension]) {
   int ValueToSet;
   Matrix_Initializer(Dimension, Matrix, INIT_VALUE);

   for (int x_index = 0; x_index < Dimension; x_index++) {
      for (int y_index = 0; y_index < Dimension; y_index++) {
         do {
            ValueToSet = randrange(LIM_INF, LIM_SUP(Dimension));
         } while (Exist(Dimension, Matrix, ValueToSet));
         Matrix[x_index][y_index] = ValueToSet;
      }
   }
}

/*
 * Funcion: ShowInfoPanel
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[Dimension][Dimension] - Matriz cuadrada dada, screenXpos - Prosicion en eje X de la matriz, screenYpos - posicion en eje Y de la matriz, oldX - Posicion antigua del cursor en eje X, oldY - Posicion antigua del cursor en eje Y, cursorX - Posicion actual del cursor en eje X, cursorY - Posicion actual del cursor en eje Y
 * Objetivo: Mostrar y actualizar la posicion del panel de informacion de cada fila y columna.
 */
void ShowInfoPanel(int Dimension, int Matrix[Dimension][Dimension], int screenXpos, int screenYpos, int oldX, int oldY, int cursorX, int cursorY) {

   int MaxWidth = MAX_DIGIT(GetMaxValue(Dimension, Dimension, Matrix)) + 1;

   gotoxy(screenXpos + Dimension * MaxWidth + 2, screenYpos + oldX + 1);
   printf("%*s", MaxWidth * 3, "");

   gotoxy(screenXpos + oldY * MaxWidth, screenYpos + Dimension + 1);
   printf("%*s", MaxWidth + 3, "");

   gotoxy(screenXpos + oldY * MaxWidth, screenYpos + Dimension + 2);
   printf("%*s", MaxWidth + 3, "");

   float rowAvg = GetAvgValue_Line(Dimension, Matrix, cursorX);
   int rowMin = GetMinValue_Line(Dimension, Matrix, cursorX);

   float colAvg = GetAvgValue_Column(Dimension, Matrix, cursorY);
   int colMax = GetMaxValue_Column(Dimension, Matrix, cursorY);

   setcolor(CURSOR_CT, CURSOR_CF);
   gotoxy(screenXpos + Dimension * MaxWidth + 2, screenYpos + cursorX + 1);
   printf("%*.*f %*d", MaxWidth, 2, rowAvg, MaxWidth, rowMin);

   setcolor(CURSOR_CT, CURSOR_CF);
   gotoxy(screenXpos + cursorY * MaxWidth, screenYpos + Dimension + 1);
   printf("%*.*f", MaxWidth, 2, colAvg);

   setcolor(CURSOR_CT, CURSOR_CF);
   gotoxy(screenXpos + cursorY * MaxWidth, screenYpos + Dimension + 2);
   printf("%*d", MaxWidth, colMax);
}

/**
 * Funcion: ShowMatrix
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[][] - Matriz a mostrar, XPos - Eje X, YPos - Eje Y.
 * Objetivo: Imprimir la matriz en la pantalla de la terminal.
 */
void ShowMatrix(int Dimension, int Matrix[Dimension][Dimension], int screenXpos, int screenYpos, int cursorXpos, int cursorYpos) {
   int MaxWidth = MAX_DIGIT(GetMaxValue(Dimension, Dimension, Matrix)) + 1;

   for (int x_index = 0; x_index < Dimension; x_index++) {
      for (int y_index = 0; y_index < Dimension; y_index++) {
         if (x_index == cursorXpos && y_index == cursorYpos) {
            if (CheckChairPoint(Dimension, Dimension, Matrix, cursorXpos, cursorYpos)) {
               setcolor(SELECTED_CT, SELECTED_CF);
            } else {
               setcolor(CURSOR_CT, CURSOR_CF);
            }
         } else {
            setcolor(CT, CF);
         }

         gotoxy(screenXpos + y_index * MaxWidth + 1, screenYpos + x_index + 1);
         printf("%*d", MaxWidth, Matrix[x_index][y_index]);
      }
      colordefault();
   }
   return;
}

/**
 * Funcion: GetMaxValue
 * Argumentos: (int) Line - Linea de la matriz, Column - Columna de la matriz, Matrix[][] - Matriz dada
 * Objetivo: Obtener el valor maximo de la matriz.
 * Retorna: El valor maximo de la matriz
 */
int GetMaxValue(int Line, int Column, int Matrix[Line][Column]) {
   int MaxVal = Matrix[0][0];

   for (int Xindex = 0; Xindex < Line; Xindex++) {
      for (int Yindex = 0; Yindex < Column; Yindex++) {
         if (MaxVal < Matrix[Xindex][Yindex]) {
            MaxVal = Matrix[Xindex][Yindex];
         }
      }
   }
   return MaxVal;
}

/**
 * Funcion: GetMaxValue_Column
 * Argumentos: (int) Dimension - Dimension de la matriz., Matrix[][] - Matriz dada, UserXPos - Posicion del usuario en eje X, UserYPos - Posicion del usuario en eje Y.
 * Objetivo: Obtener el valor maximo de la fila y de la columna.
 * Retorna: Valor Maximo de la fila y de la columna.
 */
int GetMaxValue_Column(int Dimension, int Matrix[Dimension][Dimension], int UserYPos) {
   int MaxVal_Column = Matrix[0][UserYPos];

   for (int x_index = 0; x_index < Dimension; x_index++) {
      if (Matrix[x_index][UserYPos] > MaxVal_Column) {
         MaxVal_Column = Matrix[x_index][UserYPos];
      }
   }

   return MaxVal_Column;
}

/**
 * Funcion: GetAvgValue_Column
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[][] - Matriz dada en main, UserYPos - Posicion del usuario en la matriz en eje Y
 * Objetivo: Obtener el average de la columna.
 * Retorna: el average los valores de la columna.
 */
float GetAvgValue_Column(int Dimension, int Matrix[Dimension][Dimension], int UserYPos) {
   float sum = 0;
   for (int x_index = 0; x_index < Dimension; x_index++) {
      sum += Matrix[x_index][UserYPos];
   }
   return sum / Dimension;
}

/**
 * Funcion: GetMaxValue_Line
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[][] - Matriz dada, UserXPos - Posicion del usuario en eje X, UserYPos - Posicion del usuario en eje Y.
 * Objetivo: Obtener el valor maximo de la fila y de la columna.
 * Retorna: Valor Maximo de la fila y de la columna.
 */
int GetMinValue_Line(int Dimension, int Matrix[Dimension][Dimension], int UserXPos) {
   int MaxVal_Line = Matrix[UserXPos][0];

   for (int y_index = 0; y_index < Dimension; y_index++) {
      if (Matrix[UserXPos][y_index] < MaxVal_Line) {
         MaxVal_Line = Matrix[UserXPos][y_index];
      }
   }

   return MaxVal_Line;
}

/*
 * Funcion: GetAvgValue_Line
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[][] - Matriz dada en main, UserXPos - Posicion del usuario en la matriz en eje X
 * Objetivo: Obtener el averaje de la fila de la matriz dada.
 * Retorna: El average de los valores de la fila,
 */
float GetAvgValue_Line(int Dimension, int Matrix[Dimension][Dimension], int UserXPos) {
   float sum = 0;
   for (int y_index = 0; y_index < Dimension; y_index++) {
      sum += Matrix[UserXPos][y_index];
   }
   return sum / Dimension;
}

/**
 * Funcion: CheckChairPoint
 * Argumentos: (int) x_line - El eje x de la matriz, y_line - El eje y de la matriz., int Matrix[x_line][y_line] - Matriz dada desde Main
 * Objetivo: Verificar si el valor es punto silla, un valor punto silla es el menor de la columna y el mayor de la fila.
 * Retorna: Retorna 1 si es un punto silla, retorna 0 si no es punto silla.
 */
int CheckChairPoint(int Line, int Column, int Matrix[Line][Column], int UserXPos, int UserYPos) {
   int ChairPointValue = Matrix[UserXPos][UserYPos];

   for (int x_index = 0; x_index < Line; x_index++) {
      if (Matrix[x_index][UserYPos] > ChairPointValue) {
         return 0;
      }
   }

   for (int y_index = 0; y_index < Column; y_index++) {
      if (Matrix[UserXPos][y_index] < ChairPointValue) {
         return 0;
      }
   }

   return 1;
}

/**
 * Funcion: Movement_engine
 * Argumentos: (Int) Dimension - Dimension de la matriz, Matrix[Dimension][Dimension] - Matriz a presentar, screenXpos - Posicion de la pantalla en X, screenYpos - Posicion de la pantalla en Y.
 * Objetivo: Dar habilidad al usuario de poder moverse en la matriz con las flechas del teclado.
 */
void Movement_engine(int Dimension, int Matrix[Dimension][Dimension], int screenXpos, int screenYpos) {
   char key;
   int CursorX = INIT_POSX, CursorY = INIT_POSY;

   _setcursortype(CURSOR_0);

   ShowMatrix(Dimension, Matrix, screenXpos, screenYpos, CursorX, CursorY);
   ShowInfoPanel(Dimension, Matrix, screenXpos, screenYpos, CursorX, CursorY, CursorX, CursorY);

   do {
      int OldCursorX = CursorX, OldCursorY = CursorY;

      do {
         key = getch();
      } while (key != UP && key != DOWN && key != RIGHT && key != LEFT && key != ENTER && key != ESC);
      if (key == RIGHT) {
         if (CursorY < Dimension - 1) {
            CursorY++;
         }
      }

      if (key == LEFT) {
         if (CursorY > 0) {
            CursorY--;
         }
      }

      if (key == UP) {
         if (CursorX > 0) {
            CursorX--;
         }
      }

      if (key == DOWN) {
         if (CursorX < Dimension - 1) {
            CursorX++;
         }
      }
      ShowMatrix(Dimension, Matrix, screenXpos, screenYpos, CursorX, CursorY);
      ShowInfoPanel(Dimension, Matrix, screenXpos, screenYpos, OldCursorX, OldCursorY, CursorX, CursorY);
   } while (key != ESC);
   _setcursortype(CURSOR_100);
}

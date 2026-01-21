/*
 * Carlos David ; main.c
 * Expected output file: shuffle.exe
 */

// #include <conio.c>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

#define ENTER 13
#define ESC 27

#define CT GREEN
#define CF YELLOW
#define HIDDEN_CF RED
#define HIDDEN_CT RED

#define INIT_POSX 5
#define INIT_POSY 15

#define MAX_DIM 15
#define MIN_DIM 3

#define INIT_VALUE 0

#define MAX_DIGIT(X) ((int)log10(X) + 1)

int Exist(int Dimension, int Matrix[Dimension][Dimension], int Value);
int GetMaxValue(int Line, int Column, int Matrix[Line][Column]);
int IsOrdered(int Dimension, int Matrix[Dimension][Dimension]);
int randrange(int limInf, int limSup);
void Matrix_Initializer(int Dimension, int Matrix[Dimension][Dimension], int Init_value);
void Movement_engine(int Dimension, int Matrix[Dimension][Dimension], int XPos, int YPos);
void Generate_BoxNum(int Dimension, int Matrix[Dimension][Dimension]);
void OrderingBox(int Dimension, int Matrix[Dimension][Dimension]);
void ShowMatrix(int Dimension, int Matrix[Dimension][Dimension], int XPos, int YPos);
void setcolor(int, int);
void colordefault(void);

int main() {
   srand(time(NULL));
   int MatrixDimension;

   do {
      printf("Choose matrix dimensions: ");
      scanf("%d", &MatrixDimension);
      if (MatrixDimension < MIN_DIM || MatrixDimension > MAX_DIM) {
         printf("The dimension should be in the range: [%d, %d].\n", MIN_DIM, MAX_DIM);
      }
   } while (MatrixDimension < MIN_DIM || MatrixDimension > MAX_DIM);

   int OrderBox[MatrixDimension][MatrixDimension];

   system("cls");
   Generate_BoxNum(MatrixDimension, OrderBox);
   Movement_engine(MatrixDimension, OrderBox, INIT_POSX, INIT_POSY);

   return 0;
}

/**
 * Funcion: (int) randrange
 * Argumentos: (int) LimInf [Limite inferior de la generación del numero aleatorio], LimSup [Limite superior de la generación del numero aleatorio]
 * Objetivo: Generar valores aleatoreos a parte de un limite inferior y superior.
 * Retorno: (int) Numero aleatorio entre "limInf" y "LimSup".
 */
int randrange(int limInf, int limSup) {
   return rand() % (limSup - limInf + 1) + limInf;
}

int IsOrdered(int Dimension, int Matrix[Dimension][Dimension]) {
   int Before_value = Matrix[0][0];
   for (int XIndex = 0; XIndex < Dimension; XIndex++) {
      for (int YIndex = 0; YIndex < Dimension; YIndex++) {
         if (!(XIndex == Dimension - 1 && YIndex == Dimension - 1)) {
            if (Before_value > Matrix[XIndex][YIndex]) {
               return 0;
            }
         }
         Before_value = Matrix[XIndex][YIndex];
      }
   }
   return 1;
}

void OrderingBox(int Dimension, int Matrix[Dimension][Dimension]) {
   int Next_value = 1;
   Matrix[Dimension - 1][Dimension - 1] = INIT_VALUE;
   for (int XIndex = 0; XIndex < Dimension; XIndex++) {
      for (int YIndex = 0; YIndex < Dimension; YIndex++) {
         if (!(XIndex == Dimension - 1 && YIndex == Dimension - 1)) {
            Matrix[XIndex][YIndex] = Next_value++;
         }
      }
   }
   return;
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
 * Funcion: Generate_BoxNum
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[Dimension][Dimension] - La matriz a generar
 * Objetivo: Generar una matriz a base de los parametros dados, esta matriz tendra numeros aleatoreos.
 */
void Generate_BoxNum(int Dimension, int Matrix[Dimension][Dimension]) {
   int ValueToSet;
   Matrix_Initializer(Dimension, Matrix, INIT_VALUE);

   for (int Xindex = 0; Xindex < Dimension; Xindex++) {
      for (int Yindex = 0; Yindex < Dimension; Yindex++) {
         // Fill the cell except the last cell.
         if (!(Xindex == Dimension - 1 && Yindex == Dimension - 1)) {
            do {
               ValueToSet = randrange(1, Dimension * Dimension - 1);
            } while (Exist(Dimension, Matrix, ValueToSet));
            Matrix[Xindex][Yindex] = ValueToSet;
         }
      }
   }
}

/**
 * Funcion: ShowMatrix
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[][] - Matriz a mostrar, XPos - Eje X, YPos - Eje Y.
 * Objetivo: Imprimir la matriz en la pantalla de la terminal.
 */
void ShowMatrix(int Dimension, int Matrix[Dimension][Dimension], int XPos, int YPos) {
   int MaxDigitInput = MAX_DIGIT(GetMaxValue(Dimension, Dimension, Matrix)) + 1;

   for (int Xindex = 0; Xindex < Dimension; Xindex++) {
      for (int Yindex = 0; Yindex < Dimension; Yindex++) {

         setcolor(CT, CF);
         if (Matrix[Xindex][Yindex] == INIT_VALUE) {
            setcolor(HIDDEN_CT, HIDDEN_CF);
         }

         gotoxy(XPos + Xindex * MaxDigitInput, YPos + Yindex);
         printf("%*d", MaxDigitInput, Matrix[Xindex][Yindex]);
      }
      colordefault();
   }
}

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

int Exist(int Dimension, int Matrix[Dimension][Dimension], int Value) {
   int Exist = 0;
   for (int Xindex = 0; Xindex < Dimension; Xindex++) {
      for (int Yindex = 0; Yindex < Dimension; Yindex++) {
         if (Matrix[Xindex][Yindex] == Value) {
            Exist = 1;
         }
      }
   }
   return Exist;
}

void Movement_engine(int Dimension, int Matrix[Dimension][Dimension], int XPos, int YPos) {
   char key;
   int Empty_Xpos = Dimension - 1, Empty_Ypos = Dimension - 1;
   int Movement_done = 0;

   do {
      _setcursortype(0);
      ShowMatrix(Dimension, Matrix, XPos, YPos);

      do {
         key = getch();
      } while (key != UP && key != DOWN && key != RIGHT && key != LEFT && key != ENTER && key != ESC);

      if (key == DOWN) {
         if (Empty_Ypos < Dimension - 1) {
            Matrix[Empty_Xpos][Empty_Ypos] = Matrix[Empty_Xpos][Empty_Ypos + 1];
            Matrix[Empty_Xpos][Empty_Ypos + 1] = INIT_VALUE;
            Empty_Ypos++;
            Movement_done = 1;
         }
      }

      if (key == UP) {
         if (Empty_Ypos > 0) {
            Matrix[Empty_Xpos][Empty_Ypos] = Matrix[Empty_Xpos][Empty_Ypos - 1];
            Matrix[Empty_Xpos][Empty_Ypos - 1] = INIT_VALUE;
            Empty_Ypos--;
            Movement_done = 1;
         }
      }

      if (key == LEFT) {
         if (Empty_Xpos > 0) {
            Matrix[Empty_Xpos][Empty_Ypos] = Matrix[Empty_Xpos - 1][Empty_Ypos];
            Matrix[Empty_Xpos - 1][Empty_Ypos] = INIT_VALUE;
            Empty_Xpos--;
         }
      }

      if (key == RIGHT) {
         if (Empty_Xpos < Dimension - 1) {
            Matrix[Empty_Xpos][Empty_Ypos] = Matrix[Empty_Xpos + 1][Empty_Ypos];
            Matrix[Empty_Xpos + 1][Empty_Ypos] = INIT_VALUE;
            Empty_Xpos++;
            Movement_done = 1;
         }
      }

      if (key == ENTER) {
         OrderingBox(Dimension, Matrix);
         Movement_done = 1;
      }

      if (Movement_done && IsOrdered(Dimension, Matrix)) {
         ShowMatrix(Dimension, Matrix, XPos, YPos);
         gotoxy(XPos, YPos + Dimension + 2);

         printf("Congratulation, you ordered the box");
         key = ESC;
      }
   } while (key != ESC);
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

/*
 * Battleship game
 *
 * main.c ; Charles David Jorge 22/1/2026
 *
 * Expected executable output ; battleship.exe
 */

#include <conio.c>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Game Characters
#define CHAR_USER_BOAT 66
#define CHAR_HIT_BOAT 88
#define CHAR_FRIENDLY_FIRE 33
#define CHAR_MISS 79
#define CHAR_OCEAN 126

// Controls
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75
#define ENTER 13
#define ESC 27
// Cannon configuration
#define RESULT_NONE 0
#define RESULT_HIT 1
#define RESULT_FRIENDLY 2
#define RESULT_MISS 3
#define RESULT_DONE 4
#define RESULT_EXIT 5
// Game config
#define BOAT_LOCATION 11
#define USER_BOAT_LOCATION 12
// Modes
#define MODE_PLACE 13
#define MODE_ATTACK 14
// Game generation
#define GAMEPLAY_DIM 10
#define BOATS 5
// cursor configuration
#define HIT_CT WHITE
#define HIT_CF RED
#define MISS_CT WHITE
#define MISS_CF BLACK
#define FRIENDLY_FIRE_CT MAGENTA
#define FRIENDLY_FIRE_CF WHITE
#define DEFAULT_CT GREEN
#define DEFAULT_CF BLUE
#define CURSOR_CT RED
#define CURSOR_CF GREEN
// Conio configuration
#define TITLE_POSITION 5
#define INIT_VALUE 0
#define INIT_POSX 15
#define INIT_POSY 5
// Miscellaneous
#define CURSOR_0 0
#define CURSOR_100 100
#define LOG_Y (INIT_POSY + GAMEPLAY_DIM + 3)

int ResolveTurn(int Dimension, int PlayerMatrix[Dimension][Dimension], int EnemyMatrix[Dimension][Dimension]);
int CountPlayerShips(int Dimension, int PlayerMatrix[Dimension][Dimension]);
int CountEnemyShips(int Dimension, int EnemyMatrix[Dimension][Dimension]);
int HandleAttack(int Dimension, int PlayerMatrix[Dimension][Dimension], int EnemyMatrix[Dimension][Dimension], int row, int col);
int ComputerAttack(int Dimension, int PlayerBoard[Dimension][Dimension]);
int PlayerAttack(int Dimension, int PlayerBoard[Dimension][Dimension], int EnemyBoard[Dimension][Dimension], int row, int col);
int BoatPlacer(int Dimension, int Matrix[Dimension][Dimension], int row, int col);
void Movement_engine(int Dimension, int PlayerMatrix[Dimension][Dimension], int EnemyMatrix[Dimension][Dimension], int screenXpos, int screenYpos, int mode);
void GenerateEnemyBoats(int Dimension, int Matrix[Dimension][Dimension]);
void DrawCell(int Dimension, int Matrix[Dimension][Dimension], int ScreenX, int ScreenY, int Row, int Col, int isCursor);
void colordefault(void);
void setcolor(int ct, int cf);
void ShowGameMap(int Dimension, int Matrix[Dimension][Dimension], int Xpos, int Ypos, int CursorX, int CursorY);
void Matrix_Initializer(int Dimension, int Matrix[Dimension][Dimension], int Init_value);

int main() {
   system("cls");
   srand(time(NULL));

   int key = 0;
   int PlayerBoard[GAMEPLAY_DIM][GAMEPLAY_DIM];
   int EnemyBoard[GAMEPLAY_DIM][GAMEPLAY_DIM];

   _setcursortype(CURSOR_0);

   int playAgain = 1;

   while (playAgain && key != ESC) {

      system("cls");
      while (key != ENTER) {
         printf("Bienvenido a battleship!\n");

         printf("[ENTER] para iniciar - [ESC] para salir");

         key = getch();

         if (key == ESC) {
            return 0;
         }
      }
      system("cls");

      Matrix_Initializer(GAMEPLAY_DIM, PlayerBoard, INIT_VALUE);
      Matrix_Initializer(GAMEPLAY_DIM, EnemyBoard, INIT_VALUE);

      Movement_engine(GAMEPLAY_DIM, PlayerBoard, EnemyBoard, INIT_POSX, INIT_POSY, MODE_PLACE);
      GenerateEnemyBoats(GAMEPLAY_DIM, EnemyBoard);
      Movement_engine(GAMEPLAY_DIM, PlayerBoard, EnemyBoard, INIT_POSX, INIT_POSY, MODE_ATTACK);

      gotoxy(1, LOG_Y + 8);
      printf("Desea jugar otra partida? (ENTER = Si / ESC = No): ");
      key = getch();
      if (key == ESC) {
         playAgain = 0;
      }
   }

   _setcursortype(CURSOR_100);
   return 0;
}

/*
 * Funcion: DrawCell
 * Argumentos: (int) Dimension - Dimension de la matriz,
 *                   Matrix[][Dimension] - Matriz dada,
 *                   ScreenX - Posicion de la pantalla en eje X,
 *                   ScreenY - Posicion de la pantalla en eje Y,
 *                   Row, Col - Fila y columna de la matriz
 *                   isCursor - El cursor, dependiendo de donde este el cursor, el valor de este argumento cambia a 1.
 * Objetivo: Actualizar una celda en especifico en la pantalla sin tener que reimprimir la matriz completa.
 */
void DrawCell(int Dimension, int Matrix[Dimension][Dimension], int ScreenX, int ScreenY, int Row, int Col, int isCursor) {
   gotoxy(ScreenX + Col + 1, ScreenY + Row + 1);

   if (isCursor) {
      setcolor(CURSOR_CT, CURSOR_CF);
   } else {
      setcolor(DEFAULT_CT, DEFAULT_CF);
   }

   if (Matrix[Row][Col] == USER_BOAT_LOCATION) {
      printf("%c", CHAR_USER_BOAT);
   } else if (Matrix[Row][Col] == RESULT_HIT) {
      setcolor(HIT_CT, HIT_CF);
      printf("%c", CHAR_HIT_BOAT);
   } else if (Matrix[Row][Col] == RESULT_FRIENDLY) {
      setcolor(FRIENDLY_FIRE_CT, FRIENDLY_FIRE_CF);
      printf("%c", CHAR_FRIENDLY_FIRE);
   } else if (Matrix[Row][Col] == RESULT_MISS) {
      setcolor(MISS_CT, MISS_CF);
      printf("%c", CHAR_MISS);
   } else if (Matrix[Row][Col] == BOAT_LOCATION) {
      printf("%c", CHAR_OCEAN);
   } else {
      printf("%c", CHAR_OCEAN);
   }

   colordefault();
}

/**
 * Funcion: ResolveTurn
 * Argumentos: (int) Dimension - Dimension de la matriz, PlayerMatrix[][Dimension] - Matriz del tablero del jugador, EnemyMatrix[][Dimension] - Matriz del tablero enemigo.
 * Retorna: el resultado de la movida de la computadora.
 */
int ResolveTurn(int Dimension, int PlayerMatrix[Dimension][Dimension], int EnemyMatrix[Dimension][Dimension]) {
   Sleep(700);

   gotoxy(1, LOG_Y + 2);
   printf("La computadora ataca. . .            ");

   Sleep(700);
   gotoxy(1, LOG_Y + 2);
   printf("                                      ");
   int computerResult = ComputerAttack(Dimension, PlayerMatrix);

   gotoxy(1, LOG_Y + 3);
   if (computerResult == RESULT_HIT) {
      printf("La computadora ha impactado tu barco!     ");
   } else {
      printf("La computadora ha fallado.                ");
   }
   Sleep(1000);
   gotoxy(1, LOG_Y + 3);
   printf("                                              ");

   int playerShips = CountPlayerShips(Dimension, PlayerMatrix);
   int enemyShips = CountEnemyShips(Dimension, EnemyMatrix);

   if (enemyShips == 0 || playerShips == 0) {

      gotoxy(1, LOG_Y + 5);
      printf("====================================");

      gotoxy(1, LOG_Y + 6);
      if (enemyShips == 0) {
         printf("Has ganado la batalla naval!              ");
      } else {
         printf("La computadora ha ganado!                 ");
      }

      gotoxy(1, LOG_Y + 7);
      printf("Barcos restantes del jugador: %d", playerShips);

      gotoxy(1, LOG_Y + 8);
      printf("Barcos restantes del enemigo: %d", enemyShips);

      gotoxy(1, LOG_Y + 9);
      printf("Fin del juego.");

      _setcursortype(CURSOR_100);
      getch();

      return RESULT_DONE;
   }

   return RESULT_NONE;
}

/*
 * funcion: PlayerAttack
 * Argumentos: (int) Dimension - Dimension de la matriz, PlayerBoard[][] - Matriz del jugador, EnemyBoard[][] - Matriz del enemigo, row - Filas de la matriz, col - Columna de la matriz
 * Objetivo: Retornar un valor dependiendo del resultado del ataque.
 * Retorna: Si le ha dado a un enemgio, retorna 1 ; Si le ha dado a un aliado, retorna 2 ; Si le ha fallado, retorna 3 ; Si ya ha jugado, retorna 0.
 */
int PlayerAttack(int Dimension, int PlayerBoard[Dimension][Dimension], int EnemyBoard[Dimension][Dimension], int row, int col) {
   if (EnemyBoard[row][col] == BOAT_LOCATION) {
      EnemyBoard[row][col] = RESULT_HIT;
      return RESULT_HIT;
   } else if (PlayerBoard[row][col] == USER_BOAT_LOCATION) {
      PlayerBoard[row][col] = RESULT_FRIENDLY;
      EnemyBoard[row][col] = RESULT_FRIENDLY;
      return RESULT_FRIENDLY;
   } else if (EnemyBoard[row][col] == INIT_VALUE) {
      EnemyBoard[row][col] = RESULT_MISS;
      return RESULT_MISS;
   }
   return 0;
}

/**
 * Funcion: CountPlayerShips
 * Argumentos: (int) Dimension - Dimension de la matriz, PlayerMatrix[][] - Matriz del jugador
 * Objetivo: Contar cuantos barcos quedan al jugador
 * Retorna: La cantidad de barcos restantes del jugador
 */
int CountPlayerShips(int Dimension, int PlayerMatrix[Dimension][Dimension]) {
   int count = 0;

   for (int Xindex = 0; Xindex < Dimension; Xindex++) {
      for (int Yindex = 0; Yindex < Dimension; Yindex++) {
         if (PlayerMatrix[Xindex][Yindex] == USER_BOAT_LOCATION) {
            count++;
         }
      }
   }

   return count;
}

/**
 * Funcion: CountEnemyShips
 * Argumentos: (int) Dimension - Dimension de la matriz, EnemyMatrix[][] - Matriz del jugador
 * Objetivo: Contar cuantos barcos quedan al enemigo
 * Retorna: La cantidad de barcos restantes del enemigo
 */
int CountEnemyShips(int Dimension, int EnemyMatrix[Dimension][Dimension]) {
   int count = 0;

   for (int Xindex = 0; Xindex < Dimension; Xindex++) {
      for (int Yindex = 0; Yindex < Dimension; Yindex++) {
         if (EnemyMatrix[Xindex][Yindex] == BOAT_LOCATION) {
            count++;
         }
      }
   }

   return count;
}

/**
 * Funcion: ComputerAttack
 * Argumentos: (int) Dimension, PlayerBoard[Dimension][Dimension]
 * Objetivo: Cuando es turno de la computadora, atacar al usuario utilizara esta funcion.
 */
int ComputerAttack(int Dimension, int PlayerBoard[Dimension][Dimension]) {
   int row, col;

   do {
      row = rand() % Dimension;
      col = rand() % Dimension;
   } while (PlayerBoard[row][col] == RESULT_HIT || PlayerBoard[row][col] == RESULT_MISS || PlayerBoard[row][col] == RESULT_FRIENDLY);

   if (PlayerBoard[row][col] == USER_BOAT_LOCATION) {
      PlayerBoard[row][col] = RESULT_HIT;
      return RESULT_HIT;
   } else {
      PlayerBoard[row][col] = RESULT_MISS;
      return RESULT_MISS;
   }
}

/**
 * funcion: BoatPlacer
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[][] - Matriz dada, row - Filas de la matriz, col - Columna
 * Objetivo: Señalizar si se logro poner un barco en la posicion deseada.
 * Retorna: 1 si se logro poner un barco ; 0 si a fallado en poner un barco.
 */
int BoatPlacer(int Dimension, int Matrix[Dimension][Dimension], int row, int col) {
   if (Matrix[row][col] == INIT_VALUE) {
      Matrix[row][col] = USER_BOAT_LOCATION;
      return 1;
   }
   return 0;
}

/**
 * Funcion: Matrix_Initializer
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[Dimension][Dimension] - Matriz dada, Init_value - Valor inicial
 * Objetivo: Inicializar la matriz el valor de 0.
 */
void Matrix_Initializer(int Dimension, int Matrix[Dimension][Dimension], int Init_value) {
   for (int XIndex = 0; XIndex < Dimension; XIndex++) {
      for (int YIndex = 0; YIndex < Dimension; YIndex++) {
         Matrix[XIndex][YIndex] = Init_value;
      }
   }
}

/**
 * Funcion: ShowGameMap
 * Argumentos: (int) Dimension, Matrix[][], Xpos, Ypos
 * Objetivo: Imprimir la matriz del juego.
 */
void ShowGameMap(int Dimension, int Matrix[Dimension][Dimension], int Xpos, int Ypos, int CursorX, int CursorY) {
   for (int Xindex = 0; Xindex < Dimension; Xindex++) {
      for (int Yindex = 0; Yindex < Dimension; Yindex++) {
         gotoxy(Xpos + Yindex + 1, Ypos + Xindex + 1);
         if (CursorX >= 0 && CursorY >= 0 && Xindex == CursorX && Yindex == CursorY) {
            setcolor(CURSOR_CT, CURSOR_CF);
         } else {
            setcolor(DEFAULT_CT, DEFAULT_CF);
         }

         if (Matrix[Xindex][Yindex] == USER_BOAT_LOCATION) {
            printf("%c", CHAR_USER_BOAT);
         } else if (Matrix[Xindex][Yindex] == RESULT_HIT) {
            setcolor(HIT_CT, HIT_CF);
            printf("%c", CHAR_HIT_BOAT);
         } else if (Matrix[Xindex][Yindex] == RESULT_FRIENDLY) {
            setcolor(FRIENDLY_FIRE_CT, FRIENDLY_FIRE_CF);
            printf("%c", CHAR_FRIENDLY_FIRE);
         } else if (Matrix[Xindex][Yindex] == RESULT_MISS) {
            setcolor(MISS_CT, MISS_CF);
            printf("%c", CHAR_MISS);
         } else {
            printf("%c", CHAR_OCEAN);
         }
      }
   }
   colordefault();
}

/**
 * Funcion: HandleAttack
 * Argumentos: (int) Dimension - Dimension de la matriz, PlayerMatrix[][], EnemyMatrix[][], row, col
 * Objetivo: Manejar los ataques del jugador para devolverlos como resultados hacia la funcion de ShowAttackMessage.
 */
int HandleAttack(int Dimension, int PlayerMatrix[Dimension][Dimension], int EnemyMatrix[Dimension][Dimension], int row, int col) {
   return PlayerAttack(Dimension, PlayerMatrix, EnemyMatrix, row, col);
}

/**
 * Funcion: ShowAttackMessage
 * Argumentos: (int) Result - Resultado del ataque
 * Objetivo: Mostrar un mensaje dependiendo de si el ataque fue exitoso (1), fuego aliado (2), o fallido (3)
 */
void ShowAttackMessage(int result) {
   gotoxy(1, LOG_Y);
   printf("                                                          ");
   gotoxy(1, LOG_Y);

   switch (result) {
   case RESULT_HIT:
      printf("Boom! Has hundido un barco");
      Sleep(650);
      gotoxy(1, LOG_Y);
      printf("                                                      ");
      break;

   case RESULT_FRIENDLY:
      printf("Oh no, fuego aliado!");
      Sleep(650);
      gotoxy(1, LOG_Y);
      printf("                                                      ");
      break;

   case RESULT_MISS:
      printf("Hemos fallado! Recargad las 125 y los balisticos!");
      Sleep(650);
      gotoxy(1, LOG_Y);
      printf("                                                      ");
      break;

   default:
      break;
   }
}

/*
 * Funcion: GenerateEnemyBoats
 * Argumentos: (int) Dimension - Dimension de la matriz, Matrix[][] - Matriz dada en main
 * Objetivo: Generar los barcos enemigos en la matriz.
 */
void GenerateEnemyBoats(int Dimension, int Matrix[Dimension][Dimension]) {
   int BoatsPlaced = 0;

   gotoxy(1, LOG_Y);
   printf("La computadora esta desplegando barcos");

   while (BoatsPlaced < BOATS) {
      int randRow = rand() % Dimension;
      int randCol = rand() % Dimension;

      if (Matrix[randRow][randCol] == INIT_VALUE) {
         Matrix[randRow][randCol] = BOAT_LOCATION;
         BoatsPlaced++;

         gotoxy(1, LOG_Y + BoatsPlaced);
         printf("Barco %d desplegado. . .", BoatsPlaced);
         Sleep(500);
         gotoxy(1, LOG_Y + BoatsPlaced);
         printf("                         ");
      }
   }
}

/**
 * Funcion: Movement_engine
 * Argumentos: (Int) Dimension - Dimension de la matriz, PlayerMatrix[Dimension][Dimension] - Matriz del jugador, EnemyMatrix[][] - Matriz del enemigo, screenXpos - Posicion de la pantalla en X, screenYpos - Posicion de la pantalla en Y, mode - Modo del operador
 * Objetivo: Motor de controles, manejar los controles del juego dependiendo del modo que este el usuario.
 */
void Movement_engine(int Dimension, int PlayerMatrix[Dimension][Dimension], int EnemyMatrix[Dimension][Dimension], int screenXpos, int screenYpos, int mode) {
   int key;
   int CursorX = 0;
   int CursorY = 0;
   int boatsPlaced = 0;

   _setcursortype(CURSOR_0);
   if (mode == MODE_ATTACK) {
      ShowGameMap(Dimension, EnemyMatrix, screenXpos, screenYpos, CursorX, CursorY);
      // Quitar el comentario de abajo para ver todo el juego. [CHEAT MODE]
      ShowGameMap(Dimension, PlayerMatrix, screenXpos + 25, screenYpos, -1, -1);

   } else {
      ShowGameMap(Dimension, PlayerMatrix, screenXpos, screenYpos, CursorX, CursorY);
   }

   do {
      key = getch();
      if (key == 0 || key == 224) {
         key = getch();
      }
      int OldCursorX = CursorX;
      int OldCursorY = CursorY;

      switch (key) {
      case RIGHT:
         if (CursorY < Dimension - 1)
            CursorY++;
         break;
      case LEFT:
         if (CursorY > 0)
            CursorY--;
         break;
      case UP:
         if (CursorX > 0)
            CursorX--;
         break;
      case DOWN:
         if (CursorX < Dimension - 1)
            CursorX++;
         break;
      case ENTER:
         if (mode == MODE_PLACE) {
            if (boatsPlaced < BOATS && PlayerMatrix[CursorX][CursorY] == INIT_VALUE) {
               if (BoatPlacer(Dimension, PlayerMatrix, CursorX, CursorY)) {
                  boatsPlaced++;
                  DrawCell(Dimension, PlayerMatrix, screenXpos, screenYpos, CursorX, CursorY, 1);
                  if (boatsPlaced == BOATS) {
                     gotoxy(1, LOG_Y);
                     printf("Has colocado todos los barcos. Confirmar? (ENTER = Si / ESC = No)");
                     int confirmKey = getch();

                     if (confirmKey == ESC) {
                        Matrix_Initializer(Dimension, PlayerMatrix, INIT_VALUE);
                        boatsPlaced = 0;
                        ShowGameMap(Dimension, PlayerMatrix, screenXpos, screenYpos, CursorX, CursorY);

                        gotoxy(1, LOG_Y);
                        printf("                                                                  ");
                        continue;
                     }
                     gotoxy(1, LOG_Y);
                     printf("                                                                  ");
                     return;
                  }
               }
            }
         } else if (mode == MODE_ATTACK) {
            int result = HandleAttack(Dimension, PlayerMatrix, EnemyMatrix, CursorX, CursorY);
            ShowAttackMessage(result);
            DrawCell(Dimension, EnemyMatrix, screenXpos, screenYpos, CursorX, CursorY, 1);
            if (result != RESULT_NONE) {
               int GameState = ResolveTurn(Dimension, PlayerMatrix, EnemyMatrix);
               if (GameState == RESULT_DONE) {
                  return;
               }
            }
         }
         break;
      }

      if (OldCursorX != CursorX || OldCursorY != CursorY) {
         if (mode == MODE_ATTACK) {
            DrawCell(Dimension, EnemyMatrix, screenXpos, screenYpos, OldCursorX, OldCursorY, 0);
            DrawCell(Dimension, EnemyMatrix, screenXpos, screenYpos, CursorX, CursorY, 1);
         } else {
            DrawCell(Dimension, PlayerMatrix, screenXpos, screenYpos, OldCursorX, OldCursorY, 0);
            DrawCell(Dimension, PlayerMatrix, screenXpos, screenYpos, CursorX, CursorY, 1);
         }
      }
   } while (key != ESC);

   _setcursortype(CURSOR_100);
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

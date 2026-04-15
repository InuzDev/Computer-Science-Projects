/**
 * blackjack ; Carlos David Jorge Taveras
 *
 * Archivo ; main.c
 */

#include <conio.c>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ENTER 13
#define ESC 27

#define BLACKJACK 21

#define POSXINI 1
#define POSYINI 1

#define MAXCARTAREP 11
#define MAXCARTAS 52
#define MOD_VAL 4

#define CARDSIZE 7

#define SPACE ' '

#define ESD 191
#define ESI 218
#define EID 217
#define EII 192
#define WALL 179
#define FLOOR_ROOF 196

// Funciones custom
int randrange(int, int);
void setcolor(int, int);
void colordefault(void);
// Funciones requeridas
int simbolocarta(int index);
char simbcardchr(int index);
char valcardchr(int index);
int valorcarta(int index);
int tomarcarta(int CardPack[]);
void showgame(int Player[], int nCardPlayer, int Computer[], int nCardComp, int psx, int psy);
int sumacarta(int cards[], int num);
void Interface_showcard_xy(int index, int px, int py);
void FinishShowCards_xy(int cards[], int CardAmmount, int px, int py);

int main() {
   system("cls"); // Clear the terminal before the gameplay.
   srand(time(NULL));

   int CardPack[MAXCARTAS] = {0};
   int Player[MAXCARTAREP], Computer[MAXCARTAREP];
   int indexCard = 0;
   char key;

   for (int index = 0; index < MAXCARTAS; index++) {
      if (valorcarta(index) >= 4) {
         CardPack[index] = 1;
      }
   }

   do {
      Player[indexCard] = tomarcarta(CardPack);
      Computer[indexCard] = tomarcarta(CardPack);
      showgame(Player, indexCard + 1, Computer, indexCard + 1, POSXINI, POSYINI);
      gotoxy(POSXINI, POSYINI + 20);
      printf("Desea otra carta?");

      gotoxy(POSXINI, POSXINI + 20);
      do {
         key = getch();
      } while (key != ENTER && key != ESC);
      indexCard++;
   } while (key != ESC);

   Interface_showcard_xy(14, 5, 5);

   return 0;
}

/**
 * funcion: showcart_xy
 * Argumentos: (int) index - Indice de las cartas, px - Posicion en x, py - Posicion en y
 * Objetivo: Construccion de la interfaz grafica de las cartas.
 */
void Interface_showcard_xy(int index, int px, int py) {
   char chrvalcard = valcardchr(index);
   char simbcard = simbcardchr(index);

   gotoxy(px, py);
   printf("%c%c%c%c%c%c%c", ESI, FLOOR_ROOF, FLOOR_ROOF, FLOOR_ROOF, FLOOR_ROOF, FLOOR_ROOF, ESD);
   gotoxy(px, py + 1);
   printf("%c%c%c%c%c%c%c", WALL, chrvalcard, SPACE, SPACE, SPACE, SPACE, WALL);
   gotoxy(px, py + 2);
   printf("%c%c%c%c%c%c%c", WALL, SPACE, SPACE, SPACE, SPACE, SPACE, WALL);
   gotoxy(px, py + 3);
   printf("%c%c%c%c%c%c%c", WALL, SPACE, SPACE, simbcard, SPACE, SPACE, WALL);
   gotoxy(px, py + 4);
   printf("%c%c%c%c%c%c%c", WALL, SPACE, SPACE, SPACE, SPACE, SPACE, WALL);
   gotoxy(px, py + 5);
   printf("%c%c%c%c%c%c%c", WALL, SPACE, SPACE, SPACE, SPACE, chrvalcard, WALL);
   gotoxy(px, py + 6);
   printf("%c%c%c%c%c%c%c", EII, FLOOR_ROOF, FLOOR_ROOF, FLOOR_ROOF, FLOOR_ROOF, FLOOR_ROOF, EID);
}

/**
 * Funcion: showgame
 * Argumentos: (int) Player[] - Arreglo de las cartas del jugador, nCardPlayer - Cantidad de las cartas del jugador, Computer[] - Arreglo de las cartas de la computadora, nCardcomp - Cantidad de cartas de la computadora, psx - posicion en x, psy - posicion en y
 * Objetivo: Interfaz del menu del juego, mostran la puntuacion del juego
 */
void showgame(int Player[], int nCardPlayer, int Computer[], int nCardComp, int psx, int psy) {
   gotoxy(psx + 25, psy);
   printf("BLACK JACK!");

   gotoxy(psx, psy + 1);
   printf("Jugador: %3d", sumacarta(Player, nCardPlayer));
   FinishShowCards_xy(Player, nCardPlayer, psx, psy + 2);

   gotoxy(psx, psy + 3 + CARDSIZE);
   printf("Computadora: ");
   FinishShowCards_xy(Computer, nCardComp, psx, psy + 4 + CARDSIZE);
}

/**
 * Funcion: FinishShowCards_xy
 * Objetivo:
 * Argumentos: (int) cards[], CardAmmount, px, py
 */
void FinishShowCards_xy(int cards[], int CardAmmount, int px, int py) {
   for (int index = 0; index < CardAmmount; index++) {
      Interface_showcard_xy(cards[index], px + index * CARDSIZE, py);
   }
}

/**
 * Funcion: sumacarta
 * Argumento: (int) cartas[] - Arreglo de las tarjetas, num - numero de tarjetas de cada jugador.
 * Retorna: El total de las cartas del jugador y la computadora.
 */
int sumacarta(int cards[], int num) {
   int total = 0, valcard, AmmountA = 0;

   for (int index = 0; index < num; index++) {
      valcard = valorcarta(cards[index]);

      if (valcard == 1) {
         total += 11;
         AmmountA++;
      } else {
         total += valcard;
      }
   }

   while (total > BLACKJACK && AmmountA) {
      total -= 10;
      AmmountA--;
   }

   return total;
}

/**
 * Funcion: tomarcarta
 * Argumentos: (int(array)) mazo
 * Objetivo:
 */
int tomarcarta(int CardPack[]) {
   int indice;

   do {
      indice = randrange(0, MAXCARTAS - 1);
   } while (CardPack[indice]);

   CardPack[indice] = 1;

   return indice;
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

/**
 * Función: (int) valorcarta
 * Argumentos: (int) Indice
 * Retorna: El valor de las cartas.
 */
int valorcarta(int index) {
   return (index / MOD_VAL) + 1;
}

/**
 * Funcion: (char) valcardchr
 * Argumentos: (int) indice de la tarjeta.
 * Retorna: El valor de las tarjetas dependiendo de la funcion valor carta
 */
char valcardchr(int index) {
   int valcard = valorcarta(index);

   if (valcard >= 2 && valcard <= 9) {
      return valcard + '0';
   }

   if (valcard == 1) {
      return 'A';
   }

   if (valcard == 10) {
      return 'X';
   }

   if (valcard == 11) {
      return 'J';
   }

   if (valcard == 12) {
      return 'Q';
   }

   if (valcard == 13) {
      return 'K';
   }

   return valcard;
}

/**
 * Funcion: Simbolocarta
 * Argumentos: (int) indice [Corresponde al indice del arreglo que representa el mazo]
 * Objetivo: Determinar el simbolo de la carta correspondiente al indice en el mazo.
 * Retorno: Un entero que corresponde al simbolo de la carta
 * ASCII Code: 0 ; Corazon | 1 ; Diamante | 2 ; trébol | 3 ; espadas (piques).
 */
int simbolocarta(int index) {
   return index % MOD_VAL;
}

/**
 * Funcion: (character) simbcardchr
 * Argumentos: (int) indice
 * Retorna: Simbolo de las tarjetas dependiendo el indice de este.
 */
char simbcardchr(int index) {
   int simbcard = simbolocarta(index);

   if (simbcard == 0) {
      return 'C';
   } else if (simbcard == 1) {
      return 'D';
   } else if (simbcard == 2) {
      return 'T';
   } else {
      return 'P';
   }
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

/**
 * Charles David ; PhraseGuesser
 *
 * main.c
 */

/*
 * Objetivos:
 * 1- Menu para iniciar el juego
 */

#include <conio.c>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DB_TOPIC_ID 5
#define DB_PHRASES 3
#define DB_STRING_SIZE 64

#define PHRASE_SEL_LIMINF 0
#define PHRASE_SEL_LIMSUP 2

#define TITLE_POSITION 5
#define INIT_VALUE 0
#define INIT_POSX 5
#define INIT_POSY 5

#define CURSOR_0 0
#define CURSOR_100 100

void GameInit(int UserInput);
void setcolor(int ct, int cf);
void colordefault(void);
int randrange(int limInf, int limSup);
void GetPhrase(int UserInput, char database[][DB_PHRASES][DB_STRING_SIZE]);
void GetRouletteConfig(int UserInput);

int main() {
   system("cls");
   srand(time(NULL));

   int UserInput = 0;
   // Base de datos con las frases
   char database[DB_TOPIC_ID][DB_PHRASES][DB_STRING_SIZE] = {
       {"Baseball A", "Baseball B", "Baseball C"},
       {"Basketball A", "Basketball B", "Basketball C"},
       {"Volleyball A", "Volleyball B", "Volleyball C"},
       {"Football A", "Football B", "Football C"},
       {"Soccer A", "Soccer B", "Soccer C"}};

   GameInit(UserInput);
   GetPhrase(UserInput, database);

   return 0;
}

/*
 * Funcion: GameInit
 * Objetivo: Menu de inicio del juego.
 * Argumentos: (int) *UserInput - Un valor entero que el usuario ignresa, seleccionando el topico del juego.
 */
void GameInit(int UserInput) {
   gotoxy(INIT_POSX, INIT_POSY);
   printf("-Adivinar la frase-");
   gotoxy(INIT_POSX, INIT_POSY + 2);
   printf("Elige tres topicos principales: [1- Baseball ; 2- Basketball ; 3- Voliball ; 4- Football ; 5- Soccer]");
   gotoxy(INIT_POSX, INIT_POSY + 3);

   do {
      scanf("%d", UserInput);
      if (UserInput <= 0 || UserInput > DB_TOPIC_ID) {
         gotoxy(INIT_POSX, INIT_POSY + 4);
         printf("Un valor entre 1 y 5");
         Sleep(1000);
         gotoxy(INIT_POSX, INIT_POSY + 4);
         printf("                     ");
         gotoxy(INIT_POSX, INIT_POSY + 3);
         printf("                     ");
         gotoxy(INIT_POSX, INIT_POSY + 3);
      }
   } while (UserInput < 1 || UserInput > DB_TOPIC_ID);
}

/**
 * Funcion: GetPhrase
 * Objetivo: Obtener una de las tres frases dependiendo el topico seleccionado
 * Argumentos: (int) *UserInput - Entrada de un valor entero proporcionado por el usuario.
 */
void GetPhrase(int UserInput, char database[][DB_PHRASES][DB_STRING_SIZE]) {
   if (UserInput < 1 || UserInput > DB_TOPIC_ID) {
      return;
   }

   int topic_index = UserInput - 1;
   int phrase_index = rand() % DB_PHRASES;

   printf("%s", database[topic_index][phrase_index]);
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

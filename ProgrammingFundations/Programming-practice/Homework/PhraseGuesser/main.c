/**
 *
 *
 * Charles David ; PhraseGuesser
 *
 *
 * main.c
 */
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define UI_TITLE_X 5
#define UI_TITLE_Y 5
#define UI_PHRASE_X 5
#define UI_PHRASE_Y 8
#define UI_MONEY_X 5
#define UI_MONEY_Y 10
#define UI_OPPORTUNITIES_X 5
#define UI_OPPORTUNITIES_Y 11
#define UI_HITS_X 5
#define UI_HITS_Y 13
#define UI_MISSES_X 5
#define UI_MISSES_Y 14
#define UI_ROULETTE_X 5
#define UI_ROULETTE_Y 16
#define UI_MONEY_ROULETTE_X 5
#define UI_MONEY_ROULETTE_Y 17
#define UI_CONTROLS_X 5
#define UI_CONTROLS_Y 19
#define UI_HANGMAN_X 55
#define UI_HANGMAN_Y 7

#define CLEAR_SPACE "                                                                                                                                                                                              "
#define ENTER 13
#define ESC 27
#define HIDDEN_CHAR -37

#define MAX_SIZE_ROULETTE 8
#define MIN_SIZE_ROULETTE 5
#define MAX_POINTS 100
#define MIN_POINT 25
#define MIN_POINT_VALUE 10
#define MAX_POINT_VALUE 100
#define MIN_OPORTUNITIES 3
#define MAX_OPORTUNITIES 6
#define ANIMATION_SPEED 80
#define ANIMATION_CYCLES 25

#define DB_TOPIC_ID 5
#define DB_PHRASES 3
#define DB_STRING_SIZE 128
#define ALPHA_DB 26

int PlayGame(char database[][DB_PHRASES][DB_STRING_SIZE]);
int IsLetterInDisplay(char letter, char displayed_letters[], int display_count);
int GetLetterValue(char letter);
int CountUniqueLetters(char *phrase);
int CalculateHangmanStage(int mistakes, int max_mistakes);
int GetRouletteConfig(void);
int CalculateMaxMistakes(char *phrase);
int RevealLetter(char *hidden_phrase, char *original_phrase, char letter);
int CheckWin(char *hidden_phrase);
int RemoveLetterFromAvailable(char available_letters[], int *available_count, char letter);
int CalculateRemainingLettersValue(char *hidden_phrase, char *original_phrase);

char GetMiddleLetter(char displayed_letters[], int display_count);
void DisplayUI(char *hidden_phrase, int money, int mistakes, int max_mistakes, char correct_letters[], int correct_count, char wrong_letters[], int wrong_count, char displayed_letters[], int display_count, int available_count, int money_roulette[], int money_roulette_size, int can_buy);
void TrackLetter(char letter, char correct_letters[], int *correct_count, char wrong_letters[], int *wrong_count, int isCorrect);
void DisplayHangman(int stage);
void PopulateMoneyRoulette(int money_roulette[], int *money_roulette_size);
void InitializeAlphabet(char available_letters[], int *available_count);
void SelectDisplayedLetters(char available_letters[], int available_count, char displayed_letters[], int display_size);
void AnimateLetterRoulette(char displayed_letters[], int display_count, char *selected_letter);
void AnimateMoneyRoulette(int money_roulette[], int money_roulette_size, int *selected_value);
void GameInit(int *UserInput);
void DisplayGame(int UserInput, char database[][DB_PHRASES][DB_STRING_SIZE], int *max_mistakes, char **selected_phrase, char *original_phrase);
void HidePhrase(char *phrase);
void setcolor(int ct, int cf);
void colordefault(void);

int main() {
   _setcursortype(0);
   srand(time(NULL));

   char database[DB_TOPIC_ID][DB_PHRASES][DB_STRING_SIZE] = {
       {"Baseball Game", "Home Run Derby", "World Series Win"},
       {"Basketball Match", "Three Point Shot", "Championship Game"},
       {"Volleyball Serve", "Beach Volleyball", "Olympic Gold Medal"},
       {"Football Touchdown", "Super Bowl Victory", "Quarterback Pass"},
       {"Soccer Goal Kick", "World Cup Final", "Penalty Shootout"}};

   int games_played = 0;
   int games_won = 0;
   int games_lost = 0;
   int play_again = 1;

   while (play_again) {
      int result = PlayGame(database);

      if (result != -1) {
         games_played++;
         if (result == 1) {
            games_won++;
         } else {
            games_lost++;
         }
      }

      system("cls");
      setcolor(YELLOW, BLACK);
      printf("Quieres jugar de nuevo? (S/N): ");
      colordefault();
      char choice = getch();

      if (choice != 's' && choice != 'S') {
         play_again = 0;
      }
   }

   system("cls");
   setcolor(LIGHTCYAN, BLACK);
   printf("Estad%csticas finales\n", 161);
   colordefault();
   printf("\n  Juegos jugados: ");
   setcolor(YELLOW, BLACK);
   printf("%d\n", games_played);
   colordefault();
   printf("  Juegos ganados: ");
   setcolor(LIGHTGREEN, BLACK);
   printf("%d\n", games_won);
   colordefault();
   printf("  Juegos perdidos: ");
   setcolor(LIGHTRED, BLACK);
   printf("%d\n", games_lost);
   colordefault();
   printf("Gracias por jugar!\n\n");

   _setcursortype(100);
   return 0;
}

/**
 * Funcion: PlayGame
 * Argumentos: database - arreglo 3D que contiene las frases de los tópicos del juego
 * Objetivo: Bucle principal de la lógica del juego que maneja la selección de letras, giros de ruleta y condiciones de victoria/derrota
 * return: 1 si ganó, 0 si perdió, -1 si salió con ESC
 */
int PlayGame(char database[][DB_PHRASES][DB_STRING_SIZE]) {
   int UserInput = 0;
   int max_mistakes;
   char *selected_phrase;
   char original_phrase[DB_STRING_SIZE];
   int mistakes = 0;
   int money = 0;

   char available_letters[ALPHA_DB];
   int available_count;
   char displayed_letters[MAX_SIZE_ROULETTE];
   int display_size;

   int money_roulette[MAX_POINTS];
   int money_roulette_size;

   char correct_letters[ALPHA_DB];
   int correct_count = 0;
   char wrong_letters[ALPHA_DB];
   int wrong_count = 0;

   system("cls");
   GameInit(&UserInput);
   display_size = GetRouletteConfig();
   system("cls");

   DisplayGame(UserInput, database, &max_mistakes, &selected_phrase, original_phrase);

   InitializeAlphabet(available_letters, &available_count);
   SelectDisplayedLetters(available_letters, available_count, displayed_letters, display_size);
   PopulateMoneyRoulette(money_roulette, &money_roulette_size);

   int game_over = 0;
   int won = 0;

   while (!game_over) {
      int remaining_value = CalculateRemainingLettersValue(selected_phrase, original_phrase);
      int buy_cost = remaining_value / 2;
      int can_buy = (money >= buy_cost && remaining_value > 0) ? 1 : 0;

      DisplayUI(selected_phrase, money, mistakes, max_mistakes,
                correct_letters, correct_count, wrong_letters, wrong_count,
                displayed_letters, display_size, available_count,
                money_roulette, money_roulette_size, can_buy);

      gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
      setcolor(LIGHTCYAN, BLACK);
      printf("Presione [ENTER]: ");
      colordefault();

      int key = getch();

      if (key == ESC) {
         return -1;
      } else if (key == ENTER) {
         if (available_count == 0) {
            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
            setcolor(LIGHTRED, BLACK);
            printf("No quedan letras disponibles.%s", CLEAR_SPACE);
            colordefault();
            Sleep(2000);
            continue;
         }

         SelectDisplayedLetters(available_letters, available_count, displayed_letters, display_size);

         char spun_letter;
         AnimateLetterRoulette(displayed_letters, display_size, &spun_letter);

         if (!IsLetterInDisplay(spun_letter, displayed_letters, display_size)) {
            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
            setcolor(LIGHTRED, BLACK);
            printf("ERROR: La letra no est%c en la ruleta%s", 160, CLEAR_SPACE);
            colordefault();
            Sleep(2000);
            continue;
         }

         RemoveLetterFromAvailable(available_letters, &available_count, spun_letter);

         int revealed = RevealLetter(selected_phrase, original_phrase, spun_letter);

         if (revealed > 0) {
            TrackLetter(spun_letter, correct_letters, &correct_count,
                        wrong_letters, &wrong_count, 1);

            int money_won;
            AnimateMoneyRoulette(money_roulette, money_roulette_size, &money_won);
            money += money_won;

            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
            setcolor(LIGHTGREEN, BLACK);
            printf("Acerto! Letra '%c' revelada %d veces - +$%d%s",
                   spun_letter, revealed, money_won, CLEAR_SPACE);
            colordefault();
            Sleep(2000);
         } else {
            TrackLetter(spun_letter, correct_letters, &correct_count,
                        wrong_letters, &wrong_count, 0);
            mistakes++;

            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
            setcolor(LIGHTRED, BLACK);
            printf("Fallo, La letra '%c' no est%c en la frase%s", spun_letter, 160, CLEAR_SPACE);
            colordefault();
            Sleep(2000);
         }

         gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
         printf("%s", CLEAR_SPACE);

         if (CheckWin(selected_phrase)) {
            game_over = 1;
            won = 1;
         }

         if (mistakes >= max_mistakes) {
            game_over = 1;
            won = 0;
         }
      } else if ((key == 'b' || key == 'B') && can_buy) {
         gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
         setcolor(YELLOW, BLACK);
         printf("Comprar frase por $%d? (S/N): ", buy_cost);
         colordefault();

         char buy_choice = getch();

         if (buy_choice == 's' || buy_choice == 'S') {
            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
            printf("Ingresa la frase completa: %s", CLEAR_SPACE);
            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 4);

            char user_guess[DB_STRING_SIZE];
            fflush(stdin);
            fgets(user_guess, DB_STRING_SIZE, stdin);
            user_guess[strcspn(user_guess, "\n")] = 0;

            if (strcmp(user_guess, original_phrase) == 0) {
               int bonus = remaining_value * 2;
               money += bonus;
               gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
               setcolor(YELLOW, BLACK);
               printf("Excelente. Ganaste $%d adicionales!%s", bonus, CLEAR_SPACE);
               colordefault();
               Sleep(2500);
               game_over = 1;
               won = 1;
            } else {
               money -= buy_cost;
               int lost_opportunities = max_mistakes - mistakes;
               mistakes += lost_opportunities / 2;

               gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
               setcolor(LIGHTRED, BLACK);
               printf("Incorrecto. Perdiste $%d y %d oportunidades%s",
                      buy_cost, lost_opportunities / 2, CLEAR_SPACE);
               colordefault();
               Sleep(2500);

               if (mistakes >= max_mistakes) {
                  game_over = 1;
                  won = 0;
               }
            }

            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 3);
            printf("%s", CLEAR_SPACE);
            gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 4);
            printf("%s", CLEAR_SPACE);
         }
      }
   }

   system("cls");
   if (won) {
      setcolor(YELLOW, BLACK);
      printf("GANASTE!\n");
      colordefault();
      printf("\n  Frase: ");
      setcolor(LIGHTCYAN, BLACK);
      printf("%s\n", original_phrase);
      colordefault();
      printf("Dinero ganado: ");
      setcolor(LIGHTGREEN, BLACK);
      printf("$%d\n", money);
      colordefault();
   } else {
      setcolor(LIGHTRED, BLACK);
      printf("PERDISTE");
      colordefault();
      printf("Frase correcta: ");
      setcolor(LIGHTCYAN, BLACK);
      printf("%s\n", original_phrase);
      colordefault();
      printf("Dinero perdido: ");
      setcolor(LIGHTRED, BLACK);
      printf("$%d\n", money);
      colordefault();
   }

   printf("Presiona cualquier tecla para continuar...");
   getch();

   return won ? 1 : 0;
}

/**
 * Funcion: GetMiddleLetter
 * Argumentos: displayed_letters - arreglo de letras mostradas
 *             display_count - número de letras en el arreglo
 * Objetivo: Retorna la letra del medio del arreglo de letras mostradas
 * return: El carácter en la posición del medio
 */
char GetMiddleLetter(char displayed_letters[], int display_count) {
   int middle_index = display_count / 2;
   return displayed_letters[middle_index];
}

/**
 * Funcion: IsLetterInDisplay
 * Argumentos: letter - la letra a buscar
 *             displayed_letters - arreglo de letras mostradas
 *             display_count - número de letras mostradas
 * Objetivo: Verifica si una letra específica existe en el arreglo de letras mostradas
 * return: 1 si la letra fue encontrada, 0 en caso contrario
 */
int IsLetterInDisplay(char letter, char displayed_letters[], int display_count) {
   for (int ind = 0; ind < display_count; ind++) {
      if (displayed_letters[ind] == letter) {
         return 1;
      }
   }
   return 0;
}

/**
 * Funcion: AnimateLetterRoulette
 * Argumentos: displayed_letters - arreglo de letras para animar
 *             display_count - número de letras en la ruleta
 *             selected_letter - puntero para almacenar la letra final seleccionada
 * Objetivo: Anima la ruleta de letras y selecciona una letra aleatoriamente
 */
void AnimateLetterRoulette(char displayed_letters[], int display_count, char *selected_letter) {
   char temp[MAX_SIZE_ROULETTE];

   for (int ind = 0; ind < display_count; ind++) {
      temp[ind] = displayed_letters[ind];
   }

   for (int cycle = 0; cycle < ANIMATION_CYCLES; cycle++) {
      char first = temp[0];
      for (int ind = 0; ind < display_count - 1; ind++) {
         temp[ind] = temp[ind + 1];
      }
      temp[display_count - 1] = first;

      gotoxy(UI_ROULETTE_X, UI_ROULETTE_Y);
      setcolor(YELLOW, BLACK);
      printf("Ruleta: [ ");
      for (int ind = 0; ind < display_count; ind++) {
         if (ind == display_count / 2) {
            setcolor(LIGHTRED, BLACK);
            printf(">%c< ", temp[ind]);
            setcolor(YELLOW, BLACK);
         } else {
            printf("%c ", temp[ind]);
         }
      }
      printf("]%s", CLEAR_SPACE);
      colordefault();

      Sleep(ANIMATION_SPEED);
   }

   *selected_letter = temp[display_count / 2];

   gotoxy(UI_ROULETTE_X, UI_ROULETTE_Y);
   setcolor(LIGHTGREEN, BLACK);
   printf("Letra seleccionada: %c%s", *selected_letter, CLEAR_SPACE);
   colordefault();
   Sleep(800);
}

/**
 * Funcion: AnimateMoneyRoulette
 * Argumentos: money_roulette - arreglo de valores de dinero
 *             money_roulette_size - tamaño del arreglo de dinero
 *             selected_value - puntero para almacenar el valor de dinero seleccionado
 * Objetivo: Anima la ruleta de dinero y selecciona un valor aleatoriamente
 */
void AnimateMoneyRoulette(int money_roulette[], int money_roulette_size, int *selected_value) {
   int current_index = 0;

   for (int cycle = 0; cycle < ANIMATION_CYCLES; cycle++) {
      current_index = rand() % money_roulette_size;

      gotoxy(UI_MONEY_ROULETTE_X, UI_MONEY_ROULETTE_Y);
      setcolor(LIGHTCYAN, BLACK);
      printf("Dinero: [ ");

      int start = (current_index > 2) ? current_index - 2 : 0;
      for (int ind = 0; ind < 5 && start + ind < money_roulette_size; ind++) {
         int idx = start + ind;
         if (idx == current_index) {
            setcolor(YELLOW, BLACK);
            printf(">$%d< ", money_roulette[idx]);
            setcolor(LIGHTCYAN, BLACK);
         } else {
            printf("$%d ", money_roulette[idx]);
         }
      }
      printf("]%s", CLEAR_SPACE);
      colordefault();

      Sleep(ANIMATION_SPEED);
   }

   *selected_value = money_roulette[current_index];

   gotoxy(UI_MONEY_ROULETTE_X, UI_MONEY_ROULETTE_Y);
   setcolor(LIGHTGREEN, BLACK);
   printf("Dinero ganado: $%d%s", *selected_value, CLEAR_SPACE);
   colordefault();
   Sleep(800);
}

/**
 * Funcion: InitializeAlphabet
 * Argumentos: available_letters - arreglo para almacenar el alfabeto
 *             available_count - puntero para almacenar el conteo de letras
 * Objetivo: Inicializa el arreglo de letras disponibles con el alfabeto en mayúsculas (A-Z)
 */
void InitializeAlphabet(char available_letters[], int *available_count) {
   char letters[ALPHA_DB] = {
       'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
       'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
       'u', 'v', 'w', 'x', 'y', 'z'};

   for (int i = 0; i < ALPHA_DB; i++) {
      available_letters[i] = letters[i];
   }
   *available_count = ALPHA_DB;
}

/**
 * Funcion: SelectDisplayedLetters
 * Argumentos: available_letters - arreglo de letras disponibles para elegir
 *             available_count - número de letras disponibles
 *             displayed_letters - arreglo para almacenar las letras seleccionadas para mostrar
 *             display_size - número de letras a seleccionar para mostrar
 * Objetivo: Selecciona aleatoriamente letras del conjunto disponible para mostrar en la ruleta
 */
void SelectDisplayedLetters(char available_letters[], int available_count, char displayed_letters[], int display_size) {
   int actual_display = (display_size > available_count) ? available_count : display_size;

   char temp[ALPHA_DB];
   for (int ind = 0; ind < available_count; ind++) {
      temp[ind] = available_letters[ind];
   }
   int temp_count = available_count;

   for (int ind = 0; ind < actual_display; ind++) {
      int random_index = rand() % temp_count;
      displayed_letters[ind] = temp[random_index];

      temp[random_index] = temp[temp_count - 1];
      temp_count--;
   }
}

/**
 * Funcion: RemoveLetterFromAvailable
 * Argumentos: available_letters - arreglo de letras disponibles
 *             available_count - puntero al conteo de letras disponibles
 *             letter - la letra a eliminar
 * Objetivo: Elimina una letra específica del conjunto de letras disponibles
 * return: 1 si la letra fue encontrada y eliminada, 0 en caso contrario
 */
int RemoveLetterFromAvailable(char available_letters[], int *available_count, char letter) {
   for (int ind = 0; ind < *available_count; ind++) {
      if (available_letters[ind] == letter) {
         available_letters[ind] = available_letters[(*available_count) - 1];
         (*available_count)--;
         return 1;
      }
   }
   return 0;
}

/**
 * Funcion: RevealLetter
 * Argumentos: hidden_phrase - puntero a la frase con caracteres ocultos
 *             original_phrase - puntero a la frase original sin modificar
 *             letter - la letra a revelar en la frase oculta
 * Objetivo: Revela todas las ocurrencias de una letra en la frase oculta
 * return: Número de veces que la letra fue revelada
 */
int RevealLetter(char *hidden_phrase, char *original_phrase, char letter) {
   int count = 0;
   char lower_letter = (letter >= 'A' && letter <= 'Z') ? letter + 32 : letter;
   char upper_letter = (letter >= 'a' && letter <= 'z') ? letter - 32 : letter;

   for (int ind = 0; original_phrase[ind] != '\0'; ind++) {
      if (original_phrase[ind] == lower_letter || original_phrase[ind] == upper_letter) {
         hidden_phrase[ind] = original_phrase[ind];
         count++;
      }
   }
   return count;
}

/**
 * Funcion: DisplayUI
 * Argumentos: hidden_phrase - estado actual de la frase que se está adivinando
 *             money - dinero actual del jugador
 *             mistakes - número de errores cometidos
 *             max_mistakes - máximo de errores permitidos
 *             correct_letters - arreglo de letras adivinadas correctamente
 *             correct_count - número de letras correctas
 *             wrong_letters - arreglo de letras incorrectas adivinadas
 *             wrong_count - número de letras incorrectas
 *             displayed_letters - letras mostradas en la ruleta
 *             display_count - número de letras en la ruleta
 *             available_count - número de letras disponibles restantes
 *             money_roulette - arreglo de valores de dinero en la ruleta
 *             money_roulette_size - tamaño de la ruleta de dinero
 *             can_buy - bandera que indica si el jugador puede comprar una letra
 * Objetivo: Muestra la interfaz completa del juego incluyendo frase, estadísticas y ahorcado
 */
void DisplayUI(char *hidden_phrase, int money, int mistakes, int max_mistakes,
               char correct_letters[], int correct_count, char wrong_letters[], int wrong_count,
               char displayed_letters[], int display_count, int available_count,
               int money_roulette[], int money_roulette_size, int can_buy) {

   gotoxy(UI_TITLE_X, UI_TITLE_Y);
   setcolor(YELLOW, BLACK);
   printf("ADIVINA LA FRASE");
   colordefault();

   gotoxy(UI_PHRASE_X, UI_PHRASE_Y);
   setcolor(LIGHTCYAN, BLACK);
   printf("Frase: ");
   setcolor(WHITE, BLACK);
   printf("%s%s", hidden_phrase, CLEAR_SPACE);
   colordefault();

   gotoxy(UI_MONEY_X, UI_MONEY_Y);
   printf("Dinero: ");
   setcolor(LIGHTGREEN, BLACK);
   printf("$%d%s", money, CLEAR_SPACE);
   colordefault();

   gotoxy(UI_OPPORTUNITIES_X, UI_OPPORTUNITIES_Y);
   printf("Errores: ");
   if (mistakes > max_mistakes / 2) {
      setcolor(LIGHTRED, BLACK);
   } else {
      setcolor(YELLOW, BLACK);
   }
   printf("%d/%d", mistakes, max_mistakes);
   colordefault();
   printf("- Letras: ");
   setcolor(LIGHTCYAN, BLACK);
   printf("%d restantes%s", available_count, CLEAR_SPACE);
   colordefault();

   gotoxy(UI_HITS_X, UI_HITS_Y);
   setcolor(LIGHTGREEN, BLACK);
   printf("- Aciertos (%d): ", correct_count);
   colordefault();
   for (int ind = 0; ind < correct_count; ind++) {
      printf("%c%s", correct_letters[ind], (ind < correct_count - 1) ? "," : "");
   }
   printf("%s", CLEAR_SPACE);

   gotoxy(UI_MISSES_X, UI_MISSES_Y);
   setcolor(LIGHTRED, BLACK);
   printf("- Fallos (%d): ", wrong_count);
   colordefault();
   for (int ind = 0; ind < wrong_count; ind++) {
      printf("%c%s", wrong_letters[ind], (ind < wrong_count - 1) ? "," : "");
   }
   printf("%s", CLEAR_SPACE);

   gotoxy(UI_ROULETTE_X, UI_ROULETTE_Y);
   setcolor(YELLOW, BLACK);
   printf("Ruleta: [ ");
   for (int ind = 0; ind < display_count; ind++) {
      if (ind == display_count / 2) {
         setcolor(LIGHTRED, BLACK);
         printf(">%c< ", displayed_letters[ind]);
         setcolor(YELLOW, BLACK);
      } else {
         printf("%c ", displayed_letters[ind]);
      }
   }
   printf("]%s", CLEAR_SPACE);
   colordefault();

   gotoxy(UI_MONEY_ROULETTE_X, UI_MONEY_ROULETTE_Y);
   setcolor(LIGHTCYAN, BLACK);
   printf("Dinero: [ ");
   for (int i = 0; i < 5 && i < money_roulette_size; i++) {
      printf("$%d ", money_roulette[i]);
   }
   printf("]%s", CLEAR_SPACE);
   colordefault();

   gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y);
   setcolor(LIGHTGRAY, BLACK);
   gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 1);
   setcolor(LIGHTGREEN, BLACK);
   printf("ENTER");
   setcolor(LIGHTGRAY, BLACK);
   printf(": Girar ruleta");
   if (can_buy) {
      setcolor(YELLOW, BLACK);
      printf("B");
      setcolor(LIGHTGRAY, BLACK);
      printf(": Comprar");
   } else {
      printf("               ");
   }
   setcolor(LIGHTRED, BLACK);
   printf("ESC");
   setcolor(LIGHTGRAY, BLACK);
   printf(": Salir");
   gotoxy(UI_CONTROLS_X, UI_CONTROLS_Y + 2);
   colordefault();

   DisplayHangman(CalculateHangmanStage(mistakes, max_mistakes));
}

/**
 * Funcion: TrackLetter
 * Argumentos: letter - la letra a registrar
 *             correct_letters - arreglo para almacenar letras correctas
 *             correct_count - puntero al conteo de letras correctas
 *             wrong_letters - arreglo para almacenar letras incorrectas
 *             wrong_count - puntero al conteo de letras incorrectas
 *             isCorrect - bandera que indica si la letra fue correcta (1) o incorrecta (0)
 * Objetivo: Agrega una letra a la lista de letras correctas o incorrectas
 */
void TrackLetter(char letter, char correct_letters[], int *correct_count, char wrong_letters[], int *wrong_count, int isCorrect) {
   char *target_array = isCorrect ? correct_letters : wrong_letters;
   int *target_count = isCorrect ? correct_count : wrong_count;

   for (int ind = 0; ind < *target_count; ind++) {
      if (target_array[ind] == letter) {
         return;
      }
   }

   target_array[*target_count] = letter;
   (*target_count)++;
}

/**
 * Funcion: CountUniqueLetters
 * Argumentos: phrase - puntero a la cadena de la frase
 * Objetivo: Cuenta el número de letras alfabéticas únicas en una frase
 * return: Conteo de letras únicas (sin distinguir mayúsculas/minúsculas)
 */
int CountUniqueLetters(char *phrase) {
   char found_letters[ALPHA_DB];
   int count = 0;

   for (int ind = 0; phrase[ind] != '\0'; ind++) {
      char current = phrase[ind];

      if ((current >= 'A' && current <= 'Z') || (current >= 'a' && current <= 'z')) {
         char lower = (current >= 'A' && current <= 'Z') ? current + 32 : current;

         int already_found = 0;
         for (int jind = 0; jind < count; jind++) {
            if (found_letters[jind] == lower) {
               already_found = 1;
               break;
            }
         }

         if (!already_found) {
            found_letters[count] = lower;
            count++;
         }
      }
   }
   return count;
}

/**
 * Funcion: CalculateHangmanStage
 * Argumentos: mistakes - número actual de errores
 *             max_mistakes - máximo de errores permitidos
 * Objetivo: Calcula qué etapa del ahorcado debe mostrarse basándose en la proporción de errores
 * return: Etapa del ahorcado de 0 a 6
 */
int CalculateHangmanStage(int mistakes, int max_mistakes) {
   if (max_mistakes == 0)
      return 0;
   float ratio = (float)mistakes / max_mistakes;
   int stage = (int)(ratio * 6);
   return (stage > 6) ? 6 : stage;
}

void DisplayHangman(int stage) {
   int x = UI_HANGMAN_X;
   int y = UI_HANGMAN_Y;

   if (stage >= 3) {
      setcolor(LIGHTRED, BLACK);
   } else if (stage >= 1) {
      setcolor(YELLOW, BLACK);
   } else {
      setcolor(LIGHTGRAY, BLACK);
   }

   gotoxy(x, y);
   printf("  +---+   ");
   gotoxy(x, y + 1);
   printf("  |   |   ");
   gotoxy(x, y + 2);
   printf("  %c   |   ", (stage >= 1) ? 'O' : ' ');
   gotoxy(x, y + 3);
   printf(" %c%c%c  |   ", (stage >= 3) ? '/' : ' ', (stage >= 2) ? '|' : ' ', (stage >= 4) ? '\\' : ' ');
   gotoxy(x, y + 4);
   printf(" %c %c  |   ", (stage >= 5) ? '/' : ' ', (stage >= 6) ? '\\' : ' ');
   gotoxy(x, y + 5);
   printf("      |   ");
   gotoxy(x, y + 6);
   printf("=========");
   colordefault();
}

/**
 * Funcion: GetRouletteConfig
 * Argumentos: Ninguno
 * Objetivo: Solicita al usuario configurar el tamaño de la ruleta dentro del rango válido
 * return: El tamaño de ruleta configurado
 */
int GetRouletteConfig(void) {
   int roulette_size;

   setcolor(LIGHTCYAN, BLACK);
   gotoxy(5, 5);
   printf("Configuraci%cn de la ruleta", 162);
   colordefault();

   gotoxy(5, 9);
   printf("Cantidad de letras a mostrar (%d-%d): ", MIN_SIZE_ROULETTE, MAX_SIZE_ROULETTE);

   do {
      scanf("%d", &roulette_size);
      if (roulette_size < MIN_SIZE_ROULETTE || roulette_size > MAX_SIZE_ROULETTE) {
         gotoxy(40, 9);
         setcolor(LIGHTRED, BLACK);
         printf("Invalido!%s", CLEAR_SPACE);
         colordefault();
         Sleep(1000);
         gotoxy(40, 9);
         printf("%s", CLEAR_SPACE);
         gotoxy(40, 9);
      }
   } while (roulette_size < MIN_SIZE_ROULETTE || roulette_size > MAX_SIZE_ROULETTE);

   return roulette_size;
}

/**
 * Funcion: PopulateMoneyRoulette
 * Argumentos: money_roulette - arreglo para almacenar valores de dinero
 *             money_roulette_size - puntero para almacenar el tamaño de la ruleta
 * Objetivo: Llena la ruleta de dinero con valores aleatorios únicos
 */
void PopulateMoneyRoulette(int money_roulette[], int *money_roulette_size) {
   *money_roulette_size = (rand() % (MAX_POINTS - MIN_POINT + 1)) + MIN_POINT;

   for (int ind = 0; ind < *money_roulette_size; ind++) {
      int already_used;
      int random_value;

      do {
         already_used = 0;
         random_value = (rand() % (MAX_POINT_VALUE - MIN_POINT_VALUE + 1)) + MIN_POINT_VALUE;

         for (int jind = 0; jind < ind; jind++) {
            if (money_roulette[jind] == random_value) {
               already_used = 1;
               break;
            }
         }
      } while (already_used);

      money_roulette[ind] = random_value;
   }
}

/**
 * Funcion: GameInit
 * Argumentos: UserInput - puntero para almacenar la selección de tópico del usuario
 * Objetivo: Muestra el menú de selección de tópicos y valida la entrada del usuario
 */
void GameInit(int *UserInput) {
   setcolor(YELLOW, BLACK);
   gotoxy(5, 3);
   printf("ADIVINA LA FRASE");

   colordefault();
   gotoxy(5, 7);
   printf("Elige un t%cpico:", 162);

   setcolor(LIGHTBLUE, BLACK);
   gotoxy(7, 9);
   printf("1 ");
   colordefault();
   printf("Baseball");

   setcolor(BROWN, BLACK);
   gotoxy(7, 10);
   printf("2 ");
   colordefault();
   printf("Basketball");

   setcolor(YELLOW, BLACK);
   gotoxy(7, 11);
   printf("3 ");
   colordefault();
   printf("Volleyball");

   setcolor(LIGHTGREEN, BLACK);
   gotoxy(7, 12);
   printf("4 ");
   colordefault();
   printf("Football");

   setcolor(LIGHTRED, BLACK);
   gotoxy(7, 13);
   printf("5 ");
   colordefault();
   printf("Soccer");

   gotoxy(30, 9);
   setcolor(LIGHTCYAN, BLACK);
   printf("Tu elecci%cn: ", 162);
   colordefault();

   do {
      scanf("%d", UserInput);
      if (*UserInput <= 0 || *UserInput > DB_TOPIC_ID) {
         gotoxy(43, 9);
         setcolor(LIGHTRED, BLACK);
         printf("¡Entre 1 y 5!");
         colordefault();
         Sleep(1000);
         gotoxy(43, 9);
         printf("%s", CLEAR_SPACE);
         gotoxy(43, 9);
      }
   } while (*UserInput < 1 || *UserInput > DB_TOPIC_ID);
   system("cls");
}

/**
 * Funcion: GameInit
 * Argumentos: UserInput - puntero para almacenar la selección de tópico del usuario
 * Objetivo: Muestra el menú de selección de tópicos y valida la entrada del usuario
 */
void HidePhrase(char *phrase) {
   for (int ind = 0; phrase[ind] != '\0'; ind++) {
      if ((phrase[ind] >= 'A' && phrase[ind] <= 'Z') || (phrase[ind] >= 'a' && phrase[ind] <= 'z')) {
         phrase[ind] = HIDDEN_CHAR;
      }
   }
}

/**
 * Funcion: CalculateMaxMistakes
 * Argumentos: phrase - puntero a la cadena de la frase
 * Objetivo: Calcula el máximo de errores permitidos basándose en la complejidad de la frase
 * return: Número máximo de errores permitidos
 */
int CalculateMaxMistakes(char *phrase) {
   int unique_letters = CountUniqueLetters(phrase);
   int Oportunities_Rand = (rand() % (MAX_OPORTUNITIES - MIN_OPORTUNITIES + 1)) + MIN_OPORTUNITIES;
   return unique_letters * Oportunities_Rand;
}

/**
 * Funcion: DisplayGame
 * Argumentos: UserInput - el tópico seleccionado por el usuario (1-5)
 *             database - arreglo 3D que contiene todas las frases
 *             max_mistakes - puntero para almacenar el máximo de errores calculado
 *             selected_phrase - puntero para almacenar la frase seleccionada
 *             original_phrase - buffer para almacenar la copia de la frase original
 * Objetivo: Inicializa el estado del juego seleccionando una frase aleatoria y configurando la visualización
 */
void DisplayGame(int UserInput, char database[][DB_PHRASES][DB_STRING_SIZE], int *max_mistakes, char **selected_phrase, char *original_phrase) {
   int topic_index = UserInput - 1;
   int random_phrase = rand() % DB_PHRASES;

   *selected_phrase = database[topic_index][random_phrase];
   strcpy(original_phrase, *selected_phrase);
   *max_mistakes = CalculateMaxMistakes(*selected_phrase);

   HidePhrase(*selected_phrase);
   DisplayHangman(0);
}

/**
 * Funcion: CheckWin
 * Argumentos: hidden_phrase - puntero al estado actual de la frase
 * Objetivo: Verifica si la frase ha sido completamente revelada
 * return: 1 si todas las letras están reveladas, 0 en caso contrario
 */
int CheckWin(char *hidden_phrase) {
   for (int ind = 0; hidden_phrase[ind] != '\0'; ind++) {
      if (hidden_phrase[ind] == HIDDEN_CHAR) {
         return 0;
      }
   }
   return 1;
}

/**
 * Funcion: GetLetterValue
 * Argumentos: letter - la letra a evaluar
 * Objetivo: Calcula el valor en puntos de una letra basándose en reglas predefinidas
 * return: Valor en puntos de la letra
 */
int GetLetterValue(char letter) {
   int is_uppercase = (letter >= 'A' && letter <= 'Z');
   char upper = is_uppercase ? letter : letter - 32;

   if (upper == 'C' || upper == 'E' || upper == 'J' || upper == 'K' || upper == 'N') {
      return is_uppercase ? 8 : 5;
   } else if (upper == 'B' || upper == 'M' || upper == 'R' || upper == 'W' || upper == 'Z' || upper == 'U') {
      return is_uppercase ? 10 : 15;
   } else if (upper == 'A' || upper == 'G' || upper == 'P' || upper == 'S' || upper == 'V') {
      return is_uppercase ? 25 : 20;
   } else if (upper == 'D' || upper == 'I' || upper == 'L' || upper == 'Q' || upper == 'X') {
      return is_uppercase ? 18 : 30;
   } else if (upper == 'F' || upper == 'H' || upper == 'O' || upper == 'T' || upper == 'Y') {
      return is_uppercase ? 10 : 40;
   }
   return 0;
}

/**
 * Funcion: CalculateRemainingLettersValue
 * Argumentos: hidden_phrase - estado actual de la frase con caracteres ocultos
 *             original_phrase - la frase original sin modificar
 * Objetivo: Calcula el valor total en puntos de todas las letras ocultas restantes
 * return: Valor total de las letras no reveladas
 */
int CalculateRemainingLettersValue(char *hidden_phrase, char *original_phrase) {
   int total_value = 0;
   for (int i = 0; original_phrase[i] != '\0'; i++) {
      if (hidden_phrase[i] == HIDDEN_CHAR) {
         total_value += GetLetterValue(original_phrase[i]);
      }
   }
   return total_value;
}

/**
 * Funcion: setcolor
 * Argumentos: ct - código de color del texto
 *             cf - código de color del fondo
 * Objetivo: Establece los colores del texto y fondo de la consola
 */
void setcolor(int ct, int cf) {
   textcolor(ct);
   textbackground(cf);
}

/**
 * Funcion: colordefault
 * Objetivo: Restablece los colores de la consola a los valores predeterminados (gris claro sobre negro)
 */
void colordefault(void) {
   setcolor(LIGHTGRAY, BLACK);
}

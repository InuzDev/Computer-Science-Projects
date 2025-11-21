/**
 * Archivo: Main.c
 *
 * Carlos David Jorge Taveras ; 11/20/2025
 *
 * Objetivo del programa: Verificar que el ID de una persona sea correcto y valido.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ASCII_ZERO 48

#define GENERATE_MODE 'G'
#define INPUT_MODE 'D'
#define EVEN_NUM 6
#define ODD_NUM 6
#define SERIALSEQUENCY 11
#define MAX_INPUT 14
#define ID_MAXCHAR_TYPEA 12
#define ID_MAXCHAR_TYPEB 14

void GetSequencyAndSerial(char ID[], char result[]);
int SumNumDig(int num);
int SumOddDigits(char Digits[]);
int MultiplyEvenPairs(char Digits[]);
int CalcVerifierDig(char ID[]);
void GenerateRandomID(int Quantity);
void CleanBuffer();

int main()
{
   srand(time(NULL));

   int serial = 0, sequency = 0, VerifierDigit = 0, Quantity = 0;
   char mode, QuitPrompt;
   char identityDocumentA[ID_MAXCHAR_TYPEA], identityDocumentB[ID_MAXCHAR_TYPEB];
   char StringPrompt[MAX_INPUT];

   while (1)
   {
      while (1)
      {
         printf("Quiere introducir una c%cdula %c generar una cantidad de c%cdulas?: [G]enerar, [D]igitar\n", 130, 162, 130);
         scanf("%c", &mode);
         mode = toupper(mode);

         CleanBuffer();
         if (mode != GENERATE_MODE && mode != INPUT_MODE)
         {
            printf("Error, ingrese [G] si es Generar c%cdulas validas, o [D] si es digitar una c%cdula\n\n", 130, 130);
            continue;
         }
         break;
      }

      if (toupper(mode) == INPUT_MODE)
      {
         while (1)
         {
            printf("Ingrese su cedula: ");
            fgets(StringPrompt, sizeof(StringPrompt), stdin);

            CleanBuffer();

            StringPrompt[strcspn(StringPrompt, "\n")] = '\0';

            if (sscanf(StringPrompt, "%3d-%7d-%1d", &serial, &sequency, &VerifierDigit) == 3)
            {
               break;
            }
            else
            {
               printf("Formato invalido. Por, favor. Ingrese este formado: 000-0000000-0\n");
            }
         }

         int CalcVerifier = CalcVerifierDig(StringPrompt);
         int RealVerifier = StringPrompt[strlen(StringPrompt) - 1] - '0';

         if (CalcVerifier == RealVerifier)
         {
            printf("C%cdula v%clida\n", 130, 160);
         }
         else
         {
            printf("C%cdula inv%clida. El digito verificador:%d\n", 130, 160, RealVerifier);
         }
      }
      else if (toupper(mode) == GENERATE_MODE)
      {
         printf("Cuantas c%cdulas desea generar?: ", 130);
         scanf("%d", &Quantity);

         CleanBuffer();

         GenerateRandomID(Quantity);
      }

      while (1)
      {
         printf("\n\nDesea seguir generando %c digitando c%cdulas?: [S]i [N]o\n", 162, 130);
         scanf(" %c", &QuitPrompt);

         QuitPrompt = toupper(QuitPrompt);
         CleanBuffer();

         if (QuitPrompt != 'S' && QuitPrompt != 'N')
         {
            printf("Debe ingresar [S] o [N] para decir Si, o No\n");
            continue;
         }

         if (QuitPrompt == 'S')
         {
            break;
         }
         else if (QuitPrompt == 'N')
         {
            return 0;
         }
      }
   }
}

/**
 * Función: GenerateRandomID
 * Argumentos: (int) Quantity, cantidad de IDs a generar
 * Objetivo: Generar una cantidad X (Quantity) de cédulas.
 */
void GenerateRandomID(int Quantity)
{
   for (int index = 0; index < Quantity; index++)
   {
      int Serial = rand() % 1000;
      int Sequency = rand() % 1000000;

      char ID[MAX_INPUT];
      sprintf(ID, "%03d-%07d", Serial, Sequency);

      int verifier = CalcVerifierDig(ID);

      printf("C%cdula generada: %s-%d\n", 130, ID, verifier);

      int RealVerifier = verifier;
      int CalcVerifier = CalcVerifierDig(ID);
      if (CalcVerifier == RealVerifier)
      {
         printf("C%cdula v%clida\n\n", 130, 160);
      }
      else
      {
         printf("C%cdula inv%clida\n\n", 130, 160);
      }
   }
}

/**
 * función: GetSequencyAndSerial
 * Argumentos: (char[]) ID[], Result[]
 * Objetivo: Combinar los números de la secuencia y los de serie.
 */
void GetSequencyAndSerial(char ID[], char result[])
{
   int position = 0;

   for (int index = 0; ID[index] != '\0' && position < SERIALSEQUENCY - 1; index++)
   {
      if (isdigit(ID[index]))
      {
         result[position++] = ID[index];
      }
   }
}

/**
 * Función: SumOddDigits
 * Argumentos: (char[])  Digits[]
 * Objetivo: Sumar los digitos de posición impar.
 * Retorna: Sum
 */
int SumOddDigits(char Digits[])
{
   int sum = 0;

   for (int index = 0; index < SERIALSEQUENCY - 1; index += 2)
   {
      sum += Digits[index] - ASCII_ZERO;
   }
   return sum;
}

/**
 * Función: MultiplyEvenPairs
 * Argumentos: (char[])  Digits[]
 * Objetivo: Multiplicar por dos los digitos de posición par.
 * Retorna: num * 2
 */
int MultiplyEvenPairs(char Digits[])
{
   char Evens[EVEN_NUM];

   int _index = 0;
   for (int index = 1; index < 10; index += 2)
   {
      Evens[_index++] = Digits[index];
   }
   Evens[_index] = '\0';

   int num = atoi(Evens);
   return num * 2;
}

/**
 * Función: SumNumDig
 * Argumentos: (int) num
 * Objetivo: Sumar los digitos a base del parametro dado
 * retorna: (int) sum [Sumatoria de los digitos dados por el argumento]
 */
int SumNumDig(int Num)
{
   int Sum = 0;
   while (Num > 0)
   {
      Sum += Num % 10;
      Num /= 10;
   }
   return Sum;
}

/**
 * Función: CalcVerifierDig
 * Argumentos: (char[]) ID[]
 * Objetivo: Calcular el número verificador real a base de los digitos dados,
 *           para luego compararlo con el digito verificador digitado por el usuario
 *           o generado de forma aleatoria
 * retorna: Digito verificador
 */
int CalcVerifierDig(char ID[])
{
   char Digits[11];
   GetSequencyAndSerial(ID, Digits);

   if (strlen(Digits) != 10)
   {
      return -1;
   }

   int SumOdds = SumOddDigits(Digits);
   int MultiplyResult = MultiplyEvenPairs(Digits);
   int SumEvens = SumNumDig(MultiplyResult);

   int TotalSum = SumOdds + SumEvens;
   int LastDig = TotalSum % 10;
   int Verifier = (10 - LastDig) % 10;

   return Verifier;
}

/*
Funcion: CleanBuffer();
Objetivo: Limpiar la memoria temporal, para evitar bugs con scanf(); cuando se presiona enter
*/
void CleanBuffer()
{
   while (getchar() != '\n')
      ;
}
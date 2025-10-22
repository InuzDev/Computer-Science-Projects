#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

// Tecla ENTER
#define ENTER '\n'
#define ENTER_ALT '\r'
// Columnas
#define MAX_COL 4
#define MIN_COL 1
// Lineas
#define MAX_LIN 15
#define MIN_LIN 1
// Modo
#define VERTICAL 'V'
#define HORIZONTAL 'H'
// Miscelaneos
#define INVALID_LIMIT 0
#define MINUS_WEEK 6
#define MINUS_MONTH 31

int ObtainEasterSunday(int year);
char TagGiver(int StartHolyWeek, int EndHolyWeek);
void WaitUserInput();
void PrintHData(int _year, int _StartHoly, int _EndHoly, char _MonthTag);
void PrintVData(int startYear, int endYear, int colCount);
void Clearbuffer();

int main()
{
   int StartHolyWeek, _EasterSunday, YearLimInf, YearLimSup, LineToPrint = 0, ColToPrint = 0, LinesPrint = 0;
   char MonthTag, mode = 0;

   do
   {
      printf("Especifique el limite inicial del rango de a%cos: ", 164);
      scanf("%d", &YearLimInf);
      printf("Especifique el limite final del rango de a%cos: ", 164);
      scanf("%d", &YearLimSup);

      if (YearLimInf <= INVALID_LIMIT || YearLimSup <= INVALID_LIMIT || YearLimInf >= YearLimSup)
      {
         printf("Rango invalido. Favor de ingresar un rango valido.\n");
      }
   } while (YearLimInf <= INVALID_LIMIT || YearLimSup <= INVALID_LIMIT || YearLimInf >= YearLimSup);

   Clearbuffer();

   while (mode != HORIZONTAL && mode != VERTICAL)
   {
      printf("Especifique el modo en el que quiere ver la tabla, [H]orizontal o [V]ertical?: ");
      scanf("%c", &mode);

      mode = toupper(mode);

      if (mode != HORIZONTAL && mode != VERTICAL)
      {
         printf("Modo invalido, favor espicifique si es [H]orizontal o [V]ertical\n");
         Clearbuffer();
      }
   }

   Clearbuffer();

   while (LineToPrint == 0 && ColToPrint == 0)
   {
      if (mode == HORIZONTAL)
      {
         printf("Especifique las lineas a imprimir: ");
         scanf("%d", &LineToPrint);
         if (LineToPrint > MAX_LIN)
         {
            LineToPrint = 0;
            printf("Limite de lineas m%cximo excedido\n", 160);
         }
         else if (LineToPrint < MIN_LIN)
         {
            LineToPrint = 0;
            printf("Limite de lineas m%cnimo sobrepasado\n", 161);
         }
      }
      else if (mode == VERTICAL)
      {
         printf("Especifique cuantas columnas quiere imprimir: ");
         scanf("%d", &ColToPrint);
         if (ColToPrint > MAX_COL)
         {
            ColToPrint = 0;
            printf("Limite de columnas m%cximo excedido\n", 160);
         }
         else if (ColToPrint < MIN_COL)
         {
            ColToPrint = 0;
            printf("Limite de columnas m%cnimo sobrepasado\n", 161);
         }
      }
   }

   Clearbuffer();

   printf("\n\nTabla de semanas santas de los a%cos: [ %d, %d ]\n\n", 164, YearLimInf, YearLimSup);

   if (mode == HORIZONTAL)
   {
      printf("A = Abril ; B = Abril/Marzo ; M = Marzo\n");
      printf("--------------------------------\n");
      printf("|  a%co  | Inicio | Fin\t | Mes |\n", 164);
      printf("--------------------------------\n");
      for (int index = YearLimInf; index <= YearLimSup; index++)
      {
         _EasterSunday = ObtainEasterSunday(index);
         StartHolyWeek = _EasterSunday - MINUS_WEEK;

         MonthTag = TagGiver(StartHolyWeek, _EasterSunday);

         PrintHData(index, StartHolyWeek, _EasterSunday, MonthTag);
         LinesPrint++;

         if (LinesPrint == LineToPrint && index < YearLimSup)
         {
            printf("--------------------------------\n");
            WaitUserInput();

            printf("Tabla de semanas santas de los a%cos: [ %d, %d ]\n\n", 164, YearLimInf, YearLimSup);

            printf("A = Abril ; B = Abril/Marzo ; M = Marzo\n");
            printf("--------------------------------\n");
            printf("|  a%co  | Inicio | Fin\t | Mes |\n", 164);
            printf("--------------------------------\n");

            LinesPrint = 0;
         }
      }

      printf("--------------------------------\n");

      printf("- Fin de la lista -\n\n");
   }
   else if (mode == VERTICAL)
   {
      for (int index = YearLimInf; index <= YearLimSup; index += ColToPrint)
      {
         int end = index + ColToPrint - 1;
         if (end > YearLimSup)
            end = YearLimSup;

         PrintVData(index, end, ColToPrint);
         if (end < YearLimSup)
         {
            WaitUserInput();
            printf("Tabla de semanas santas de los a%cos: [ %d, %d ]\n\n", 164, YearLimInf, YearLimSup);
         }
      }

      printf("- Fin de la lista -\n\n");
   }
   system("pause");
   return 0;
}

/**
 * Función: ObtainEasterSunday
 * Argumento: (int) year: El año el cual se calcula cuando cae domingo de Pascua
 * Objetivo: Calcular el domingo de pascua a base del año
 * Retorno: (int) Domingo de Pascua, fin de la semana santa
 */
int ObtainEasterSunday(int year)
{
   int A, B, C, D, E, EasterSunday;

   A = year % 19;
   B = year % 4;
   C = year % 7;
   D = (19 * A + 24) % 30;
   E = (2 * B + 4 * C + 6 * D + 5) % 7;
   EasterSunday = (22 + D + E); // N es el primer Domingo de Pascua, Es decir, el ultimo dia de semana santa.

   return EasterSunday;
}

/**
 * Función: CalculateTag
 * Argumentos: (int) StartHolyWeek, EndHolyWeek.
 * Objetivo: Especificar si semana santa cae en Marzo,
 *          Abril o Entre ambos a base de calcular la
 *          fecha del Domingo de Pascua, y Lunes santo
 * Retorna: Dependiendo de los valores de StartHolyWeek y EndHolyWeek, retorna A (Abril), M (Marzo), o B (Marzo y Abril)
 */

char TagGiver(int StartHolyWeek, int EndHolyWeek)
{
   if (EndHolyWeek <= MINUS_MONTH)
      return 'M';
   else if (StartHolyWeek <= MINUS_MONTH && EndHolyWeek > MINUS_MONTH)
      return 'B';
   else
      return 'A';
}

/**
 * Función: PrintHData
 * Argumentos: (int) _year, _StartHoly, EndHoly, _MonthTag, utilizados para clear la tabla
 * Objetivo: Imprimir la tabla a base de los datos obtenidos de los calculos y demas funciones. En horizontal
 */
void PrintHData(int _year, int _StartHoly, int _EndHoly, char _MonthTag)
{
   if (_EndHoly <= MINUS_MONTH)
   {
      printf("| %5d | %5d\t | %5d | (%c) |\n", _year, _StartHoly, _EndHoly, _MonthTag);
   }
   else if (_StartHoly <= MINUS_MONTH && _EndHoly > MINUS_MONTH)
   {
      _EndHoly -= MINUS_MONTH;
      printf("| %5d | %5d\t | %5d | (%c) |\n", _year, _StartHoly, _EndHoly, _MonthTag);
   }
   else
   {
      _StartHoly -= MINUS_MONTH;
      _EndHoly -= MINUS_MONTH;
      printf("| %5d | %5d\t | %5d | (%c) |\n", _year, _StartHoly, _EndHoly, _MonthTag);
   }
}

/**
 * Función: PrintVData
 * Argumentos: (int) startYear, endYear, colCount. Utilizados para clear la tabla.
 * Objetivo: Imprimir la tabla a base de los datos obtenidos de los calculos y demas funciones. En Vertical
 */
void PrintVData(int startYear, int endYear, int colCount)
{
   printf("\nA = Abril ; B = Abril/Marzo ; M = Marzo\n");
   printf("-----------------------------------\n");

   printf("A%co    | ", 164);
   for (int index = startYear; index <= endYear; index++)
      printf("%4d | ", index);
   printf("\n");

   printf("Inicio | ");
   for (int index = startYear; index <= endYear; index++)
   {
      int EasterSunday = ObtainEasterSunday(index);
      int StartHolyWeek = EasterSunday - MINUS_WEEK;

      if (StartHolyWeek > MINUS_MONTH)
         StartHolyWeek -= MINUS_MONTH;

      printf("%4d | ", StartHolyWeek);
   }
   printf("\n");

   printf("Fin    | ");
   for (int index = startYear; index <= endYear; index++)
   {
      int EasterSunday = ObtainEasterSunday(index);

      if (EasterSunday > MINUS_MONTH)
         EasterSunday -= MINUS_MONTH;

      printf("%4d | ", EasterSunday);
   }
   printf("\n");

   printf("Mes    | ");
   for (int index = startYear; index <= endYear; index++)
   {
      int EasterSunday = ObtainEasterSunday(index);
      int StartHolyWeek = EasterSunday - MINUS_WEEK;

      char MonthTag = TagGiver(StartHolyWeek, EasterSunday);
      printf(" (%c) | ", MonthTag);
   }
   printf("\n");

   printf("-----------------------------------\n");
}

/**
 * Función: WaitUserInput
 * Objetivo: Esperar que el usuario presione la tecla enter, a base del modo imprime
 */
void WaitUserInput()
{
   int Keybind = 0;

   printf("Presione [ENTER] para continuar.");

   while (Keybind != ENTER && Keybind != ENTER_ALT && Keybind != EOF)
   {
      Keybind = getchar();
   }
   system("cls");
}

/**
 * Función: Clearbuffer
 * Objetivo: Evitar bugs de que scanf utiliza datos de la memoria temporal.
 */
void Clearbuffer()
{
   while (getchar() != '\n')
      ;
}

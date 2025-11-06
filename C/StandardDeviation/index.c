#include <stdio.h>
#include <ctype.h>
#include <math.h>

// Tecla ENTER
#define ENTER '\n'
#define ENTER_ALT '\r'
// Macros del programa
#define MINDATA 2
#define MAXDATA 9999
#define STARTLIMIT 1

void CleanBuffer();
float NumberMinusAvg(float Average, float Num);
float StandarDeviation(int DataSize, float TotalNiPromSquare);
float NumberMinusAvgSquare(float NiProm);
float AverageOfData(float SumNum, int DataSize);

int main()
{
   int Keybind = 0, DataCount = 0;
   float Data[MAXDATA], DataSum = 0.0f, _Average = 0.0f, NiProm, NiPromAlSquare[MAXDATA], totalNiProm;
   char UserPrompt = 'N';

   for (int index = 0; index < MAXDATA; index++)
   {
      printf("Ingrese un n%cmero: ", 163);
      scanf("%f", &Data[index]);

      CleanBuffer();

      DataCount++;
      DataSum += Data[index];

      if (DataCount >= 1)
      {
         printf("\nQuiere agregar otro n%cmero? [S]i [N]o ", 163);
         scanf("%c", &UserPrompt);

         CleanBuffer();

         UserPrompt = toupper(UserPrompt);

         if (UserPrompt == 'N' || Keybind == '\r' || Keybind == '\n')
         {
            break;
         }
      }
   }
   _Average = AverageOfData(DataSum, DataCount);

   for (int index = 0; index < DataCount; index++)
   {
      NiProm = NumberMinusAvg(_Average, Data[index]);
      printf("Ni - Prom: %.2f\n", NiProm);
      NiPromAlSquare[index] = NumberMinusAvgSquare(NiProm);
      printf("(Ni-Prom)^2: %.2f\n", NiPromAlSquare[index]);
   }

   for (int index = 0; index < DataCount; index++)
   {
      totalNiProm += NiPromAlSquare[index];
   }
   printf("Total (Ni-Prom)^2: %.2f\n\nTotal N%cmeros: %.2f\n\n", totalNiProm, 163, DataSum);

   float _PopulationDeviation = StandarDeviation(DataCount, totalNiProm);
   printf("Deviaci%cn: %.2f", 162, _PopulationDeviation);

   return 0;
}

/*
Funcion: AverageOfData
Objetivo Calcular el average de los datos.
Argumentos: (float) SumNum -> Suma de los numeros introducidos por el usuario,
(int) DataSize -> Cantidad de datos ingresados en el arreglo.
*/
float AverageOfData(float SumNum, int DataSize)
{
   float _AverageOfData = 0;
   return _AverageOfData = (float)SumNum / (float)DataSize;
}

float NumberMinusAvg(float Average, float Num)
{
   float NiProm = 0;
   return NiProm = Num - Average;
}

float NumberMinusAvgSquare(float NiProm)
{
   float NiPromAlSquare = 0;
   return NiPromAlSquare = NiProm * NiProm;
}

float StandarDeviation(int DataSize, float TotalNiPromSquare)
{
   float _PopulationStandardDeviation = 0;
   return _PopulationStandardDeviation = sqrt((TotalNiPromSquare / DataSize));
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

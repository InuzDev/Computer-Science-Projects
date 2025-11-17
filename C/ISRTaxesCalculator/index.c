/**
 * Programa para calcular el ISR.
 *
 * Charles David Jorge Taveras ; 16 - 11
 *
 * Archivo: main.c
 */

#include <stdio.h>
#include <stdlib.h>

#define MONTHS 12

#define LIM_1 416220.01f
#define LIM_2 624329.01f
#define LIM_3 867123.01f

#define TAX_1 0.15f
#define TAX_2 0.20f
#define TAX_3 0.25f

float CalcYearlyProjection(float);
char DefineMonth(int);
float CalcTaxes(float);
float TotalDeduction(float, float);
void ShowTable();

int main()
{
   float MonthlyPayments[MONTHS] = {0};
   int SizeOfMP = sizeof(MonthlyPayments) / sizeof(MonthlyPayments[0]);

   for (int index = 0; index < SizeOfMP; index++)
   {
      switch (index + 1)
      {
      case 1:
         printf("Enero     : ");
         break;
      case 2:
         printf("Febrero   : ");
         break;
      case 3:
         printf("Marzo     : ");
         break;
      case 4:
         printf("Abril     : ");
         break;
      case 5:
         printf("Mayo      : ");
         break;
      case 6:
         printf("Junio     : ");
         break;
      case 7:
         printf("Julio     : ");
         break;
      case 8:
         printf("Agosto    : ");
         break;
      case 9:
         printf("Septiembre: ");
         break;
      case 10:
         printf("Octubre   : ");
         break;
      case 11:
         printf("Noviembre : ");
         break;
      case 12:
         printf("Diciembre : ");
         break;
      default:
         printf("undefined");
         break;
      }
      scanf("%f", &MonthlyPayments[index]);
   }
   return 0;
}

/**
 * Función: CalcYearlyProjection
 * Argumento(s): (float) RawPayment
 * Objetivo: Calcular la proyección anual
 */
float CalcYearlyProjection(float RawPayment)
{
   int YearlyProjection;
   return YearlyProjection = RawPayment * MONTHS;
}

float CalcTaxes(float YearlyProjection)
{
   float Deduction;
   if (YearlyProjection >= LIM_1)
   {
      Deduction = (YearlyProjection - LIM_1) * TAX_1;
      return Deduction;
   }
   else if (YearlyProjection >= LIM_2)
   {
      Deduction = (LIM_2 - LIM_1) * TAX_1 + (YearlyProjection - LIM_2) * TAX_2;
      return Deduction;
   }
   else if (YearlyProjection >= LIM_3)
   {
      Deduction = (LIM_2 - LIM_1) * TAX_1 + (LIM_2 - LIM_3) * TAX_2 + (YearlyProjection - LIM_3) * TAX_3;
      return Deduction;
   }
}

/**
 * Función: TotalDeduction
 * Argumento: (float) Deduction, RawPayment
 * Objetivo: Calcular la deducción total
 */
float TotalDeduction(float Deduction, float RawPayment)
{
   float YearlyProjection = CalcYearlyProjection(RawPayment);
   float _Taxes = CalcTaxes(YearlyProjection);

   return _Taxes / MONTHS;
}
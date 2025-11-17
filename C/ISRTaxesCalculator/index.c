/**
 * Programa para calcular el ISR.
 *
 * Charles David Jorge Taveras ; 16 - 11 - 2025
 *
 * Archivo: main.c
 */

#include <stdio.h>
#include <stdlib.h>

#define MONTHS 12

#define TAX_THRESHOLD_1 416220.01f
#define TAX_THRESHOLD_2 624329.01f
#define TAX_THRESHOLD_3 867123.01f

#define TAX_PERCENTAGE_1 0.15f
#define TAX_PERCENTAGE_2 0.20f
#define TAX_PERCENTAGE_3 0.25f

float ComputeYearlyTax(float);
float CalcTaxes(float, int, float[]);
float CalcNetSalary(float, float);
void ShowTable(float, float, float, int);

int main()
{
   float MonthlyPayments[MONTHS] = {0.00f}, YearlyProjection[MONTHS] = {0.00f}, deduction[MONTHS] = {0.00f}, NetSalary[MONTHS] = {0.00f};
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

   for (int index = 0; index < SizeOfMP; index++)
   {
      float sumPrev = 0.0f;
      for (int _index = 0; _index < index; _index++)
      {
         sumPrev += MonthlyPayments[_index];
      }

      int remainingMonths = MONTHS - index;
      YearlyProjection[index] = sumPrev + MonthlyPayments[index] * remainingMonths;

      deduction[index] = CalcTaxes(YearlyProjection[index], index, deduction);
      NetSalary[index] = CalcNetSalary(MonthlyPayments[index], deduction[index]);
   }

   printf("    Mes         Sueldo Bruto     IRS   Sueldo Neto\n");
   for (int index = 0; index < SizeOfMP; index++)
   {
      ShowTable(MonthlyPayments[index], deduction[index], NetSalary[index], index);
   }

   return 0;
}

/**
 * Función: ShowTable
 * Argumento(s): (float) RawSalary, IRSTax, NetSalary
 *               (int) Index
 * Objetivo: Imprimir la tabla, utilizando los datos proporcionados por la función main.
 */
void ShowTable(float RawSalary, float Deduction, float NetSalary, int Index)
{
   int Month = Index + 1;
   switch (Month)
   {
   case 1:
      printf("Enero     : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 2:
      printf("Febrero   : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 3:
      printf("Marzo     : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 4:
      printf("Abril     : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 5:
      printf("Mayo      : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 6:
      printf("Junio     : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 7:
      printf("Julio     : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 8:
      printf("Agosto    : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 9:
      printf("Septiembre: %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 10:
      printf("Octubre   : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 11:
      printf("Noviembre : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   case 12:
      printf("Diciembre : %12.2f %12.2f %12.2f\n", RawSalary, Deduction, NetSalary);
      break;
   default:
      printf("undefined");
      break;
   }
}
/**
 * Función: ComputeYearlyTax
 * Argumento(s): (float) yearlyProjection
 * Objetivo: Calcular el impuesto anual total aplicando los tramos
 * Retorna: impuesto anual sobre la renta (total anual)
 */
float ComputeYearlyTax(float yearlyProjection)
{
   float tax = 0.0f;

   if (yearlyProjection > TAX_THRESHOLD_1)
   {
      float upper = (yearlyProjection < TAX_THRESHOLD_2) ? yearlyProjection : TAX_THRESHOLD_2;
      tax += (upper - TAX_THRESHOLD_1) * TAX_PERCENTAGE_1;
   }

   if (yearlyProjection > TAX_THRESHOLD_2)
   {
      float upper = (yearlyProjection < TAX_THRESHOLD_3) ? yearlyProjection : TAX_THRESHOLD_3;
      tax += (upper - TAX_THRESHOLD_2) * TAX_PERCENTAGE_2;
   }

   if (yearlyProjection > TAX_THRESHOLD_3)
   {
      tax += (yearlyProjection - TAX_THRESHOLD_3) * TAX_PERCENTAGE_3;
   }

   return tax;
}

/**
 * Función: CalcTaxes
 * Argumento(s): (float) YearlyProjection, (int) monthIndex,
 *               (float[]) previousDeductions
 * Objetivo: Calcular la deducción mensual considerando:
 *   1) la proyección anual actual (suponiendo el salario del mes actual
 *      se mantiene para los meses restantes),
 *   2) restar el total ya retenido en meses anteriores y
 *   3) distribuir el restante entre los meses que faltan (incluyendo el actual).
 * Retorna: deducción mensual para el mes actual
 */
float CalcTaxes(float YearlyProjection, int monthIndex, float previousDeductions[])
{
   float yearlyTax = ComputeYearlyTax(YearlyProjection);

   float sumPrevWithheld = 0.0f;
   for (int index = 0; index < monthIndex; index++)
   {
      sumPrevWithheld += previousDeductions[index];
   }

   int remainingMonths = MONTHS - monthIndex;
   float remainingTax = yearlyTax - sumPrevWithheld;
   if (remainingTax < 0.0f)
      remainingTax = 0.0f;

   float monthlyDeduction = remainingTax / (float)remainingMonths;
   return monthlyDeduction;
}

/**
 * Función: CalcNetSalary
 * Arugumento(s): (float) RawSalary, Deduction
 * Objetivo: Calcular el salario neto
 * Retorna: Salario neto (NetSalary)
 */
float CalcNetSalary(float RawSalary, float Deduction)
{
   float NetSalary = 0.0f;
   NetSalary = RawSalary - Deduction;
   return NetSalary;
}
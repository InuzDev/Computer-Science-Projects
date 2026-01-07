#include <stdio.h>

#define DEDUCTMAX 0.20f
#define DEDUCTMID 0.15f
#define DEDUCTMIN 0.10f
#define MAX_PAYMENT 550.00f
#define MIN_PAYMENT 100.00f
#define MAX_EMPLOYEES 20
#define MIN_EXMPLOYEES 1
#define MAX_HOURS 80
#define MIN_HOURS 30

void ExtraBreakdown(int hours, int extraHours[4]);
float RawSalary(float Payment, int HourWork);
float CalcDeduction(float _rawSalary);
float NetSalary(float _rawSalary);

int main()
{
   int Employees, extra = 0, totalHours = 0, totalExtra = 0;
   int ExtraHours[4];
   int HoursWork[MAX_EMPLOYEES];
   float Payment[MAX_EMPLOYEES];
   int totalExtraHours[4] = {0, 0, 0, 0};
   float totalRaw = 0.0f, totalDeduction = 0.0f, totalNeto = 0.0f;

   while (1)
   {
      printf("Introduzca cuantos empleados va a calcular: ");
      scanf("%d", &Employees);
      if (Employees > MAX_EMPLOYEES || Employees < MIN_EXMPLOYEES)
      {
         printf("Cantidad de empleados invalida, los cantidad de empleados debe estar en [1, 20]\n");
         continue;
      }
      break;
   }

   for (int index = 1; index <= Employees; index++)
   {
      printf("Empleado: %d\n", index);
      while (1)
      {
         printf("Introduzca las horas trabajadas: ");
         scanf("%d", &HoursWork[index]);
         if (HoursWork[index] > MAX_HOURS || HoursWork[index] < MIN_HOURS)
         {
            printf("Horas invalidas, ingrese una hora valida [30, 80]\n");
            continue;
         }
         break;
      }

      while (1)
      {
         printf("Pago por hora: ");
         scanf("%f", &Payment[index]);
         if (Payment[index] > MAX_PAYMENT || Payment[index] < MIN_PAYMENT)
         {
            printf("Pago por hora invalido, el pago debe estar entre [100.00, 550.00]\n");
            continue;
         }
         break;
      }
   }
   printf("\n");
   printf("| %-8s | %-5s | %-7s | %-12s | %-12s | %-3s | %-3s | %-3s | %-3s | %-10s | %-11s |\n",
          "Empleado", "Horas", "Precio", "Horas Extras", "Sueldo Bruto", "34%", "45%", "50%", "100%", "Deduccion", "Sueldo Neto");
   for (int index = 1; index <= Employees; index++)
   {
      int extra = (HoursWork[index] > 44) ? (HoursWork[index] - 44) : 0;
      ExtraBreakdown(HoursWork[index], ExtraHours);
      float _rawSalary = RawSalary(Payment[index], HoursWork[index]);
      float _deduction = CalcDeduction(_rawSalary);
      float _netSalary = NetSalary(_rawSalary);

      totalHours += HoursWork[index];
      totalExtra += extra;

      for (int _Index = 0; _Index < 4; _Index++)
         totalExtraHours[_Index] += ExtraHours[_Index];

      totalRaw += _rawSalary;
      totalDeduction += +_deduction;
      totalNeto += _netSalary;

      printf("| %8d | %5d | %7.2f | %12d | %12.2f | %3d | %3d | %3d | %4d | %10.2f | %11.2f |\n", index, HoursWork[index], Payment[index], extra, _rawSalary, ExtraHours[0], ExtraHours[1], ExtraHours[2], ExtraHours[3], _deduction, _netSalary);
   }

   printf("| %-8s | %5d | %-7s | %12d | %12.2f | %3d | %3d | %3d | %4d | %10.2f | %11.2f |\n", "Total", totalHours, "", totalExtra, totalRaw, totalExtraHours[0], totalExtraHours[1], totalExtraHours[2], totalExtraHours[3], totalDeduction, totalNeto);

   return 0;
}

/**
 * Función: CalcDeduction
 * Argumentos: (float) _rawSalary
 * Objetivo: Calcular la deducción del salario a base de cuanto gana
 * Retorna: (float) Deducción del salario.
 */
float CalcDeduction(float _rawSalary)
{
   if (_rawSalary >= 17000.0f)
   {
      return (12000.0f - 8000.0f) * DEDUCTMIN + (17000.0f - 12000.0) * DEDUCTMID + (_rawSalary - 17000.0f) * DEDUCTMAX;
   }
   else if (_rawSalary >= 12000.0f)
   {
      return (12000.0f - 8000.0f) * DEDUCTMIN + (17000.0f - 12000.0) * DEDUCTMID;
   }
   else if (_rawSalary >= 8000.0f)
   {
      return (12000.0f - 8000.0f) * DEDUCTMIN;
   }
   else
   {
      return 0.0f;
   }
}

/**
 * Función: NetSalary
 * Argumentos: (float) _rawSalary
 * Objetivo: Calcular el salario neto, restando la deducción
 * Retorna: (float) Salario neto.
 */
float NetSalary(float _rawSalary)
{
   float deduction = CalcDeduction(_rawSalary);
   return _rawSalary - deduction;
}

/**
 * Función: ExtraBreakdown
 * Argumentos: (int) hours, extraHours[4]
 * Objetivo: Guardar información de cuantas horas extras para cada rango de porcentaje en la tabla.
 */
void ExtraBreakdown(int hours, int extraHours[4])
{
   int extra = hours - 44;
   if (extra < 0)
      extra = 0;

   int limits[4] = {10, 10, 4, MAX_HOURS};
   for (int i = 0; i < 4; i++)
   {
      if (extra > limits[i])
      {
         extraHours[i] = limits[i];
         extra -= limits[i];
      }
      else
      {
         extraHours[i] = extra;
         extra = 0;
      }
   }
}

/**
 * Función: RawSalary
 * Argumentos: (float) Payment, (int) HourWork
 * Objetivo: Calcular el salario bruto (incluyendo horas extras)
 * Retorna: (float) salario bruto.
 */
float RawSalary(float Payment, int HourWork)
{
   float RawSalary = 0.0f;

   if (HourWork <= 44)
   {
      RawSalary = Payment * HourWork;
   }
   else
   {
      // Base salary for first 44 hours
      RawSalary = Payment * 44;
      int extra = HourWork - 44;

      // Apply overtime in stages
      if (extra <= 10)
         RawSalary += extra * (Payment * 1.35f);
      else if (extra <= 20)
         RawSalary += (10 * (Payment * 1.35f)) + ((extra - 10) * (Payment * 1.45f));
      else if (extra <= 24)
         RawSalary += (10 * (Payment * 1.35f)) + (10 * (Payment * 1.45f)) + ((extra - 20) * (Payment * 1.5f));
      else // more than 24 extra hours
         RawSalary += (10 * (Payment * 1.35f)) + (10 * (Payment * 1.45f)) + (4 * (Payment * 1.5f)) + ((extra - 24) * (Payment * 2.0f));
   }

   return RawSalary;
}
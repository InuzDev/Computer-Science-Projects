#include <stdio.h>
#include <ctype.h>

#define JUNIOR 1
#define SENIOR 5
#define MAX_SALARY 320000.00
#define MIN_SALARY 8000.00
#define MAX_AGE 40
#define MIN_AGE 1
#define MONTHS_IN_YEAR 12
#define AVG_DAYS_PER_MONTH 23.83

float VacationBonus(int age, float MonthlySalary);
float CalcMonthlyComission(int age, float MonthlySell);
float MonthlyAvgSalary(float _TotalSalary);
float AverageDailySalary(float MonthlySalary);
int CalcVacationDays(int _Age);
char *DefineMonth(int index);

int main()
{
   float BaseSalary = 0.0f;
   float MonthlySellingComissions[MONTHS_IN_YEAR] = {0.0f};
   float _MonthlyComission = 0.0f;
   float _TotalSalary = 0.0f;
   float _ChristmasBonus = 0.0f;
   float _TotalArraySum = 0.0f;
   float _Bonification = 0.0f;
   float _MonthlyAvgSalary = 0.0f;
   float _DailyAvgSalary = 0.0f;
   float _VacationBonus = 0.0f;
   int Age = 0;

   char PromptUser = 0;
   do
   {
      while (1)
      {
         printf("Ingrese la antiguedad del empleado: ");
         scanf("%d", &Age);
         if (Age > MAX_AGE || Age < MIN_AGE)
         {
            printf("Valor ingresado invalido, la antiguedad debe estar entre 0 y 40\n");
            continue;
         }
         break;
      }
      fflush(stdin);

      while (1)
      {
         printf("Ingrese el sueldo base [8000.00 , 320000.00]: ");
         scanf("%f", &BaseSalary);
         if (BaseSalary > MAX_SALARY || BaseSalary < MIN_SALARY)
         {
            printf("Valor ingresado invalido, el salario base debe estar entre 8000.00 y 320000.00\n");
            continue;
         }
         break;
      }
      fflush(stdin);

      printf("Ventas realizadas [RD$]\n");
      for (int index = 0; index < MONTHS_IN_YEAR; index++)
      {
         do
         {
            printf("%-10s:", DefineMonth(index));
            scanf("%f", &MonthlySellingComissions[index]);
            if (MonthlySellingComissions[index] < 0 || MonthlySellingComissions[index] > 75000.00)
               printf("Valor ingresado invalido, las ventas deben estar entre 0.00 y 75000.00\n");
         } while (MonthlySellingComissions[index] < 0 || MonthlySellingComissions[index] > 75000.00);
      }

      printf("Datos mensuales:\n");
      printf("----------------------------------------------------------------------\n");
      printf("|     Mes     | Sueldo Base | Ventas    | Comisi%cn   | Sueldo Total |\n", 162);
      printf("----------------------------------------------------------------------\n");
      _TotalArraySum = 0.0f;
      for (int index = 0; index < MONTHS_IN_YEAR; index++)
      {
         _MonthlyComission = CalcMonthlyComission(Age, MonthlySellingComissions[index]);
         _TotalSalary = BaseSalary + _MonthlyComission;
         printf("| %-11s | %11.2f | %9.2f | %10.2f | %12.2f |\n", DefineMonth(index), BaseSalary, MonthlySellingComissions[index], _MonthlyComission, _TotalSalary);
         _TotalArraySum += _TotalSalary;
      }
      printf("----------------------------------------------------------------------\n");

      _MonthlyAvgSalary = MonthlyAvgSalary(_TotalArraySum);
      _ChristmasBonus = _MonthlyAvgSalary;
      _Bonification = _ChristmasBonus + _MonthlyAvgSalary;
      _DailyAvgSalary = AverageDailySalary(_MonthlyAvgSalary);
      _VacationBonus = VacationBonus(Age, _MonthlyAvgSalary);

      printf("\nResumen anual:\n");
      printf("Salario mensual promedio: RD$ %7.2f\n", _MonthlyAvgSalary);
      printf("Salario diario promedio : RD$ %7.2f\n", _DailyAvgSalary);
      printf("Bono vacacional         : RD$ %7.2f\n", _VacationBonus);
      printf("Salario de navidad      : RD$ %7.2f\n", _ChristmasBonus);
      printf("Bonificaci%cn           : RD$ %7.2f\n", 160, _Bonification);

      printf("Desea salir o calcular a otro beneficiado? [S]i / [N]o ");

      scanf(" %c", &PromptUser);

      PromptUser = toupper(PromptUser);
      if (PromptUser != 'S' && PromptUser != 'N')
      {
         do
         {
            printf("Las respuestas deben ser [S]i / [N]o \n");
            scanf(" %c", &PromptUser);
            PromptUser = toupper(PromptUser);
         } while (PromptUser != 'S' && PromptUser != 'N');
      }
   } while (PromptUser == 'S');

   return 0;
}

/** Funcion DefineMonth
 *  Argumentos: (int) index
 *  Objetivo: Definir cada valor del indice del primer bucle de la funcion principal del programa.
 * Retorna: El nombre del mes dependiendo el numero que sea el indice del bucle.
 */
char *DefineMonth(int index)
{
   switch (index)
   {
   case 0:
      return "Enero";
   case 1:
      return "Febrero";
   case 2:
      return "Marzo";
   case 3:
      return "Abril";
   case 4:
      return "Mayo";
   case 5:
      return "Junio";
   case 6:
      return "Julio";
   case 7:
      return "Agosto";
   case 8:
      return "Septiembre";
   case 9:
      return "Octubre";
   case 10:
      return "Noviembre";
   case 11:
      return "Diciembre";
   default:
      return "Invalido";
   }
}

/**
 * Funcion MonthlyAvgSalary
 * Argumentos: _TotalSalary
 * Objetivo: Calcular el Average del salario mensual
 * Retorna: El average del salario mensual.
 */
float MonthlyAvgSalary(float _TotalSalary)
{
   return _TotalSalary / MONTHS_IN_YEAR;
}

/** Funcion AverageDailySalary
 *  Argumentos: (float) MonthlySalary.
 * Objetivo: Calcular el promedio del salario diario.
 * Retorna: Un flotante representando el average del salario diario.
 */
float AverageDailySalary(float MonthlySalary)
{
   return MonthlySalary / AVG_DAYS_PER_MONTH;
}

/** Funcion VacationBonus
 *  Argumentos: (int) age, salary.
 * Objetivo: Calcular el bonus vacacional.
 * Retorna: un flotante representando el bonus vacacional
 */
float VacationBonus(int age, float MonthlySalary)
{
   float VacationBonus = 0.0f;
   int VacationDays = CalcVacationDays(age);
   VacationBonus = VacationDays * AverageDailySalary(MonthlySalary);

   return VacationBonus;
}

/**
 * Funcion CalcMonthlyComission
 * Argumentos: (int) _Age, (float) MonthlySell
 * Objetivo: Calcular la comision para el empleado dependiendo el tiempo que tenga en la empresa
 * Retorna: La comision mensual que se utiliza para la tabla
 */
float CalcMonthlyComission(int Age, float MonthlySell)
{
   float MonthlyComission = MonthlySell;
   if (Age <= JUNIOR)
   {
      MonthlyComission *= 0.03;
   }
   else if (Age < SENIOR && Age > JUNIOR)
   {
      MonthlyComission *= 0.08;
   }
   else
   {
      MonthlyComission *= 0.15;
   }
   return MonthlyComission;
}
/**
 * Funcion CalcVacationDays
 * Argumentos: (int) _Age
 * Objetivo: Calcular los dias de vacaciones de cada empleado dependiendo del tiempo que tengan en la empresa
 * Retorna: los dias de vacaciones del empleado.
 */
int CalcVacationDays(int _Age)
{
   if (_Age <= 1)
      return 14;
   else if (_Age < 5)
      return 18;
   else
      return 21;
   return 0;
}
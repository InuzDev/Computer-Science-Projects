#include <stdio.h>

#define _INPUT 5

float FahrenheitToCelsius(float fahrenheitInput);

int main()
{
   float fahrenheitTempInput[_INPUT] = {32.0f, 68.0f, 77.0f, 104.0f, 0.0f};
   float celsiusResult[_INPUT] = {0.0f};

   printf("Fahrenheit   |   Celsius\n");
   printf("------------------------\n");
   for (int index = 0; index < _INPUT; index++)
   {
      celsiusResult[index] = FahrenheitToCelsius(fahrenheitTempInput[index]);
      printf("%10.2f   |   %7.2f\n", fahrenheitTempInput[index], celsiusResult[index]);
   }

   return 0;
}

float FahrenheitToCelsius(float fahrenheitInput)
{
   float celsiusConversion = 0.0f;
   celsiusConversion = (fahrenheitInput - 32.0f) / 1.8f;
   return celsiusConversion;
}
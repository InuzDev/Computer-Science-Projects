#include <stdio.h>
#include <stdlib.h>

// Function to calculate the sum of proper divisors
int sumOfDivisors(int num)
{
   int sum = 0;
   for (int i = 1; i < num; i++)
   {
      if (num % i == 0)
         sum += i;
   }
   return sum;
}

// Function to classify the number.
int classify(int num)
{
   int sum = sumOfDivisors(num);
   if (sum < num)
      return -1; // Abundant

   if (sum == num || sum == 0)
      return 0; // Perfect

   return 1; // Deficient
}

// Function to check if a number is contradictory
int isContradictory(int num)
{
   int class1 = classify(num);
   if (class1 == 0)
      return 0; // Perfect numbers are not contradictory

   int sum1 = sumOfDivisors(num);
   int class2 = classify(sum1);

   return (class1 != class2);
}

// Main function
int main()
{
   int rangeA, rangeB = 0;

   do
   {
      printf("Enter the start of the positive range: ");
      scanf("%d", &rangeA);

      printf("Enter the end of the positive range: ");
      scanf("%d", &rangeB);

      if (rangeA <= 0 || rangeB <= 0 || rangeA >= rangeB)
         printf("Invalid range. Please enter positive numbers with start <= end.\n");
   } while (rangeA <= 0 || rangeB <= 0 || rangeA > rangeB);

   printf("Contradictory numbers in the range:\n");

   for (int i = rangeA + 1; i <= rangeB; i++)
      if (isContradictory(i))
         printf("%d is contradictory.\n", i);

   return 0;
}
#include <stdio.h>

// Function to calculate the sum of proper divisors
int sumOfDivisors(int num)
{
   int sum = 0;
   for (int i = 1; i < num; i++)
   {
      if (num % i == 0)
      {
         sum += i;
      }
   }
   return sum;
}

// Function to classify a number: -1 = deficient, 0 = perfect, 1 = abundant
int classify(int num)
{
   int sum = sumOfDivisors(num);
   if (sum < num)
      return -1;
   else if (sum == num)
      return 0;
   else
      return 1;
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
   int rangeA, rangeB;

   printf("Enter the start of the positive range: ");
   scanf("%d", &rangeA);
   printf("Enter the end of the positive range: ");
   scanf("%d", &rangeB);

   if (rangeA <= 0 || rangeB <= 0 || rangeA > rangeB || rangeA == rangeB)
   {
      printf("Invalid range. Please enter positive numbers with start <= end.\n");
      return 1;
   }

   printf("Contradictory numbers in the range:\n");
   for (int i = rangeA; i <= rangeB; i++)
   {
      if (isContradictory(i))
      {
         printf("%d is contradictory.\n", i);
      }
   }

   return 0;
}
/*
 * I want to make a disclaimer, this program is full of bad practice, I'm testing C++ and learning how it works.
 * I know a lot of the practice made here are bad, and not good for an introduction to Object Oriented Programming; other codes like ClassesAndObjects are better structured
 * And more organized, because the point of them is to understand the logic of the parts. In this cpp file I experiment with the studied theory and try to practice with it.
 */

#include <cstdio>
#include <iostream>

// to use mathematical functions like square root, rounds a number, or natural logarithm. Include cmath.
#include <cmath>

#define A_MACRO "Hello, I'm a macro\n"

#define MAX_ITERATIONS 10

using namespace std;

// This is a class
class ExampleClass {
 public:
   int Val = 0;
   int Iterations = 0;

   int ForceChange = 0;

   int SafeValue() {
      if (ForceChange == 1) {
         bool Safeguard = false;
         return 1;
      }
      return 0;
   }

   void ExampleFunction() {
      if (SafeValue() != 0) {
         ForceChange = 1;
      }
      // Overengineered aah thing
      while (Val < Iterations) {
         // We compare if the iterations defined in `int main() {...}` are below or equal to max iterations.
         if (Iterations <= MAX_ITERATIONS) {
            // If it below or equal MAX_ITERATIONS, run this
            if (Val < Iterations) {
               cout << "Hi! Iterations left: " << Iterations - Val << endl;
            }
            // If the iterations defined in `int main() {...}` are above MAX_ITERATIONS, it will print this and break the function.
         } else {
            cout << "The max iterations are " << MAX_ITERATIONS << endl;

            if (ForceChange == 1) {
               cout << "Forcing the iterations to be MAX_ITERATIONS . . ." << endl;

               Iterations = MAX_ITERATIONS;
            }

            return;
         }
         Val += 1;
      }
   }
};

int main() {
   /*
    * Unsigned integer: Can only store positive values (non-negative), from 0 to (2^32)-1, and wraps
    *                   around to 0 if the value exceeds the maximum limit.
    * Signed integer: Can store both positive and negative values, from -((2^32)/2) - 1 to ((2^32)/2) - 1.
    *                 It will get a overflow error while used in a program with a larger values.
    */

   /* Now, getting in object oriented programming: */
   ExampleClass ExampleObj; // We create the object

   ExampleObj.Iterations = MAX_ITERATIONS; // Define the variables/values inside the class for the object.
   ExampleObj.Val = 0;                     // ...

   ExampleObj.ForceChange = 0;

   // User defined object values
   cout << "Do you want to force change if iterations are above limit?" << endl;
   cin >> ExampleObj.ForceChange;
   if (ExampleObj.ForceChange > 0) {
      ExampleObj.ForceChange = 1;
   }

   cout << "User input iterations, MAX: 10" << endl;
   cin >> ExampleObj.Iterations;

   ExampleObj.ExampleFunction(); // Since in the class we also defined a function, we can call it and use it.

   signed int ErrorNum;
   unsigned int unsignedNum;

   ErrorNum = 4294967295; // If I try and compile this will throw an overflow error. (!NEVER DO THIS!) ((I force it to be unsigned in a cout, AGAIN IS NOT EFFICIENT NOR SAFE))
   unsignedNum = 4294967295;
   // You can do the same techniques in C
   unsigned int x = 5, y = 6, z = 4294967295;

   cout << "\n\nHello World" << endl;

   cout << A_MACRO; // The Macro, which got a string.

   // To print a number:
   cout << (unsigned)ErrorNum << '\n'; // <- From diffent from C, you just put the number. (If I use `(unsigned)` to convert from signed, it will run without any issue. Yet not efficient.)
   printf("Hello World, %d\n", 45);    // Yet; we can use printf.

   // ---
   // Math code
   int index = 0;
   while (index < MAX_ITERATIONS) { // C++ introduce booleans to programming
      cout << sqrt(index);
      if (sqrt(index) > 1) {
         cout << round((double)(index, MAX_ITERATIONS)) << endl;
         if (round((double)(index, MAX_ITERATIONS) > 2)) {
            cout << log(index) << endl;
         }
      }
      index += 1;
   }

   return 0;
}

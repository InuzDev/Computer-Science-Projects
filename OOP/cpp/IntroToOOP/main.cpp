#include <cstdio>
#include <iostream>

// to use mathematical functions like square root, rounds a number, or natural logarithm. Include cmath.
#include <cmath>

#define A_MACRO "Hello, I'm a macro\n"

#define MAX_ITERATIONS 10

using namespace std;

int main() {
   /*
    * Unsigned integer: Can only store positive values (non-negative), from 0 to (2^32)-1, and wraps
    *                   around to 0 if the value exceeds the maximum limit.
    * Signed integer: Can store both positive and negative values, from -((2^32)/2) - 1 to ((2^32)/2) - 1.
    *                 It will get a overflow error while used in a program with a larger values.
    */

   signed int ErrorNum;
   unsigned int unsignedNum;

   ErrorNum = 4294967295; // If I try and compile this will throw an overflow error. (!NEVER DO THIS!)
   unsignedNum = 4294967295;
   // You can do the same techniques in C
   unsigned int x = 5, y = 6, z = 4294967295;

   cout << "Hello World" << endl;

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
         cout << round((double)(index, MAX_ITERATIONS));
         if (round((double)(index, MAX_ITERATIONS) > 2)) {
            cout << log(index);
         }
      }
   }

   return 0;
}

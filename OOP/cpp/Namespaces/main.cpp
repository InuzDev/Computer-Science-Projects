#include <iostream>

/*
 * You can define multiple namespaces with the same
 */
namespace Test {
int a = 5;
}

namespace Test {
int b = 10;
}

namespace Space1 {
void func() {
   std::cout << "Hello world!\n";
}
} // namespace Space1

namespace Space2 {
void func() {
   std::cout << "Hello World 2!\n";
}
} // namespace Space2

// We cannot use two spaces that share the same function name.
using namespace Space1;

/* Nested namespaces */
namespace nestedSpace_outer {
void func() {
   std::cout << "We are in the outside" << std::endl;
}

namespace inner {
void func() {
   std::cout << "Now, this is the inside of the nestedSpace" << std::endl;
}
} // namespace inner
} // namespace nestedSpace_outer

int main() {
   /* Code used for a GeeksforGeeks quiz */
   std::cout << Test::a + Test::b;
   /*...*/

   /* In the following line, I will show how to call each namespace. */

   // To call the inside of a nested namespace, you just call the outside, then the inside, then the function from the inside.
   nestedSpace_outer::inner::func();
   // If you want to call any function of the outer namespace, just call the outer layer.
   nestedSpace_outer::func();

   // namespaces are used to create functions that share the same name:
   Space1::func(); // Hello World!
   Space2::func(); // Hello world 2!

   return 0;
}

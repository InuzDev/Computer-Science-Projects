#include <iostream>

using namespace std;

// Define the class above the functions prototype.
// > Source: GeeksforGeeks
class GfG {
 public:
   // Data member
   int val;
   void show() {
      cout << "Value: " << val << endl;
   }
};

void GetClass();

int main() {
   // We need to create an object for the class.
   cout << "Objects and class programs\n";

   GfG Obj;
   Obj.val = 4; // This define the value inside the class. (int val)
   Obj.show();  // This call the function the class: show() {...}; Which also involve the variable val.

   return 0;
}

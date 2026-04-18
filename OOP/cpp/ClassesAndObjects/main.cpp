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

// you can create nested classes in C++
class Outer {
 public:
   class inner {
    public:
      void display() {
         cout << "this function is in the inside class" << endl;
      }
   };

   void show() {
      cout << "This function is in the outside class" << endl;
   }
};

int main() {
   // We need to create an object for the class.
   cout << "Objects and class program\n";

   GfG Obj;
   Obj.val = 4; // This define the value inside the class. (int val)
   Obj.show();  // This call the function the class: show() {...}; Which also involve the variable val.

   /* Calling the nested class */
   // First we create an object again.
   Outer outerObj;
   Outer::inner innerObj;
   outerObj.show();
   innerObj.display();

   return 0;
}

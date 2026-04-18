# Object Oriented Programming

## Introduction to Object Oriented Programming

### C++

> My favourite language by the way.

#### Compile a C++ program

To compile a C++ program, we need a compiler. I will skip the installation process. I use the gcc compiler.

```PowerShell
# Run the following command
g++ main.c -std=c++17 -Wall -Wextra
```

This will compile and build your \*.exe file. Which if you don't specify the name it will output an `a.exe` file. To specify use the `-o` flag. For example

```PowerShell
g++ main.c -std=c++17 -Wall -Wextra -o ExampleProgram
```

This will output an `*.exe` file named `ExampleProgram.exe`.

#### Classes and Objects

> Source: [Geeks for Geeks](https://www.geeksforgeeks.org/cpp/c-classes-and-objects/)

In C++, which is an object oriented programming language, we got `classes` and `objects`. So, what exactly they are?

Let see this code example:

```C++
#include <iostream>
using namespace std;

class GfG {
  public:
  // Data member
  int val;
  // Member function
  void show() {
    cout << "Value: " << val << endl;
  }
}

int main() {
  // To create and object, check the following code:
  GfG Object;
  // Now, we define the variable val, found in the class.
  Object.val = 3;
  // Now, we call the function, which print in the console the variable.
  Object.show();

  // Expected output in the console:
  // Value: 3

  return 0;
}
```

##### Classes

A class is a **user-defined data type**, which holds its own data members and member functions that can be accessed and used by creating an instance of that class. A C++ clas is like a blueprint for an object.

##### Objects.

In this section I will focus only in C++, how it works, errors types and more.

#### Namespaces

Namespaces are special container or a label that holds a group of names - like variables, functions, or classes - to avoid confusion when we have the same name used in different parts of the program.

**Code example**

```C++
#include <iostream>

namespace room1 {
  void greet() {
    std::cout << "Hello from room 1!" << std::endl;
  }
}

namespace room2 {
  void greet() { // The function is the same name as room1.
    std::cout << "Hello from room 2!" << std::endl;
  }
}

int main() {
  Room1::greet(); // With *:: we specify the namespace to use.
  Room2::greet();

  return 0;
}
```

There is inbuilt namespaces, like `std::*`

---

### Java

#### Errors and exception types

In Java there are multiple errors and exception, understanding them and how to approach them will give us a better advantage, because we know what we could be dealing with.

> Source: [w3school Java course](https://www.w3schools.com/java/java_ref_errors.asp)

- `ArithmeticError`: this one means there is a numeric calculation mistake.

- `ArrayIndexOutOfBoundsException`: Is when trying to access an index number that does not exist in an array.

- `ClassFormatError`: Is when a class file cannot be accessed.

> A class file in OOP is a file that contains compiled code for a class, this are usually executed by a virtual machine, such as _JVM (Java Virtual Machine)_.

- `ClassNotFoundException`: Occurs when trying to access a class that does not exist.

- `ConcurrentModificationException`: Occurs when an element is added or removed from iterables.

- `IncompatibleClasChangeError`: Occurss when there's been a change in a base class after a child class has already been initialized.

- `InputMismatchException`: Occurs when entering a wrong input (e.g: text in a numerical input).

- `InterruptedException`: Occurs when a thread is interrupted while waiting/sleeping.

- `InvalidClassException`: Occurs when the Serialization runtime observes a problem with a class.

- `IOException`: Occurs when an input or output operation fails.

- `NegativeArraySizeException`: Occurs when trying to create an array with negative size.

- `NoClassDefFoundError`: Occurs when the class is not found at runtime.

- `NoSuchFieldException`: Occurs when trying to access a field/variable that does not exist.

- `NoSuchMethodException`: Occurs when trying to access a class method that does not exist.

- `NullPointerException`: Occurs when trying to access an object reference that is `null`.

- `NumberFormatException`: Occurs when is not possible to convert a specified string to a numeric type.

- `RuntimeException`: Occurs when an exception occurs at runtime.

- `StringIndexOutOfboundsException`: Occurs when trying to access a character in a string that does not exist.

- `TypeNotPresentException`: Occurs when a type cannot be found.

- `IllegalArgumentException`: Occurs when an illegal argument is passed to a method.

- `IllegalStateException`: Occurs when a method is called at an illegal time.

---

### Types of errors in Java

There are three types of errors in Java, and I believe in most of programming language.

- Compile-Time error

- Runtime Error

- Logical error

**Compile time error**: Detected by the compiler. Prevents code from running.

**Runtime error**: Occurs while the program is running, often causes crashes.

**Logical error**: Usually the code runs, but gives a unexpected results. Usually are the hardest to find, the IDE never show them.

An example of **Compile time error**:

```Java
int x = 5 // Expected ';'. The compiler wont finish the task and will throw an error.
System.out.print(x);
```

Another could be a `undeclared variable`

```Java
System.out.println(x); // In this code snippet, I didn't define x.
```

Another example, is a mismatched type. If I define `int x` but the definition is another type like `"hello"` which is an string, there is a mismatch type.

Now, **run time errors** examples.

The two most common are _division by zero_ and _array index out of bounds_.

> If you check the errors and exception type beforehand, you could see that _division by zero_ throw ArithmeticException and _array index out of bounds_ throw ArrayIndexOutOfBoundsException.

**Division by zero code example**

```Java
int x = 10;
int y = 0;

int result = x / y; // Will throw a runtime error; 10 / 0 is undefined.
System.out.println(result);
```

**Array index out of bounds**

```Java
int[] numbers = {1, 2, 3};

System.out.println(numbers[8]); // The array length is 3, the max index would be 2.
```

**Logical errors** which are the hardest to find if you code is bad and not documented, are errors that usually doesn't show up in the logs unless you handle them.

A clear example is:

```Java
int x = 10;
int y = 2;

int sum = x - y;
System.out.println("x + y" + sum); // Expected 12. Since 10 + 2 is 12. But gets 8, because we typed x - y.
```

The logical error are mostly the programmer's mistake.

#### Good habits to avoid errors.

- Use meaningful variables names

- Read the error message carefully. What lines does it mention?

> this involve understanding the types of errors and exception

- Check for missing semicolons or braces.

- Look for typos in variable or method names.

#### UML (Unified Modeling Language)

Unified Modeling Language is a general-purpose modeling language. the main aim of UML is to define a standard way to visualize the way a system has been designed. It is quite similar to blueprints used in other fields of engineering.

UML is not a programming language, it is rather a visual language.

> Source: [GeeksforGeeks - Unified Modeling Language](https://www.geeksforgeeks.org/system-design/unified-modeling-language-uml-introduction/)

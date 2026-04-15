# Documentation for the file str02.c

Example of what is correct and incorrect.

```c
      if ( frase == "Si" )

      if ( strcmp(frase,"Si") == 0)

      frase = "Si";

      frase = otra;

      if (strcpy(frase,"Si")) {...} // Correct

      strcpy("Si",frase); // Incorrect

      strcpy(frase,otra); //correct
```

When try to uppercase without `toupper();` function, you approach with the next equation:

```c
letter - ('a' - 'A'); // This equals to letter - 32
```

If you wish, to change from uppercase to lowercase without using `tolower();`. Just do the opposite:

```c
letter + ('a' - 'A');
```

## Word case - First letter of the phrase ALWAYS uppercase

In C programming language, strings are treated as arrays. This means, the first letter is `String[0];`. If you wish, you could just do

```c
// Uppercase function
void UpperCase(char string[]) {
   if (string[0] <= 'a' && >= 'z') { // This check if the following data is a character.
      string[0] = string[0] - ('a' - 'A'); // This change it from lowercase to upperCase
   }
   printf("Phrase: %s", string);
}
```

In production, obviously, you would use the function `isalpha()` which checks if the character is alphabetic.
> Quick note: there is also a `isdigit()` function.

```c
void UpperCase(char string[]) {
   if (isalpha(string[0])) {
      string[0] = toupper(string[0]);
   }
}
```

So, now. How we would do uppercase to lowercase?
Simple!

```c
void LowerCase(char string[]) {
   if (string[0] <= 'a' && >= 'z') {// Check if it is a character
      string[0] = string[0] + ('a' - 'A'); // As you can see, instead of a minus 32, is a plus 32.
   }
}
```

### Avoid buffer overflow

When using `strncpy()`, you can get buffer overflow if you don't define an string size. Let see the exame:

```c
#include <stdio.h>

int main(void) {
   char String1[] =-"Hello World" // This is a 12 array. (remember the null value at the end.)
   char Target[] = "Hello"; // This, automatically get defined as `char Target[6] = {'H', 'e', 'l','l','o','\0'};`

   strncpy(Target, String1, 10); // The text you want to copy in another string goes after the targetted string.
   printf("%s\n", Target);
}
```

> You may wonder what is int main(void), more specifically what is (void), it means get no arguments.

As you can see, and if you know how the compiler works, String1 will be automatically defined as `char String1[12]` and `char Target[6]`. So, when we try to copy `String1` into `Target`, we end up in a weird output and we get buffer overflow.

You can avoid this issue, for example:

```c
#include <stdio.h>
#include <ctype.h>

int main(void) {
   char String1[] =-"Hello World" 
   char Target[] = "Hello";

   strncpy(Target, String1, sizeof(otra) - 1);
   otra[sizeof(Target) - 1] = '\0';
   printf("Estado 3: %s\n", Target);

   return 0
}
```

Or, you can try this other solution, which is easier.

```c
#include <stdio.h>
#include <ctype.h>

// Define a macro
#define MAXSTRLENGTH 128

int main(void) {
   char String1[MAXSTRLENTGH] =-"Hello World" 
   char Target[MAXTRLENGTH] = "Hello";

   strncopy(Target, String1, 10);
   printf("%s\n", Target);

   return 0;
}

```

Defining the array size, to avoid this issue is better and even more understandable.

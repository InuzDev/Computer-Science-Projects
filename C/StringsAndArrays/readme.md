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
   if (string[0] <= 'a' && >= 'z') {
      string[0] = string[0] - ('a' - 'A');
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

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

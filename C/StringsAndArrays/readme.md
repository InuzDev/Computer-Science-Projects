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

# Practice Documentation

Here I will explain the flow of the data, of each practice.

## Characters

First, the program in general, manage arrays of characters, in this case a phrase. There are five functions

The first one being:

```c
void Mayusculas(char *str, int n) {...}
```

Are used to turn the character chosen to upper case

```c
void Minusculas(char *str, int n) {...}
```

The second, which is to turn the character to lower case

```c
void TitleCase(char *str, int n) {...}
```

This one being used to change the title's first letter from lower to upper case and set everything else to lower case

```c
void TitleInvertCase(char *str, int n) {...}
```

To invert the case, depending their actual case, if is it upper, it will turn to lower case, else if it is lower case, it will turn to upper case.

Now, the most important function being `void buscarReemplazar` is used to search and replace a word.

It have three integers variables:

```c
   int lenStr = strlen(str);
   int lenSearch = strlen(search);
   int lenReplace = strlen(replace);
```

> The original code may be in spanish, but same dynamic and logic

Then, we will run a loop that will check if it match.

If it doesn't match, the variable `coincide` will be 0 and break the second loop and keep checking. else if it does match, it will run a loop to change the world, depending on the case of the first world.

## Matrix exercise

So the problem is that we got a disordered matrix, and we need to sort it.

The approach we take is to sort first the matrix in arrays of integers, then we make the function `void ordernar(int matrix[][100], int f, int c, int n) {...}`

Back to the first function is

`int *datos` are the data of the matrix, and `int ndatos` are the amount of data stored in the matrix.

```c
void ordernar (int *datos, int ndatos)
```

We make a loop to scan the matrix, `ndatos - 1`, the `-1` is because arrays index start in 0, so we need to reduce 1.

Example; if the array size is 10, then the the item in the last index would be `arr[9]` and the first is `arr[0]`.

```c
for (int i = 0; i < ndatos - 1; i++)
```

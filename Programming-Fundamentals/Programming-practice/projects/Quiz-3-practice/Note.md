# Quiz 3 practice

Since quiz 3 is mostly reading code and explaining it, I'm going to make it a md.

To important points involve knowing the needed code to reserve memory for a value.

## Point 1

We got defined:

```c
typedef struct {
   char matricula[9], nombre[41];
   int cantcred, cantasig, totalpuntos;
} EST;
```

From the pointer `pest` we got access to `n` students. Write the instructions that allow to reduce a `m` students. Note that `m` is minor than `n`.

The approach I would take to do this is:

```c
EST *temp = (EST *) realloc(pest, m * sizeof(EST));
if (temp == NULL) {
   // An error occured, but pest is still valid.
   printf("Error at reassigning memory\n");
} else {
   pest = temp;
}
```

Now, if the task is to store n students in a way that allow adding more in case it is requested by the user, we do the following:

```c
EST *pest;
pest = (EST *) malloc(n * sizeof(EST));
```

If you want to reserve clean memory, use `calloc();`

```c
EST *pest;
pest = (EST *) calloc(n, sizeof(EST));
```

Now, the part to allow adding more students:

```c
int Newstudents;
printf("How many students want to add? ");
scanf("%d", &Newstudents);

EST *temp = (EST *) realloc(pest, (n + Newstudents) * sizeof(EST));

if (temp == NULL) {
   printf("Error assigning memory\n");
} else {
   pest = temp;
   n = n + Newstudents;
}
```

## After taking the quiz

All the examples shown before, are made to try and understand.

Now, in the quiz, I made right some parts about the dynamic memory, but I need to get better understanding on the logical steps.

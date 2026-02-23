# Computer Science Projects - Charles David

This repository is to showcase my progress in the major of computer science.

## Topics

I will share any code learnt in the computer science class. Also planning on integrating how I learn flowcharts and how I create them, so I can receive easy feedback.

The actual topics right now are:

- Introduction to algorithms, Algorithms and data structure (Basic)
- How C manipulates data like character and a strings.

## Procedure to run the programs

You can simply run the compiler as normal

```bash
gcc -std=gnu99 main.c -o {name of the file} -Wall -Wextra
```

Or run the Makefile, I often leave this one in every project from now on to simplify the process.

```makefile
CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra

SRC = main.c
# I often specify the targetted output as the name of the project.
TARGET = {name of the project}

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) && .\$(TARGET)
```

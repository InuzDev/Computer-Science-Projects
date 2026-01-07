/*
 *
 * Zed Editor test zone - Charles David.
 *
 * Main.c - C Lang test
 */

#include <stdio.h> // It seems to automatically find the path for the header file.

#define VariableA 10

int main(void) {
    int Input = 0;

    printf("Welcome to Zed Editor Test Zone :");

    // blame me, but I'm going to use AI lol
    // scanf("%d", &Input); // Even though we use auto completation, making us go fast in the editor. . .
    scanf_s("%d", &Input); // We got an example, it shows us the format we need to code it.
    printf("DEBUGGER : -> %d", Input);
    return 0;
}

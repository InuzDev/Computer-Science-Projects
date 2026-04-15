#include <conio.c>

#define POSXINI 15
#define POSYINI 5

void setcolor(int ct, int cf);
void colordefault(void);

int main() {
   setcolor(MAGENTA, YELLOW);
   gotoxy(POSXINI, POSYINI);
   colordefault();
   return 0;
}

/**
 * Funcion: (void) setcolor
 * Argumentos: (int) ct, cf
 * Objetivo: Poner un color especifico dado en los parametros.
 */
void setcolor(int ct, int cf) {
   textcolor(ct);
   textbackground(cf);
}

/**
 * Funcion: (void) colordefault
 * Objetivo: poner el color que esta por defecto en la terminal
 */
void colordefault(void) { setcolor(LIGHTGRAY, BLACK); }

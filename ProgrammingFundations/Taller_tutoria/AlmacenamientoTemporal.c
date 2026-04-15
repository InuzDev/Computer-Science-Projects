#include <stdio.h>

#define FILAS_MATRIX 3

long generarNumeroTicket(int num, int clientesCajero[][num], int serviciosAtiende[][num], int *cajeroSeleccionado, int *posicionCliente);

int main() {
   int MaxClientLine = 1;

   while (1) {
      printf("Introduzca la maxima cantidad de clientes en fila: ");
      scanf("%d", &MaxClientLine);
      if (MaxClientLine <= 0) {
         printf("El maximo de clientes debe ser 1 o mayor a 1\n");
         continue;
      } else {
         break;
      }
   }

   int clientesCajero[FILAS_MATRIX][MaxClientLine];
   int ServiciosAtiendeCajero[FILAS_MATRIX][MaxClientLine];

   for (int fil = 0; fil < FILAS_MATRIX; fil++) {
      for (int col = 0; col < MaxClientLine; col++) {
         clientesCajero[fil][col] = 0;
         ServiciosAtiendeCajero[fil][col] = 0;

         printf("Clientes cajero: %d\nServicios Atiende Cajero: %d\n", clientesCajero[fil][col], ServiciosAtiendeCajero[fil][col]);
      }
   }

   return 0;
}
//                                                                                                     3                       3
// long generarNumeroTicket(int num, int clientesCajero[][num], int serviciosAtiende[][num], int *cajeroSeleccionado, int *posicionCliente) {
//    long NumTicket = 0;

//    return NumTicket;
// }

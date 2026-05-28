#include <stdio.h>

int main() {
   int MaxCap = 0, CantStop = 0, NoSubieron = 0, ParadasHechas = 0, PasajerosAbordo = 0, DispEspacio = MaxCap, PasajerosSubieron = 0, PasajerosBajaron = 0, EmptyStops = 0, ContadorParadas = 0;

   printf("Introducir la capacidad maxima del autobus\n");
   scanf("%d", &MaxCap);

   while (MaxCap <= 0) {
      printf("La capacidad maxima debe ser mayor a 0\n");
      scanf("%d", &MaxCap);
   }
   DispEspacio = MaxCap;

   printf("Introducir la cantidad total de paradas");
   scanf("%d", &CantStop);

   while (CantStop < 2) {
      printf("La cantidad total de paradas deben ser mayor o igual 2\n");
      scanf("%d", &CantStop);
   }

   while (ContadorParadas < CantStop) {
      if (ContadorParadas <= 1) {
         printf("Ingrese la cantidad de pasajeros que subieron");
         scanf("%d", &PasajerosSubieron);

         PasajerosAbordo += PasajerosSubieron;
         printf("Pasajeros Actuales: %d", PasajerosAbordo);
      } else {
         printf("Cantidad de pasajeros que bajan: ");
         scanf("%d", &PasajerosBajaron);

         if (PasajerosBajaron > PasajerosAbordo) {
            printf("Los pasajeros que bajan no puede ser mayor a los pasajeros abordo");
            scanf("%d, &PasajerosBajaron");
         }

         PasajerosAbordo -= PasajerosBajaron;
         DispEspacio += PasajerosBajaron;

         printf("Cantidad de pasajeros que suben: ");
         scanf("%d", &PasajerosSubieron);

         if (PasajerosSubieron == 0) {
            EmptyStops++;
         }

         if (PasajerosSubieron > DispEspacio) {
            NoSubieron += PasajerosSubieron - DispEspacio;

            PasajerosAbordo = MaxCap;
            printf("Solo pudieron entrar %d personas, No pudieron entrar %d", DispEspacio, (PasajerosSubieron - DispEspacio));
         } else {
            PasajerosAbordo += PasajerosSubieron;
         }
      }

      if (ContadorParadas == CantStop - 1) {
         printf("Ingrese la cantidad de pasajeros que bajaron");
         scanf("%d", &PasajerosBajaron);

         PasajerosAbordo -= PasajerosBajaron;
         printf("Pasajeros actuales: %d", PasajerosAbordo);
      }

      ParadasHechas++;
   }
   return 0;
}

#include <stdio.h>

// Macros definitions
#define DEFAULT_VAL 100
#define DISTANCE_6_VAL 30
#define DISTANCE_6_EQ ((6.0 - 3.0) * 30)
#define DISTANCE_10_VAL 15

#define DISTANCE_3_KM 3.00
#define DISTANCE_6_KM 6.00
#define DISTANCE_10_KM 10

#define CLIENT_CONNECTED_TAX_MIN 20
#define CLIENT_CONNECTED_TAX_MAX 40

#define CLIMATE_RAIN_TAX 0.3

#define DECLINED_TASK -1

#define CLIMATE_RAIN 2

#define MIN_VALUE 200.00
#define MID_VALUE 300.00
#define MAX_VALUE 400.00

// Definition of prototypes
float tarifaAPagarCliente(float distanciaCliente, int clima, int Cantidadclientesconectados);

int main(void) {
   // Variables definition
   int ClientConnected = 0, ClientToEvaluate = 0, ClimateStatus = 1,
       EvaluatedClient = 0, _200And300_Tasks = 0, _300And400_Tasks = 0,
       _MoreThan400_tasks = 0, Declined_Tasks = 0, Temp_ClientToEvaluate = 0;

   float MaxValue = 0.0f, MinValue = 999999.0f, ClientDistance = 0.0f,
         Payments[ClientToEvaluate], ActualPayment = 0.0f;

   printf("Introducir la cantidad de clientes a evaluar: ");
   scanf("%d", &ClientToEvaluate);

   while (ClientToEvaluate <= 0) {
      printf("Los clientes a evaluar debe ser mayor a 0");
      scanf("%d", &ClientToEvaluate);
      if (ClientToEvaluate != 0) {
         break;
      } else {
         continue;
      }
   }

   printf("Introducir la cantidad de clientes conectados: ");
   scanf("%d", &ClientConnected);

   Temp_ClientToEvaluate = ClientToEvaluate;

   while (ClientToEvaluate != 0) {
      printf("Introduzca la distancia del cliente en Kilometros (Valores decimales) [0.0]: ");
      scanf("%f", &ClientDistance);

      if (ClientDistance > 10.0) {
         printf("No existen locales a una distancia mayor a 10 kilometros\n");
      } else if (ClientDistance <= 10.0) {
         printf("Introducir el estado del clima actual: [1 = Soleado ; 2 = Lluvioso]: ");
         scanf("%d", &ClimateStatus);
      }

      Payments[EvaluatedClient] = tarifaAPagarCliente(ClientDistance, ClimateStatus, ClientConnected);

      if (Payments[EvaluatedClient] == DECLINED_TASK) {
         printf("Se ha declinado el envio, ya sea por no haber surcursales en su area o por decision propia.\n");
      } else {
         printf("Tarifa a pagar: %2f\n", Payments[EvaluatedClient]);
      }

      EvaluatedClient++;
      ClientToEvaluate--;
   }

   for (int index = 0; index < Temp_ClientToEvaluate; index++) {
      if (Payments[index] > MaxValue && Payments[index] != DECLINED_TASK) {
         MaxValue = Payments[index];
      }

      if (Payments[index] < MinValue && Payments[index] != DECLINED_TASK) {
         MinValue = Payments[index];
      }

      if (Payments[index] >= MIN_VALUE && Payments[index] <= MID_VALUE) {
         _200And300_Tasks++;
      }
      if (Payments[index] > MID_VALUE && Payments[index] <= MAX_VALUE) {
         _300And400_Tasks++;
      }
      if (Payments[index] > MAX_VALUE) {
         _MoreThan400_tasks++;
      }

      if (Payments[index] == DECLINED_TASK) {
         Declined_Tasks++;
      }
   }

   printf("---------------------- Resumen Datos -----------------------\n");
   printf("Informacion                                   Valor\n");
   printf("Pedidos entre 200.00 y 300.00 %17d\n", _200And300_Tasks);
   printf("Pedidos entre 300.01 y 400.00 %17d\n", _300And400_Tasks);
   printf("Pedidos mayores a 400.00 pesos %16d\n", _MoreThan400_tasks);
   printf("Cantidad de pedidos rechazados %16d\n", Declined_Tasks);
   printf("------------------------------------------------------------\n");
   printf("Mayor tarifa: %f  ; Menor tarifa: %f\n", MaxValue, MinValue);

   return 0;
}

// Prototype build

float tarifaAPagarCliente(float distanciaCliente, int clima, int Cantidadclientesconectados) {
   float Tarifa = 0;

   if (distanciaCliente <= 3) {
      Tarifa = DEFAULT_VAL;
   } else if (distanciaCliente > DISTANCE_3_KM && distanciaCliente <= DISTANCE_6_KM) {
      Tarifa = DEFAULT_VAL + (distanciaCliente - DISTANCE_3_KM) * DISTANCE_6_VAL;
   } else if (distanciaCliente > DISTANCE_6_KM && distanciaCliente <= DISTANCE_10_KM) {
      Tarifa = DEFAULT_VAL + (DISTANCE_6_EQ) + (distanciaCliente - DISTANCE_6_KM) * DISTANCE_10_VAL;
   } else {
      return -1;
   }

   if (clima == CLIMATE_RAIN) {
      Tarifa += (Tarifa * CLIMATE_RAIN_TAX);
   }

   if (Cantidadclientesconectados >= CLIENT_CONNECTED_TAX_MIN && Cantidadclientesconectados <= CLIENT_CONNECTED_TAX_MAX) {
      Tarifa += (Tarifa * 0.03);
   } else if (Cantidadclientesconectados > CLIENT_CONNECTED_TAX_MAX) {
      Tarifa += (Tarifa * 0.05);
   }

   return Tarifa;
}

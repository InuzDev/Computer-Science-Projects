void ordenar(int *datos, int ndatos) {
   for (int i = 0; i < ndatos - 1; i++) {
      for (int j = 0; j < ndatos - 1 - i; j++) {
         if (datos[j] > datos[j + 1]) {
            int temp = datos[j];
            datos[j] = datos[j + 1];
            datos[j + 1] = temp;
         }
      }
   }
}

void ordenarMatriz(int matriz[][100], int f, int c, int n) {
   int total = f * c;
   int temp[total];
   int index = 0;

   for (int i = 0; i < f; i++) {
      for (int j = 0; j < c; j++) {
         temp[index++] = matriz[i][j];
      }
   }

   for (int i = 0; i < total; i += n) {
      int elementos = n;
      if (i + n > total) {
         elementos = total - i;
      }
      ordenar(&temp[i], elementos);
   }

   index = 0;
   for (int i = 0; i < f; i++) {
      for (int j = 0; j < c; j++) {
         matriz[i][j] = temp[index++];
      }
   }
}

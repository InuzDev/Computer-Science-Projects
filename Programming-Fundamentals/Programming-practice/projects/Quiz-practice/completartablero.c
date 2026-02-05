void completar_tablero(int dim, char tablero[dim][dim]) {
   for (int index = 0; index < dim; index++) {
      for (int jindex = 0; jindex < dim; jindex++) {
         if (tablero[index][jindex] == ' ') {
            int counter = 0;

            // Check all neighbors
            for (int dx = -1; dx < 1; dx++) {
               for (int dy = -1; dy < 1; dy++) {
                  if (dx == 0 && dy == 0) {
                     continue;
                  }

                  int ni = index + dx;
                  int nj = jindex + dy;

                  // check boundaries
                  if (ni >= 0 && ni < dim && nj >= 0 && nj < dim) {
                     if (tablero[ni][nj] >= 'A' && tablero[ni][nj] <= 'Z') {
                        counter++;
                     }
                  }
               }
            }
            tablero[index][jindex] = '0' + counter;
         }
      }
   }
}

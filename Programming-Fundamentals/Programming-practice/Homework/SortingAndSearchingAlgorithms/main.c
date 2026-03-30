#include <conio.c>
#include <string.h>

#define CANTIDAD_PALABRAS 8
#define LONGITUD_MAXIMA 30
#define ANCHO_OPCION_MENU 36

#define TECLA_PREFIJO_A 0
#define TECLA_PREFIJO_B 224
#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_ENTER 13

#define COLOR_NORMAL LIGHTGRAY
#define COLOR_SELECCION BLACK
#define FONDO_SELECCION LIGHTCYAN
#define COLOR_TITULO YELLOW
#define COLOR_DATO WHITE
#define COLOR_EXITO LIGHTGREEN
#define COLOR_ERROR LIGHTRED

#define INDICE_ATRAS_ORDENAMIENTO 5
#define INDICE_ATRAS_BUSQUEDA 2
#define INDICE_SALIR_PRINCIPAL 2

#define OPCIONES_MENU_ORDENAMIENTO 6
#define OPCIONES_MENU_BUSQUEDA 3
#define OPCIONES_MENU_PRINCIPAL 3

#define CANTIDAD_ALGORITMOS 5

#define OPCION_ORDENAMIENTO 0
#define OPCION_BUSQUEDA 1

#define OPCION_SECUENCIAL 0
#define OPCION_BINARIA 1

#define NO_ENCONTRADO -1

/*
 *  TIPO: PUNTERO A FUNCION DE ORDENAMIENTO
 *
 *  Representa cualquier funcion con firma:
 *    void f(char[][LONGITUD_MAXIMA], int)
 *  Permite invocar algoritmos desde una tabla sin if/else.
 */
typedef void (*FuncionOrdenamiento)(char[][LONGITUD_MAXIMA], int);

void pausar();
void imprimir_arreglo(char arreglo[][LONGITUD_MAXIMA], int cantidad);
void copiar_arreglo(char destino[][LONGITUD_MAXIMA], char origen[][LONGITUD_MAXIMA], int cantidad);
void intercambiar(char primera[], char segunda[]);
void obtener_datos_frescos(char destino[][LONGITUD_MAXIMA]);
void ordenar_burbuja(char arreglo[][LONGITUD_MAXIMA], int cantidad);
void ordenar_insercion(char arreglo[][LONGITUD_MAXIMA], int cantidad);
void ordenar_seleccion(char arreglo[][LONGITUD_MAXIMA], int cantidad);
void ordenar_shell(char arreglo[][LONGITUD_MAXIMA], int cantidad);
int particionar(char arreglo[][LONGITUD_MAXIMA], int izquierda, int derecha);
void quicksort_recursivo(char arreglo[][LONGITUD_MAXIMA], int izquierda, int derecha);
void ordenar_quicksort(char arreglo[][LONGITUD_MAXIMA], int cantidad);
int buscar_secuencial(char arreglo[][LONGITUD_MAXIMA], int cantidad, char *objetivo);
int buscar_binaria(char arreglo[][LONGITUD_MAXIMA], int cantidad, char *objetivo);
void ejecutar_ordenamiento(FuncionOrdenamiento algoritmo, char *nombreAlgoritmo);
void demo_busqueda_secuencial();
void demo_busqueda_binaria();
int ejecutar_menu(char *titulo, char *opciones[], int cantidadOpc);
void menu_ordenamiento();
void menu_busqueda();
void menu_principal();

int main() {
   _setcursortype(0);
   menu_principal();
   _setcursortype(100);
   return 0;
}

/*
 * Descripcion : Espera a que el usuario presione cualquier tecla.
 */
void pausar() {
   textcolor(DARKGRAY);
   cprintf("\n  Presione cualquier tecla para continuar...");
   textcolor(COLOR_NORMAL);
   getch();
}

/*
 * Descripcion : Imprime en pantalla los elementos de un arreglo de cadenas,
 *               con formato de lista entre corchetes.
 * Parametros  : arreglo  - arreglo de cadenas a imprimir.
 *               cantidad - numero de elementos del arreglo.
 */
void imprimir_arreglo(char arreglo[][LONGITUD_MAXIMA], int cantidad) {
   textcolor(COLOR_DATO);
   cprintf("  [ ");
   for (int indice = 0; indice < cantidad; indice++) {
      textcolor(LIGHTCYAN);
      cprintf("\"%s\"", arreglo[indice]);
      textcolor(COLOR_DATO);
      if (indice < cantidad - 1)
         cprintf(", ");
   }
   cprintf(" ]\r\n");
   textcolor(COLOR_NORMAL);
}

/*
 * Descripcion : Copia el contenido de un arreglo de cadenas origen
 *               en un arreglo de cadenas destino.
 * Parametros  : destino  - arreglo donde se copiara el contenido.
 *               origen   - arreglo fuente (solo lectura).
 *               cantidad - numero de elementos a copiar.
 */
void copiar_arreglo(char destino[][LONGITUD_MAXIMA],
                    char origen[][LONGITUD_MAXIMA],
                    int cantidad) {
   for (int indice = 0; indice < cantidad; indice++) {
      strncpy(destino[indice], origen[indice], LONGITUD_MAXIMA - 1);
      destino[indice][LONGITUD_MAXIMA - 1] = '\0';
   }
}

/*
 * Descripcion : Intercambia el contenido de dos cadenas de caracteres.
 * Parametros  : primera - primera cadena a intercambiar.
 *               segunda - segunda cadena a intercambiar.
 */
void intercambiar(char primera[], char segunda[]) {
   char temporal[LONGITUD_MAXIMA];
   strncpy(temporal, primera, LONGITUD_MAXIMA - 1);
   strncpy(primera, segunda, LONGITUD_MAXIMA - 1);
   strncpy(segunda, temporal, LONGITUD_MAXIMA - 1);
}

/*
 * Descripcion : Carga una copia fresca del arreglo de datos de prueba
 *               en el destino dado, para que cada demo parta del mismo estado.
 * Parametros  : destino - arreglo donde se cargaran los datos originales.
 */
void obtener_datos_frescos(char destino[][LONGITUD_MAXIMA]) {
   char datosPrueba[CANTIDAD_PALABRAS][LONGITUD_MAXIMA] = {
       "Mango", "Banana", "Pera", "Naranja",
       "Uva", "Kiwi", "Limon", "Cereza"};
   copiar_arreglo(destino, datosPrueba, CANTIDAD_PALABRAS);
}

/*
 * Descripcion : Ordena un arreglo de cadenas usando el algoritmo Burbuja.
 *               Compara pares adyacentes e intercambia si estan en orden
 *               incorrecto. Repite hasta que no ocurra ningun intercambio
 *               en una pasada completa (flag de optimizacion).
 *               Complejidad: O(n^2) peor caso, O(n) mejor caso.
 * Parametros  : arreglo  - arreglo de cadenas a ordenar (se modifica).
 *               cantidad - numero de elementos del arreglo.
 */
void ordenar_burbuja(char arreglo[][LONGITUD_MAXIMA], int cantidad) {
   int huboIntercambio;
   for (int pasada = 0; pasada < cantidad - 1; pasada++) {
      huboIntercambio = 0;
      for (int posicion = 0; posicion < cantidad - pasada - 1; posicion++) {
         if (strcmp(arreglo[posicion], arreglo[posicion + 1]) > 0) {
            intercambiar(arreglo[posicion], arreglo[posicion + 1]);
            huboIntercambio = 1;
         }
      }
      if (!huboIntercambio)
         break;
   }
}

/*
 * Descripcion : Ordena un arreglo de cadenas usando el algoritmo de Insercion.
 *               Toma cada elemento y lo inserta en su posicion correcta
 *               dentro de la parte ya ordenada del arreglo.
 *               Complejidad: O(n^2) peor caso, O(n) si esta casi ordenado.
 * Parametros  : arreglo  - arreglo de cadenas a ordenar (se modifica).
 *               cantidad - numero de elementos del arreglo.
 */
void ordenar_insercion(char arreglo[][LONGITUD_MAXIMA], int cantidad) {
   char elementoClave[LONGITUD_MAXIMA];
   for (int actual = 1; actual < cantidad; actual++) {
      strncpy(elementoClave, arreglo[actual], LONGITUD_MAXIMA - 1);
      elementoClave[LONGITUD_MAXIMA - 1] = '\0';
      int anterior = actual - 1;
      while (anterior >= 0 && strcmp(arreglo[anterior], elementoClave) > 0) {
         strncpy(arreglo[anterior + 1], arreglo[anterior], LONGITUD_MAXIMA - 1);
         anterior--;
      }
      strncpy(arreglo[anterior + 1], elementoClave, LONGITUD_MAXIMA - 1);
   }
}

/*
 * Descripcion : Ordena un arreglo de cadenas usando el algoritmo de Seleccion.
 *               En cada iteracion busca el minimo en la parte no ordenada
 *               y lo coloca al inicio de esa parte.
 *               Complejidad: O(n^2) siempre.
 * Parametros  : arreglo  - arreglo de cadenas a ordenar (se modifica).
 *               cantidad - numero de elementos del arreglo.
 */
void ordenar_seleccion(char arreglo[][LONGITUD_MAXIMA], int cantidad) {
   for (int inicio = 0; inicio < cantidad - 1; inicio++) {
      int indiceMinimo = inicio;
      for (int buscador = inicio + 1; buscador < cantidad; buscador++) {
         if (strcmp(arreglo[buscador], arreglo[indiceMinimo]) < 0)
            indiceMinimo = buscador;
      }
      if (indiceMinimo != inicio)
         intercambiar(arreglo[inicio], arreglo[indiceMinimo]);
   }
}

/*
 * Descripcion : Ordena un arreglo de cadenas usando el algoritmo Shell.
 *               Extension de Insercion: ordena elementos separados por un
 *               intervalo (secuencia de Knuth) que se reduce hasta llegar a 1.
 *               Complejidad: O(n log^2 n) con secuencia de Knuth.
 * Parametros  : arreglo  - arreglo de cadenas a ordenar (se modifica).
 *               cantidad - numero de elementos del arreglo.
 */
void ordenar_shell(char arreglo[][LONGITUD_MAXIMA], int cantidad) {
   int intervalo = 1;
   while (intervalo < cantidad / 3)
      intervalo = intervalo * 3 + 1;

   while (intervalo >= 1) {
      for (int actual = intervalo; actual < cantidad; actual++) {
         char elementoClave[LONGITUD_MAXIMA];
         strncpy(elementoClave, arreglo[actual], LONGITUD_MAXIMA - 1);
         elementoClave[LONGITUD_MAXIMA - 1] = '\0';
         int anterior = actual - intervalo;
         while (anterior >= 0 && strcmp(arreglo[anterior], elementoClave) > 0) {
            strncpy(arreglo[anterior + intervalo], arreglo[anterior], LONGITUD_MAXIMA - 1);
            anterior -= intervalo;
         }
         strncpy(arreglo[anterior + intervalo], elementoClave, LONGITUD_MAXIMA - 1);
      }
      intervalo /= 3;
   }
}

/*
 * Descripcion : Reordena un subarreglo en torno a un pivote (ultimo elemento),
 *               dejando a su izquierda los menores y a su derecha los mayores.
 *               Funcion auxiliar de ordenar_quicksort.
 * Parametros  : arreglo   - arreglo de cadenas a particionar.
 *               izquierda - indice inicial del subarreglo.
 *               derecha   - indice final del subarreglo (pivote).
 * Retorno     : int - indice final del pivote tras la particion.
 */
int particionar(char arreglo[][LONGITUD_MAXIMA], int izquierda, int derecha) {
   char *pivote = arreglo[derecha];
   int indiceMenor = izquierda - 1;
   for (int explorador = izquierda; explorador < derecha; explorador++) {
      if (strcmp(arreglo[explorador], pivote) <= 0) {
         indiceMenor++;
         intercambiar(arreglo[indiceMenor], arreglo[explorador]);
      }
   }
   intercambiar(arreglo[indiceMenor + 1], arreglo[derecha]);
   return indiceMenor + 1;
}

/*
 * Descripcion : Funcion recursiva interna de QuickSort. Particiona el
 *               subarreglo y ordena recursivamente cada mitad.
 * Parametros  : arreglo   - arreglo de cadenas a ordenar.
 *               izquierda - indice inicial del subarreglo actual.
 *               derecha   - indice final del subarreglo actual.
 */
void quicksort_recursivo(char arreglo[][LONGITUD_MAXIMA], int izquierda, int derecha) {
   if (izquierda < derecha) {
      int posicionPivote = particionar(arreglo, izquierda, derecha);
      quicksort_recursivo(arreglo, izquierda, posicionPivote - 1);
      quicksort_recursivo(arreglo, posicionPivote + 1, derecha);
   }
}

/*
 * Descripcion : Ordena un arreglo de cadenas usando el algoritmo QuickSort.
 *               Divide el arreglo en torno a un pivote y ordena cada particion
 *               de forma recursiva (divide y conquista).
 *               Complejidad: O(n log n) promedio, O(n^2) peor caso.
 * Parametros  : arreglo  - arreglo de cadenas a ordenar (se modifica).
 *               cantidad - numero de elementos del arreglo.
 */
void ordenar_quicksort(char arreglo[][LONGITUD_MAXIMA], int cantidad) {
   quicksort_recursivo(arreglo, 0, cantidad - 1);
}

/*
 * Descripcion : Busca una cadena en un arreglo recorriendolo de inicio a fin.
 *               No requiere que el arreglo este ordenado.
 *               Complejidad: O(n).
 * Parametros  : arreglo  - arreglo de cadenas donde buscar.
 *               cantidad - numero de elementos del arreglo.
 *               objetivo - cadena que se desea encontrar.
 * Retorno     : int - indice del elemento encontrado, o NO_ENCONTRADO (-1).
 */
int buscar_secuencial(char arreglo[][LONGITUD_MAXIMA], int cantidad,
                      char *objetivo) {
   for (int posicion = 0; posicion < cantidad; posicion++) {
      if (strcmp(arreglo[posicion], objetivo) == 0)
         return posicion;
   }
   return NO_ENCONTRADO;
}

/*
 * Descripcion : Busca una cadena en un arreglo ORDENADO dividiendo el espacio
 *               de busqueda a la mitad en cada paso.
 *               PRECONDICION: el arreglo debe estar ordenado alfabeticamente.
 *               Complejidad: O(log n).
 * Parametros  : arreglo  - arreglo de cadenas ordenado donde buscar.
 *               cantidad - numero de elementos del arreglo.
 *               objetivo - cadena que se desea encontrar.
 * Retorno     : int - indice del elemento encontrado, o NO_ENCONTRADO (-1).
 */
int buscar_binaria(char arreglo[][LONGITUD_MAXIMA], int cantidad, char *objetivo) {
   int limiteInferior = 0;
   int limiteSuperior = cantidad - 1;
   while (limiteInferior <= limiteSuperior) {
      int puntoMedio = (limiteInferior + limiteSuperior) / 2;
      int comparacion = strcmp(arreglo[puntoMedio], objetivo);
      if (comparacion == 0)
         return puntoMedio;
      if (comparacion < 0)
         limiteInferior = puntoMedio + 1;
      else
         limiteSuperior = puntoMedio - 1;
   }
   return NO_ENCONTRADO;
}

/*
 * Descripcion : Ejecuta y muestra en pantalla la demostracion de un algoritmo
 *               de ordenamiento recibido por puntero a funcion. Imprime el
 *               arreglo antes y despues del ordenamiento.
 * Parametros  : algoritmo        - puntero a la funcion de ordenamiento.
 *               nombreAlgoritmo  - nombre del algoritmo para mostrar en pantalla.
 */
void ejecutar_ordenamiento(FuncionOrdenamiento algoritmo, char *nombreAlgoritmo) {
   char datos[CANTIDAD_PALABRAS][LONGITUD_MAXIMA];
   obtener_datos_frescos(datos);

   clrscr();
   textcolor(COLOR_TITULO);
   cprintf("  === %s ===\r\n\n", nombreAlgoritmo);

   textcolor(COLOR_NORMAL);
   cprintf("  Arreglo original:\r\n");
   imprimir_arreglo(datos, CANTIDAD_PALABRAS);

   algoritmo(datos, CANTIDAD_PALABRAS); /* invocacion via puntero a funcion */

   cprintf("\r\n  Arreglo ordenado:\r\n");
   imprimir_arreglo(datos, CANTIDAD_PALABRAS);
   pausar();
}

/*
 * Descripcion : Muestra la demostracion del algoritmo de busqueda secuencial
 *               con dos ejemplos: un elemento existente y uno inexistente.
 */
void demo_busqueda_secuencial() {
   char datos[CANTIDAD_PALABRAS][LONGITUD_MAXIMA];
   obtener_datos_frescos(datos);

   clrscr();
   textcolor(COLOR_TITULO);
   cprintf("  === Busqueda Secuencial ===\r\n\n");
   textcolor(COLOR_NORMAL);
   cprintf("  Arreglo (sin ordenar):\r\n");
   imprimir_arreglo(datos, CANTIDAD_PALABRAS);

   char *objetivoExistente = "Naranja";
   char *objetivoInexistente = "Durazno";

   textcolor(COLOR_NORMAL);
   cprintf("\r\n  Buscando: ");
   textcolor(LIGHTCYAN);
   cprintf("\"%s\"", objetivoExistente);
   textcolor(COLOR_NORMAL);
   cprintf(" -- recorre posicion a posicion.\r\n");

   int resultado = buscar_secuencial(datos, CANTIDAD_PALABRAS, objetivoExistente);
   if (resultado != NO_ENCONTRADO) {
      textcolor(COLOR_EXITO);
      cprintf("\r\n  >> Encontrado en la posicion [%d]\r\n", resultado);
   } else {
      textcolor(COLOR_ERROR);
      cprintf("\r\n  >> No encontrado.\r\n");
   }

   textcolor(COLOR_NORMAL);
   cprintf("\r\n  Buscando: ");
   textcolor(LIGHTCYAN);
   cprintf("\"%s\"", objetivoInexistente);
   textcolor(COLOR_NORMAL);
   cprintf(" (no existe en el arreglo)\r\n");

   resultado = buscar_secuencial(datos, CANTIDAD_PALABRAS, objetivoInexistente);
   if (resultado != NO_ENCONTRADO) {
      textcolor(COLOR_EXITO);
      cprintf("\r\n  >> Encontrado en la posicion [%d]\r\n", resultado);
   } else {
      textcolor(COLOR_ERROR);
      cprintf("\r\n  >> No encontrado (recorrio todo el arreglo).\r\n");
   }

   textcolor(COLOR_NORMAL);
   pausar();
}

/*
 * Descripcion : Muestra la demostracion del algoritmo de busqueda binaria.
 *               Ordena el arreglo previamente con QuickSort (requisito previo)
 *               y busca un elemento existente.
 */
void demo_busqueda_binaria() {
   char datos[CANTIDAD_PALABRAS][LONGITUD_MAXIMA];
   obtener_datos_frescos(datos);
   ordenar_quicksort(datos, CANTIDAD_PALABRAS);

   clrscr();
   textcolor(COLOR_TITULO);
   cprintf("  === Busqueda Binaria ===\r\n\n");
   textcolor(COLOR_NORMAL);
   cprintf("  Arreglo (ordenado previamente con QuickSort):\r\n");
   imprimir_arreglo(datos, CANTIDAD_PALABRAS);

   char *objetivo = "Limon";
   textcolor(COLOR_NORMAL);
   cprintf("\r\n  Buscando: ");
   textcolor(LIGHTCYAN);
   cprintf("\"%s\"", objetivo);
   textcolor(COLOR_NORMAL);
   cprintf(" -- divide el arreglo a la mitad cada paso.\r\n");

   int resultado = buscar_binaria(datos, CANTIDAD_PALABRAS, objetivo);
   if (resultado != NO_ENCONTRADO) {
      textcolor(COLOR_EXITO);
      cprintf("\r\n  >> Encontrado en la posicion [%d]\r\n", resultado);
   } else {
      textcolor(COLOR_ERROR);
      cprintf("\r\n  >> No encontrado.\r\n");
   }

   textcolor(COLOR_NORMAL);
   pausar();
}

/*
 * Descripcion : Dibuja un menu interactivo en consola con navegacion mediante
 *               las teclas de flecha arriba/abajo. La opcion activa se resalta
 *               con fondo LIGHTCYAN. Retorna cuando el usuario presiona Enter.
 * Parametros  : titulo       - texto del encabezado del menu.
 *               opciones     - arreglo de strings con las opciones a mostrar.
 *               cantidadOpc  - numero total de opciones en el menu.
 * Retorno     : int - indice (base 0) de la opcion seleccionada con Enter.
 */
int ejecutar_menu(char *titulo, char *opciones[], int cantidadOpc) {
   int seleccion = 0;
   int tecla;

   clrscr();
   while (1) {
      gotoxy(1, 1);

      textcolor(COLOR_TITULO);
      cprintf("  %s\r\n", titulo);

      for (int indice = 0; indice < cantidadOpc; indice++) {
         if (indice == seleccion) {
            textbackground(FONDO_SELECCION);
            textcolor(COLOR_SELECCION);
            cprintf("  > %-*s\r\n", ANCHO_OPCION_MENU, opciones[indice]);
            textbackground(BLACK);
            textcolor(COLOR_NORMAL);
         } else {
            textbackground(BLACK);
            textcolor(COLOR_NORMAL);
            cprintf("    %-*s\r\n", ANCHO_OPCION_MENU, opciones[indice]);
         }
      }

      cprintf("\r\n");
      textcolor(DARKGRAY);
      cprintf("  [Arriba/Abajo] Navegar   [Enter] Seleccionar\r\n");
      textcolor(COLOR_NORMAL);

      tecla = getch();
      if (tecla == TECLA_PREFIJO_A || tecla == TECLA_PREFIJO_B) {
         tecla = getch();
         if (tecla == TECLA_ARRIBA)
            seleccion = (seleccion - 1 + cantidadOpc) % cantidadOpc;
         else if (tecla == TECLA_ABAJO)
            seleccion = (seleccion + 1) % cantidadOpc;
      } else if (tecla == TECLA_ENTER) {
         return seleccion;
      }
   }
}

/*
 * Descripcion : Despliega el menu de ordenamiento y ejecuta la demo del
 *               algoritmo seleccionado usando una tabla de punteros a funcion.
 *               Permanece en el menu hasta que el usuario seleccione "Atras".
 * Parametros  : Ninguno.
 */
void menu_ordenamiento() {
   /*
    * Tabla de punteros a funciones: indexar con la opcion del menu
    * invoca el algoritmo correcto sin necesitar if/else.
    * Esta es la aplicacion directa del concepto de punteros a funcion.
    */
   FuncionOrdenamiento algoritmos[CANTIDAD_ALGORITMOS] = {
       ordenar_burbuja,
       ordenar_insercion,
       ordenar_seleccion,
       ordenar_shell,
       ordenar_quicksort};

   char *nombresAlgoritmos[CANTIDAD_ALGORITMOS] = {"Burbuja", "Insercion", "Seleccion", "Shell", "QuickSort"};
   char *opcionesMenu[OPCIONES_MENU_ORDENAMIENTO] = {"Burbuja", "Insercion", "Seleccion", "Shell", "QuickSort", "Atras"};

   int opcion;
   do {
      opcion = ejecutar_menu("ORDENAMIENTO", opcionesMenu, OPCIONES_MENU_ORDENAMIENTO);
      if (opcion != INDICE_ATRAS_ORDENAMIENTO)
         ejecutar_ordenamiento(algoritmos[opcion], nombresAlgoritmos[opcion]);
   } while (opcion != INDICE_ATRAS_ORDENAMIENTO);
}

/*
 * Descripcion : Despliega el menu de busqueda y ejecuta la demo del algoritmo
 *               seleccionado. Permanece en el menu hasta seleccionar "Atras".
 */
void menu_busqueda() {
   char *opcionesMenu[OPCIONES_MENU_BUSQUEDA] = {"Secuencial", "Binaria", "Atras"};

   int opcion;
   do {
      opcion = ejecutar_menu("BUSQUEDA", opcionesMenu, OPCIONES_MENU_BUSQUEDA);
      if (opcion == OPCION_SECUENCIAL)
         demo_busqueda_secuencial();
      if (opcion == OPCION_BINARIA)
         demo_busqueda_binaria();
   } while (opcion != INDICE_ATRAS_BUSQUEDA);
}

/*
 * Descripcion : Despliega el menu principal y redirige al sub-menu
 *               correspondiente segun la opcion elegida.
 *               Permanece activo hasta que el usuario seleccione "Salir".
 */
void menu_principal() {
   char *opcionesMenu[OPCIONES_MENU_PRINCIPAL] = {"Ordenamiento", "Busqueda", "Salir"};

   int opcion;
   do {
      opcion = ejecutar_menu(
          "ALGORITMOS DE ORDENAMIENTO Y BUSQUEDA",
          opcionesMenu,
          OPCIONES_MENU_PRINCIPAL);
      if (opcion == OPCION_ORDENAMIENTO)
         menu_ordenamiento();
      if (opcion == OPCION_BUSQUEDA)
         menu_busqueda();
   } while (opcion != INDICE_SALIR_PRINCIPAL);

   system("cls");
   textcolor(COLOR_TITULO);
   cprintf("  Hasta luego!\r\n\n");
   textcolor(COLOR_NORMAL);
}

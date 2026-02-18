# Notas - Charles David ; Estudio sobre lenguaje C

## Estructuras de control: Bucles

Veamos el siguiente ejemplo:

```c
#include <stdio.h>

int main() {
   int num_lines = 5;
   char white = ' ';
   char drawing_point = '*';

   int row, whites, count_as;
   puts(" "); // leave a white space

   for (row = 0; row <= num_lines; row++) {
      putchar('\t');
      // Write spaces
      for (whites = num_lines - row; whites > 0; whites--) {
         putchar(white);
      }

      for (count_as = 1; count_as < 2 * row; count_as++) {
         putchar(drawing_point);
      }
      puts(" ");
   }
   return 0;
}
```

El programa dibuja un triángulo isosceles. Cada vez que se repite el bucle externo se ejecutan los dos bucles internos. El bucle eterno se repite cinco veces (cinco filas); el numero de repeticiones realizadas por los bucles internos se basan en el valor de la variable `fila`. El primer bucle interno visualiza los espacios en blanco no significativos; el segundo bucle interno visualiza uno o mas asteriscos.

### Ejercicios

- Cual es la salida del siguiente segmento de programa?

```c
for (cuenta = 1; cuenta < 5; cuenta++) {
   printf("%d ", (2 * cuenta));
}
```

_Iteraciones_

1. 2 \* 1 = 2 ; cuenta = 2 ; cuenta < 5? Verdadero
2. 2 \* 2 = 4 ; cuenta = 3 ; cuenta < 5? Verdadero
3. 2 \* 3 = 6 ; cuenta = 4 ; cuenta < 5? Verdadero
4. 2 \* 4 = 8 ; cuenta = 5 ; cuenta < 5? Falso

- Cual es la salida de los siguientes bucles?

```c
for (n = 10; n > 0; n = n-2) {
   printf("Hola");
   printf(" %d \n", n);
}
```

```c
double n = 2;
for (; n > 0; n = n-0.5) {
   printf("%g ", n);
}
```

## Concepto de función

C es un programa diseñado para ser estructurado, por eso se le dice que es un lenguaje de programación modular. Los programas modulares operan con diferentes modulos, en lenguaje C se le conocen como funciones. Las funciones tienen objetivos especificos en un programa, esto hace que sea más comodo el mantenimiento a futuro y el desarrollo en el presente más rápido.

Un ejemplo de lo que no se debe hacer en un programa en C seria:

```c
int main() {
   /* Código para obtener una lista de caracteres */
   ...
   /* Código para alfabetizar los caracteres */
   ...
   /* Código para visualizar la lista por orden alfabético */
   ...
   return 0;
}
```

Los programas deben ser modulares, el método mostrado anteriormente es incomodo para el futuro, y dificil de trabajar ya que se puede perder facil el código. El mejor medio seria haciendo funciones prototipo y citando las funciones en el `main()`.

```c
#include <stdio.h>

int obtenercaracteres();
int alfabetizar();
void verletras();

int main() {
   obtenercaracteres();
   alfabetizar();
   verletras();

   return 0;
}

int obtenercaracteres() {
   /* Código para obtener caracteres */
}

int alfabetizar() {
   /* Código para organizar los caracteres en orden alfabetico */
}

void verletras() {
   /* Código para visualizar lista de alfabetizada. */
}
```

Aqui se ve que cada función realiza una determinada tarea y cuando se ejecuta `return`, se retorna en que fue llamada por el programa o función principal.

## Estructura de una función

Las funciones son sentencias las cuales se pueden llamar desde cualquier parte de un programa. Las funciones permiten un grado de abstracción en la resolución de un problema especifico.

**Las funciones en C no se pueden anidar**. Esto significa que una función no puede ser declarada dentro de otra función. La razón de esto para permitir un acceso muy eficiente a los datos. En C todas las funciones son externas o globales, es decir, pueden ser llamdas desde cualquier punto del programa.

La estructura de una función en C se muestra aqui:

```
tipo-de-retorno <int, char, double, long double, ...> nombreFunción (Parametros) {
   <Código de la función>

   <return expresión>
}
```

- tipo-de-retorno <- tipo de valor devuelto por la función o la palabra reservada void si la función no devuelve ningún valor.
- nombreFunción <- Identificador o nombre de la función.
- listaDeParámetros <- Lista de declaraciones de los parámetros de la función separados por comas.
- expresión <- Expresión que se evalúa y se devuelve como resultado de la función.

Ejemplo de una función:

```c
float suma(float num1, float num2) {
   float resp;
   resp = num1 + num2;
   return resp;
}
```

- `float suma(float num1, float num2) {...}` ; Viene siendo la cabezera de una función.
- `float num1`y`float num2`vienen siendo parametros de la funcion.
- `float resp` ; viene siendo una variable de la función.
- `resp = num1 + num2;` ; Esto es una operación de suma.
- `return resp` ; Aqui se esta devolviendo o retornando el valor de la función.

Desde main, se llama la función suma:

```c
int main() {
   int num1 = 3, num2 = 34;

   printf("%.2f", suma((float)num1, (float)num2));

   return 0;
}
```

> (float)<variable> es para convertir una variable que no es flotante o decimal, a flotante o decimal.

## Resultados de una función

Una función puede devolver un único valor. El resultado puede ser cualquier tipo de dato EXCEPTO otra función o un arreglo _(array)_. Se pueden devolver valores multiples devolviendo un puntero o una estructura. El valor de retorno debe seguir las mismas reglas que se aplican a un operador de asignación. Por ejemplo, no se puede devolver un valor int, si el tipo de retorno es un puntero Sin embargo, si se devuelve un int y el tipo de retorno es un float, se realiza la conversión automáticamente.

_Se recomienda igualmente devolver o retornar un valor del mismo tipo que de la función para una mayor claridad._

Una función puede tener cualquier número de sentencias `return`. Tan pronto como el programa encuentre cualquiera de las sentencias return, devuelve control a la sentencia llamadora. La ejecución de la función termina si no se encuentra ninguna sentencia return, en este caso, la ejecución continúa hasta la llave final del cuerpode la función.

Si el tipo de retorno es `void`, la sentencia return se puede escribir como `return;` sin ninguna expresión de retorno, o bien, de modo alternativo se puede omitir la sentencia `return;`

```c
void func1(void) {
   puts("Esta funci%cn no devuelve valores", 162); // 162 es código ASCII de la 'ó'.
}
```

### Consejo

- Aunque no es obligatorio el uso de la sentencia `return` en la última linea, se recomienda su uso, ya que ayuda a recordar el retorno en ese punto a la función llamadora.

## Funciones numéricas

Virtualmente cualquier operación aritmética es posible en un programa C. Las funciones matemáticas disponibles son:

- matemáticas
- trigonométricas
- logarítmicas
- exponenciales
- aleatorias

La mayoría de las funciones numéricas están en el archivo de cabecera `MATH.H`; las funciones abs y labs están definidas en `MATH.H`, y las rutinas div y ldiv en `STDLIB.H`.

## Funciones matemáticas

Las funciones matemáticas usuales en la biblioteca estándar son

- `ceil(x)` ; retorna al entero más cercano
- `fabs(x)` ; devuelve el valor absoluto de x (un valor positivo)
- `floor(x)` ; redondea por defecto al entero más próximo

## Funciones trigonométricas

Las biblioteca de C incluye una serie de funciones que sirven para realizar cálculos trigonométricos. Es necesario incluir en su programa el archivo de cabecera `MATH.H` para utilizar cualquier función.

- `acos(x)` ; Calcula el arco coseno del argumento x. El argumento x debe estar entre -1 y 1.
- `asin(x)` ; Calcula el arco seno del argumento x. El argumento x debe estar entre -1 y 1.
- `atan(x)` ; Calcula el arco tangente del argumento x.
- `atan2(x, y)` ; Calcula el arco tangente de x divido por y.
- `cos(x)` ; Calcula el coseno del ángulo x ; x se expresa en radianes.
- `sin(x)` ; Calcula el seno del ángulo x ; x se expresa en radianes.
- `tan(x)` ; Devuelve la tangente del ángulo x ; x se expresa en radianes.

> Si necesita pasar un ángulo expresado en grados a radianes, para poder utilizarlo con las funciones trigonométricas, multiplique los grados por pi/180, donde pi = 3.14159...

## Funciones logarítmicas y exponenciales

Las funciones logarítmicas y exponenciales suelen ser utilizadas en frecuencia no sólo en matemáticas, sino también en el mundo de la empresa y los negocios. Estas funciones requieren también el archivo de inclusión `MATH.H`

- `exp(x), expl(x)` ; Calcula el exponencial e, donde e es la base de logaritmos naturales de valor 2.718282.

```c
valor = exp(5.0);
```

Una variante de esta función es `expl`, que calcula e utilizando un valor long double.

- `log(x), logl(x)` ; La funcion log calcula el logaritmo natural del argumento x y `logl(x)` calcula el citado logaritmo natural del argumento x de valor long double.
- `log10(x), log10l(x)` ; Calcula el logaritmo decimal del argumento x, de valor real double en `log10(x)` y de valor real long double en `log10l(x)` ; x ha de ser positivo.

## Funciones aleatorias

Los números aleatorios son de gran utilidad en numerosas aplicaciones y requieren un trato especial en cualquier lenguaje de programación. C no es una excepción y la mayoría de los compiladores incorporan funciones que generan números aleatorios. Las funciones usuales de la biblioteca estándar de C son:
`rand, random, randomize y srand`. Estas funciones se encuentran en el archivo `STDLIB.H`.

### rand(void)

La funcion rand genera un número aleatorio. El número calculado por rand varía en el rango entero de 0 y `RAND-MAX`. La constante `RAND-MAX` se define en el archivo `STDLIB.H` en forma hexadecimal (por ejemplo, 7FFF). En consecuencia, asegúrese incluir dicho archivo en la parte superior de su programa.

## Estructuras y uniones

Se examinaran las estructuras, uniones, enumeraciones y tipos definidos por el usuario. Esto permite al programador crear programas mas complejos y completos, libera al programador de las restricciones al uso de los tipos ofrecidos por el lenguaje. Una estructura contiene multiples variables, que pueden ser de tipos diferentes. Las estructureas es importante para crear programas mas complejos, tales como bases de datos u otras aplicaciones que requieran grandes cantidades de datos. Por otra parte, se analizará el concepto de unión, otro tipo de dato no tan importante como las estructuras array y estructura, pero necesarias en algunos casos.

Un tipo de dato enumerado es una colección de miembros con nombre que tienen valores enteros equivalentes. Un typedef es de hecho no un nuevo tipo de dato sino simplemente un sinónimo de un tipo existente.

---

Imaginemos que necesitamos **almacenar** los datos de una colección de discos compactos (CD) de música. Estos datos pueden ser.

- Titulo
- Artista
- Número de canciones
- Precio
- Fecha de compra

La estructura CD contiene cinco miembros. Tras decidir los miembros, se debe decirid cuáles son los tipos de datos para utilizar por los miembro. Esta información se representa de la siguiente forma:

Las estructuras es un tipo de dato definido por el usuario, que se debe declarar antes que se pueda utilizar. El formato de declaración es:

```c
struct <nombre> {
   <tipo de dato> <nombre de miembro>
   <tipo de dato> <nombre de miembro>
   ...
   <tipo de dato> <nombre de miembro>
}
```

Ejemplo de estructura para los artistas.

```c
struct Artista {
   char TitleArtist[30];
   char NameArtist[25];
   int SongCount;
   float Price;
   char Date[8];
}
```

### Inicialización de una declaración de estructuras

Se puede inicializar la estructura de dos formas. Se puede inicializar una estructura dentro de la sección del código de su programa, o bien se puede inicializar la estructura como parte de la definición

Hay dos conceptos que hay que definir antes de continuar, declarar y definir no son lo mismo; declarar es simplemente especificar el nombre y el formato de la estructura de datos

## Punteros

Los punteros en C tienen la fama, en el mundo de la programación, de dificultad, tanto en el aprendizaje como en su uso. En esta sección vamos a explicar con la mayor claridad que son los punteros, y como usarlo correctamente. El puntero, no es más que una herramienta muy potente que puede utilizar en sus programas para hacerlos más eficientes y flexibles. Los punteros son, sin género de dudas, una de las razonas fundamentales para que el lenguaje C sea tan potente y tan utilizado.

Una variable de puntero (o puntero, como se le llama normalemnte) es una variable que contiene direcciones de otras variables, no el valor, si no la dirección de memoria de otra variable.

Todas las variables vistas hasta este momento contienen valores de datos, por el contrario las variables de punteros contienne valores que son direcciones de memorias donde se almacenan datos. En resumen, un puntero es una variable que contiene una dirección de memoria, y utilizando punteros su programa puede realizar muchas tareas que no seria posible utilizando tipos de datos estándar.

Objetivos claves de esta sección:

- Punteros
- Utilización de los punteros
- Asignación dinámica de la memoria
- Aritmética de punteros
- arreglos de punteros
- punteros a punteros, funciones y estructuras.

### Direcciones en memoria.

Cuando una variable se declara, se asocian tres atributs fundamentales.

- Su nombre
- Su tipo
- y su _dirección_ en memoria

```c
int n;
```

imaginemos que n, se aloja en la memoria (0x4fffd34) y es un int. Imaginemos que definimos `n = 75`, entonces el compilador lo de como entero `(int)`. El valor de una variable se accede por medio de su nombre. Por ejemplo, se puede imprimir el valor de n con la sentencia:

```c
printf("%d", n);
```

A la dirección de una variable se puede acceder por medio del operador de direccion `&`. Por ejemplo, se puede imprimir la dirección de `n` con la sentencia

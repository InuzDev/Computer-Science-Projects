# Laboratorio - 01 ; Carlos David Jorge Taveras, 1016-5529

Primer paso que hice, fue revisar todo el codigo, al ver que un requisito es utilizar linux, prepare Kali linux en mi laptop

Luego, instale los esencial para compilar el codigo dado, utilizando el siguiente comando:

```bash
sudo apt upgrade && sudo apt install build-essential -y
```

Ya luego de tener el compilador y poder utilizar el comando `make`, procedi a trabajar en el archivo `queue.h`, el cual tiene algunas asignaciones por hacer.

## Arreglos hechos en queue.h

El inicio del archivo, se utilizan varias importanciones como:

```c
#include <cstddef>
#include <optional>
```

Estos archivos son de C++ y generan errores de importacion, en mi caso lo que hice fue modificar `cstddef` a `stddef.h` y eliminar `optional`.

En las lineas `28-34` hay una estructura (`queue_t`), pide que ingrese una cola y un tamaño, como la peticion de la profesore pide que sea una lista monoenlazada, solo agrego el tamaño, en el tipo de dato (`int`).

## Arreglos hechos en queue.c

### queue_t \*q_new()

Luego de modificar la estructura para introducir `size`, procedo a hacer la asignacion; el cual pide completar las funciones alojadas en `queue.c`.

Empezando con `queue_t *q_new() {...}`, pregunta que pasa si el alojamiento de memoria retorna null, la solucion que propongo es:

```c
if (!q) {
   return NULL;
}
```

De este modo, la funcion retorna nulo, evitando un error en la memoria.

Luego de verificar que hacer cuando `*q` retorna `NULL`, procedo a definir `q->head` y `q->size`, definiendolos en `NULL`.

### void q_free(queue_t \*q)

Aqui, pide liberar la memoria cuando se deja de utilizar la lista enlazada.

Aqui, igualmente verifico si `q` retorna nulo, en tal caso, la funcion se retornara.

```c
if (!q) {
   return;
}
```

Para liberar la memoria de la lista enlazada hice uso del siguiente codigo:

```c
list_ele_t *curr = q->head;

while (curr) {
   list_ele_t *next = curr->next;

   free(curr);
   curr = next;
}
```

El bucle se ejecutara mientras el puntero `curr` no retorne nulo.

Ya al final el codigo hace uso de `free(q)` para liberar los datos restantes.

### bool q_insert_head(queue_t \*q, int v)

Aqui, nos pide retornar falso si al insernar un elemento en la acabeza fracasa, verdadero si es exitoso.

Para verificar si q es null o ha fracasado, utilizados:

```c
// Si q es null
if (!q) {
   return false;
}

// Si al ingresar un dato fracasa
if (!newh) {
   return false
}
```

Despues de esto, ponemos que la cola apunte a la cabeza, y al final, aumentamos el tamaño.

```c
if (!q->tail) {
   q->tail = newh;
}

q->size++;
```

### bool q_insert_tail(queue_t \*q, int v)

Aqui intentamos insertar un elemento en la cola de queue, retorna `true` si el proceso fue exitoso, y `false`, si fue NULL o ha fracasado.

Hacemos el mismo proceso de verificar si q es nulo, luego, agregamos memoria utilizando `malloc();`, verificar si se agrego la memoria de forma exitosa con una condicional.

Si `newt` es nulo, la funcion retornara falso.

Ahora si, agregamos valor a la cola, y apuntamos al siguiente nodo, que es nulo.

verificamos si la cola es nulo, en tal caso, definimos la cabeza y la cola con `newt`, sino, definimos que `q->tail->next` sea `newt`, y que `q->tail` sea `newt`.

Aumentamos al final, el tamaño con `q->size++;` y retornamos `true`.

### bool q_remove_head(queue \*q, int \*vp)

En este codigo, hay que arreglar el la funcion, actualmente solo redefine, pero esto causara errores en la definicion y los valores del nodo.

Lo que haremos primero que todo, es verificar que el nodo y la cabeza no sean nulos, luego verificamos que vp no sea nulo, almacenamos el valor removido en vp:

```c
if (vp) {
   *vp = q->head->value;
}
```

luego, definimos `*old` almacenando la cabeza que va a ser liberada, y avanzamos al siguiente nodo:

```c
list_ele_t *old = q->head;
q->head = q->head->next;

/* Condicional para verificar que la cabeza no sea nula */
if (!q->head) {
   q->tail = NULL;
}

free(old);
```

Con el codigo mostrado anteriormente, verificamos que la cabeza no sea nula, en tal caso de que no sea nula, definimos la cola de la lista enlazada en Nulo.

luego con `free()`, liberamos la cabeza removiendola.

y con:

```c
q->size--;
```

reducimos el tamaño.

Al final retornamos verdadero.

### int q_size(queue_t \*q)

Esta funcion retorna el numero de elementos en espera, retorna 0 si el la lista esta vacia o es nula.

Tiene que operar en _O(1)_, es decir en constante.

Lo que haremos, es utilizar el valor de `size`, el cual dice el indice del nodo.

```c
int q_size(queue_t *q) {
   if (!q) {
      return 0;
   }

   return q->size;
}
```

### q_reverse(queue_t \*q)

En esta funcion, no podemos alojar o liberar ningun elemento, lo que debemos hacer el modificar los punteros en la estructura de datos existente.

Como siempre, verificamos que el nodo no sea nulo, y que la cabeza tampoco sea nulo.

```c
if (!q || !q->head) {
   return;
}
```

Despues de verificar eso, lo que hacemos es que la cola se convierte en la cabeza:

```c
q->tail = q->head;
```

definimos los punteros `*prev` a nulo, y `*curr` apunta a `q->head` y hacemos un bucle el cual modifica los punteros en la estructura de datos existente.

```c
while (curr) {
   list_ele_t *next = curr->next;

   curr->next = prev;

   prev = curr;
   curr = next;
}
```

El bucle corre mientras el puntero `curr` no retorne null.

Finalizamos la funcion con:

```c
q->head = prev;
```

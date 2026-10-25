ICC-133 Lab 01 - API de Memoria Dinamica
Entorno: Linux (Kali Linux WSL) con gcc, gdb y valgrind.
Todos los comandos se ejecutan dentro de la carpeta con los archivos .c

Compilacion general (con simbolos de depuracion, util para gdb y valgrind):

```sh
gcc -g -o null null.c
gcc -g -o leak leak.c
gcc -g -o data data.c
gcc -g -o AfterFree AfterFree.c
gcc -g -o InvalidFree InvalidFree.c
```

Pregunta 1 (null.c)

```sh
gcc -o null null.c
./null
```

Pregunta 2 (null.c con gdb)

```sh
gcc -g -o null null.c
gdb ./null
```

(dentro de gdb) run
(dentro de gdb) quit

Pregunta 3 (null.c con valgrind)

```sh
valgrind --leak-check=yes ./null
```

Pregunta 4 (leak.c)

```sh
gcc -g -o leak leak.c
./leak
gdb ./leak (luego: run, quit)
valgrind --leak-check=yes ./leak
```

Pregunta 5 (data.c)

```sh
gcc -g -o data data.c
./data
valgrind --leak-check=yes ./data
```

Pregunta 6 (AfterFree.c)

```sh
gcc -g -o AfterFree AfterFree.c
./AfterFree
valgrind --leak-check=yes ./AfterFree
```

Pregunta 7 (InvalidFree.c)

```sh
gcc -g -o InvalidFree InvalidFree.c
./InvalidFree
valgrind --leak-check=yes ./InvalidFree
```

Atajo: `./run_all.sh` compila todo y guarda las salidas en la carpeta `Output/`

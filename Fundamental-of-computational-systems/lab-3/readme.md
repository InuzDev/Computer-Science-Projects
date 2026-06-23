# Assembly lab.

## Nota a tener en cuenta

El codigo de assembly, fue orientado al sistema operativo de Linux; este no tiene compatibilidad con Windows, o macOS.

## Compilacion y corrida del programa

Para compilar el programa de assembly, debe correr el siguiente comando:

```sh
nasm -f elf64 <nombre del archivo>.asm -o <nombre del archivo>.o && ld <nombre del archivo>.o -o <nombre del archivo>

./<nombre del archivo>
```

Este comando se debe ejecutar en WSL (Windows Subsystem Linux), primero que todo hay que instalar `nasm`

## Instalacion del compilador de assembly

```sh
sudo apt install nasm
```

> Claro esta que hay que ingresar la contraseña del usuario creade en WSL.

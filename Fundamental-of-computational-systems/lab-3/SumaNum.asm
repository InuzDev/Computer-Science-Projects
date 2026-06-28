; Ejercicio 1 ; Suma de dos numeros ingresados por el usuario

%include "linux64io.inc"

section .data
    msg1 db "Ingrese el primer numero: "
    msg1_len equ $ - msg1
    msg2 db "Ingrese el segundo numero: "
    msg2_len equ $ - msg2
    msg_res db "La suma es: "
    msg_res_len equ $ - msg_res

section .text
    global _start

_start:
   PRINT_STR msg1, msg1_len
   READ_INT r12

   PRINT_STR msg2, msg2_len
   READ_INT r13

   add r12, r13

   PRINT_STR msg_res, msg_res_len
   PRINT_INT r12

   mov rax, 60
   xor rdi, rdi
   syscall

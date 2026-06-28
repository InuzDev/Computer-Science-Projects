; meow meow meow
; Ejercicio 3 ; Verificar si un numero es primo

%include "linux64io.inc"

section .data
   msg_in db "Ingrese un numero: "
   msg_in_len equ $ - msg_in
   msg_primo db "Es un numero primo", 0x0a
   msg_primo_len equ $ - msg_primo
   msg_no_primo db "No es un numero primo", 0x0a
   msg_no_primo_len equ $ - msg_no_primo

section .text
   global _start

_start:
   PRINT_STR msg_in, msg_in_len
   READ_INT r12

   ; Verificar si el valor ingresado es 1 o menor.
   cmp r12, 1
   jle .no_primo

   mov r13, 2

.loop:
   cmp r13, r12

   jge .es_primo

   mov rax, r12
   xor rdx, rdx
   div r13

   test rdx, rdx
   jz .no_primo

   inc r13
   jmp .loop

.es_primo:
   PRINT_STR msg_primo, msg_primo_len
   jmp .end

.no_primo:
   PRINT_STR msg_no_primo, msg_no_primo_len

.end:
   mov rax, 60
   xor rdi, rdi
   syscall

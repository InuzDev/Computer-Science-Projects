; Ejercicio 2 ; Factorial de un numero

%include "linux64io.inc"

section .data
   msg_in db "Ingrese un numero: "
   msg_in_len equ $ - msg_in
   msg_res db "El factorial es: "
   msg_res_len equ $ - msg_res

section .text
   global _start

_start:
   PRINT_STR msg_in, msg_in_len
   READ_INT r12

   mov r13, 1
   mov rcx, r12

.loop:
   cmp rcx, 1
   jle .end
   imul r13, rcx
   dec rcx
   jmp .loop

.end:
   PRINT_STR msg_res, msg_res_len
   PRINT_INT r13

   mov rax, 60
   xor rdi, rdi
   syscall

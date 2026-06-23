; meow meow meow
; Ejercicio 3 ; Verificar si un numero es primo

section .data
   msg_in db "Ingrese un numero: "
   msg_in_len equ $ - msg_in
   msg_primo db "Es un numero primo", 0x0a
   msg_primo_len equ $ - msg_primo
   msg_no_primo db "No es un numero primo", 0x0a
   msg_no_primo_len equ $ - msg_no_primo

section .bss
   inbuf resb 20

section .text
   global _start

_start:
   ; Aqui leemos el numero
   mov rax, 1
   mov rdi, 1
   mov rsi, msg_in
   mov rdx, msg_in_len
   syscall

   mov rax, 0
   mov rdi, 0
   mov rdi, inbuf
   mov rsi, msg_in
   mov rdx, msg_in_len
   syscall

   mov rax, 0
   mov rdi, 0
   mov rsi, inbuf
   mov rdx, 20
   syscall

   mov rsi, inbuf
   call str_to_int
   mov r12, rax ; r12 = n

   ; Si el numero es menor o igual a 1, no son primos
   cmp r12, 1
   jle .no_primo

   ; Probamos divisores desde 2 hasta n-1
   mov r13, 2 ; r13 = divisor actual

.loop:
   cmp r13, r12 ; Si divisor >= n, es primo

   jge .es_primo

   mov rax, r12
   xor rdx, rdx
   div r13 ; rdx = n % r13

   test rdx, rdx ; Si el residuo es 0, no es primo
   jz .no_primo

   inc r13
   jmp .loop

.es_primo:
   mov rax, 1
   mov rdi, 1
   mov rsi, msg_primo
   mov rdx, msg_primo_len
   syscall
   jmp .salir

.no_primo:
   mov rax, 1
   mov rdi, 1
   mov rsi, msg_no_primo
   mov rdx, msg_no_primo_len
   syscall

.salir:
   mov rax, 60
   xor rdi, rdi
   syscall

; str_to_int: Esto lo que hace convertir un string de digitos en rsi a entero en rax
str_to_int:
   xor rax, rax
   xor rcx, rcx

.loop:
   movzx rcx, byte [rsi]
   cmp cl, '0'
   jl .done
   cmp cl, '9'
   jg .done
   sub cl, '0'
   imul rax, rax, 10
   add rax, rcx
   inc rsi
   jmp .loop

.done:
   ret

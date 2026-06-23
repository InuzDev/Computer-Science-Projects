; Ejercicio 2 ; Factorial de un numero
section .data
   msg_in db "Ingrese un numero: "
   msg_in_len equ $ - msg_in
   msg_res db "El factorial es: "
   msg_res_len equ $ - msg_res

section .bss
   inbuf resb 20
   outbuf resb 25

section .text
   global _start

_start:
   ; Aqui lee el numero
   mov rax, 1
   mov rdi, 1
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
   mov r12, rax  ; r12 = n

   ; Ahora vamos a calcular el factorial
   ; El resultadoo empieza en 1, multiplaremos por n, n-1, n-2, hasta llegar a 1.
   mov r13, 1 ; r13 = result
   mov rcx, r12 ; contador = n

.loop:
   cmp rcx, 1
   jle .done
   imul r13, rcx
   dec rcx
   jmp .loop

.done:
   ; Mostraremos el resultado ahora
   mov rax, 1
   mov rdi, 1
   mov rsi, msg_res
   mov rdx, msg_res_len
   syscall

   mov rax, r13
   call print_int

   mov rax, 60
   xor rdi, rdi
   syscall

; str_to_int: Convierte string de digitos en rsi a entero en rax
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

; print_int: imprimiremos ele ntero en rax seguido de un salto de linea
print_int:
   lea r9, [outbuf + 24]
   mov byte [r9], 0x0a
   dec r9
   mov r10, 1

   test rax, rax
   jnz .digits
   mov byte [r9], '0'
   dec r9
   inc r10
   jmp .write

.digits:
   test rax, rax
   jz .write
   xor rdx, rdx
   mov rbx, 10
   div rbx
   add dl, '0'
   mov [r9], dl
   dec r9
   inc r10
   jmp .digits

.write:
   inc r9
   mov rax, 1
   mov rdi, 1
   mov rsi, r9
   mov rdx, r10
   syscall
   ret

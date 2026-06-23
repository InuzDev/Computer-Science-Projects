; Ejercicio 1 ; Suma de dos numeros ingresados por el usuario

section .data
    msg1 db "Ingrese el primer numero: "
    msg1_len equ $ - msg1
    msg2 db "Ingrese el segundo numero: "
    msg2_len equ $ - msg2
    msg_res db "La suma es: "
    msg_res_len equ $ - msg_res

section .bss
    inbuf resb 20     ; buffer para leer entrada del usuario
    outbuf resb 25     ; buffer para convertir numero a string

section .text
    global _start

_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg1
    mov rdx, msg1_len
    syscall

    mov rax, 0
    mov rdi, 0
    mov rsi, inbuf
    mov rdx, 20
    syscall

    mov rsi, inbuf
    call str_to_int
    mov r12, rax


    mov rax, 1
    mov rdi, 1
    mov rsi, msg2
    mov rdx, msg2_len
    syscall

    mov rax, 0
    mov rdi, 0
    mov rsi, inbuf
    mov rdx, 20
    syscall

    mov rsi, inbuf
    call str_to_int
    mov r13, rax        ; r13 = num2

    add r12, r13

    mov rax, 1
    mov rdi, 1
    mov rsi, msg_res
    mov rdx, msg_res_len
    syscall

    mov rax, r12
    call print_int

    mov rax, 60
    xor rdi, rdi
    syscall


; str_to_int: convierte string de digitos en rsi a entero en rax
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


; print_int: imprime el entero en rax seguido de un salto de linea
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

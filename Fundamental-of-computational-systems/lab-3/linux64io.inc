   section .data
   _io_newline db 0x0a

section .bss
   _io_byte resb 1
   _io_outbuf resb 32

%macro PRINT_STR 2
   mov rsi, %1
   mov rdx, %2
   call print_str
%endmacro

%macro PRINT_INT 1
   mov rax, %1
   call print_int
%endmacro

%macro READ_INT 1
   call read_int
   mov %1, rax
%endmacro

%macro READ_STR 2
   mov rsi, %1
   mov rdx, %2
   call read_str
%endmacro

section .text

print_str:
   mov rax, 1
   mov rdi, 1

   syscall

   ret

print_newline:
   mov rax, 1
   mov rdi, 1
   mov rsi, _io_newline
   mov rdx, 1

   syscall

   ret

print_int:
   push rbx
   lea r9, [_io_outbuf + 31]
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

   pop rbx
   ret

read_int:
   xor r9, r9

.loop:
   mov rax, 0
   mov rdi, 0
   mov rsi, _io_byte
   mov rdx, 1

   syscall

   test rax, rax
   jz .done
   movzx rax, byte [_io_byte]
   cmp al, 0x0a

   je .done
   cmp  al, '0'
   jl .done
   cmp al, '9'
   jg .done
   sub al, '0'
   imul r9, r9, 10
   add r9, rax
   jmp .loop

.done:
   mov rax, r9
   ret

read_str:
   mov rax, 0
   mov rdi, 0

   syscall

   ret

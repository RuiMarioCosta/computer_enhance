global RATAdd
global RATMovAdd

section .text

RATAdd:
align 64
    mov rax, 1000000000
.loop:
    add rcx, 1
    add rcx, 1
    dec rax
    jnz .loop
    ret

RATMovAdd;
align 64
    mov rax, 1000000000
.loop:
    mov rcx, rax
    add rcx, 1
    mov rcx, rax
    add rcx, 1
    dec rax
    jnz .loop
    ret


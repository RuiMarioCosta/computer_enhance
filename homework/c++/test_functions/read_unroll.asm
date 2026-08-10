global Read_x1
global Read_x2
global Read_x3
global Read_x4

section .text

Read_x1:
align 64
.loop:
    mov rax, [rdx]
    sub rcx, 1
    jnle .loop
    ret

Read_x2:
align 64
.loop:
    mov rax, [rdx]
    mov rax, [rdx]
    sub rcx, 2
    jnle .loop
    ret

Read_x3:
align 64
.loop:
    mov rax, [rdx]
    mov rax, [rdx]
    mov rax, [rdx]
    ; mov rax, [rdx+8]
    ; mov rax, [rdx+16]
    sub rcx, 3
    jnle .loop
    ret

Read_x4:
align 64
.loop:
    mov rax, [rdx]
    mov rax, [rdx]
    mov rax, [rdx]
    mov rax, [rdx]
    ; mov rax, [rdx+8]
    ; mov rax, [rdx+16]
    ; mov rax, [rdx+24]
    sub rcx, 4
    jnle .loop
    ret


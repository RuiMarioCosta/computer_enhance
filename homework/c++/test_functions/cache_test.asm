global Read_32x8
global DoubleLoopRead_32x8

section .text

; rcx: count (must be divisible by 256)
; rdx: data pointer
; r8: mask
Read_32x8:
    xor r9, r9
    mov rax, rdx
    align 64
.loop:
    ; Read 256 bytes
    vmovdqu ymm0, [rax]
    vmovdqu ymm0, [rax + 0x20]
    vmovdqu ymm0, [rax + 0x40]
    vmovdqu ymm0, [rax + 0x60]
    vmovdqu ymm0, [rax + 0x80]
    vmovdqu ymm0, [rax + 0xa0]
    vmovdqu ymm0, [rax + 0xc0]
    vmovdqu ymm0, [rax + 0xe0]

    ; Advance and mask the read offset
    add r9, 0x100
    and r9, r8

    ; Update the read base pointer to point to the new offset
    mov rax, rdx
    add rax, r9

    ; Repeat
    sub rcx, 0x100
    jnz .loop

    ret

DoubleLoopRead_32x8:
    align 64
.outer:
    mov r9, r8
    mov rax, rdx
.inner:
    vmovdqu ymm0, [rax]
    vmovdqu ymm0, [rax + 0x20]
    vmovdqu ymm0, [rax + 0x40]
    vmovdqu ymm0, [rax + 0x60]
    vmovdqu ymm0, [rax + 0x80]
    vmovdqu ymm0, [rax + 0xa0]
    vmovdqu ymm0, [rax + 0xc0]
    vmovdqu ymm0, [rax + 0xe0]
    add rax, 0x100
    dec r9
    jnz .inner

    dec rcx
    jnz .outer
    ret


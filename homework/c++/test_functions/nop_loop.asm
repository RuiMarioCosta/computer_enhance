; nasm -f win64 -o nop_loop.obj nop_loop.asm
; lib nop_loop.obj
; dumbin /exports nop_loop.lib
; dumbin /all nop_loop.lib

default rel

section .text

global MOVAllBytesASM
global NOPAllBytesASM
global CMPAllBytesASM
global DECAllBytesASM

; Windows x64 ABI: RCX = iteration count, RDX = data pointer
MOVAllBytesASM:
    xor rax, rax
.loop:
    mov [rdx + rax], al
    inc rax
    cmp rax, rcx
    jb .loop
    ret

NOPAllBytesASM:
    xor rax, rax
.loop:
    db 0x0f, 0x1f, 0x00 ; NOTE(casey): This is the byte sequence for a 3-byte NOP
    inc rax
    cmp rax, rcx
    jb .loop
    ret

CMPAllBytesASM:
    xor rax, rax
.loop:
    inc rax
    cmp rax, rcx
    jb .loop
    ret

DECAllBytesASM:
.loop:
    dec rcx
    jnz .loop
    ret

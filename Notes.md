# Performance aware course

<!--toc:start-->
- [Performance aware course](#performance-aware-course)
  - [Reading ASM](#reading-asm)
    - [Instruction Decoding on the 8086](#instruction-decoding-on-the-8086)
    - [Decoding Multiple Instructions and Suffixes](#decoding-multiple-instructions-and-suffixes)
    - [Opcode Patterns in 8086 Arithmetic](#opcode-patterns-in-8086-arithmetic)
    - [Simulating ADD, SUB, and CMP](#simulating-add-sub-and-cmp)
    - [Simulating Conditional Jumps](#simulating-conditional-jumps)
  - [Basic Profiling](#basic-profiling)
    - [Introduction to RDTSC](#introduction-to-rdtsc)
    - [How does QueryPerformanceCounter measure time?](#how-does-queryperformancecounter-measure-time)
<!--toc:end-->

## Reading ASM

### Instruction Decoding on the 8086

Understanding `mov ax,bx` copies the bits from register bx to ax and the
instruction has a specific bit pattern

### Decoding Multiple Instructions and Suffixes

The length of the instruction changes depending on the instruction

memory to register = load (reading from memory)
`mov bx,[75]` -> reads slot 75 and 76 because bx is 16bit

register to memory = store (writing to memory)
`mov [75],bx` -> writes bx into address 75 and 76

`mov bx,[bp+75]` -> effective address calculation

`mov ax, 12` -> Immediate to register move, the value 12 is the immediate
When moving an immediate to memory, e.g., `mov [bp+75], 12`, the assembler
doesn't know if this is supposed
to write 12 as 8bits or 16bits. In these cases what is actually written is
`move [bp+75], word 12`.

### Opcode Patterns in 8086 Arithmetic

Similar to `mov`, other operations like `add`, `sub`, `cmp`, have the same type
of patterns of register
to register, memory to register, etc, the only difference is the opcode.

`jnz <8bit signed increment>` jump if not zero

### Simulating ADD, SUB, and CMP

There is a flags register. A `mov` instruction has no effect on this flag but
`sub` will change the flags register.

The flags register has bits for:

- CF - carry
- ZF - zero (gets set if the result of an operation is zero)
- PF - parity
- SF - sign (gets set if the highest bit is set)
- OF - overflow
- etc

### Simulating Conditional Jumps

`ip` (instruction pointer) lets the processor know where in the instruction
stream and be able to advance to the next instruction. It gets incremented by
the number of bytes decoded for the instruction prior to the instruction being
executed. Some instruction, like conditions of function calls need to modify
this register.

Jump instruction will jump according to the respective bit in the flag register.

### Other Common Instructions

- multiplication: `mul` , `imul`, division: `div`, `idiv`
- logic shifts: `shr` (shifts in 0s), arithmetic shift:`sar` (shifts in 1s),
`shl` = `sal`
- logic operations: `and`, `xor`, `or`, `not`, `test` (`test` is the same as
`and` but it doesn't write back the result, it has the same relation as `sub`
and `cmp`)
- load effective address: `lea`
- increment: `inc`, decrement: `dec`

### The Stack

#### caller

```assembler
mov ax, 1000
sub ax, bx
jmp 2000
cmp ax, 500 // assume this is at address [1000]
```

#### function

```assembler
mov ax, [1000] // assume starts at [2000]
add ax, 15
cmp ax, 1000
jnz <to somewhere>
...
jmp  [1000]
```

`jmp` sets the ip to anywhere. If jump directly to the address of function this
leads to problem because it has instructions also using `ax` which will override
the value not only that but we would have to read the function to know that `ax`
is used and avoid it, etc.

The solution to this problem is the stack. It starts at a very high number and
there is a register to keep track of where the stack is on memory and anytime
any value needs to be saved, for example, when calling another function, the
value is pushed onto the stack before the function is executed and it is popped
after executing the function. Somewhere at the start of the program there is a
instruction initializing the stack pointer, `sp`, to its starting value, e.g.,
`mov sp, 40000`. When a value needs to be saved it needs to know the size of the
object being saved as subtract it, e.g., `sub sp, 2`, then move the value to be
saved into that location, `mov [sp], ax`. To get back the value the opposite is
done, e.g., `mov ax, [sp]`, `add sp, 2`. This stack management could be the
responsibility of the caller or callee, it is arbitrary. Since these instruction
sequences are so common there are specific instruction for this: `push` and `pop`

Rewriting the examples with these instruction:

#### caller (with push and pop)

```assembler
mov ax, 1000
sub ax, bx
push ax
jmp 2000
pop ax
cmp ax, 500 // assume this is at address [1000]
```

#### function (with push and pop)

```assembler
mov ax, [1000] // assume starts at [2000]
add ax, 15
cmp ax, 1000
jnz <to somewhere>
...
jmp  [1000]
```

Note: `push` and `pop` could alternatively be in the function instead as long as
one of them saves the value.

There is another problem which are the hard-coded jumps because the function
will always jump to location 1000 so it cannot be reused in any other context.
So the solution is to store the `ip` register on the stack, the same thing as
with `ax` could be done but the `rip` is one of the register that cannot be an
operand to simple instructions so there are specific instructions for this:

- `call` which is exactly the same as `jmp` but right before doing the jump it
will put in the stack the `ip` register
- `ret` which takes whatever the top values is on the stack and writes it into
the `ip` register

Application Binary Interface (ABI) or "Calling convention"" are conventions for
what is going to happen in this context of what responsibilities does the caller
and callee have. For example, it specifies which register does the caller or
callee push, pop or if it is changed at all, specifies how parameters are passed
to functions, if it is some register, the stack, etc.

A possible assembly code for the example above could be:

#### caller (with push, pop and call)

```assembler
mov ax, 1000
sub ax, bx
push ax
call 2000
pop ax
cmp ax, 500 // assume this is at address [1000]
```

#### function (with push, pop and call)

```assembler
mov ax, [1000] // assume starts at [2000]
add ax, 15
cmp ax, 1000
jnz <to somewhere>
...
ret
```

### Estimating Cycles

1. Choices, be able to recognize if generated code is good or bad
2. Optimization
3. Estimation, estimate how long do some operations take

```assembler
mov bx, 10
add bx, cx
mov [bp+si+50], bx
```

To estimate the number of cycles/clocks, for the 8086, there is a table with the
number of cycles each instruction take. For modern chips this is not that
straightforward because they can execute some instruction at the same time.

## Basic Profiling

### Introduction to RDTSC

On Pentium 586 it was introduced `rdtsc`, read timestamp counter, which writes
into 2 32bit registers (one for the low part and another for the high part)
combined into a 64bit value which is a measure of how many cycles elapsed since
the processor first started (booted or restarted).

On modern cpus not all cores have the same frequency, there is a boost frequency
, the cores might sleep to save energy, etc. For some reason `rdtsc` ended up
getting changed and instead of measuring cycles it measures something called
"invariant tsc" and all cores will see the same the same frequency regardless if
it's sleeping or boosted because it's not counting the actual cycles but the
cycles of this other things that computes the rdtsc, so in essence it is a very
precise wall clock. There are other instructions like `rdpmc` and `rdpru` which
count the actual cycles but these might not always be available for a given
processor and the OS might also block access to them.

Unfortunately, vendors don't provide information about the frequency of the
`rdtsc` so the alternative is to compare it with another timer, the OS timer, on
windows it would be the QueryPerformanceCounter.

### How does QueryPerformanceCounter measure time?

QueryPerformanceCounter is calling RDTSCP which is the same as RDTSC with two caveats:

- eax the low
- edx the high
- ecx the pid (processor core id) of the cores was running the instruction

If I have a binary that will run in arbitrary machines and if I want some
reliable time source then QueryPerformanceCounter makes sense because it will
use the instruction the system. If the objective is to profile then there is no
point because it drops the resolution of the timer down to 10MHz.

### Instrumentation-Based Profiling

### Profiling Nested Blocks

1. Easy to deploy
2. Fairly accurate
3. Low overhead
4. Easy to read/compare/interpret (optional)

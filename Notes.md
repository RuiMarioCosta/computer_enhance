# Performance aware course

<!--toc:start-->

- [Performance aware course](#performance-aware-course)
  - [Reading ASM](#reading-asm)
    - [Instruction Decoding on the 8086](#instruction-decoding-on-the-8086)
    - [Decoding Multiple Instructions and Suffixes](#decoding-multiple-instructions-and-suffixes)
    - [Opcode Patterns in 8086 Arithmetic](#opcode-patterns-in-8086-arithmetic)
    - [Simulating ADD, SUB, and CMP](#simulating-add-sub-and-cmp)
    - [Simulating Conditional Jumps](#simulating-conditional-jumps)
    - [Other Common Instructions](#other-common-instructions)
    - [The Stack](#the-stack)
      - [caller](#caller)
      - [function](#function)
      - [caller (with push and pop)](#caller-with-push-and-pop)
      - [function (with push and pop)](#function-with-push-and-pop)
      - [caller (with push, pop and call)](#caller-with-push-pop-and-call)
      - [function (with push, pop and call)](#function-with-push-pop-and-call)
    - [Estimating Cycles](#estimating-cycles)
  - [Basic Profiling](#basic-profiling)
    - [Introduction to RDTSC](#introduction-to-rdtsc)
    - [How does QueryPerformanceCounter measure time?](#how-does-queryperformancecounter-measure-time)
    - [Instrumentation-Based Profiling](#instrumentation-based-profiling)
    - [Profiling Nested Blocks](#profiling-nested-blocks)
    - [A First Look at Profiling Overhead](#a-first-look-at-profiling-overhead)
  - [Moving Data](#moving-data)
    - [Monitoring OS Performance Counters](#monitoring-os-performance-counters)
    - [Page Faults](#page-faults)
    - [Probing OS Page Fault Behavior](#probing-os-page-fault-behavior)
    - [Four-Level Paging](#four-level-paging)
    - [Analyzing Page Fault Anomalies](#analyzing-page-fault-anomalies)
    - [Powerful Page Mapping Techniques](#powerful-page-mapping-techniques)
    - [Faster Reads with Large Page Allocations](#faster-reads-with-large-page-allocations)
    - [Inspecting Loop Assembly](#inspecting-loop-assembly)

<!--toc:end-->

## Reading ASM

### Instruction Decoding on the 8086

Understanding `mov ax,bx` copies the bits from register bx to ax and the
instruction has a specific bit pattern

### Decoding Multiple Instructions and Suffixes

The length of the instruction changes depending on the instruction

memory to register = load (reading from memory) `mov bx,[75]` -> reads slot 75
and 76 because bx is 16bit

register to memory = store (writing to memory) `mov [75],bx` -> writes bx into
address 75 and 76

`mov bx,[bp+75]` -> effective address calculation

`mov ax, 12` -> Immediate to register move, the value 12 is the immediate When
moving an immediate to memory, e.g., `mov [bp+75], 12`, the assembler doesn't
know if this is supposed to write 12 as 8bits or 16bits. In these cases what is
actually written is `move [bp+75], word 12`.

### Opcode Patterns in 8086 Arithmetic

Similar to `mov`, other operations like `add`, `sub`, `cmp`, have the same type
of patterns of register to register, memory to register, etc, the only
difference is the opcode.

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
sequences are so common there are specific instruction for this: `push` and
`pop`

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

QueryPerformanceCounter is calling RDTSCP which is the same as RDTSC with two
caveats:

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

### A First Look at Profiling Overhead

Adding profile blocks add overhead and the key point is to minimize this
overhead by being able to turn on and off the profiler so that the runtime can
be compared and keep only the main profile blocks.

## Moving Data

### Monitoring OS Performance Counters

Windows has "Performance Monitor" which can be used to investigate a process.

### Page Faults

Page Fault is the interrupt that happens when you write into a piece of virtual
address space that hasn't been mapped but the OS knows about.

### Probing OS Page Fault Behavior

Where are the extra page faults coming from? Why does the page faults have an
almost staircase pattern?

### Four-Level Paging

A pointer of size 64 bits is: | 16 bits (always zero) | 9 bits | 9 bits | 9 bits
| 9 bits | 12 bit (offset) |

The bottom 48 bits is what is used to do the virtual to physical address
translation. The last 12 bits say which bytes are in physical RAM, 2^12 = 4096 =
4KiB. The middle 36 bits that from a hierarchical 4-level tree.

Inside the CPU there is a register called CR3 is used when the CPU needs to do
address translation and is used when it needs to find the first table of the
4-level tree. This table has pointers to the next table in the tree, and so on.
In the last table is where it's the pointer to the actual physical RAM.

The reason for the 9 bits is, since the OS works with pages of 4KiB in size then
it makes sense to make the table itself fit in a page, therefore,
4096/sizeof(pointer) = 4096/8 = 512 = 2^9, thus the 9.

The CPU allows to increase the page size by extending the physical index by
including the bits in the levels, for example, from 12 to 12 + 9 = 21 this means
that the page size is 2^21 = 2MiB.

### Analyzing Page Fault Anomalies

The extra page faults come from the additional pages needed for the tables
themselves. When the table reaches the 512th index it needs a new page.

### Powerful Page Mapping Techniques

Circular buffers:

- Assume buffer of 16KiB
- Replicate the buffer in virtual memory | 16 KiB | 16 KiB | 16 KiB |
- Map the 3 chunks into the same physical memory

Sparse memory

### Faster Reads with Large Page Allocations

By increasing the page size we can significantly reduce the overhead introduced
by the page faults and get a performance similar to a reused buffer.

### Inspecting Loop Assembly

### CPU Front End Basics

Frontend is the part of the CPU responsible for decoding instructions into some
kind of format that backend of the cpu can execute those things.

We can think of the frontend and the backend being connected by a queue with the
decoded things that the backend has to do.

No matter how fast the backend is it will always be bottlenecked on the queue,
if the queue isn't full then there is no way to improve performance.

The frontend produces micro-ops (uops) that go into the queue. uops are specific
to the architecture (more specifically to the backend).

The frontend gets code (instructions) and the backend gets data from the caches,
L1i and L1d respectively.

There is a uops cache that improves the performance of decoding.

### Branch Prediction

### The RAT and the Register File

The expectation is that both code examples below take more or less the same
cycles.

```asm
add rcx, 1
add rcx, 1
...
```

The add's are serially linked forming a giant dependency chain

```asm
mov rcx, rax
add rcx, 1
mov rcx, rax
add rcx, 1
...
```

In this case the move breaks the dependency and the add's only depend on the
previous move making this example faster.

When the uops enters the backend it first encounters the RAT.

The RAT (Register Allocation Table but it has other names) is responsible for
translating the registers name into the register file and has 16 entries for the
usual registers. The register file has hundreds of registers.

When a uop comes in, goes into the RAT and gets its registers translated and to
some slots in the register file.

What the RAT and register file are cooperating to do is to translate something
with only 16 names into something that is working with several hundred names and
because of that it takes something that it should have been serially dependent
on a particular register and make that serial dependency go away.

### Execution Ports and the Scheduler

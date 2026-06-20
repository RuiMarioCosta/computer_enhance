# Computer Enhance Course Summary

- [Computer Enhance Course Summary](#computer-enhance-course-summary)
  - [Prologue: The Five Multipliers](#prologue-the-five-multipliers)
    - [1. Welcome to the Performance-Aware Programming Series!](#1-welcome-to-the-performance-aware-programming-series)
    - [2. Waste](#2-waste)
    - [3. Instructions Per Clock](#3-instructions-per-clock)
    - [4. Single Instruction, Multiple Data](#4-single-instruction-multiple-data)
    - [5. Caching](#5-caching)
    - [6. Multithreading](#6-multithreading)
    - [7. Python Revisited](#7-python-revisited)
  - [Interlude](#interlude)
    - [1. The Haversine Distance Problem](#1-the-haversine-distance-problem)
    - [2. "Clean" Code, Horrible Performance](#2-clean-code-horrible-performance)
  - [Part 1: Reading ASM](#part-1-reading-asm)
    - [1. Instruction Decoding on the 8086](#1-instruction-decoding-on-the-8086)
    - [2. Decoding Multiple Instructions and Suffixes](#2-decoding-multiple-instructions-and-suffixes)
    - [3. Opcode Patterns in 8086 Arithmetic](#3-opcode-patterns-in-8086-arithmetic)
    - [4. 8086 Decoder Code Review](#4-8086-decoder-code-review)
    - [5. Using the Reference Decoder as a Shared Library](#5-using-the-reference-decoder-as-a-shared-library)
    - [6. Simulating Non-memory MOVs](#6-simulating-non-memory-movs)
    - [7. Simulating ADD, SUB, and CMP](#7-simulating-add-sub-and-cmp)
    - [8. Simulating Conditional Jumps](#8-simulating-conditional-jumps)
    - [9. Simulating Memory](#9-simulating-memory)
    - [10. Simulating Real Programs](#10-simulating-real-programs)
    - [11. Other Common Instructions](#11-other-common-instructions)
    - [12. The Stack](#12-the-stack)
    - [13. Estimating Cycles](#13-estimating-cycles)
    - [14. From 8086 to x64](#14-from-8086-to-x64)
    - [15. 8086 Simulation Code Review](#15-8086-simulation-code-review)
  - [Part 2: Basic Profiling](#part-2-basic-profiling)
    - [1. Generating Haversine Input JSON](#1-generating-haversine-input-json)
    - [2. Writing a Simple Haversine Distance Processor](#2-writing-a-simple-haversine-distance-processor)
    - [3. Initial Haversine Processor Code Review](#3-initial-haversine-processor-code-review)
    - [4. Introduction to RDTSC](#4-introduction-to-rdtsc)
    - [5. How does QueryPerformanceCounter measure time?](#5-how-does-queryperformancecounter-measure-time)
    - [6. Instrumentation-Based Profiling](#6-instrumentation-based-profiling)
    - [7. Profiling Nested Blocks](#7-profiling-nested-blocks)
    - [8. Profiling Recursive Blocks](#8-profiling-recursive-blocks)
    - [9. A First Look at Profiling Overhead](#9-a-first-look-at-profiling-overhead)
    - [10. Comparing the Overhead of RDTSC and QueryPerformanceCounter](#10-comparing-the-overhead-of-rdtsc-and-queryperformancecounter)
  - [Part 3: Moving Data](#part-3-moving-data)
    - [1. Measuring Data Throughput](#1-measuring-data-throughput)
    - [2. Repetition Testing](#2-repetition-testing)
    - [3. Monitoring OS Performance Counters](#3-monitoring-os-performance-counters)
    - [4. Page Faults](#4-page-faults)
    - [5. Probing OS Page Fault Behavior](#5-probing-os-page-fault-behavior)
    - [6. Four-Level Paging](#6-four-level-paging)
    - [7. Analyzing Page Fault Anomalies](#7-analyzing-page-fault-anomalies)
    - [8. Powerful Page Mapping Techniques](#8-powerful-page-mapping-techniques)
    - [9. Faster Reads with Large Page Allocations](#9-faster-reads-with-large-page-allocations)
    - [10. Memory-Mapped Files](#10-memory-mapped-files)
    - [11. Inspecting Loop Assembly](#11-inspecting-loop-assembly)
    - [12. Intuiting Latency and Throughput](#12-intuiting-latency-and-throughput)
    - [13. Analyzing Dependency Chains](#13-analyzing-dependency-chains)
    - [14. Linking Directly to ASM for Experimentation](#14-linking-directly-to-asm-for-experimentation)
    - [15. CPU Front End Basics](#15-cpu-front-end-basics)
    - [16. Branch Prediction](#16-branch-prediction)
    - [17. Code Alignment](#17-code-alignment)
    - [18. The RAT and the Register File](#18-the-rat-and-the-register-file)
    - [19. Execution Ports and the Scheduler](#19-execution-ports-and-the-scheduler)
    - [20. Increasing Read Bandwidth with SIMD Instructions](#20-increasing-read-bandwidth-with-simd-instructions)
    - [21. Cache Size and Bandwidth Testing](#21-cache-size-and-bandwidth-testing)
    - [22. Non-Power-of-Two Cache Size Testing](#22-non-power-of-two-cache-size-testing)
    - [23. Latency and Throughput, Again](#23-latency-and-throughput-again)
    - [24. Unaligned Load Penalties](#24-unaligned-load-penalties)
    - [25. Cache Sets and Indexing](#25-cache-sets-and-indexing)
    - [26. Non-temporal Stores](#26-non-temporal-stores)
    - [27. Prefetching](#27-prefetching)
    - [28. Prefetching Wrap-up](#28-prefetching-wrap-up)
    - [29. A Closer Look at the Prefetching Performance Graph](#29-a-closer-look-at-the-prefetching-performance-graph)
    - [30. 2x Faster File Reads](#30-2x-faster-file-reads)
    - [31. Overlapping File Reads with Computation](#31-overlapping-file-reads-with-computation)
    - [32. Testing Memory-Mapped Files](#32-testing-memory-mapped-files)
  - [Part 4: Polynomial Evaluation (Optional)](#part-4-polynomial-evaluation-optional)
    - [1. Reference Haversine Code](#1-reference-haversine-code)
    - [2. Identifying Non-inlined Math Functions](#2-identifying-non-inlined-math-functions)
    - [3. Determining Input Ranges](#3-determining-input-ranges)
    - [4. Introduction to SSE Intrinsics](#4-introduction-to-sse-intrinsics)
    - [5. Function Approximation](#5-function-approximation)
    - [6. Range Reduction](#6-range-reduction)
    - [7. Approximation Using Higher-Power Polynomials](#7-approximation-using-higher-power-polynomials)
    - [8. Horner's Rule](#8-horners-rule)
    - [9. Fused Multiply-Add](#9-fused-multiply-add)
    - [10. Coefficient Arrays for Polynomial Evaluation](#10-coefficient-arrays-for-polynomial-evaluation)
    - [11. Approximating Arcsine](#11-approximating-arcsine)
    - [12. Extending Arcsine to the Full Input Range](#12-extending-arcsine-to-the-full-input-range)
  - [Part 5: Computation](#part-5-computation)
    - [1. Our Very Own Haversine](#1-our-very-own-haversine)
    - [2. Removing Waste](#2-removing-waste)
    - [3. Simplified Haversine Candidate](#3-simplified-haversine-candidate)
    - [4. Selectively Preventing Optimizations](#4-selectively-preventing-optimizations)
    - [5. Reading CPU Diagrams](#5-reading-cpu-diagrams)
    - [6. Better Dead Code Elimination - Or Is It?](#6-better-dead-code-elimination---or-is-it)
    - [7. Our Nemesis Returns](#7-our-nemesis-returns)
    - [8. Dead Code Elimination Prevention Macros](#8-dead-code-elimination-prevention-macros)
    - [9. Dependency Chain Stalls](#9-dependency-chain-stalls)
  - [ETW Challenge Series: Halloween Spooktacular Challenge (17 videos + 1 solution)](#etw-challenge-series-halloween-spooktacular-challenge-17-videos--1-solution)
    - [ETW Video 1. Announcing the 2024 Halloween Spooktacular Challenge](#etw-video-1-announcing-the-2024-halloween-spooktacular-challenge)
    - [ETW Video 2. Halloween Spooktacular Day 1: The Challenge](#etw-video-2-halloween-spooktacular-day-1-the-challenge)
    - [ETW Video 3. Halloween Spooktacular Day 2: Reboot Your Machine](#etw-video-3-halloween-spooktacular-day-2-reboot-your-machine)
    - [ETW Video 4. Halloween Spooktacular Day 3: Trace in Real-Time Mode](#etw-video-4-halloween-spooktacular-day-3-trace-in-real-time-mode)
    - [ETW Video 5. Halloween Spooktacular Day 4: Use TraceQueryInformation](#etw-video-5-halloween-spooktacular-day-4-use-tracequeryinformation)
    - [ETW Video 6. Halloween Spooktacular Day 5: Call TraceSetInformation Twice](#etw-video-6-halloween-spooktacular-day-5-call-tracesetinformation-twice)
    - [ETW Video 7. Halloween Spooktacular Day 6: PMCs Only Work for A Subset of Event Types](#etw-video-7-halloween-spooktacular-day-6-pmcs-only-work-for-a-subset-of-event-types)
    - [ETW Video 8. Halloween Spooktacular Day 7: Look for PMCs in the ExtendedData](#etw-video-8-halloween-spooktacular-day-7-look-for-pmcs-in-the-extendeddata)
    - [ETW Video 9. Halloween Spooktacular Day 8: MÄrtiÅ†Å¡ MoÅ¾eiko's Miniperf](#etw-video-9-halloween-spooktacular-day-8-märtiåå-moåeikos-miniperf)
    - [ETW Video 10. Halloween Spooktacular Day 9: What's Left?](#etw-video-10-halloween-spooktacular-day-9-whats-left)
    - [ETW Video 11. Halloween Spooktacular Day 10: Use TraceEvent](#etw-video-11-halloween-spooktacular-day-10-use-traceevent)
    - [ETW Video 12. Halloween Spooktacular Day 11: Define Your Own Event UserData](#etw-video-12-halloween-spooktacular-day-11-define-your-own-event-userdata)
    - [ETW Video 13. Halloween Spooktacular Day 12: Find Another PMC Event Type](#etw-video-13-halloween-spooktacular-day-12-find-another-pmc-event-type)
    - [ETW Video 14. Halloween Spooktacular Day 13: Use SysCallExit to Mark Start Points](#etw-video-14-halloween-spooktacular-day-13-use-syscallexit-to-mark-start-points)
    - [ETW Video 15. Halloween Spooktacular Day 14: Use SysCallEnter to Mark Stop Points](#etw-video-15-halloween-spooktacular-day-14-use-syscallenter-to-mark-stop-points)
    - [ETW Video 16. Halloween Spooktacular Day 15: Use GetEventProcessorIndex](#etw-video-16-halloween-spooktacular-day-15-use-geteventprocessorindex)
    - [ETW Video 17. Real-time PMCs on Windows with ETW](#etw-video-17-real-time-pmcs-on-windows-with-etw)

## Prologue: The Five Multipliers

### 1. Welcome to the Performance-Aware Programming Series!

This introductory video establishes the foundational concept of the entire course: "Performance-Aware Programming." Casey Muratori defines this as a distinct discipline separate from traditional "optimization." While optimization focuses on maximizing performance through specialized hardware knowledge and techniques, modern software faces a different problem—typical programs are written in ways that make them roughly 1000x slower than they should be. The key insight is that modern practitioners don't need to become expert optimizers; instead, they need general awareness of how performance works so they can make smart, reasonable decisions throughout their development process.

Muratori argues that performance awareness always has a strong business case, whether measured in improved user experience or reduced server infrastructure costs. The course aims to equip programmers with the knowledge to understand why software is fast or slow, and how their decisions affect performance characteristics. This represents a paradigm shift from accepting bloated, inefficient software as inevitable to consciously building reasonably performant systems as a standard practice. The goal is to bring the software industry back to a baseline where programs aren't orders of magnitude slower than necessary.

### 2. Waste

This video introduces "Waste" as the first and biggest multiplier that causes programs to run slowly. Muratori establishes a fundamental principle: there are only two things that will improve a program's performance—either reduce the total number of instructions the CPU must execute, or alter those instructions to move more efficiently through the CPU pipeline. This sets up the framework for the entire course. The video demonstrates concrete examples of how high-level languages and common programming practices introduce enormous amounts of wasteful instructions compared to what a CPU actually needs to accomplish a task. The driving question is: "How many CPU instructions does Python take to do one CPU instruction?" The answer illustrates the dramatic overhead introduced by language runtime, interpretation layers, and inefficient abstractions.

Understanding waste is critical because it's often the easiest multiplier to address. By eliminating unnecessary work before worrying about CPU efficiency or hardware features, programmers can achieve massive performance improvements with relatively straightforward refactoring. This video sets the stage for the remaining multipliers and demonstrates why a bottom-up approach to performance—starting with reducing waste—is more productive than trying to optimize already-bloated code.

### 3. Instructions Per Clock

This video introduces IPC (Instructions Per Clock), also known as ILP (Instruction-Level Parallelism), as the second major performance multiplier. After eliminating wasted instructions, there remains significant variability in how efficiently a CPU can execute the necessary instructions. Modern CPUs possess multiple ways to perform basic operations like addition, and the choice of how to structure code directly impacts how many instructions per clock cycle the CPU can execute. Muratori explains that even a well-optimized summation loop might only achieve 0.8 adds per cycle on certain CPU configurations—not because the CPU lacks capability, but because the code structure prevents the CPU from exploiting its parallel execution capabilities.

The video establishes that IPC is fundamentally about instruction-level parallelism—the CPU's ability to execute multiple instructions simultaneously across its pipeline and execution units. Understanding and maximizing IPC involves restructuring code to expose opportunities for parallel execution, such as reducing data dependencies and organizing operations to allow the CPU's out-of-order execution engine to find independent work. This multiplier becomes increasingly important once waste has been minimized, as it often delivers the next tier of performance improvements.

### 4. Single Instruction, Multiple Data

This video introduces SIMD (Single Instruction, Multiple Data) as the third major performance multiplier. Rather than improving the efficiency of existing instructions like IPC does, SIMD takes a different approach: it reduces the total number of instructions the CPU must execute by performing multiple operations with a single instruction. Modern CPUs feature vector instructions (such as SSE, AVX, and NEON) that can operate on multiple data elements simultaneously in wider registers. For example, instead of issuing four separate add instructions to sum four integers, a SIMD add instruction can perform all four operations at once. This approach bridges the gap between reducing instruction count (like eliminating waste) and improving instruction efficiency (like maximizing IPC).

Muratori explains how SIMD represents a complementary technique to instruction-level parallelism. While ILP focuses on making independent instructions execute faster by reordering and scheduling, SIMD explicitly packages multiple independent operations into a single CPU instruction. This video establishes that SIMD is essential for modern high-performance computing, and later parts of the course will dive deeply into how to leverage vector instructions effectively. Understanding SIMD as a fundamental multiplier helps programmers recognize when vectorization opportunities exist in their code.

### 5. Caching

This video introduces the fourth performance multiplier: Caching, with the driving question "How many bytes per cycle can the CPU read in?" The focus shifts from pure compute (adds, operations) to memory access patterns. Muratori explains that loads (reading from memory) and stores (writing to memory) are critical components of any loop that don't operate at CPU speeds—they have their own latency and throughput characteristics. When the CPU executes an instruction that loads a value from memory into an accumulator, it creates a dependency: the CPU cannot proceed with dependent instructions until the load completes and the data arrives. This creates a bottleneck separate from computation itself.

The video establishes that understanding memory access patterns is essential for performance analysis. Even with perfect instruction-level parallelism and SIMD utilization, poor cache locality and long memory latencies can completely dominate runtime. The CPU's cache hierarchy (L1, L2, L3) becomes critical to performance—cache hits are measured in single-digit cycles, while main memory accesses can take hundreds of cycles. This multiplier introduces the reality that modern program performance is often limited not by how fast the CPU can compute, but by how fast it can access the data it needs to compute on.

### 6. Multithreading

This video introduces the fifth and final multiplier from the Prologue: Multithreading, with the question "How many instruction streams can the CPU execute at once?" The concept is straightforward—if one processor can perform a task at a certain speed, multiple processors should achieve faster speeds (ideally nearly linear scaling with the number of processors). Modern CPUs have evolved from single-core systems to multi-core designs where a single CPU package contains multiple cores, each capable of executing independent instruction streams simultaneously. Most consumer computers now contain at least four cores, meaning a single physical machine is effectively multiple computers working in parallel.

The critical performance insight is that if software cannot utilize multiple cores through multithreading or parallel processing, it forfeit a massive performance multiplier. Muratori emphasizes that in modern computing, failing to exploit multiple cores represents a significant performance wastage. Understanding how to structure software to take advantage of parallel execution across cores becomes increasingly important as core counts grow. This multiplier often represents one of the largest potential performance gains available to programmers, yet it's frequently overlooked or implemented inefficiently.

### 7. Python Revisited

This final Prologue video synthesizes all five multipliers by revisiting the original Python vs. C comparison. Muratori demonstrates a concrete achievement: improving a simple summation loop from 0.006 adds per cycle (in Python) to 52 adds per cycle (in optimized C)—a speedup of over 8,000 times. This isn't theoretical; it's a practical demonstration of how the five multipliers (waste elimination, IPC improvement, SIMD utilization, cache optimization, and multithreading) compound to create massive real-world performance differences.

Crucially, Muratori reframes the course's philosophy: the goal is not to chase the full 8,000x speedup, but to understand where that gap comes from and how to achieve reasonable multipliers (like 100x or 1,000x) through relatively straightforward optimization. Since modern software is often 1,000x or 10,000x slower than necessary, even partial optimization efforts yield enormous benefits. The video emphasizes that programmers don't need to become professional optimizers; they simply need awareness and selective application of these multipliers to move from absurdly inefficient baselines to reasonably performant systems. This sets the stage for the course's deeper dives into each multiplier, armed with the knowledge that these gaps are real, pervasive, and exploitable.

## Interlude

### 1. The Haversine Distance Problem

This interstitial video introduces the primary example problem that will serve as a reference point for the remainder of the course. Rather than continuing with artificial simple loops, Muratori presents a more realistic programming problem: calculating haversine distances (great-circle distances between points on Earth given their latitude and longitude). This problem was selected by searching Stack Overflow for actual programmer questions about performance, ensuring it represents a real-world scenario that programmers care about optimizing.

The haversine problem encompasses several representative performance challenges beyond simple arithmetic: it involves trigonometric and square root calculations, multiple input data points, and complex control flow patterns. Throughout the course, viewers will apply the five multipliers from the Prologue to progressively optimize a haversine implementation, seeing how waste elimination, instruction-level parallelism, SIMD, caching, and multithreading each contribute to real improvements in a practically relevant algorithm. This problem serves as a consistent benchmark for demonstrating how the course's theoretical principles translate into measurable performance gains on genuine computational workloads.

### 2. "Clean" Code, Horrible Performance

This provocative bonus video directly challenges common programming best practices by demonstrating their catastrophic performance costs through concrete measurements. Muratori analyzes five widely-taught "clean code" rules: preferring polymorphism over switch statements, objects hiding their internals, small functions, functions doing one thing, and DRY (Don't Repeat Yourself). Using real examples from clean code literature (shape class hierarchies computing areas), he measures the performance impact of each rule individually. The results are damning: using polymorphism and encapsulation instead of a simple switch statement incurs a 1.5x performance penalty (equivalent to hardware aging 3-4 years). When adding a second property to the classes and applying additional rules, the penalty grows to 10x-15x slower than a table-driven implementation.

Muratori argues that "clean code" methodology, despite intentions of improving maintainability, is systematically destroying performance across the industry. The video shows that violating just two of the five rules (preferring polymorphism and encapsulation) erases 12+ years of hardware evolution. When scaled to real-world codebases with many properties, performance penalties can reach 20-25x compared to optimized implementations. The core message is provocative but data-driven: blindly following clean code principles without regard to performance is professionally irresponsible when the performance cost is so severe.

## Part 1: Reading ASM

### 1. Instruction Decoding on the 8086

This opening video for Part 1 introduces the pedagogical approach for the remainder of the course: building a virtual CPU simulator in software. Rather than attempting to understand modern CPUs like Alder Lake or Zen4 (which represent 40+ years of evolutionary complexity), Muratori strategically steps back to the original 8086 CPU—the foundation of the IBM PC from the 1980s. This architectural simplification allows learners to understand core CPU concepts without being overwhelmed by modern complexity. The 8086 is the ideal reference point because the entire x86 lineage traces back to it; understanding the 8086 provides foundational knowledge applicable to understanding modern CPUs through the lens of accumulated features.

Throughout Part 1, viewers will implement increasingly sophisticated decoders and simulators capable of parsing 8086 machine code (the byte-level binary format) and executing simulated instructions. The goal is not just to understand 8086 specifically, but to internalize how CPUs fundamentally work: how they fetch instruction bytes, decode them into operations and operands, execute those operations, and move to the next instruction. This hands-on simulation exercise grounds the theoretical performance concepts from the Prologue in concrete implementation details.

### 2. Decoding Multiple Instructions and Suffixes

Building directly on the previous video, this lesson expands the decoding challenge from single register-to-register MOV instructions to handling multiple instruction types with varying encodings and lengths. The key insight Muratori emphasizes is that even a single assembly mnemonic like MOV can be encoded in radically different ways in machine code depending on its operands (register-to-register, register-to-immediate, memory-to-register, etc.). Viewers must learn to properly check MOD fields, opcodes, and handle variable-length instructions to correctly decode diverse instructions. This reinforces that CPU instruction decoders cannot make assumptions; they must check all fields and handle the full complexity of the instruction format.

The video demonstrates that instruction decoding is substantially more intricate than homework might suggest—the 8086 instruction set was already complicated in its original form. By working through multiple instruction types, learners begin to appreciate the architectural decisions baked into CPUs and how those decisions affect both the design of the CPU itself and the performance implications of executing varied instruction patterns. Understanding these encoding details is critical for later performance analysis.

### 3. Opcode Patterns in 8086 Arithmetic

This final video on 8086 instruction decoding consolidates the core principles while introducing one last critical pattern: the systematic design decisions underlying instruction encodings. After completing homework on decoding MOV and other instructions, learners understand that while the 8086 instruction set appears voluminous, all instructions follow consistent bit-pattern logic—extracting opcode bits, checking fields, and using them for table lookups or immediate values. The patterns have evolved in modern x64, but the fundamental decoding strategy remains identical. Muratori emphasizes a crucial design pattern: instructions like MOV use a "d bit" and "w bit" to encode direction and width, allowing a single opcode template to represent multiple instruction variants compactly. This design decision, intrinsic to all x86-derived processors, has important performance implications that will resurface later in the course.

### 4. 8086 Decoder Code Review

This code review video walks through Muratori's reference implementation of an 8086 instruction decoder—the solution to the three previous homework assignments. Rather than requiring all viewers to use the reference decoder, Muratori explicitly notes that those satisfied with their own implementations can continue with their code for subsequent assignments. This video serves two purposes: first, to provide a working reference decoder in the course materials (available on GitHub); second, to demonstrate one complete, working approach to the pedagogical problem of building an instruction decoder from scratch.

### 5. Using the Reference Decoder as a Shared Library

This logistical video addresses a practical accessibility concern: viewers using Python, C#, or other non-C languages lack easy access to the reference decoder since it's written in C. Muratori explains the transition from the decoding homework (which taught core concepts) to simulation homework (which requires functional decoders). To lower the barrier to entry, he refactored the reference decoder into a shared library callable from any language via FFI. This democratizes access to a working decoder for viewers who understand decoding principles but either didn't have time to complete production-quality implementations or prefer to focus their effort on the simulation and analysis tasks ahead. The move reflects good course design: eliminate incidental friction (language barriers) while preserving essential learning (understanding how decoding and simulation work).

### 6. Simulating Non-memory MOVs

This video marks a major transition from instruction decoding to instruction execution. After learning how CPUs represent instructions in binary, viewers now learn what those instructions actually do when executed. Muratori establishes a critical insight: instruction set encodings are far more uniform than they appear—understanding 8086 decoding provides immediate comprehension of x64 (with minor differences), and makes RISC-V or ARM feel simpler by comparison. The real mystery lies not in decoding, but in execution: understanding how decoded operations combine to form executable programs. The shift from "how is this represented?" to "what does this do?" marks the beginning of CPU simulation, where viewers must track register state, implement operation semantics, and manage instruction flow through a simulated processor.

### 7. Simulating ADD, SUB, and CMP

This video extends simulation beyond MOV instructions to arithmetic operations like ADD, SUB, and CMP. While MOV simulation was straightforward (simply copying values), arithmetic instructions are fundamentally more complex because they modify more than just their destination—they also set flags that record operation outcomes (zero, sign, carry, overflow). The 8086 has limited general-purpose registers (four 16-bit and four 8-bit accessible, with the 8-bit versions being subregisters), and arithmetic operations must correctly update not only register values but also the CPU flags register. Understanding how flags work becomes critical later for control flow (conditional jumps) and performance analysis (dependency chains). The video introduces the subtlety that seemingly simple operations like arithmetic carry significant implementation complexity when faithfully simulating CPU behavior.

### 8. Simulating Conditional Jumps

This video introduces the Instruction Pointer (IP) register—the CPU's mechanism for tracking position in the instruction stream. During decoding homework, viewers already implemented a similar concept informally (counting bytes or using iterators), but now it must become a real register in the simulation. The IP register is special: unlike general-purpose registers, it cannot be manipulated by arithmetic instructions; only dedicated control-flow instructions (jumps, calls, returns) can modify it. Conditional jumps decide whether to branch based on flag states set by previous arithmetic operations. This video establishes the critical connection between flags (modified by arithmetic) and control flow (using those flags to make branching decisions). The IP register is foundational for converting sequential instruction execution into loops, conditionals, and complex program control flow—the structures that make programs useful.

### 9. Simulating Memory

At this point, viewers have implemented decoders, register simulation, arithmetic operations, flags, and control flow. This video adds the final piece: memory-to-register and register-to-memory MOV instructions. With only five instruction types (MOV with memory, ADD, SUB, CMP, and conditional jumps), surprisingly complex real programs can be built. Muratori emphasizes that most assembly language reading ability comes from understanding a small core of instructions well rather than memorizing thousands. By adding memory access to the simulation, viewers unlock the ability to examine real programs beyond trivial examples. Memory operations are critical for performance analysis introduced in earlier sections: loads and stores dominate latency, create dependencies, and interact with cache systems. Implementing memory in the simulator—even as a simple array—establishes the foundation for understanding why real program performance is so dependent on how cleverly data is accessed.

### 10. Simulating Real Programs

This capstone video for Part 1's simulation work demonstrates that the minimal instruction set (MOV, ADD, SUB, CMP, conditional jumps, with memory access) is sufficient to write genuinely complex programs. Muratori challenges the intuition that such a limited set couldn't power real applications. He argues that even massive codebases (game renderers, image processors) compile to relatively few instruction types repeated millions of times in loops. The key insight is that real complexity in programs comes from loops and data manipulation, not from instruction diversity. He demonstrates this principle by sketching how a pixel-drawing program could work: loop through memory addresses, write RGBA values, save the result to file. With this minimal 8086 simulator, viewers can now run and observe real programs executing machine code, setting the stage for cycle counting and performance analysis in subsequent videos.

### 11. Other Common Instructions

With a solid foundation in instruction decoding and simulation, this video extends viewers' instruction set knowledge without requiring additional homework. The key insight is that understanding decoding, registers, the IP, and flags provides the framework to rapidly learn new instructions by consulting CPU manuals. Rather than memorizing thousands of instructions, programmers need only understand the common ones they'll encounter in practice. The video overviews frequently-used instructions beyond the core set: multiplication and division (MUL, IMUL, DIV, IDIV), logical operations (AND, OR, XOR, NOT), bit rotations (ROL, ROR), and other arithmetic variants. Viewers learn that instructions follow predictable patterns: once you understand how the core instructions modify registers and flags, extending to similar instructions becomes mechanical. This democratizes x86 assembly literacy—viewers no longer need to be assembly experts to understand most real code.

### 12. The Stack

This video introduces the stack mechanism, which underlies function calls in virtually all programming languages but remains invisible at the source code level. Compilers generate stack manipulation instructions (PUSH, POP, CALL, RET) to implement function calls, parameters, and local variables. Understanding the stack is critical for reading real compiler-generated assembly language, where stack instructions are intertwined with application logic. Muratori explains that the stack is a Last-In-First-Out (LIFO) data structure stored in memory, accessed through the Stack Pointer (SP) register. When a function is called, the CPU must save the return address (where execution should resume after the function completes), manage parameter passing, and allocate space for local variables. The stack convention has been standardized across the x86 lineage for decades, enabling reliable calling conventions. Without understanding the stack, many real programs' assembly code would appear incomprehensible; with stack knowledge, the structure of function calls becomes obvious.

### 13. Estimating Cycles

This capstone to Part 1 pivots from "what does the CPU do?" to "how long does it take?" With assembly language literacy established, viewers now learn to estimate performance by reading instruction sequences and calculating expected CPU cycles. Muratori emphasizes a critical point: programmers rarely need to write assembly language for performance optimization; instead, they need to read compiler-generated code and estimate its performance. This estimate-and-analyze skill underpins three key performance-programming activities: validating compiler output matches performance expectations, identifying bottleneck regions in code, and assessing whether code matches hardware capabilities. The 8086 is ideal for teaching cycle estimation because it's simpler than modern CPUs but still representative of the fundamental principles. Learning to estimate cycles on the 8086 provides a foundation for understanding more complex out-of-order execution in modern processors introduced later in the course.

### 14. From 8086 to x64

This video bridges Part 1 (8086 foundation) and the remaining modern CPU analysis by demonstrating that x64 assembly language is fundamentally familiar to viewers who understand 8086. Despite 40+ years of evolution from the 8086 to modern x64, the core instruction semantics have remained remarkably consistent. Most non-SIMD x64 instructions are nearly identical to their 8086 counterparts; the main new concepts are extended registers (8 additional general-purpose registers, plus 64-bit versions of existing ones) and SIMD extensions. The architectural foundation—opcode structure, flags, memory addressing, calling conventions—persists from 1978 to 2023. This remarkable backward compatibility is both a strength (8086 knowledge transfers directly) and a constraint (x86 carries legacy complexity). With this transition, viewers understand that Part 1's investment in 8086 mastery directly enables x64 literacy, multiplying the return on learning effort.

### 15. 8086 Simulation Code Review

This final Part 1 video provides a reference implementation walkthrough of a complete 8086 simulator covering instruction execution and cycle estimation. Like the decoder code review, this serves viewers who either built unsatisfactory simulators or prefer focusing efforts on subsequent analysis tasks rather than implementation details. The simulator includes cycle counting capability not just for the instructions explicitly simulated in homework but for all common x86 instructions, enabling direct performance analysis. Viewing this code is optional; those satisfied with their own implementations can skip directly to Part 2. This video completes Part 1's pedagogical arc: understand decoding → implement execution → estimate performance → examine reference implementation.

## Part 2: Basic Profiling

### 1. Generating Haversine Input JSON

Part 2 shifts focus from assembly literacy to practical benchmarking and profiling of the haversine distance problem introduced earlier. This opening video addresses the prerequisite: generating realistic input data at scale. The haversine problem requires pairs of latitude/longitude coordinates (conceptualized as X/Y spherical coordinates ranging -180 to 180 degrees for X, -90 to 90 for Y), and the task is to compute the average arc length between point pairs using the haversine formula. Muratori deliberately chose JSON as the input format specifically because it presents realistic performance challenges—text parsing is inherently slower than binary formats, forcing developers to grapple with real-world constraints. The task requires generating variable-sized datasets from kilobytes to gigabytes, allowing viewers to test how performance scales with input size. This homework sets up the infrastructure for Parts 2 and 3, where various haversine implementations will be benchmarked and optimized.

### 2. Writing a Simple Haversine Distance Processor

With input data generation complete, now comes the real challenge: implementing a haversine processor that can parse the JSON and compute the average haversine distance. While the mathematical haversine distance function can be copied from the input generator, the bulk of the work lies in JSON parsing—a task most programmers delegate to libraries without understanding its performance cost. Muratori deliberately requires manual JSON parsing (rather than library usage) to make the CPU work visible and measurable. This pedagogical choice elegantly demonstrates algorithmic choice implications: JSON parsing is hundreds of lines of code; binary input would be zero lines. This gap directly translates to CPU cycles, memory bandwidth, and cache efficiency. By implementing parsing from scratch, viewers experience firsthand the performance cost of choosing text-based data formats over binary, a ubiquitous tradeoff in real systems.

### 3. Initial Haversine Processor Code Review

This code review video provides a walkthrough of the reference starting codebase for the haversine processor built in the first two Part 2 assignments. As with the earlier review videos, it is optional for viewers whose own implementation is already in good shape. The value of the episode is that it freezes a known-correct baseline before the course shifts into timing and profiling work. From this point forward, the goal is no longer merely to have working code, but to understand where the time goes and how much of that time is justified by the problem itself.

That baseline matters because the upcoming measurements only mean something if the program is doing the intended work correctly. Muratori uses the review to anchor the rest of Part 2 around a concrete codebase that reads JSON, computes haversine distances, and produces a result that can be trusted. Once correctness is established, performance questions become actionable instead of speculative.

### 4. Introduction to RDTSC

This video introduces the x86 RDTSC instruction, which reads the processor's time-stamp counter and gives programmers access to a very high-resolution timer. Muratori makes clear that the instruction itself is simple, but the surrounding context is not: over multiple processor generations, the meaning of the counter changed from something closer to actual core cycles into something more like a stable wall-clock source. That history matters because if you want to use RDTSC for benchmarking, you need to know what the values actually represent on modern hardware.

The episode therefore treats timing as a systems problem rather than just an API call. A raw TSC value is not useful until it is calibrated against a known interval, and that calibration forces the programmer to think about frequency, invariance, core behavior, and measurement methodology. This becomes the foundation for the profiler work that follows, because the entire rest of Part 2 depends on collecting timing data with enough precision and enough understanding to trust it.

### 5. How does QueryPerformanceCounter measure time?

This episode investigates Windows' QueryPerformanceCounter by stepping into its assembly implementation and seeing what the operating system actually does. Muratori shows that on modern x64 Windows systems, QueryPerformanceCounter commonly derives its value from RDTSCP, not from some entirely separate hardware timer. The function then scales the raw TSC value down to the advertised QueryPerformanceFrequency using a multiply-and-shift trick that effectively performs a division by a constant much faster than a literal DIV instruction would.

The important lesson is not just that QueryPerformanceCounter uses the TSC under the hood, but that Part 1's assembly-reading work now pays off directly. Instead of guessing, searching forum posts, or trusting outdated folklore about HPET and Windows timers, the programmer can inspect the code path personally and reason about it. That makes the tradeoff concrete: QueryPerformanceCounter is useful as a portable OS abstraction, but for local benchmarking it is often just a slower, lower-precision wrapper around the same fundamental clock source.

### 6. Instrumentation-Based Profiling

With RDTSC available, this video turns a timing primitive into an actual profiler by introducing instrumentation-based profiling. The core idea is simple: insert extra code into the program to record entry and exit times for the regions you care about, then accumulate those measurements into counters. Muratori emphasizes that this is still profiling even if a compiler or tool inserts the probes automatically; what defines the method is that the profiled program has been modified to gather data.

He also distinguishes instrumentation from sampling. Instrumentation gives precise measurements for the marked regions and is easy to understand, which makes it a good first profiling method to build by hand. But it also changes the program's execution, sometimes significantly, and that fact becomes a recurring theme. The video sets up the practical profiler used for the rest of the haversine work while also introducing the habit of being skeptical about the measurement process itself.

### 7. Profiling Nested Blocks

This video improves the first instrumentation profiler by addressing a clarity problem: nested profile regions make the raw numbers hard to interpret. If one timed block contains another, simply summing elapsed times can blur the distinction between time spent directly in the outer block and time spent in the child region it called. Muratori refines the design so that the profiler can report both total time and time exclusive of children, which makes the output much easier to read and much more useful when you are trying to decide where optimization effort should go.

The implementation discussion is also a good example of balancing engineering ambition with simplicity. The profiler is not intended to be a commercial tool with every edge case polished away; it is an integrated utility meant to stay small and easy to deploy. Even so, the video shows that a modest design improvement can make profiling output far more intelligible, which is often worth the extra code.

### 8. Profiling Recursive Blocks

Recursive call graphs expose a flaw in the nested profiler design, and this video is about fixing that flaw rather than living with it. Muratori points out that a profiler that misreports recursive regions is dangerous not only when the error is obvious, but especially when the numbers still look plausible. Bad profiling data leads directly to bad optimization choices, which defeats the entire purpose of profiling.

The discussion therefore focuses on correctness of the measurement model itself. The previous design tracked total time and child time in a way that worked for simple nesting but broke once the same region reentered itself. The episode treats that as a real engineering hazard and pushes the design forward so that recursive regions no longer silently poison the output. The broader lesson is that profiling tools need to be trustworthy under realistic call graphs, not just idealized ones.

### 9. A First Look at Profiling Overhead

After building a usable profiler, this video asks the next necessary question: what is the profiler doing to the program's behavior? Muratori stresses that profiling overhead is not an optional footnote. Without special hardware support, collecting performance data changes execution, and in modern software that change can be large enough to distort the conclusions if you are not careful. That is why building a profiler by hand is valuable: you can see exactly what extra work gets inserted and start learning how that work perturbs the program.

The haversine code provides a concrete example. If every JSON parser function is instrumented, the added timing logic can materially alter function cost, call structure, and even the compiler's optimization choices. The result is still useful, but only if the programmer understands that the reported numbers may include artifacts introduced by the measurement itself. This episode marks the point where profiling becomes not just a way to observe performance, but also a thing that must be profiled and evaluated on its own.

### 10. Comparing the Overhead of RDTSC and QueryPerformanceCounter

This final Part 2 video closes the initial timer-and-profiler arc with a direct A/B comparison between using RDTSC and using QueryPerformanceCounter as the underlying profiling timer. Since the profiling framework is already in place, the experiment is straightforward: swap the timer implementation and compare the cost. Muratori frames this as exactly the sort of controlled measurement programmers should get used to doing whenever a design choice might matter.

The conclusion follows naturally from the earlier reverse engineering: because QueryPerformanceCounter is often a wrapper around RDTSCP plus extra arithmetic and function-call overhead, it is a worse choice for fine-grained local profiling when RDTSC is already available. That does not make QueryPerformanceCounter useless; it just clarifies where it belongs. The episode also serves as a clean stopping point for Part 2: the course now has a working profiler, a basic mental model of timer behavior, and enough measurement discipline to move into more serious performance modeling in Part 3.

## Part 3: Moving Data

### 1. Measuring Data Throughput

Part 3 begins by shifting the question from “where is time going?” to “how fast should this code be able to run at all?” Muratori uses the haversine processor as the motivating example: Part 2 already showed that JSON parsing and allocation dominate runtime, but that alone does not tell us whether the observed speed is close to the hardware limit or embarrassingly far from it. To answer that, programmers need to measure data throughput, meaning how much input data a piece of code processes per unit time.

This is the first step toward building an upper-bound model for performance. If the application reads or processes a certain number of bytes per second, that number can be compared against plausible limits imposed by storage, memory bandwidth, or other parts of the machine. The video starts teaching the discipline of converting profiler timings into throughput figures, which then become the bridge between observed behavior and theoretical peak performance.

### 2. Repetition Testing

This video introduces repetition testing as a practical way to deal with the massive variability present in real systems. Normal block profiling is useful for showing where time goes in a full application run, but it is too noisy to reveal the best-case speed of an individual code path. Repetition testing tackles that by isolating a small operation, running it many times, and keeping the minimum observed time. The goal is not to produce a perfectly realistic number, but to approximate the fastest performance the system can deliver when caches, predictors, and surrounding state line up favorably.

Muratori is careful to frame this as a bypass rather than a full solution. Repetition testing does not remove hidden state from the machine; it simply gives the measured code many chances to land in a favorable state. Even so, that is often good enough to estimate whether a path like fread is anywhere near its practical ceiling. The broader takeaway is that performance-aware programming depends as much on designing the right experiment as it does on reading the result.

### 3. Monitoring OS Performance Counters

This video extends the investigation of read-bandwidth discrepancies by turning to operating-system counters as an external source of evidence. Rather than guessing blindly about why the application's file reads are slower than the repetition tester suggests they should be, Muratori frames the problem as one of guided discovery: use the data the OS is willing to expose, compare it to what you already know, and let that narrow the search space. The real lesson is methodological. Programmers will always run into performance problems caused by subsystems they do not yet understand, and the right response is not hand-waving but systematic expansion of their knowledge.

The OS counters become a bridge between the profiler and the deeper virtual-memory topics that follow. They reveal that page faults correlate strongly with the observed slowdown, which tells us that the problem is not merely about raw storage speed or fread itself. This makes the episode a transition point from general throughput testing into a more detailed exploration of how the operating system's memory management interferes with apparently simple file I/O.

### 4. Page Faults

This video explains what page faults actually are and why they can dramatically slow down a program. Muratori ties them back to two core features of modern systems that the 8086 lacked: virtual memory and memory protection. On the 8086, a computed address was essentially a direct route to physical memory. On modern x64 systems, the addresses programs use are virtual addresses that must be translated before the CPU can reach real RAM. That translation process is what makes per-process address spaces, protection, and paging possible.

Page faults are the visible cost of that abstraction. When a program touches a page that is not currently mapped in the way the hardware expects, the CPU must trap into the operating system so the mapping can be established or repaired before execution continues. That work is not free. The video makes clear that the slowdown seen in the haversine reader is not mysterious at all once you remember that every first touch of unmapped memory may require cooperation between hardware and kernel before useful work can resume.

### 5. Probing OS Page Fault Behavior

After defining page faults, this bonus video asks a more careful question: what exactly is Windows counting when it reports process-level page faults? Muratori points out that a counter named page faults might count interrupts, or it might instead count the number of pages that get mapped as a consequence of those interrupts. That distinction matters because one interrupt mapping many pages would have very different performance implications from one interrupt per page, even if the counter values looked superficially similar.

The episode is therefore about building experiments that separate those possibilities. Instead of treating the operating system's terminology as automatically precise, Muratori shows how to probe it empirically and discover whether Windows is pre-mapping additional pages or following some other pattern. This is a good example of performance work at its best: even when the system provides a counter, you still have to determine what real-world event that counter corresponds to before using it to guide design decisions.

### 6. Four-Level Paging

This bonus entry digs into the structure of modern x64 pointers so that the strange page-fault patterns seen earlier can be explained rather than merely observed. Muratori argues that most programmers, including many C and C++ programmers, treat pointers as opaque 64-bit integers when they are actually packed structures interpreted by the hardware paging system. That abstraction is usually fine, right up until the hardware behavior leaks through and creates anomalies the abstraction cannot explain.

The purpose of the video is to break that abstraction intentionally. By learning how x64 virtual addresses are divided across the levels of the page-table hierarchy, viewers gain the tools to reason about why some page faults appear “extra,” why certain patterns repeat at regular boundaries, and why low-level memory effects often line up with bit fields inside the address itself. It is deeper OS and hardware material than the course strictly requires, but it pays off by making later anomalies feel principled rather than magical.

### 7. Analyzing Page Fault Anomalies

With the page-table model in hand, this video returns to the unexplained Windows anomalies and subjects them to targeted measurement. The key shift is from noticing that odd page-fault spikes exist to designing tests that reveal when they occur and under what address patterns. Muratori uses backward page touches to suppress Windows' normal pre-fault-ahead behavior, making the anomalies easier to isolate and count. This transforms a vague hunch about “extra faults” into a repeatable signal.

The episode shows how theoretical understanding and empirical testing reinforce each other. Four-level paging suggested where the discontinuities might come from; the experiment checks whether those predictions match reality. This kind of loop between model and measurement is central to the course: you do not just collect numbers, and you do not just study architecture diagrams. You use each to pressure-test the other until the behavior of the system becomes predictable.

### 8. Powerful Page Mapping Techniques

This bonus video broadens the page-mapping discussion from diagnosis to capability. Once you understand that virtual memory is a programmable translation layer rather than just a safety feature, a number of useful techniques become available through ordinary OS APIs. Muratori highlights three especially practical examples on Windows: circular buffers implemented by aliasing pages so the wraparound disappears, change detection driven by dirty bits, and sparse virtual-memory regions that let programs act as though enormous memory ranges exist even when only a small subset is actually backed.

The deeper message is that address translation can be a design tool, not just an implementation detail. Many tasks that look like they require special data-structure logic or explicit bookkeeping can instead be moved into the mapping layer and made transparent to the code using the memory. Even though the video is somewhat orthogonal to the haversine optimization thread, it reinforces the idea that low-level systems knowledge opens up whole categories of solutions that are invisible from a purely high-level programming viewpoint.

### 9. Faster Reads with Large Page Allocations

This video returns directly to the original read-performance problem and asks whether the page-fault overhead can be reduced in practice. Since the previous entries established that first-use mapping interrupts are a major source of slowdown, the natural next step is to reduce how often that mapping work has to happen. Large pages provide exactly that possibility: by backing memory with 2 MB pages instead of many 4 KB pages, the operating system can cut the number of mappings required to provision the destination buffer for a large read.

Muratori uses this to show that virtual-memory knowledge can translate into immediate performance wins. The OS file cache may already let reads come from RAM instead of disk, which means the real bottleneck can be the allocation and mapping of the destination memory rather than the storage subsystem. Large page support, where available, therefore becomes a practical technique for turning the page-fault excursion into a measurable improvement in actual application startup behavior.

### 10. Memory-Mapped Files

This video examines memory-mapped files as a form of lazy file I/O built on the same paging machinery discussed in the previous entries. Instead of explicitly allocating memory and then calling ReadFile to copy bytes into it, a program can map the file into its virtual address space and let the operating system populate pages on demand as the program touches them. In effect, file access becomes a special case of page-fault handling: a fault on an unmapped file-backed page causes the OS to read the corresponding file contents into physical memory and resume execution.

The appeal is obvious: the API surface is small, the code is simple, and read-on-demand behavior can sometimes avoid work. But the course context also makes the tradeoff clear. Memory-mapped files do not eliminate paging overhead; they simply shift when and how it happens. The episode therefore fits naturally as the final page-mapping topic before the course moves on to CPU internals, because it shows one of the most common real-world ways that file I/O and virtual memory become the same problem.

### 11. Inspecting Loop Assembly

This video serves as a hands-on bridge back from OS behavior to CPU-level analysis. Muratori states the key requirement plainly: to understand the performance of high-level code, you must identify the exact assembly instructions that implement the hot loop and isolate the critical region from surrounding setup and bookkeeping. The video is demonstration-heavy rather than transcript-heavy, using debuggers and Compiler Explorer to show how to locate and inspect those loop bodies in practice.

That makes the episode more procedural than conceptual, but it is still important. The course has already established that performance arguments eventually have to bottom out in assembly. This video teaches the practical workflow for getting from source code to the assembly you actually need to reason about, which is a prerequisite for the latency and throughput modeling that follows.

### 12. Intuiting Latency and Throughput

This video gives an intuitive model for latency, throughput, reciprocal throughput, and dependency chains using a laundry-room analogy. One load of laundry in either a combined washer-dryer or separate washer and dryer still takes two hours from start to finish, so the latency is the same. But once there are multiple independent loads, the split setup wins because washing one load can overlap with drying another. The latency of a single load stays constant while the throughput of the system improves.

Muratori uses this analogy to make a subtle CPU concept feel ordinary instead of abstract. Latency is the time a single operation spends traversing the pipeline; throughput is the rate at which completed operations emerge once the pipeline is full. Reciprocal throughput is often used because it expresses throughput in time-per-operation units, which lines up with latency and makes comparisons easier. The discussion then naturally introduces dependency chains: overlap only helps when operations are independent. Once dependencies force one step to wait for another, throughput falls back toward latency-dominated behavior. That framing becomes essential for understanding modern CPU pipelines in the episodes that follow.

### 13. Analyzing Dependency Chains

This video takes the intuition from the laundry analogy and applies it to more realistic chains of work. The central idea is that once a program is expressed as a sequence of dependent steps, the total latency is set by the longest chain of operations that must happen in order, while throughput depends on how much independent work can be kept in flight alongside that chain. Muratori moves from the intuitive model into a more explicit way of reading dependency graphs, because modern CPU performance is largely determined by whether the processor can find independent instructions to overlap while earlier results are still pending.

That makes dependency-chain analysis the key bridge from intuition to practical CPU modeling. If a hot loop consists mostly of one long chain where every operation waits on the previous one, then better throughput elsewhere in the machine will not help much. If instead the loop contains multiple shorter, partially independent chains, the out-of-order engine may be able to overlap them and approach the machine's back-end limits. This episode is where the course starts turning those ideas into a repeatable analysis method.

### 14. Linking Directly to ASM for Experimentation

This video turns assembly inspection into a controlled experimental workflow. Once a critical loop has been extracted from compiler output, Muratori's next step is to assemble that loop into a directly linkable library so it can be called from a small test harness. That setup isolates the exact instruction sequence under study and removes much of the ambiguity introduced by surrounding code, compiler decisions, and higher-level runtime behavior.

The point is not to abandon normal source-level programming, but to create a precise laboratory for microarchitectural questions. If you want to know how a CPU handles a particular dependency pattern, addressing mode, or instruction mix, you need a way to run just that sequence under controlled conditions. Linking directly to handwritten or extracted assembly gives you that control and makes later experiments on front-end, scheduling, and bandwidth behavior much more trustworthy.

### 15. CPU Front End Basics

This video introduces the front end of a modern CPU: the machinery responsible for fetching, decoding, and preparing instructions before they ever reach the execution units. Muratori emphasizes that if the processor is going to exploit instruction-level parallelism, it cannot wait until the last possible moment to understand the instruction stream. It must decode well ahead of execution, maintain a supply of work, and keep the back end fed even while branches and data dependencies complicate the path forward.

This reframes performance analysis yet again. Up to this point, it was possible to think mostly about the instructions themselves and their direct data dependencies. The front end adds another limit: even if the back end could execute more work, it only matters if the processor can fetch and decode enough useful instructions in time. That is why things like branch behavior, instruction size, and code layout start to matter so much in the following episodes.

### 16. Branch Prediction

Once the front end is decoding ahead of execution, it faces an immediate problem: conditional branches depend on flags or data values that may not be known yet. This video explains how branch prediction solves that problem by letting the processor guess which path control flow will take and continue fetching and decoding along that predicted path. Without that mechanism, the front end would constantly stall at every branch while waiting for the back end to resolve the condition.

The consequence is that control flow becomes a performance issue in its own right. Correct predictions let the machine maintain a deep pipeline of useful work; bad predictions cause the front end to fill itself with the wrong instructions and later flush them out, wasting cycles and bandwidth. Muratori uses this to show why seemingly small changes in branch structure can have large runtime effects, even when the actual arithmetic being performed stays the same.

### 17. Code Alignment

This video explores how the physical placement of instructions in memory affects performance. Because the front end fetches and decodes code in chunks, the boundaries where instructions land relative to cache lines, fetch windows, or decoder-friendly regions can change how efficiently the processor consumes the instruction stream. A tiny source-level change can therefore move hot code into a better or worse alignment and produce surprisingly large timing differences.

The broader lesson is that performance is sometimes sensitive to layout effects that are invisible at the algorithmic level. Two binaries with effectively identical logic can behave differently because the front end sees them differently. Muratori uses code alignment to reinforce the idea that once you are operating near hardware limits, details that look cosmetic from the source-code point of view can become performance-relevant facts of the machine.

### 18. The RAT and the Register File

This video introduces the rename machinery that lets modern CPUs track more in-flight register values than the architectural register names would suggest. Muratori explains the role of the RAT, which maps programmer-visible register names onto a larger pool of internal storage so the machine can break false dependencies and keep multiple versions of a logical register alive at once. This is one of the core tricks that makes out-of-order execution practical.

The payoff is that architectural registers are no longer the whole story. From the programmer's perspective, there may be only a small fixed set of register names, but the processor is internally managing a much richer set of physical values and renamings. That helps explain how modern CPUs can overlap instructions so aggressively and why some apparent dependencies are not real performance limits while others still are. The episode deepens the mental model of the back end just before scheduling and port limits are introduced.

### 19. Execution Ports and the Scheduler

This episode moves from rename and dependency handling to the hard structural limits of the back end. Once instructions are decoded, renamed, and have their dependencies satisfied, they still need available execution resources. Muratori focuses on the scheduler and execution ports, which determine how many operations of each kind can issue in parallel and where they can go. Integer arithmetic, loads, stores, branches, and vector operations often compete for different subsets of these resources.

That makes port pressure a concrete ceiling on performance. Even if a loop has abundant independent work, it still cannot exceed the number and type of operations the back end can retire per cycle. The lesson here is that modern CPU performance is a resource-allocation problem as much as a dependency problem: once the chains are short enough, the next question is whether the available ports and queues can sustain the desired mix of work.

### 20. Increasing Read Bandwidth with SIMD Instructions

This video returns to the earlier question of read throughput and applies SIMD as the next obvious lever once scalar instruction processing is no longer the main bottleneck. If the machine is already near its scalar issue limits, the only way to move more data per cycle is to make each instruction cover more bytes. SIMD loads and operations do exactly that by widening the amount of data handled at once.

Muratori frames this as a direct continuation of the Prologue's SIMD multiplier, but now with much more detailed microarchitectural context. The question is no longer just “can SIMD be faster?” but “under what back-end and bandwidth limits does SIMD actually increase observed read throughput?” That turns vectorization from a generic optimization slogan into a specific tool for attacking a clearly modeled bottleneck.

### 21. Cache Size and Bandwidth Testing

This video revisits the cache discussion from the Prologue with enough knowledge to reproduce and fully interpret the classic bandwidth graphs. By sweeping working-set sizes and measuring throughput, Muratori shows how the different cache levels reveal themselves empirically. Small working sets fit in the fastest levels and achieve high bandwidth; larger ones spill into lower cache levels and eventually main memory, where throughput drops and latency effects become more visible.

The significance of the episode is that it turns a conceptual hierarchy into something measurable. Cache levels stop being abstract boxes on a slide and become observable plateaus and cliffs in performance data. That gives viewers a practical way to estimate cache capacities and bandwidth limits on their own machines and connect those measurements back to the memory-bound behavior of real code.

### 22. Non-Power-of-Two Cache Size Testing

After measuring cache behavior with power-of-two working sets, this video asks how to test cache capacities that do not line up neatly with those boundaries. Real caches are not always exact powers of two in the simple way beginner models imply, and even when their nominal sizes are, replacement behavior, associativity, and set mapping can make naive tests misleading. Muratori therefore pushes the measurement methodology forward so that the inferred capacity reflects what the machine actually does rather than what the test accidentally assumes.

This is an important refinement of the experimental mindset developed throughout the course. Once you start measuring hardware seriously, the first simple test often gives you a useful picture, but not always the complete one. Non-power-of-two cache testing shows how to improve a benchmark when the first version is too coarse, and how to keep adjusting the experiment until the performance model matches the real machine more closely.

### 23. Latency and Throughput, Again

This video revisits latency and throughput now that caches, execution units, and memory systems are all in play at once. Muratori emphasizes that the core ideas have not changed since the laundry-machine analogy: latency is still the time required for one operation to complete, and throughput is still the rate at which completed operations emerge from a sufficiently full pipeline. What changes is that those ideas now have to be applied across multiple parts of the machine at once, and that can make it easy to confuse cache behavior with execution behavior if you are not careful.

The point of the episode is to stabilize the mental model before the course goes deeper. Whether the bottleneck is a dependency chain through arithmetic operations or a chain of cache misses through the memory hierarchy, the same conceptual vocabulary still applies. By re-grounding those ideas in the current hardware context, Muratori makes it easier to reason consistently about seemingly different sources of delay without inventing a new theory for each subsystem.

### 24. Unaligned Load Penalties

This video examines what happens when loads do not begin on memory addresses that are naturally aligned to the size of the data being read. Modern CPUs handle unaligned loads far better than older processors did, which means many programmers can go years without noticing a penalty. But Muratori shows that alignment can still matter in specific cases, especially when a load spans boundaries that are inconvenient for the cache hierarchy or load hardware.

The larger lesson is that hardware improvements do not make layout concerns disappear; they merely move the threshold at which those concerns become visible. Once a program is pushing for high bandwidth, details like whether a vector load crosses a line boundary can still show up in the measurements. This episode therefore sharpens the view of how memory organization constrains the idealized bandwidth numbers from the previous experiments.

### 25. Cache Sets and Indexing

This video explains why caches can behave pathologically even when the total working set seems small enough to fit. Hardware caches are not free-form associative lookup structures; they must meet strict timing and area constraints, so they are organized into sets and indexed by specific address bits. That means two memory regions that are far apart in the virtual address space can still compete for exactly the same subset of cache entries if those index bits line up badly.

Muratori uses this to show that capacity alone is not the whole story. A program can experience cache-thrashing behavior long before it exhausts the nominal cache size if its access pattern keeps mapping different addresses onto the same sets. This is one of the most important moments in Part 3 because it turns cache analysis from a simple “does it fit?” question into a more realistic “how is it distributed across the cache’s internal structure?” question.

### 26. Non-temporal Stores

This episode looks at a deliberately counterintuitive technique: sometimes the best way to use the cache hierarchy is to bypass it. Non-temporal stores are designed for write streams where the program does not expect to reuse the written data soon. Instead of filling cache lines with values that will only evict more useful data, the CPU can write them in a way that minimizes pollution of the cache hierarchy.

The value of the technique depends heavily on context. If the written data will be read again immediately, bypassing cache can hurt. But for one-way streaming output, Muratori shows why a store that avoids dragging cold results through the cache can improve overall throughput. The video reinforces a recurring theme of the course: the right optimization often depends on whether the data is part of a reuse-heavy working set or just transient traffic passing through the machine.

### 27. Prefetching

This video introduces explicit prefetching as a way to tell the CPU about future memory needs before the loads themselves occur. In principle, prefetching can hide memory latency by causing lines to begin their trip through the hierarchy early enough that they are ready by the time computation needs them. Muratori also notes, however, that modern hardware prefetchers are already quite capable in many common cases, which means the set of situations where manual prefetching is truly necessary is shrinking.

That makes the topic more nuanced than it first appears. Prefetch instructions are not a universal speed button; they are a hint whose value depends on access patterns, reuse distance, and whether the machine can already infer the same pattern automatically. The episode positions prefetching as a specialized tool for pathological or unusually demanding loops, not as a blanket recommendation for performance work in general.

### 28. Prefetching Wrap-up

This video consolidates the prefetching experiments and uses them to set up the final homework stretch of Part 3. Muratori pulls together the various cases where prefetching helped, where it failed to help, and where the graph of results became complicated enough that raw measurement was no longer self-explanatory. That is an important transition: by this point in the course, the experiments are producing curves that need interpretation, not just simple before-and-after numbers.

The wrap-up also underscores the limited but real role of explicit prefetching. It is neither obsolete nor broadly magical. It belongs in the toolbox for specific workloads, particularly those where the access pattern is known in advance but the hardware prefetchers do not quite lock on. The episode leaves the viewer with a more realistic sense of when prefetching deserves investigation and when attention is better spent elsewhere.

### 29. A Closer Look at the Prefetching Performance Graph

This bonus video expands one of the prefetching questions into a full walkthrough of graph interpretation. Rather than introducing a new optimization mechanism, Muratori uses the unusual curve from the earlier experiment as an opportunity to model how a performance engineer should think when the measured results are not immediately obvious. The exercise is about turning a graph from “interesting shape” into an explanation grounded in the machine’s behavior.

That makes the video especially valuable as methodology. It shows that performance graphs are not merely outputs to admire; they are compressed evidence about multiple competing mechanisms interacting at once. Walking through the curve in detail gives viewers practice in extracting hypotheses from a graph, checking those hypotheses against the hardware model built up so far, and refining the explanation until the shape starts to make sense.

### 30. 2x Faster File Reads

This video returns to the practical haversine workload and cashes in the lessons from the memory-system deep dive. Muratori shows that what first looked like a largely fixed cost of reading from the OS filesystem cache can in fact be improved substantially once page tables, mapping behavior, and cache effects are understood. The read path is no longer treated as a monolithic OS service but as a sequence of concrete costs, some of which can be avoided or reduced.

The significance of the episode is that it reconnects Part 3’s hardware analysis to visible application-level wins. Understanding the hierarchy and its failure modes is useful precisely because it gives you leverage over real workloads, not just synthetic tests. Achieving something like a 2x gain on file reads after so much groundwork makes the course’s style of detailed systems reasoning feel justified and practical.

### 31. Overlapping File Reads with Computation

This episode applies the earlier latency-and-throughput framework to I/O. Even when file reads cannot be made much faster on their own, the program can often be structured so that the read latency overlaps with useful computation. Muratori treats this as the systems equivalent of the washer-dryer pipeline: if one stage of work spends time waiting on data, another stage can often consume data that has already arrived.

The important shift is from optimizing a component in isolation to optimizing the whole processing pipeline. A file-read stage and a computation stage may each have fixed costs, but the user-visible runtime depends on how well they are overlapped. This video therefore extends the course’s throughput reasoning beyond CPU internals and into application structure, showing that the same ideas scale upward into software architecture decisions.

### 32. Testing Memory-Mapped Files

This final video of Part 3 closes the loop on the earlier discussion of memory-mapped files by benchmarking them directly against buffered reads in the same test framework. Rather than speculating about whether mapped I/O ought to be faster, Muratori adds a reference implementation to the read-and-sum benchmark so the two approaches can be compared concretely. That is consistent with the course’s general discipline: every interesting systems question eventually gets turned into an experiment.

As the closing entry for the part, the episode also acts as a summary of the larger lesson. By the end of Part 3, file reads, page faults, cache behavior, prefetch effects, and overlap strategies are all part of one coherent performance story. Testing memory-mapped files against buffered I/O is therefore not just a side question; it is a final demonstration that the programmer can now reason across the whole path from storage and virtual memory to cache hierarchy and CPU execution.

## Part 4: Polynomial Evaluation (Optional)

Part 4 is designed as an optional deep dive into how to replace standard library math functions with hand-optimized implementations. Rather than leaving the haversine distance calculation to vendor-supplied `sin`, `cos`, `sqrt`, and `asin` functions, the course explores the theory and practice of function approximation. This prepares viewers for Part 5, where a completely custom haversine implementation can be analyzed at full granularity. Those focused purely on computational optimization can skip directly to Part 5; Part 4 is included for viewers interested in how math libraries actually work and how approximation fits into the performance picture.

### 1. Reference Haversine Code

This opening video for Part 4 establishes a baseline reference haversine implementation that uses only standard library functions and provides a clear starting point for the approximation work ahead. Muratori reverse-engineers what the haversine algorithm needs: input latitude/longitude pairs, trigonometric functions (sin, cos, asin), and square root. Rather than hand-rolling these, the reference code uses the C standard library versions. The purpose of establishing this baseline is to create a known-correct result against which all subsequent approximations can be validated.

The video also clarifies the scope of Part 4: the focus is on replacing math library calls with faster approximations, not on JSON parsing or file I/O. The reference implementation isolates just the expensive mathematical operations so viewers can focus entirely on function approximation. This modular approach makes it clear which layer of the system is being optimized and helps avoid conflating parsing efficiency with computation efficiency.

### 2. Identifying Non-inlined Math Functions

This video uses assembly-reading techniques from Part 1 to examine where the reference implementation calls external math functions and where function calls could be eliminated through inlining. Many modern C compilers can inline simple operations, but transcendental functions like sin and cos are expensive and often remain as explicit calls that cross function-call boundaries. Muratori demonstrates how to inspect generated assembly to confirm that expensive calls actually occur and to identify how frequently they are invoked.

The key insight is that function-call overhead is often invisible at the source-code level but becomes obvious once assembly is analyzed. Even before pursuing any approximation strategy, understanding the current call graph and where the expensive calls happen is essential. This video reinforces the Part 1 skill of using assembly inspection not just for curiosity but for guiding optimization decisions about which functions are worth approximating.

### 3. Determining Input Ranges

Before building approximations, it is critical to understand what range of inputs the functions will actually receive. An approximation that works perfectly for `sin(x)` where `x âˆˆ [-Ï€, Ï€]` might fail catastrophically if `x` suddenly includes arbitrarily large values. Muratori walks through analyzing the haversine algorithm to determine what input ranges the sin, cos, asin, and sqrt functions will see in practice. This analysis constrains the approximation problem to a manageable domain rather than requiring solutions that work across the full input range of mathematical functions.

The broader lesson is that careful problem analysis often reduces scope dramatically. Instead of building a general-purpose sin approximation that works for any IEEE 754 double value, the task becomes much simpler when you know the actual inputs will be latitude and longitude calculations, which have inherent constraints. This kind of requirement analysis prevents over-engineered solutions and focuses approximation effort where it will actually matter in the target application.

### 4. Introduction to SSE Intrinsics

This video introduces SSE intrinsics as the tool for controlling SIMD operations from C/C++ code. While Part 3 touched on SIMD from a theoretical perspective, this episode shows how to actually generate vector instructions using compiler intrinsics like `_mm_add_ps` and `_mm_mul_ps`. Muratori explains that intrinsics are essentially function-call-like wrappers that map directly to SIMD instructions, giving programmers control over vectorization without requiring inline assembly.

The importance of intrinsics is that they bridge the gap between high-level C code and specific CPU vector operations. When building polynomial approximations that will process multiple values simultaneously, intrinsics make it possible to maintain reasonable code readability while still getting explicit control over instruction choice. This video establishes the mechanics of SIMD programming that Part 5 will rely on extensively.

### 5. Function Approximation

This is the core theoretical video explaining how to build polynomial approximations to transcendental functions. Muratori explains the basic approach: given a function f(x) and a limited input range, construct a polynomial p(x) that closely matches f(x) values over that range. He discusses why polynomials are useful (they are cheap to evaluateâ€”just multiplication and addition), how to choose polynomial degree based on required accuracy, and how tools like Remez exchange algorithm can find optimal coefficients.

The video makes clear that the challenge is not deriving mathematical theory from first principles but understanding the tradeoff between polynomial degree (more terms = better accuracy = more computation) and the available accuracy requirements. For haversine, going from a linear approximation to quadratic or cubic often provides enormous accuracy improvement per additional term, which makes moderate-degree polynomials practical. The episode establishes the theoretical foundation that the following videos will apply in specific contexts.

### 6. Range Reduction

This video addresses a fundamental challenge in approximation: while a polynomial can approximate a function over a limited interval like `[0, Ï€]`, math function inputs often exceed that range. Rather than requiring a separate polynomial for every possible range of x-values, range reduction techniques exploit symmetries in the function to map arbitrary inputs back into a tractable interval. Muratori explains how to use properties like `sin(x + 2Ï€) = sin(x)` or `cos(Ï€ - x) = -cos(x)` to reduce any x-value to one that falls within the approximation's range of validity.

The elegant outcome is that approximating one interval plus clever range reduction can effectively approximate the entire function. This makes the polynomial approximation approach much more general and practical than naive quadratic fitting would appear. Range reduction is as important as the approximation itself; without it, the whole approach becomes a collection of local approximations with no way to handle real-world inputs that may be far outside the designed interval.

### 7. Approximation Using Higher-Power Polynomials

After establishing basic approximation theory and range reduction, this video experiments with higher-degree polynomials to achieve better accuracy without additional range-reduction work. Sometimes the right tradeoff is not to optimize the reduction phase but to accept a higher-degree polynomial that achieves better accuracy per evaluation. Muratori compares quadratic and cubic and higher-degree approximations side by side, showing how error decreases as polynomial degree increases.

The practical question becomes: how much accuracy is actually needed for haversine distance calculations? If the application only cares about distances to a few decimal places, a relatively low-degree polynomial suffices and evaluation remains fast. If higher precision is required, the CPU may need to evaluate more terms, but the added cost might still be worthwhile compared to calling a library function. This video shifts the perspective from theoretical correctness to practical engineering of the accuracy-cost tradeoff.

### 8. Horner's Rule

This video presents a critical optimization for polynomial evaluation: Horner's rule reformulates a polynomial from the naive form (evaluate each term separately, then sum them) into a nested form that reduces the number of multiplications required. For a polynomial like `aâ‚€ + aâ‚x + aâ‚‚xÂ² + aâ‚ƒxÂ³`, the naive approach computes xÂ¹, xÂ², xÂ³ separately. Horner's rule instead computes `aâ‚€ + x(aâ‚ + x(aâ‚‚ + x(aâ‚ƒ)))`, which reuses computed powers and requires fewer operations.

Beyond the operation count reduction, Horner's form has superior numerical stability properties. Small rounding errors in intermediate results are less likely to accumulate catastrophically because the final result depends less sensitively on early approximations. Muratori emphasizes that Horner's rule is a standard technique in numerical computing and essential for building high-quality polynomial evaluators. Using it is not optional if you want approximations that remain accurate through accumulated rounding.

### 9. Fused Multiply-Add

This video examines how modern CPUs optimize the `multiply then add` pattern that dominates Horner evaluation through fused multiply-add (FMA) instructions. An FMA instruction computes `(a * b) + c` in a single CPU operation, with full precision on the intermediate result before final rounding. Rather than the traditional multiply-add sequence which rounds after multiplication, then rounds again after addition, FMA eliminates one rounding step and improves accuracy. Muratori explains that both scalar and SIMD variants of FMA exist, and using them dramatically improves both speed and accuracy of polynomial evaluation.

FMA is therefore not just a performance trick but a correctness requirement for production approximations. CPUs have invested heavily in FMA support because it is so central to numerical computing, scientific computation, and high-quality polynomial evaluation. For the haversine problem, using FMA in the polynomial core makes the approximation competitive with library implementations.

### 10. Coefficient Arrays for Polynomial Evaluation

Now that Horner's rule and FMA are understood, this video shows how to structure the data and code so that polynomial evaluation becomes simple and reusable. Rather than hardcoding a specific polynomial with its coefficients directly in the code, the video demonstrates how to represent polynomials as arrays of coefficients and write a generic evaluation loop that works for any polynomial. This separation of data (coefficients) from algorithm (Horner's rule + FMA) makes it easy to experiment with different polynomial degrees or coefficients without rewriting the core evaluator.

The practical benefit is reusability: once a well-optimized generic polynomial evaluator is written, approximating a new function simply requires computing new coefficients and passing them to the same evaluation code. This transforms function approximation from an ad-hoc one-off effort into a systematic, repeatable process. It also makes it easier to compare different approximations side by side by just swapping coefficient arrays.

### 11. Approximating Arcsine

After establishing general approximation techniques on simpler functions like sine, this video applies those same techniques to arcsine, which is mathematically more complex. The key insight is that all of the previously learned techniquesâ€”polynomial approximation, range reduction, Horner's rule, FMAâ€”apply equally to arcsine as they did to sine. The mathematics might be different, but the engineering approach is identical. Muratori shows that once you have built one function approximator, building others follows the same recipe with adjusted coefficients and range considerations.

This video reinforces a valuable lesson: pattern recognition and reusable process matter as much as mathematical fluency. A practitioner who understands approximation methodology can tackle unfamiliar functions by applying proven techniques rather than starting from scratch each time. The episode also highlights that arcsine has different domain and range constraints than sine, which means range reduction and approximation intervals need adjustment, but nothing is fundamentally new.

### 12. Extending Arcsine to the Full Input Range

Finally, this video addresses the full input domain for arcsine by combining the basic approximation with trigonometric identities that map inputs outside the primary approximation range. For arcsine whose natural domain is `[-1, 1]` and yet must handle all Euclidean geometry needs up to that boundary carefully, extending beyond isn't needed. But the video demonstrates careful identity-based reduction strategies. Muratori uses this finale to show how to combine all the pieces: approximation core, range reduction, and sometimes trigonometric identities, into a complete implementation that appears to work across the whole space the function might be called with.

This closes Part 4's arc from theory through polynomial approximation, optimization techniques, and practical implementation of a mathematical function library. The viewer now understands not just how to use library functions but how those functions are typically implemented internally. Importantly, all of this understanding is now deployable in Part 5, where vectors of these operations combined with CPU execution analysis will produce the final optimized haversine implementation.

## Part 5: Computation

Part 5 extends the optimization techniques from Part 3 (memory operations) into pure computation. Where Part 3 focused on moving data efficiently, Part 5 focuses on processing that data with minimal waste and maximum CPU throughput. The context is the now-fully-hand-optimized haversine implementation built with math approximations from Part 4, allowing full control and visibility into the actual computation happening inside the hot loop.

### 1. Our Very Own Haversine

This opening video for Part 5 assembles all the work from previous parts into a haversine distance function that uses only hand-coded math approximationsâ€”no library sin, cos, sqrt, or asin calls. By constructing this fully custom implementation, the course reaches a crucial inflection point: the code is now simple enough and predictable enough that its full performance can be understood from first principles and assembly inspection. All the mathematics and systems knowledge from Parts 1-4 now applies directly to a single function that performs a meaningful real-world calculation.

The significance is that this custom haversine becomes both a benchmark target and a learning tool. Its approximate runtime can be estimated from CPU architecture knowledge; its actual runtime can be measured; the gap between estimates and measurements becomes debugging information. If the function runs slower than the model predicts, that gap narrows down the culprit (cache misses? poor branch prediction? scheduling stalls?). If it runs faster, that's also informative (perhaps the CPU prefetcher is doing better than expected). This custom implementation is therefore the test harness that validates everything else learned in the course.

### 2. Removing Waste

With the custom haversine in place, this video asks the Part 1 question again from a completely different angle: "How many instructions are actually necessary?" Using profiling and inspection, Muratori identifies unnecessary calculations, redundant work, and operations whose results are never used. Removing that waste directly translates to faster execution. Unlike the earlier waste identification (which often involved changing algorithms), this waste removal works within the fixed haversine mathematics and focuses on algorithmic efficiency: fewer variables, fewer intermediate calculations, tighter loop structure.

The key insight is that explicit waste in tight computational kernels is often surprisingly easy to spot once you look at the actual instruction sequence. What appeared like clean, straightforward code at the source level might include setup, bookkeeping, or dead stores that the compiler did not eliminate. Hand-optimization of the haversine loop demonstrates that even after the compiler has done its best, there is usually latitude for further improvement once you are willing to read and reason about assembly directly.

### 3. Simplified Haversine Candidate

This video refines the previous waste-removal work into a candidate implementation that represents a significant simplification from the initial reference code. By removing unnecessary copies, avoiding redundant calculations, and tightening the loop structure, a dramatically simpler version emerges that produces identical results with less work. Muratori measures the improvement explicitly, showing that removing algorithmic waste produces measurable speedups even before addressing CPU-level issues like pipelining or cache behavior.

The importance is demonstrating that waste removal is often the highest-leverage optimization available. Before worrying about CPU microarchitecture details, ensure the algorithm itself is not making unnecessary work. Once that is addressed, then the harder optimizations like memory layout or branch prediction become worth pursuing.

### 4. Selectively Preventing Optimizations

This video addresses a subtle but important issue in performance measurement: sometimes the compiler optimizes away the exact code you wanted to benchmark. If an intermediate computation is never used, the compiler may eliminate it entirely. While that is good for real programs, it is disastrous during microbenchmarking because the measured speedup no longer reflects the actual cost of the computation being studied. Muratori shows how to use volatile and similar techniques to prevent over-aggressive elimination while still allowing the compiler to optimize what genuinely matters.

This is a meta-level lesson in experimental methodology. Once you start measuring code in isolation, you must be careful that the isolation does not change what is being measured. Compiler optimizations can invalidate a benchmark in subtle ways. Understanding how to preserve the code you care about while allowing reasonable compilation is as important as understanding the CPU's behavior. This video therefore extends the course's already-sophisticated measurement discipline even further.

### 5. Reading CPU Diagrams

This video teaches a practical skill often left implicit in performance courses: how to read the CPU block diagrams that semiconductor manufacturers publish. These diagrams are dense with information: how many execution units exist, what operations they can perform, how data flows between them, where bottlenecks likely occur. Muratori walks through interpreting such a diagram using knowledge of front-end, back-end, cache, and memory subsystems accumulated throughout the course. The goal is to build an intuition for which design features of the CPU will likely constrain your workload based on its characteristics.

The significance is that these diagrams are the bridge between abstract performance concepts studied in Parts 1-3 and the actual resources available in your machine. Once you can read them fluently, you can ballpark computational limits without requiring detailed simulation or specialized tools. The video also reinforces that most hardware information is publicly available; you need only the capability to interpret it.

### 6. Better Dead Code Elimination - Or Is It?

This video revisits the technique for preventing unwanted compiler optimization, but with a subtle catch: the most straightforward approach can have surprising side effects. Muratori shows that preventing dead-code elimination in one way might interfere with other compiler optimizations or introduce unexpected instructions into the hot loop. Finding the right balance between preserving the code you want to measure and allowing beneficial optimizations requires careful experimentation.

The broader lesson is that compiler interplay is complex and sometimes counterintuitive. What looks like the simplest fix may have side effects that poison the measurement in different ways. The video emphasizes the importance of always verifying that the benchmark actually measures what you think it measures by inspecting the generated assembly. Another recurring theme: trust your tools, but verify.

### 7. Our Nemesis Returns

This tantalizing video warns of a sophisticated performance problem that will be addressed directly in the next installment but does not yet reveal its solution. Muratori hints that the optimized haversine, despite careful waste removal and thoughtful compiler interactions, still exhibits unexpected performance characteristics that defy easy explanation. The episode positions this as an unsolved mystery that Part 5 has been preparing to address with all the accumulated knowledge of CPU internals, dependency chains, and instruction-level parallelism.

The cliffhanger serves a pedagogical purpose: it motivates investment in the complex material that follows. Performance problems that resist simple explanations usually point toward something interesting about how CPUs actually execute code. This video teases that there is indeed something fascinating to discover about the seemingly-simple haversine loop.

### 8. Dead Code Elimination Prevention Macros

Once compiler optimization side effects are well understood, this video solidifies them into a reusable macro-based approach. Rather than ad-hoc volatile declarations or inline assembly, Muratori defines portable, reliable macros that consistently prevent dead-code elimination without introducing unnecessary code. These macros become part of the benchmark infrastructure and part of the course's distributed code repositories. The practical benefit is that future performance work building on this foundation can use the same proven prevention technique.

The pattern here is turning an ad-hoc solution into a robust tool, which reflects good software engineering at any level. Once you have solved a problem well, codify the solution so it is available for reuse and spreads throughout the codebase. This applies to performance work just as it does to any other engineering discipline.

### 9. Dependency Chain Stalls

This video directly tackles the mystery from "Our Nemesis Returns" by analyzing the CPU's view of dependency chains in the haversine computation. Even with waste removed and compiler optimizations handled, the loop contains data dependencies that prevent full CPU parallelism. Muratori walks through the instruction sequence, identifies which operations must wait for previous results, and quantifies how much of the CPU's potential can actually be exploited given those constraints. The analysis shows that the CPU cannot maintain full utilization through the computation without additional parallelism from unrolling, SIMD, or multithreading.

The significance is that this video completes the course's conceptual arc. Viewers have learned assembly language, profiling, memory systems, caches, CPU front-end and back-end details, and optimization techniques. Now all those pieces come together in analyzing a realistic workload and understanding precisely why it runs at its observed speed on the target CPU. The dependency chain analysis is not theoretical; it directly explains the discrepancy between observed performance and naive hopes. This is systems programming at its finest: building a complete model of a complex system and using it to explain concrete behavior.

## ETW Challenge Series: Halloween Spooktacular Challenge (17 videos + 1 solution)

### ETW Video 1. Announcing the 2024 Halloween Spooktacular Challenge

Casey Muratori introduces the 2024 Halloween Spooktacular Challenge, a 15-day event where programmers must implement their own PMC (Performance Monitoring Counter) collection library using only the Windows API on a standard installation. The challenge centers on the terrifying Event Tracing for Windows (ETW) API, which is notoriously complex and undocumented. Rather than using command-line tools like Windows Performance Toolkit (WPT), participants must build a thread-safe, continuous PMC collection system that mimics the simplicity of RDTSC bracketing for arbitrary code regions. Muratori explains the historical frustration: while Linux makes PMC access relatively straightforward, Windows deliberately obscures it behind ETW, which has been described as one of the worst APIs ever designed. The video establishes the three-part challenge requirement: PMC selection, collection within code brackets (asynchronous), and result retrieval. Daily hints will escalate in helpfulness, with the first half covering basic ETW PMC collection and the second half addressing novel techniques never before published.

### ETW Video 2. Halloween Spooktacular Day 1: The Challenge

Muratori provides the formal challenge specification requiring contestants to implement an API supporting three core features. First, PMC selection: users must register which PMCs to collect using CPU-specific names (AMD vs Intel variants) and map them to kernel indices. Second, bracketed collection: users call StartCountingPMCs/StopCountingPMCs around code sections, with results returned asynchronously via pmc_traced_thread structures (due to ETW's inherent asynchronicity). Third, result retrieval with IsComplete/GetResult functions allowing polling or blocking with WaitForResult. The no-cheating clause prohibits custom kernel drivers or third-party monitoring tools; code must run on vanilla Windows 10/11 in Administrator mode. However, documentation, Stack Overflow posts, and GitHub code are fair gameâ€”contestants are expected to scavenge resources rather than figure everything out blind. The challenge emphasizes that standard knowledge of ETW is insufficient; the novel aspects involve continuous real-time PMC collection from user-space applications, something Muratori claims he'd never seen published before attempting it himself.

### ETW Video 3. Halloween Spooktacular Day 2: Reboot Your Machine

Day 2 hint addresses a brutal practical issue: other programs may already be collecting PMCs, and since the CPU can only sample a limited subset simultaneously, ETW prevents PMC collection via TraceSetInformation if another trace holds the resource. The solution is rebooting immediately after login before system services reactivate their own PMC traces. Bonus hint: if wpr -pmcsources lists only "Timer", virtualization is enabled and must be disabled in BIOS or Windows Features to allow PMC collection. Muratori candidly admits he didn't discover these blockers until after getting stuck himself, forcing him to compare periodic system trace listings to identify rogue processes. This hint represents raw survival advice rather than algorithmic insight, highlighting how ETW's design deliberately frustrates standard expectations about CPU access.

### ETW Video 4. Halloween Spooktacular Day 3: Trace in Real-Time Mode

Day 3 covers real-time event processing: contestants must call OpenTraceW with ProcessTraceMode flags including PROCESS_TRACE_MODE_EVENT_RECORD, PROCESS_TRACE_MODE_RAW_TIMESTAMP, and PROCESS_TRACE_MODE_REAL_TIME. Without these flags, ETW writes events to logfiles rather than streaming them to the reader thread in real timeâ€”a critical distinction for continuous profiling. The hint is minimal but essential: proper flag combination ensures EVENT_RECORD structures arrive promptly after generation, allowing the collection thread to maintain running PMC samples without waiting for post-run file parsing. This architectural patternâ€”using real-time mode rather than offline loggingâ€”enables the low-latency collection model required by the challenge.

### ETW Video 5. Halloween Spooktacular Day 4: Use TraceQueryInformation

Day 4 hints at infrastructure: before ETW will accept PMC collection requests, you must discover PMC index-to-name mappings. Microsoft doesn't expose a built-in API for this, so contestants must retrieve the mapping manually using TraceQueryInformation with TraceProfileSourceListInfo. Two calls are needed: first to query buffer size, then to retrieve PROFILE_SOURCE_INFO structures. These structures contain the PMC names and their corresponding kernel indices, enabling translation from user-friendly names (like "L3CacheMisses") to the numeric indices ETW requires. Without this mapping, TraceSetInformation calls will fail silently or return cryptic errors. This hint essentially reveals that data normalization is the first real ETW hurdle.

### ETW Video 6. Halloween Spooktacular Day 5: Call TraceSetInformation Twice

Day 5 reveals a critical dual-API pattern: contestants must call TraceSetInformation twiceâ€”once with TracePmcCounterListInfo to specify which PMCs to collect, and again with TracePmcEventListInfo to specify which kernel event types should include PMC data. This two-phase configuration is deeply unintuitive because PMCs are meaningless without events to attach them to, and events are meaningless without PMCs. The CLASSIC_EVENT_ID array for TracePmcEventListInfo requires trial-and-error discovery, as Microsoft doesn't clearly document which event IDs support PMC collection. This hint forces contestants to confront ETW's intentionality: the API isn't just complicatedâ€”it's actively hostile to discoverable design patterns.

### ETW Video 7. Halloween Spooktacular Day 6: PMCs Only Work for A Subset of Event Types

Day 6 explains that only specific kernel event types support PMC collection, but Microsoft never published the list. Muratori resorted to "wardialing" the APIâ€”programmatically testing event type combinations to find working pairs. The hint directs contestants to MSDN kernel event documentation (Thread_V2, Process_V2, Registry_V1, etc.) where tables list event GUIDs and opcodes. Contestants must also remember to set EVENT_TRACE_FLAG_SYSTEMCALL and other flags in EVENT_TRACE_PROPERTIES_V2.EnableFlags to enable kernel events. Discovery requires systematic trial-and-error testing, but the MSDN pages provide the necessary reference material. This meta-hint is about learning to mine documentation rather than memorizing specific answers.

### ETW Video 8. Halloween Spooktacular Day 7: Look for PMCs in the ExtendedData

Day 7 guides extraction of PMC data from received events: when an EVENT_RECORD arrives, iterate its ExtendedData array searching for ExtType == EVENT_HEADER_EXT_TYPE_PMC_COUNTERS. Once found, cast the DataPtr to EVENT_EXTENDED_ITEM_PMC_COUNTERS, which contains a simple array of 64-bit counter values. This seems simple once explained, but discovering the pattern requires inspecting undocumented structures and understanding ETW's event composition model. The counter order in the array matches the order specified during configuration, enabling proper interpretation of results. This hint represents the threshold between "acquiring data" and "parsing that data correctly."

### ETW Video 9. Halloween Spooktacular Day 8: MÄrtiÅ†Å¡ MoÅ¾eiko's Miniperf

Day 8 provides the "giant hint": sharing Miniperf (https://gist.github.com/mmozeiko/bd5923bcd9d20b5b9946691932ec95fa?ts=4), a working PMC collection library by MÄrtiÅ†Å¡ MoÅ¾eiko that Muratori credits as the catalyst for his own understanding. Miniperf demonstrates real-time PMC collection via ETWâ€”something Muratori hadn't even thought possible before seeing it. While Miniperf doesn't implement the full challenge requirements (it collects for entire thread runs using CSwitch events), it provides crucial insight into ETW API plumbing. Muratori acknowledges he'd never have completed the challenge without Miniperf's published source. This hint is a turning point: contestants now have a working foundation rather than a conceptual roadmap.

### ETW Video 10. Halloween Spooktacular Day 9: What's Left?

Day 9 acknowledges that Miniperf-dependent collection (via CSwitch events) is insufficient for arbitrary bracketing. The challenge requires flexible start/stop bracketing around user code, not just thread-level collection. Forcing the OS to context-switch at both start and stop points would introduce massive variabilityâ€”disastrous for repetition testing. The hint: contestants must figure out how to force ETW to record EVENT_RECORDs with PMCs attached at specific user-code points without heavy performance penalties. This requires ingenuity beyond Miniperf's baseline, setting up the innovation arc for the remaining hints.

### ETW Video 11. Halloween Spooktacular Day 10: Use TraceEvent

Day 10 provides the path forward: use TraceEvent to inject custom events into the ETW stream at arbitrary program locations. By calling TraceEvent at start and stop points, contestants can force ETW to generate events with precise timing. While TraceEvent-generated events don't natively support PMCs (they're user events, not kernel events), the subsequent hints explain how to pair them with kernel events that do carry PMCs. This hint begins the bridge between user-space arbitrary bracketing and kernel-space PMC collection.

### ETW Video 12. Halloween Spooktacular Day 11: Define Your Own Event UserData

Day 11 teaches the technique for distinguishing between multiple concurrent profiling regions: use RegisterTraceGuids to create custom event GUIDs, then embed arbitrary UserData in the EVENT_TRACE_HEADER for each TraceEvent call. Since EVENT_TRACE_HEADER's Size field indicates total event size, contestants can append custom data structures after the header. This UserData contains context (like a region ID or pointer) allowing the collection thread to correlate events with profiling requests. The critical insight: custom events provide identity metadata that kernel events lack, enabling association between user code brackets and collected PMC data.

### ETW Video 13. Halloween Spooktacular Day 12: Find Another PMC Event Type

Day 12 addresses a blocker: TraceEvent-generated user events don't support PMC collectionâ€”only kernel events do. Contestants need to identify at least one kernel event type that (a) supports PMC collection and (b) can be reliably forced to occur at desired profiling points. This requires combining the kernel event discovery learnings with the TraceEvent injection pattern, setting up Day 13's revelation about SysCallExit.

### ETW Video 14. Halloween Spooktacular Day 13: Use SysCallExit to Mark Start Points

Day 13 reveals the innovation: SysCallExit events are kernel events that support PMC collection AND are guaranteed to fire immediately after TraceEvent returns (since TraceEvent is itself a system call). By calling TraceEvent at profiling start points, contestants can arrange for a SysCallExit event to immediately follow, carrying PMC data. The EVENT_TRACE_FLAG_SYSTEMCALL flag enables SysCallExit collection, and the appropriate MSDN page (perfinfo) provides the GUID and opcode. Collection threads can now watch for custom TraceEvent events, then harvest PMC data from the immediately-following SysCallExit, using UserData to identify which profiling region the pair corresponds to. This asynchronous pairing pattern is the core innovation never before published.

### ETW Video 15. Halloween Spooktacular Day 14: Use SysCallEnter to Mark Stop Points

Day 14 refines the stop-point accuracy: SysCallExit fires AFTER TraceEvent completes, so using it for stop points includes TraceEvent's overhead in the profiling region. Better practice: use SysCallEnter events for stop points (which fire BEFORE TraceEvent executes), improving measurement accuracy by excluding TraceEvent's own execution. This requires adding one more GUID/opcode pair to the event configuration array. Start/stop asymmetry (SysCallExit for starts, SysCallEnter for stops) minimizes measurement artifacts while exploiting ETW's guaranteed event ordering around system calls.

### ETW Video 16. Halloween Spooktacular Day 15: Use GetEventProcessorIndex

Day 15's final hint addresses multi-threaded collection: use GetEventProcessorIndex on each received EVENT_RECORD to determine which CPU core generated it. Since cores are numbered 0 to N-1, contestants can maintain per-core tracking structures for active profiling regions. This enables correct PMC accumulation when multiple threads simultaneously profile regions on different coresâ€”a requirement for production profiling libraries. The hint elegantly maps ETW's fundamentally distributed event stream onto a simple array indexing scheme.

### ETW Video 17. Real-time PMCs on Windows with ETW

The post-Halloween solution video (NOV 02, 2024) walks through Muratori's complete working implementation of thread-safe, real-time PMC collection satisfying all three challenge requirements. Muratori acknowledges trade-offs and drawbacks: the necessity of rebooting, sensitivity to other tracing software, complexity that borders on unmaintainable. The final implementation worksâ€”proving the challenge solvableâ€”but its contorted design raises questions about whether Microsoft ever intended this use case. The video represents both celebration of ingenuity and indictment of API design philosophy. Muratori publishes his solution code for reference, inviting challengers to compare approaches.

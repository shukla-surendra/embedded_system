# Assembly Programming — Complete Guide from Zero

## Table of Contents

1. [What is Assembly Language?](#1-what-is-assembly-language)
2. [How Computers Execute Code](#2-how-computers-execute-code)
3. [CISC vs RISC Architectures](#3-cisc-vs-risc-architectures)
4. [Registers, Memory, and the Stack](#4-registers-memory-and-the-stack)
5. [x86 Assembly (Intel/AMD)](#5-x86-assembly-intelamd)
6. [ARM Assembly (Cortex-M / ESP32 / STM32)](#6-arm-assembly-cortex-m--esp32--stm32)
7. [RISC-V Assembly](#7-risc-v-assembly)
8. [Comparison Table — x86 vs ARM vs RISC-V](#8-comparison-table--x86-vs-arm-vs-risc-v)
9. [Practical Patterns](#9-practical-patterns)
10. [Tools and How to Run Assembly](#10-tools-and-how-to-run-assembly)

---

## 1. What is Assembly Language?

Every program you write in C, Python, or Rust eventually becomes **machine code** — a sequence of binary numbers that the CPU understands directly.

```
High-level (C):     int x = 5 + 3;
Assembly:           MOV R0, #5
                    ADD R0, R0, #3
Machine code:       E3A00005 E2800003   (32-bit ARM binary)
```

Assembly language is **one step above machine code**. Instead of writing raw binary, you write human-readable mnemonics (short words like `MOV`, `ADD`, `JMP`) that map directly 1-to-1 to machine instructions.

A tool called an **assembler** converts assembly → machine code. There is no compiler optimization, no abstraction — what you write is exactly what the CPU executes.

### Why Learn Assembly?

- Understand what C/C++ compilers actually generate
- Write ultra-low-level hardware drivers (bare-metal embedded)
- Debug at the instruction level using a debugger
- Optimize performance-critical inner loops
- Understand security vulnerabilities (buffer overflows, shellcode)
- Read disassembled firmware

### Layers of Abstraction

```
Your C code
    ↓  (compiler: gcc, clang)
Assembly (.s file)
    ↓  (assembler: as, nasm)
Object file (.o) — machine code + relocation metadata
    ↓  (linker: ld)
Executable (binary that CPU runs)
```

---

## 2. How Computers Execute Code

### The CPU Fetch-Decode-Execute Cycle

Every CPU, no matter how complex, repeats this loop billions of times per second:

```
┌─────────────────────────────────────────────┐
│  1. FETCH   — Read next instruction from    │
│               memory at address in PC       │
│                                             │
│  2. DECODE  — Figure out what the           │
│               instruction means             │
│                                             │
│  3. EXECUTE — Perform the operation         │
│               (math, memory read/write,     │
│               branch, etc.)                 │
│                                             │
│  4. UPDATE  — Move PC to next instruction   │
└─────────────────────────────────────────────┘
```

### Key CPU Components

```
┌──────────────────────────────────────────────────────┐
│                        CPU                           │
│                                                      │
│  ┌──────────┐    ┌──────────┐    ┌────────────────┐  │
│  │ Registers│    │   ALU    │    │ Control Unit   │  │
│  │ (fast    │◄──►│(Arith.   │◄──►│(fetch/decode/  │  │
│  │ storage) │    │Logic     │    │ execute)       │  │
│  │          │    │Unit)     │    │                │  │
│  └──────────┘    └──────────┘    └────────────────┘  │
│                                                      │
│  ┌──────────────────────────────────────────────┐    │
│  │  Program Counter (PC) — holds address of     │    │
│  │  next instruction to execute                 │    │
│  └──────────────────────────────────────────────┘    │
│                                                      │
└──────────────────────────┬───────────────────────────┘
                           │ memory bus
                    ┌──────▼──────┐
                    │    RAM      │
                    │  (stores    │
                    │ code+data)  │
                    └─────────────┘
```

### Registers

Registers are the CPU's internal, ultra-fast storage. Think of them as named variables built directly into the silicon. There are only a handful (8 to 32 typically), but accessing them is **instantaneous** — no memory bus needed.

```
RAM access:      ~100 clock cycles
Cache (L1):      ~4 clock cycles
Register:        1 clock cycle (same cycle as the instruction)
```

---

## 3. CISC vs RISC Architectures

This is the most fundamental design split in CPU history, and it directly determines what assembly you write.

### CISC — Complex Instruction Set Computer

**Philosophy:** Give the CPU many powerful, complex instructions. One instruction can do a lot of work (read memory + do math + write back in a single instruction).

**Origin:** Intel x86 (1978), Motorola 68000. Designed when memory was expensive — fewer instructions meant smaller programs.

#### CISC Characteristics

| Property | Detail |
|---|---|
| Instruction count | Hundreds to thousands of different instructions |
| Instruction length | **Variable** — some instructions are 1 byte, others are 15 bytes |
| Instructions per cycle | Often more than 1 clock cycle per instruction |
| Memory access | Instructions can read/write memory directly |
| Registers | Fewer registers (x86-32 has only 8 general-purpose) |
| Hardware complexity | Decoder hardware is very complex |

#### CISC Example (x86)

```nasm
; x86 can do memory + arithmetic in ONE instruction
ADD [rbx + 4], rax       ; Read from memory, add rax, write back to memory
                          ; A RISC CPU would need 3 separate instructions for this

; Complex string operation — one instruction copies an entire block
REP MOVSB                ; Repeat: copy byte from [RSI] to [RDI], increment both
```

---

### RISC — Reduced Instruction Set Computer

**Philosophy:** Give the CPU a small set of simple, fast instructions. Every instruction does exactly one thing. Complex operations are built by combining simple ones.

**Origin:** Stanford MIPS (1981), Berkeley RISC (1982) — academic research projects that became ARM and MIPS.

#### RISC Characteristics

| Property | Detail |
|---|---|
| Instruction count | ~50–200 instructions |
| Instruction length | **Fixed** — always 4 bytes (32-bit) or 2/4 bytes (Thumb/ARM) |
| Instructions per cycle | Most complete in 1 cycle |
| Memory access | **Load/Store only** — math only works on registers, not memory |
| Registers | More registers (ARM: 16, RISC-V: 32) |
| Hardware complexity | Simple decoder, room for pipelining |

#### RISC Example (ARM)

```asm
; ARM cannot do ADD directly on memory — must load first
LDR R0, [R1, #4]     ; Load from memory into register R0
ADD R0, R0, R2       ; Add register R2 to R0 (result in R0)
STR R0, [R1, #4]     ; Store result back to memory
; Three instructions — but each is simple and fast (1 cycle each)
```

---

### CISC vs RISC Side-by-Side

```
Task: Add two numbers that are in RAM and store result back to RAM

CISC (x86):
    ADD [0x1004], [0x1000]    ; 1 instruction (hypothetical combined form)
    ; Modern x86 actually: ADD reg, [mem] only, but still complex

RISC (ARM):
    LDR R0, [R1]              ; Load first value
    LDR R2, [R3]              ; Load second value
    ADD R4, R0, R2            ; Add
    STR R4, [R5]              ; Store result
    ; 4 instructions — but each takes 1 cycle, hardware is simpler
```

### Which is "Better"?

Neither. They have different trade-offs:

| Use Case | Winner | Why |
|---|---|---|
| Desktop/Server PCs | CISC (x86) | Decades of software, maximum raw throughput |
| Smartphones / Tablets | RISC (ARM) | Power efficiency, heat |
| Microcontrollers (STM32, ESP32) | RISC (ARM Cortex-M) | Simple, cheap, low power |
| Academic / Open Hardware | RISC-V | Free, no licensing fees |
| Supercomputers | Both | Depends on workload |

> **Modern reality:** Modern x86 CPUs internally translate CISC instructions into RISC-like micro-ops before executing them. So Intel/AMD chips are CISC on the outside, RISC on the inside.

---

## 4. Registers, Memory, and the Stack

These concepts apply to ALL architectures. Understand them once, apply everywhere.

### 4.1 Registers

```
┌─────────────────────────────────────────────────────┐
│                   Register File                     │
│                                                     │
│   General Purpose:  R0, R1, R2 ... (do math/logic) │
│   Program Counter:  PC  (address of next instr.)   │
│   Stack Pointer:    SP  (top of stack address)     │
│   Link Register:    LR  (return address for calls) │
│   Status Register:  CPSR/FLAGS (condition codes)   │
└─────────────────────────────────────────────────────┘
```

**General Purpose Registers:** Hold any integer value (addresses, counters, data). You use these for all computation.

**Program Counter (PC):** The CPU automatically reads from this address for the next instruction. Changing PC = jumping to a different location in code.

**Stack Pointer (SP):** Points to the current top of the stack in RAM. Push/pop operations automatically adjust this.

**Flags / Status Register:** After an arithmetic instruction, the CPU sets condition bits:
- `Z` (Zero): result was zero
- `N` (Negative): result was negative
- `C` (Carry): unsigned overflow occurred
- `V` (Overflow): signed overflow occurred

These flags are what make conditional jumps work.

### 4.2 Memory Layout

```
High address  ┌──────────────────┐
              │      Stack       │  ← grows downward
              │        ↓        │     (local variables, return addresses)
              │                  │
              │     (free)       │
              │                  │
              │        ↑        │
              │      Heap        │  ← grows upward (malloc/new)
              ├──────────────────┤
              │    BSS (.bss)    │  ← uninitialized globals (zeroed)
              ├──────────────────┤
              │    Data (.data)  │  ← initialized globals
              ├──────────────────┤
              │    Text (.text)  │  ← your code (instructions)
Low address   └──────────────────┘  ← address 0x00000000
```

### 4.3 The Stack

The stack is a region of RAM used for:
- **Function local variables**
- **Return addresses** (where to go back after a function returns)
- **Saving registers** before a function call

```
Stack grows DOWNWARD — higher address → lower address

Before push:      After PUSH R0:    After PUSH R1:
                                    
SP → [0x2000]     [0x2000] = old    [0x2000] = old
                  SP → [0x1FFC]     [0x1FFC] = R0
                                    SP → [0x1FF8] = R1
```

**Push** = subtract 4 from SP, write value at new SP
**Pop**  = read value at SP, add 4 to SP

### 4.4 Addressing Modes

How you specify WHERE data comes from or goes to:

```
; Immediate — value is part of the instruction itself
MOV R0, #42          ; R0 = 42

; Register — data is in another register
MOV R0, R1           ; R0 = R1

; Register Indirect — register holds a memory address
LDR R0, [R1]         ; R0 = RAM[R1]

; Base + Offset — address is register + constant
LDR R0, [R1, #8]     ; R0 = RAM[R1 + 8]

; Base + Register — address is sum of two registers
LDR R0, [R1, R2]     ; R0 = RAM[R1 + R2]

; Pre-indexed with writeback — update base register
LDR R0, [R1, #4]!    ; R1 = R1+4 first, then R0 = RAM[R1]

; Post-indexed — load then update base register
LDR R0, [R1], #4     ; R0 = RAM[R1], then R1 = R1+4
```

---

## 5. x86 Assembly (Intel/AMD)

x86 is the architecture in all desktop PCs, laptops, and servers. You'll encounter it when reading disassembly from your host computer.

### 5.1 Register Set

#### 32-bit (IA-32) Registers

```
General Purpose (32-bit):
  EAX — Accumulator     (return values, arithmetic)
  EBX — Base            (general purpose, sometimes base pointer)
  ECX — Counter         (loop counter, shift amounts)
  EDX — Data            (I/O, multiply/divide overflow)
  ESI — Source Index    (string source pointer)
  EDI — Destination Index (string dest pointer)
  ESP — Stack Pointer   (top of stack)
  EBP — Base Pointer    (frame pointer — base of current stack frame)

Special:
  EIP — Instruction Pointer (Program Counter)
  EFLAGS — Condition flags (CF, ZF, SF, OF, PF, AF)
```

#### 64-bit (x86-64) Registers

```
64-bit extended versions:
  RAX, RBX, RCX, RDX, RSI, RDI, RSP, RBP
  + 8 new registers: R8, R9, R10, R11, R12, R13, R14, R15
  RIP — 64-bit instruction pointer

Partial register access:
  RAX (64-bit)
   └── EAX (lower 32 bits)
        └── AX (lower 16 bits)
             ├── AH (bits 8-15)
             └── AL (bits 0-7)
```

#### x86-64 Calling Convention (System V ABI — Linux)

```
Function arguments (in order):  RDI, RSI, RDX, RCX, R8, R9
Return value:                   RAX
Caller-saved (can be clobbered): RAX, RCX, RDX, RSI, RDI, R8-R11
Callee-saved (must preserve):   RBX, RBP, R12-R15, RSP
```

### 5.2 x86 Instruction Syntax

x86 uses two syntaxes. They are the SAME instructions, just written differently:

```
Intel syntax (NASM, MASM — Windows, most books):
    MOV destination, source
    MOV eax, 5          ; eax = 5

AT&T syntax (GAS — Linux gcc default):
    MOV source, destination
    movl $5, %eax       ; eax = 5
    ; prefixes: % for registers, $ for immediates, l/w/b size suffix
```

This guide uses **Intel syntax** (easier to read).

### 5.3 Core x86 Instructions

#### Data Movement

```nasm
; MOV — copy data
MOV eax, 5           ; eax = 5 (immediate)
MOV eax, ebx         ; eax = ebx (register to register)
MOV eax, [rbx]       ; eax = RAM[rbx] (memory to register)
MOV [rbx], eax       ; RAM[rbx] = eax (register to memory)
MOV eax, [rbx + 8]   ; eax = RAM[rbx + 8] (base+offset)

; MOVZX — move with zero extension (zero-pad to larger size)
MOVZX eax, byte [rbx]    ; Load 1 byte, fill upper bits with 0

; MOVSX — move with sign extension (sign-pad to larger size)
MOVSX eax, byte [rbx]    ; Load 1 byte, extend sign bit

; LEA — Load Effective Address (compute address, don't dereference)
LEA rax, [rbx + rcx*4 + 8]  ; rax = rbx + rcx*4 + 8 (no memory read!)
; Trick: LEA is used for fast multiply-and-add: LEA eax, [eax + eax*4] = eax*5

; XCHG — exchange two values
XCHG eax, ebx        ; swap eax and ebx

; PUSH / POP — stack operations
PUSH rax             ; rsp -= 8; RAM[rsp] = rax
POP rbx              ; rbx = RAM[rsp]; rsp += 8
```

#### Arithmetic

```nasm
; ADD / SUB
ADD eax, 10          ; eax = eax + 10
ADD eax, ebx         ; eax = eax + ebx
ADD eax, [rbx]       ; eax = eax + RAM[rbx]
SUB eax, 5           ; eax = eax - 5

; INC / DEC — increment/decrement by 1
INC eax              ; eax++
DEC ecx              ; ecx--

; MUL / IMUL — unsigned/signed multiply
MUL ebx              ; EDX:EAX = EAX * EBX (result split across two regs)
IMUL eax, ebx        ; eax = eax * ebx (signed, truncated to 32 bits)
IMUL eax, ebx, 10    ; eax = ebx * 10

; DIV / IDIV — unsigned/signed divide
; Before: dividend in EDX:EAX, divisor in operand
DIV ecx              ; EAX = EDX:EAX / ECX,  EDX = remainder
IDIV ecx             ; signed version

; NEG — negate (two's complement)
NEG eax              ; eax = -eax

; ADC — add with carry (for multi-precision arithmetic)
ADC eax, ebx         ; eax = eax + ebx + CF
```

#### Bitwise Logic

```nasm
AND eax, ebx         ; eax = eax & ebx
OR  eax, ebx         ; eax = eax | ebx
XOR eax, eax         ; eax = 0  (idiom: fastest way to zero a register)
NOT eax              ; eax = ~eax (bitwise NOT)

; Shifts
SHL eax, 3           ; eax <<= 3 (multiply by 8)
SHR eax, 1           ; eax >>= 1 (unsigned divide by 2)
SAR eax, 1           ; eax >>= 1 (signed, preserves sign bit)
ROL eax, 4           ; rotate left 4 bits
ROR eax, 4           ; rotate right 4 bits

; TEST — AND without storing result (only sets flags)
TEST eax, eax        ; set ZF if eax == 0, does NOT change eax

; CMP — SUB without storing result (only sets flags)
CMP eax, 10          ; set flags based on eax - 10, does NOT change eax
```

#### Control Flow

```nasm
; Unconditional jump
JMP label            ; jump to label

; Conditional jumps (check flags after CMP/TEST)
JE  label            ; Jump if Equal        (ZF=1)
JNE label            ; Jump if Not Equal    (ZF=0)
JL  label            ; Jump if Less         (signed: SF≠OF)
JLE label            ; Jump if Less/Equal   (signed)
JG  label            ; Jump if Greater      (signed)
JGE label            ; Jump if Greater/Equal(signed)
JB  label            ; Jump if Below        (unsigned: CF=1)
JA  label            ; Jump if Above        (unsigned)
JZ  label            ; Jump if Zero         (same as JE)
JNZ label            ; Jump if Not Zero     (same as JNE)
JS  label            ; Jump if Sign (negative)
JC  label            ; Jump if Carry

; Loop (uses ECX as counter)
LOOP label           ; ECX--; if ECX != 0, jump to label

; Function calls
CALL func            ; push return address, JMP to func
RET                  ; pop return address, JMP there
RET 8                ; pop, then add 8 to RSP (clean up args)
```

### 5.4 x86 Program Structure (NASM, Linux)

```nasm
; hello.asm — print "Hello" to stdout and exit
; Assemble: nasm -f elf64 hello.asm -o hello.o
; Link:     ld hello.o -o hello

section .data
    msg db "Hello, World!", 0x0A   ; string + newline
    len equ $ - msg                ; length = current pos - msg start

section .bss
    ; uninitialized data would go here
    ; buffer resb 64    ; reserve 64 bytes

section .text
    global _start      ; entry point for linker

_start:
    ; sys_write(fd=1, buf=msg, count=len)
    mov rax, 1         ; syscall number: write
    mov rdi, 1         ; arg1: file descriptor 1 = stdout
    mov rsi, msg       ; arg2: pointer to string
    mov rdx, len       ; arg3: length
    syscall            ; invoke kernel

    ; sys_exit(status=0)
    mov rax, 60        ; syscall number: exit
    xor rdi, rdi       ; arg1: exit code 0
    syscall
```

### 5.5 x86 If-Else and Loops

```nasm
; --- IF (x > 10) THEN ... ELSE ... ---
    mov eax, [x]       ; load x
    cmp eax, 10        ; compare x to 10
    jle else_branch    ; if x <= 10, go to else

then_branch:
    ; code for "if x > 10"
    jmp end_if

else_branch:
    ; code for "else"

end_if:
    ; code continues here


; --- FOR LOOP: for(i=0; i<10; i++) ---
    mov ecx, 0         ; i = 0
for_start:
    cmp ecx, 10        ; i < 10?
    jge for_end        ; if i >= 10, exit loop
    ; loop body here
    inc ecx            ; i++
    jmp for_start
for_end:


; --- WHILE LOOP: while(eax != 0) ---
while_start:
    test eax, eax      ; sets ZF if eax == 0
    jz while_end
    ; loop body
    ; (eax gets modified in body)
    jmp while_start
while_end:


; --- FUNCTION (cdecl convention) ---
my_function:
    push rbp           ; save caller's base pointer
    mov rbp, rsp       ; set our frame pointer

    ; access arguments: [rbp+16] = first arg, [rbp+24] = second
    ; (in 64-bit System V: args in rdi, rsi, rdx... not on stack)

    ; local variables: sub rsp, 16 to make room
    sub rsp, 16
    ; [rbp-4] = first local (int), [rbp-8] = second local

    ; ... function body ...

    mov rsp, rbp       ; restore stack
    pop rbp            ; restore caller's base pointer
    ret                ; return (result in rax)
```

---

## 6. ARM Assembly (Cortex-M / ESP32 / STM32)

ARM is the architecture in your microcontrollers. ESP32 uses Xtensa (ARM-like), STM32 uses ARM Cortex-M. This section focuses on **ARM Cortex-M** (Thumb-2 instruction set), which is what STM32 uses.

### 6.1 ARM Register Set

```
General Purpose Registers (32-bit each):
  R0  — argument 1 / return value
  R1  — argument 2 / return value (upper 32 bits of 64-bit result)
  R2  — argument 3
  R3  — argument 4
  R4–R11 — callee-saved variables (must preserve across calls)
  R12 — Intra-procedure scratch register (IP)
  R13 — Stack Pointer (SP)
  R14 — Link Register (LR) — holds return address for BL instruction
  R15 — Program Counter (PC)

Special Registers:
  CPSR — Current Program Status Register (flags: N, Z, C, V)
         Also: processor mode, interrupt enable bits
  SPSR — Saved Program Status Register (saved on exception entry)
```

#### ARM Calling Convention (AAPCS)

```
Arguments:   R0, R1, R2, R3  (first 4 args; more go on stack)
Return:      R0 (and R1 for 64-bit)
Caller-saved (scratch): R0–R3, R12
Callee-saved (must restore): R4–R11, LR (if you call other functions)
```

### 6.2 ARM Instruction Syntax

```asm
MNEMONIC{condition}{S}  Rd, Rn, Operand2

; MNEMONIC  — instruction name (MOV, ADD, LDR, etc.)
; {condition} — optional: EQ, NE, GT, LT, etc.  (predication!)
; {S}        — optional S suffix: update flags (ADDS, SUBS, etc.)
; Rd         — destination register
; Rn         — first source register
; Operand2   — second source: register, immediate, or shifted register
```

#### ARM's Unique Feature: Condition Codes on ANY Instruction

Unlike x86 where only jump instructions are conditional, in ARM **almost any instruction** can be made conditional using a 4-bit condition code suffix:

```asm
; Without condition — always executes
MOV R0, #1

; With condition — only executes if condition is true
MOVEQ R0, #1     ; Execute only if Zero flag set (previous result == 0)
MOVNE R0, #1     ; Execute only if Zero flag clear
MOVGT R0, #1     ; Execute only if Greater Than (signed)
ADDLT R1, R1, #1 ; Add only if Less Than

; This allows if-else WITHOUT branches:
CMP R0, #5
MOVLT R1, #0     ; if R0 < 5, R1 = 0
MOVGE R1, #1     ; if R0 >= 5, R1 = 1
; Both lines always fetched, but only one executes — avoids branch misprediction
```

### 6.3 ARM Instructions — Data Processing

```asm
; MOV — move / load immediate
MOV R0, #42          ; R0 = 42
MOV R0, R1           ; R0 = R1
MVN R0, #0           ; R0 = ~0 = 0xFFFFFFFF  (Move NOT)

; For large immediates (> 8 bits rotated), use MOVW + MOVT
MOVW R0, #0x5678     ; R0 = 0x00005678  (lower 16 bits)
MOVT R0, #0x1234     ; R0[31:16] = 0x1234, so R0 = 0x12345678

; Arithmetic
ADD R0, R1, R2       ; R0 = R1 + R2
ADD R0, R0, #8       ; R0 = R0 + 8
ADDS R0, R1, R2      ; R0 = R1 + R2, AND update flags
ADC R0, R1, R2       ; R0 = R1 + R2 + Carry (64-bit add support)
SUB R0, R1, R2       ; R0 = R1 - R2
SUBS R0, R1, R2      ; R0 = R1 - R2, update flags
SBC R0, R1, R2       ; R0 = R1 - R2 - ~Carry (64-bit sub)
RSB R0, R1, #0       ; R0 = 0 - R1 = -R1  (Reverse SuBtract)
MUL R0, R1, R2       ; R0 = R1 * R2 (lower 32 bits)
MLA R0, R1, R2, R3   ; R0 = R1*R2 + R3  (Multiply-Accumulate)
UDIV R0, R1, R2      ; R0 = R1 / R2 (unsigned, Cortex-M3+)
SDIV R0, R1, R2      ; R0 = R1 / R2 (signed)

; Bitwise
AND R0, R1, R2       ; R0 = R1 & R2
ORR R0, R1, R2       ; R0 = R1 | R2
EOR R0, R1, R2       ; R0 = R1 ^ R2  (XOR)
BIC R0, R1, R2       ; R0 = R1 & ~R2  (Bit Clear — mask off bits)
ORN R0, R1, R2       ; R0 = R1 | ~R2  (OR NOT)
MVN R0, R1           ; R0 = ~R1

; Shifts (as Operand2 of another instruction, or standalone)
LSL R0, R1, #3       ; R0 = R1 << 3
LSR R0, R1, #1       ; R0 = R1 >> 1 (logical, zero fill)
ASR R0, R1, #1       ; R0 = R1 >> 1 (arithmetic, sign fill)
ROR R0, R1, #4       ; rotate right 4

; Combined: shift inside operand2
ADD R0, R1, R2, LSL #2   ; R0 = R1 + (R2 << 2) = R1 + R2*4

; Comparison (sets flags only, no destination register)
CMP R0, R1           ; flags = R0 - R1 (like SUBS but discards result)
CMN R0, R1           ; flags = R0 + R1 (Compare Negative)
TST R0, R1           ; flags = R0 & R1 (like ANDS, test bits)
TEQ R0, R1           ; flags = R0 ^ R1 (test equality bitwise)
```

### 6.4 ARM Memory Instructions (Load/Store)

```asm
; LDR / STR — 32-bit word
LDR R0, [R1]         ; R0 = RAM[R1]          (base)
LDR R0, [R1, #4]     ; R0 = RAM[R1 + 4]     (base + offset)
LDR R0, [R1, R2]     ; R0 = RAM[R1 + R2]    (base + register)
LDR R0, [R1, R2, LSL #2]  ; R0 = RAM[R1 + R2*4]  (scaled index)
LDR R0, [R1, #4]!    ; R1 = R1+4, then R0 = RAM[R1]   (pre-index writeback)
LDR R0, [R1], #4     ; R0 = RAM[R1], then R1 = R1+4   (post-index)
STR R0, [R1]         ; RAM[R1] = R0
STR R0, [R1, #8]     ; RAM[R1+8] = R0

; LDRB / STRB — 8-bit byte
LDRB R0, [R1]        ; R0 = (uint8_t)RAM[R1]   (zero-extended to 32 bits)
LDRSB R0, [R1]       ; R0 = (int8_t)RAM[R1]    (sign-extended)
STRB R0, [R1]        ; RAM[R1] = R0 & 0xFF

; LDRH / STRH — 16-bit halfword
LDRH R0, [R1]        ; R0 = (uint16_t)RAM[R1]
LDRSH R0, [R1]       ; R0 = (int16_t)RAM[R1]   (sign-extended)
STRH R0, [R1]

; LDRD / STRD — 64-bit (two consecutive 32-bit registers)
LDRD R0, R1, [R2]    ; R0 = RAM[R2], R1 = RAM[R2+4]

; LDM / STM — Load/Store Multiple (efficient block transfer)
PUSH {R4, R5, R6, LR}   ; push four registers onto stack
POP  {R4, R5, R6, PC}   ; pop four registers (loading PC = return)

; LDM variants:
LDMIA R0!, {R1-R4}   ; load R1–R4 from [R0], R0 += 16 (Increment After)
STMIA R0!, {R1-R4}   ; store R1–R4 to [R0], R0 += 16
```

### 6.5 ARM Branch Instructions

```asm
; B — branch (unconditional)
B   loop_start        ; jump to loop_start

; BL — branch with link (function call)
BL  my_function       ; LR = next instruction address, PC = my_function
; Return with: BX LR

; BX — branch and exchange (also changes instruction set Thumb↔ARM)
BX  LR                ; return from function (PC = LR)
BX  R0                ; jump to address in R0

; BLX — branch with link and exchange
BLX R3                ; call function pointer in R3

; Conditional branches
B{cond} label
; Condition codes:
;   EQ — Equal (Z=1)
;   NE — Not Equal (Z=0)
;   CS/HS — Carry Set / Higher or Same (C=1, unsigned >=)
;   CC/LO — Carry Clear / Lower (C=0, unsigned <)
;   MI — Minus/Negative (N=1)
;   PL — Plus/Non-negative (N=0)
;   VS — Overflow (V=1)
;   VC — No Overflow (V=0)
;   HI — Higher (unsigned >)  C=1 and Z=0
;   LS — Lower or Same (unsigned <=)
;   GE — Signed >=
;   LT — Signed <
;   GT — Signed >
;   LE — Signed <=
;   AL — Always (default, same as no condition)

; Example: if (R0 == R1) call func
CMP R0, R1
BLEQ my_function      ; call only if equal
```

### 6.6 ARM Program Example: Sum of Array

```asm
; sum_array: compute sum of n 32-bit integers
; Input:  R0 = pointer to array, R1 = count n
; Output: R0 = sum
; Clobbers: R2, R3

sum_array:
    PUSH {R4, LR}        ; save R4 (callee-saved) and return address
    MOV  R4, #0          ; R4 = sum = 0
    CMP  R1, #0
    BEQ  done            ; if n == 0, return 0

loop:
    LDR  R2, [R0], #4    ; R2 = *ptr++  (load, then advance pointer)
    ADD  R4, R4, R2      ; sum += R2
    SUBS R1, R1, #1      ; n-- and set flags
    BNE  loop            ; if n != 0, continue

done:
    MOV  R0, R4          ; return value in R0
    POP  {R4, PC}        ; restore R4, return (POP into PC = return)
```

### 6.7 Thumb and Thumb-2

ARM Cortex-M processors use **Thumb-2** instruction encoding by default.

```
ARM (32-bit) encoding:    every instruction = exactly 4 bytes
Thumb (16-bit) encoding:  most instructions = 2 bytes (smaller code size)
Thumb-2:                  mix of 16-bit and 32-bit instructions
                          Cortex-M uses this — you get both density + power
```

When writing assembly for STM32/Cortex-M:
- Use `.thumb` or `.thumb_func` directives
- The syntax is the same, the assembler picks the right encoding
- 16-bit Thumb instructions have restrictions (only low registers R0–R7, limited immediates)

### 6.8 Bare-Metal ARM: STM32 Startup Example

```asm
; startup.s — minimal STM32 startup (Cortex-M)
.syntax unified
.thumb

.section .isr_vector, "a"
    .word  _estack          ; Initial stack pointer (end of RAM)
    .word  Reset_Handler    ; Reset vector
    .word  NMI_Handler
    .word  HardFault_Handler
    ; ... more vectors ...

.section .text
.global Reset_Handler
.thumb_func
Reset_Handler:
    ; Copy .data section from Flash to RAM
    ldr  r0, =_sdata        ; destination start
    ldr  r1, =_edata        ; destination end
    ldr  r2, =_sidata       ; source (in Flash)
copy_data:
    cmp  r0, r1
    bge  zero_bss
    ldr  r3, [r2], #4
    str  r3, [r0], #4
    b    copy_data

zero_bss:
    ; Zero out .bss section
    ldr  r0, =_sbss
    ldr  r1, =_ebss
    mov  r2, #0
zero_bss_loop:
    cmp  r0, r1
    bge  call_main
    str  r2, [r0], #4
    b    zero_bss_loop

call_main:
    bl   main               ; call your C main()
    b    .                  ; infinite loop if main returns
```

---

## 7. RISC-V Assembly

RISC-V (pronounced "risk five") is a **free, open-source instruction set architecture** — no licensing fees, anyone can implement it. It's gaining massive popularity in academia, embedded systems, and even data centers.

### 7.1 Why RISC-V?

| Feature | Benefit |
|---|---|
| Open standard, no royalties | Anyone can make a RISC-V chip |
| Modular extensions | Use only what you need |
| Clean RISC design | No historical baggage from 1978 |
| Growing ecosystem | SiFive, Espressif (ESP32-C3!), Western Digital |

> Your **ESP32-C3** uses a RISC-V core (not Xtensa) — so this architecture runs on hardware you may already own!

### 7.2 RISC-V Modular ISA

RISC-V is defined as a **base ISA** + **optional extensions**:

```
Base ISA:
  RV32I   — 32-bit integer base (47 instructions)
  RV64I   — 64-bit integer base
  RV128I  — 128-bit (future)

Extensions (letters after the I):
  M — Integer Multiply/Divide (MUL, DIV, REM)
  A — Atomic instructions (for multi-core synchronization)
  F — Single-precision floating point
  D — Double-precision floating point
  C — Compressed 16-bit instructions (like Thumb)
  V — Vector operations
  B — Bit manipulation

Common combinations:
  RV32IMAC — 32-bit + Multiply + Atomic + Compressed  (ESP32-C3, STM32WL)
  RV64GC   — 64-bit + G(=IMAFD) + Compressed          (Linux capable)
```

### 7.3 RISC-V Register Set

```
32 general-purpose registers, each 32 bits (RV32) or 64 bits (RV64):

x0  / zero  — hardwired to 0 (always reads as 0, writes discarded)
x1  / ra    — Return Address
x2  / sp    — Stack Pointer
x3  / gp    — Global Pointer (points to global data area)
x4  / tp    — Thread Pointer (TLS base)
x5  / t0    — Temporary / alternate link register
x6  / t1    — Temporary
x7  / t2    — Temporary
x8  / s0/fp — Saved register 0 / Frame Pointer
x9  / s1    — Saved register 1
x10 / a0    — Argument 0 / Return value 0
x11 / a1    — Argument 1 / Return value 1
x12 / a2    — Argument 2
x13 / a3    — Argument 3
x14 / a4    — Argument 4
x15 / a5    — Argument 5
x16 / a6    — Argument 6
x17 / a7    — Argument 7 / syscall number
x18 / s2    — Saved register 2
...
x27 / s11   — Saved register 11
x28 / t3    — Temporary
x29 / t4    — Temporary
x30 / t5    — Temporary
x31 / t6    — Temporary

pc          — Program Counter (not a general register, separate)
```

**ABI Names:** you can use either `x10` or `a0` — they're the same register.

### 7.4 RISC-V Instruction Format

RISC-V instructions are always 32 bits wide (or 16 bits for Compressed 'C' extension). There are 6 base formats:

```
R-type (register-register):
  [31:25] funct7 | [24:20] rs2 | [19:15] rs1 | [14:12] funct3 | [11:7] rd | [6:0] opcode

I-type (immediate, loads, JALR):
  [31:20] imm[11:0] | [19:15] rs1 | [14:12] funct3 | [11:7] rd | [6:0] opcode

S-type (stores):
  [31:25] imm[11:5] | [24:20] rs2 | [19:15] rs1 | [14:12] funct3 | [11:7] imm[4:0] | [6:0] opcode

B-type (branches):
  [31] imm[12] | [30:25] imm[10:5] | [24:20] rs2 | [19:15] rs1 | [14:12] funct3 | [11:8] imm[4:1] | [7] imm[11] | [6:0] opcode

U-type (upper immediate: LUI, AUIPC):
  [31:12] imm[31:12] | [11:7] rd | [6:0] opcode

J-type (JAL):
  [31] imm[20] | [30:21] imm[10:1] | [20] imm[11] | [19:12] imm[19:12] | [11:7] rd | [6:0] opcode
```

You don't need to memorize these — the assembler handles encoding — but it's good to know that every format is fixed-width and easy to decode.

### 7.5 RISC-V Instructions

#### Integer Arithmetic (RV32I)

```asm
# ADD / SUB
add  a0, a1, a2       # a0 = a1 + a2
sub  a0, a1, a2       # a0 = a1 - a2
addi a0, a1, 10       # a0 = a1 + 10  (immediate, sign-extended 12-bit)
# Note: there is NO subi — use addi with negative: addi a0, a1, -5

# Shifts
sll  a0, a1, a2       # a0 = a1 << a2   (shift left logical)
srl  a0, a1, a2       # a0 = a1 >> a2   (shift right logical, zero-fill)
sra  a0, a1, a2       # a0 = a1 >> a2   (shift right arithmetic, sign-fill)
slli a0, a1, 3        # a0 = a1 << 3    (immediate shift)
srli a0, a1, 1        # a0 = a1 >> 1
srai a0, a1, 1        # a0 = a1 >> 1  (signed)

# Bitwise
and  a0, a1, a2       # a0 = a1 & a2
andi a0, a1, 0xFF     # a0 = a1 & 0xFF  (mask lower byte)
or   a0, a1, a2       # a0 = a1 | a2
ori  a0, a1, 0x1      # a0 = a1 | 1  (set bit 0)
xor  a0, a1, a2       # a0 = a1 ^ a2
xori a0, a1, -1       # a0 = ~a1  (XOR with all-ones = bitwise NOT)

# Set if Less Than
slt  a0, a1, a2       # a0 = (a1 < a2) ? 1 : 0  (signed)
sltu a0, a1, a2       # a0 = (a1 < a2) ? 1 : 0  (unsigned)
slti a0, a1, 5        # a0 = (a1 < 5) ? 1 : 0

# Upper immediate (for building 32-bit constants)
lui  a0, 0x12345      # a0 = 0x12345000  (load upper 20 bits)
auipc a0, 0x10        # a0 = PC + 0x10000  (PC-relative address)

# Full 32-bit constant: use lui + addi
lui  a0, 0x12345      # a0 = 0x12345000
addi a0, a0, 0x678    # a0 = 0x12345678
# (assembler pseudo-instruction li handles this automatically)

# Pseudo-instructions (assembler expands to real instructions):
li   a0, 12345678     # load any 32-bit immediate
mv   a0, a1           # a0 = a1  (expands to addi a0, a1, 0)
neg  a0, a1           # a0 = -a1 (expands to sub a0, x0, a1)
not  a0, a1           # a0 = ~a1 (expands to xori a0, a1, -1)
nop                   # no operation (expands to addi x0, x0, 0)
```

#### Multiply / Divide (M extension)

```asm
mul    a0, a1, a2     # a0 = a1 * a2  (lower 32 bits)
mulh   a0, a1, a2     # a0 = (a1 * a2) >> 32  (upper 32 bits, signed*signed)
mulhu  a0, a1, a2     # upper 32 bits, unsigned*unsigned
mulhsu a0, a1, a2     # upper 32 bits, signed*unsigned
div    a0, a1, a2     # a0 = a1 / a2  (signed)
divu   a0, a1, a2     # a0 = a1 / a2  (unsigned)
rem    a0, a1, a2     # a0 = a1 % a2  (signed remainder)
remu   a0, a1, a2     # a0 = a1 % a2  (unsigned remainder)
```

#### Memory (Load / Store)

```asm
# Loads (base + 12-bit signed offset)
lw   a0, 0(a1)        # a0 = (int32_t)RAM[a1 + 0]   (word, 4 bytes)
lh   a0, 4(a1)        # a0 = (int16_t)RAM[a1 + 4]   (halfword, sign-extend)
lhu  a0, 4(a1)        # a0 = (uint16_t)RAM[a1 + 4]  (halfword, zero-extend)
lb   a0, 8(a1)        # a0 = (int8_t)RAM[a1 + 8]    (byte, sign-extend)
lbu  a0, 8(a1)        # a0 = (uint8_t)RAM[a1 + 8]   (byte, zero-extend)

# Stores
sw   a0, 0(a1)        # RAM[a1 + 0] = a0  (word)
sh   a0, 4(a1)        # RAM[a1 + 4] = a0[15:0]  (halfword)
sb   a0, 8(a1)        # RAM[a1 + 8] = a0[7:0]   (byte)

# Indirect call via register:
lw   t0, 0(a1)        # load function pointer
jalr zero, 0(t0)      # jump to it (and discard return addr into x0)
```

#### Control Flow

```asm
# Unconditional jump (JAL = Jump And Link)
jal  ra, target       # ra = PC+4, jump to target  (function call)
j    target           # jump, no link  (pseudo: jal x0, target)

# Indirect jump (JALR = Jump And Link Register)
jalr ra, 0(t0)        # ra = PC+4, jump to address in t0  (call via pointer)
ret                   # return  (pseudo: jalr x0, 0(ra))
jr   t0               # jump to t0  (pseudo: jalr x0, 0(t0))

# Conditional branches (compare two registers, branch if true)
beq  a0, a1, label    # if (a0 == a1) jump to label
bne  a0, a1, label    # if (a0 != a1) jump
blt  a0, a1, label    # if (a0 < a1) jump  (signed)
bge  a0, a1, label    # if (a0 >= a1) jump (signed)
bltu a0, a1, label    # if (a0 < a1) jump  (unsigned)
bgeu a0, a1, label    # if (a0 >= a1) jump (unsigned)

# Pseudo-branches (compare with zero, or swap operands)
beqz a0, label        # if (a0 == 0) jump  (expands to beq a0, x0, label)
bnez a0, label        # if (a0 != 0) jump
bltz a0, label        # if (a0 < 0) jump
bgez a0, label        # if (a0 >= 0) jump
bgtz a0, label        # if (a0 > 0) jump
blez a0, label        # if (a0 <= 0) jump
```

### 7.6 RISC-V Program Example: Sum of Array

```asm
# sum_array: sum n 32-bit integers
# Arguments: a0 = pointer to array, a1 = count n
# Returns:   a0 = sum

sum_array:
    addi sp, sp, -8       # allocate stack frame
    sw   ra, 4(sp)        # save return address
    sw   s0, 0(sp)        # save s0 (callee-saved)

    mv   s0, x0           # s0 = sum = 0
    beqz a1, .done        # if n == 0, done

.loop:
    lw   t0, 0(a0)        # t0 = *array
    add  s0, s0, t0       # sum += t0
    addi a0, a0, 4        # array++
    addi a1, a1, -1       # n--
    bnez a1, .loop        # if n != 0, continue

.done:
    mv   a0, s0           # return value = sum

    lw   ra, 4(sp)        # restore return address
    lw   s0, 0(sp)        # restore s0
    addi sp, sp, 8        # deallocate stack frame
    ret                   # return
```

### 7.7 RISC-V Program Example: Hello World (Linux syscall)

```asm
# hello.s — RISC-V RV32I, Linux
# Assemble: riscv32-linux-gnu-as hello.s -o hello.o
# Link:     riscv32-linux-gnu-ld hello.o -o hello

.section .rodata
msg:    .string "Hello, RISC-V!\n"
len = . - msg

.section .text
.global _start
_start:
    # write(fd=1, buf=msg, count=len)
    li   a7, 64           # syscall: write
    li   a0, 1            # fd: stdout
    la   a1, msg          # buf: address of msg
    li   a2, len          # count
    ecall                 # invoke syscall

    # exit(0)
    li   a7, 93           # syscall: exit
    li   a0, 0            # exit code 0
    ecall
```

---

## 8. Comparison Table — x86 vs ARM vs RISC-V

| Feature | x86-64 | ARM Cortex-M | RISC-V (RV32I) |
|---|---|---|---|
| **Type** | CISC | RISC | RISC |
| **Instruction width** | 1–15 bytes (variable) | 2/4 bytes (Thumb-2) | 4 bytes (2 bytes with C ext.) |
| **General registers** | 16 (64-bit) | 16 (32-bit) | 32 (32-bit) |
| **Instruction count** | ~1500+ | ~200 | ~47 (base) |
| **Memory in ALU ops** | Yes (`ADD [mem], reg`) | No (load-store only) | No (load-store only) |
| **Conditional ops** | Branches only | Any instruction | Branches only |
| **Calling convention** | RSI/RDI for args | R0–R3 for args | a0–a7 for args |
| **Return address** | Stack (CALL/RET) | Link register (LR) | ra register |
| **License** | Intel/AMD proprietary | ARM proprietary ($$$) | Open source, free |
| **Primary use** | Desktop, server | Mobile, microcontrollers | Emerging embedded, server |
| **Your hardware** | Your PC | STM32, nRF52 | ESP32-C3, SiFive |

### Equivalent Operations Compared

```
Task: if (a == b) then c = 1, else c = 0

x86-64:
    cmp   rdi, rsi       ; compare a and b
    sete  al             ; al = (ZF==1) ? 1 : 0
    movzx rax, al        ; zero-extend to 64-bit

ARM Cortex-M:
    CMP   R0, R1         ; compare a and b
    MOVEQ R2, #1         ; if equal: c = 1
    MOVNE R2, #0         ; if not equal: c = 0

RISC-V:
    sub   t0, a0, a1     ; t0 = a - b
    seqz  a2, t0         ; a2 = (t0 == 0) ? 1 : 0
    ; (seqz pseudo: sltiu a2, t0, 1)
```

---

## 9. Practical Patterns

### 9.1 Bit Manipulation (useful in embedded/GPIO)

```asm
; --- Set bit N of register ---
; C: reg |= (1 << N)

; ARM:
MOV R1, #1
LSL R1, R1, #N        ; R1 = 1 << N
ORR R0, R0, R1        ; R0 |= R1

; RISC-V:
li   t0, 1
slli t0, t0, N        # t0 = 1 << N
or   a0, a0, t0       # a0 |= t0


; --- Clear bit N ---
; C: reg &= ~(1 << N)

; ARM:
MOV R1, #1
LSL R1, R1, #N
BIC R0, R0, R1        ; R0 &= ~R1  (Bit Clear)

; RISC-V:
li   t0, 1
slli t0, t0, N
not  t0, t0           # t0 = ~t0
and  a0, a0, t0       # a0 &= t0


; --- Toggle bit N ---
; C: reg ^= (1 << N)

; ARM:
MOV R1, #1
LSL R1, R1, #N
EOR R0, R0, R1        ; R0 ^= R1

; --- Test bit N (check if set) ---
; ARM:
TST R0, R1            ; sets ZF if bit is 0
BEQ bit_was_clear
; RISC-V:
andi a1, a0, (1 << N) # a1 = a0 & mask
beqz a1, bit_was_clear
```

### 9.2 Loop Patterns

```asm
; --- Count-down loop (most efficient — tests against zero) ---

; ARM:
    MOV R1, #100          ; counter = 100
loop:
    ; ... body ...
    SUBS R1, R1, #1       ; counter--, set flags
    BNE  loop             ; if counter != 0, loop

; RISC-V:
    li   t0, 100          # counter = 100
loop:
    # ... body ...
    addi t0, t0, -1       # counter--
    bnez t0, loop         # if counter != 0, loop


; --- Array iteration ---

; C: for(int i=0; i<n; i++) sum += array[i]

; RISC-V:
    # a0 = array ptr, a1 = n, result in a2
    mv   a2, x0           # sum = 0
    beqz a1, done         # n == 0?
loop:
    lw   t0, 0(a0)        # t0 = *array
    add  a2, a2, t0       # sum += t0
    addi a0, a0, 4        # array++
    addi a1, a1, -1       # n--
    bnez a1, loop
done:
```

### 9.3 Function Call Patterns

```asm
; --- ARM function (callee saves what it uses) ---

my_func:
    PUSH {R4, R5, LR}     ; save R4, R5 (used below), LR (we'll call BL)
    ; R0-R3 = arguments, free to use without saving
    MOV  R4, R0           ; copy arg to callee-saved reg (survives BL calls)
    BL   other_func       ; call — R0-R3, LR modified by other_func
    ADD  R0, R0, R4       ; R0 = other_func result + original arg
    POP  {R4, R5, PC}     ; restore R4, R5; pop into PC = return


; --- RISC-V function ---

my_func:
    addi sp, sp, -16      # allocate stack frame
    sw   ra, 12(sp)       # save return address
    sw   s0, 8(sp)        # save s0
    sw   s1, 4(sp)        # save s1

    mv   s0, a0           # save arg0 across calls
    call other_func       # call (a0-a7, t0-t6, ra clobbered)
    add  a0, a0, s0       # result + saved arg

    lw   ra, 12(sp)
    lw   s0, 8(sp)
    lw   s1, 4(sp)
    addi sp, sp, 16
    ret
```

### 9.4 Reading from Hardware Registers (Embedded)

In embedded systems, peripheral registers are **memory-mapped** — reading/writing a specific memory address controls hardware directly.

```asm
; STM32 example: set GPIO pin high
; GPIOA_ODR (Output Data Register) address: 0x40020014

; ARM:
    LDR R0, =0x40020014   ; R0 = address of GPIO ODR register
    LDR R1, [R0]          ; R1 = current register value
    ORR R1, R1, #(1 << 5) ; set bit 5 (PA5 = LED pin on Nucleo)
    STR R1, [R0]          ; write back to hardware register

; RISC-V (ESP32-C3 GPIO):
    li   t0, 0x60004004   ; GPIO_OUT_REG address
    lw   t1, 0(t0)        # read current value
    ori  t1, t1, (1 << 5) # set pin 5
    sw   t1, 0(t0)        # write to hardware
```

---

## 10. Tools and How to Run Assembly

### 10.1 Assemble and Run on Your Linux PC (x86-64)

```bash
# Install NASM assembler
sudo apt install nasm

# Write hello.asm (see Section 5.4 example)
nasm -f elf64 hello.asm -o hello.o    # assemble
ld hello.o -o hello                    # link
./hello                                # run

# See the generated machine code
objdump -d hello                       # disassemble

# Assemble with GAS (AT&T syntax, used by gcc)
gcc -S -O2 my_program.c -o my_program.s  # generate assembly from C
```

### 10.2 ARM Assembly for STM32 (Cross-Compile)

```bash
# Install ARM cross-assembler
sudo apt install binutils-arm-none-eabi gcc-arm-none-eabi

# Assemble
arm-none-eabi-as -mcpu=cortex-m4 -mthumb startup.s -o startup.o

# Compile C + link with assembly
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -nostdlib \
    startup.s main.c -T linker.ld -o firmware.elf

# Convert to binary for flashing
arm-none-eabi-objcopy -O binary firmware.elf firmware.bin

# Flash with OpenOCD / st-flash
st-flash write firmware.bin 0x08000000
```

### 10.3 RISC-V Assembly for ESP32-C3

```bash
# Install RISC-V toolchain
sudo apt install binutils-riscv64-linux-gnu gcc-riscv64-linux-gnu
# Or use Espressif's ESP-IDF which bundles riscv32-esp-elf-gcc

# Using ESP-IDF (recommended for ESP32-C3):
# Write assembly in a .S file, add to CMakeLists.txt
# idf.py build && idf.py flash

# Standalone RISC-V assembly (Linux RISC-V emulator with QEMU)
sudo apt install qemu-user-static gcc-riscv64-linux-gnu
riscv64-linux-gnu-as hello.s -o hello.o
riscv64-linux-gnu-ld hello.o -o hello
qemu-riscv64-static hello
```

### 10.4 See Assembly Generated from Your C Code

The single most useful learning tool:

```bash
# Generate ARM assembly from C (for STM32-like Cortex-M)
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O2 -S main.c -o main.s
cat main.s

# Generate x86-64 assembly from C
gcc -O2 -S -fverbose-asm main.c -o main.s   # -fverbose-asm adds C source comments
cat main.s

# Online: https://godbolt.org  (Compiler Explorer)
# — paste C code, choose ARM gcc or RISC-V gcc, see assembly live
# — shows which C line maps to which assembly instruction
# — try different optimization levels (-O0, -O1, -O2, -Os)
```

### 10.5 Debugging Assembly

```bash
# GDB with ARM bare-metal (OpenOCD as debug server)
arm-none-eabi-gdb firmware.elf
(gdb) target remote :3333    # connect to OpenOCD
(gdb) monitor reset halt
(gdb) layout asm             # show assembly pane
(gdb) layout regs            # show register pane
(gdb) si                     # step one instruction
(gdb) info registers         # show all registers
(gdb) x/4xw 0x20000000      # examine 4 words at address
```

---

## Summary: Key Concepts to Internalize

| Concept | What to Remember |
|---|---|
| CISC vs RISC | CISC = complex variable-length instructions; RISC = simple fixed-length, load-store |
| Registers | Fastest storage; limited count; some have special roles (PC, SP, LR) |
| Flags | Set after arithmetic; control conditional branches |
| Load-Store | RISC: math only on registers; access memory ONLY via LDR/STR or lw/sw |
| Stack | Grows downward; push = SP-=4 then write; pop = read then SP+=4 |
| Function call | Save LR/RA; save callee-saved regs; set up frame; return restores all |
| Memory-mapped I/O | Hardware registers are just memory addresses — LDR/STR controls hardware |
| Assembler pseudos | `li`, `mv`, `ret`, `j` — assembler expands to real instructions |

---

*Next steps: Read compiler-generated assembly on [godbolt.org](https://godbolt.org) for your STM32/ESP32 code to see how C maps to instructions. Then try writing a simple bare-metal blink LED in assembly on your STM32 Nucleo.*

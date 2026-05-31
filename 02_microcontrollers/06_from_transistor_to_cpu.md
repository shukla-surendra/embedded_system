# From Transistor to CPU — How a Computing Chip Works

> This guide explains how a chip is **designed at the logical level** — not how it
> is physically manufactured. By the end you will understand how a single line of
> assembly like `ADD R0, R1, R2` actually flows through millions of switches to
> produce a result.

## Table of Contents

1. [The Transistor — The Universal Switch](#1-the-transistor--the-universal-switch)
2. [Logic Gates — Teaching Switches to Think](#2-logic-gates--teaching-switches-to-think)
3. [Boolean Algebra — The Math of Logic](#3-boolean-algebra--the-math-of-logic)
4. [Combinational Circuits — Logic That Computes](#4-combinational-circuits--logic-that-computes)
5. [Binary Arithmetic — Adding with Gates](#5-binary-arithmetic--adding-with-gates)
6. [Sequential Circuits — Logic That Remembers](#6-sequential-circuits--logic-that-remembers)
7. [Registers — Storing a Word](#7-registers--storing-a-word)
8. [The ALU — Arithmetic Logic Unit](#8-the-alu--arithmetic-logic-unit)
9. [The Clock — Heartbeat of the CPU](#9-the-clock--heartbeat-of-the-cpu)
10. [The Datapath — Moving Data Through the CPU](#10-the-datapath--moving-data-through-the-cpu)
11. [The Control Unit — The CPU's Brain](#11-the-control-unit--the-cpus-brain)
12. [Instruction Encoding — Numbers as Commands](#12-instruction-encoding--numbers-as-commands)
13. [Full Trace: ADD R0, R1, R2 Through Hardware](#13-full-trace-add-r0-r1-r2-through-hardware)
14. [Memory Hierarchy — Why Registers Exist](#14-memory-hierarchy--why-registers-exist)
15. [The Complete Picture](#15-the-complete-picture)

---

## 1. The Transistor — The Universal Switch

A transistor is the most fundamental building block in any computer chip. Modern
CPUs contain **billions** of them — an M3 chip has 25 billion transistors packed
onto a piece of silicon the size of your thumbnail.

At the logical level (ignoring physics), a transistor behaves as a **voltage-controlled
switch**. It has three terminals:

```
       Gate (G)
         │
         ▼
Drain ───┤├─── Source
  (D)    │      (S)
         │
  (current flows D→S when Gate is HIGH)
```

| Gate voltage | Switch state | Current flows D→S? |
|---|---|---|
| LOW  (0V, logic 0) | OPEN  (off) | No  |
| HIGH (3.3V, logic 1) | CLOSED (on) | Yes |

That is the entire concept you need. **One voltage controls whether another
current flows.** Everything else — logic gates, arithmetic, memory, CPUs — is
just clever arrangements of this one idea.

### The Two Transistor Types (CMOS)

Real chips use two complementary types wired together — this is called **CMOS**
(Complementary Metal-Oxide-Semiconductor):

```
 PMOS (P-type):              NMOS (N-type):
  turns ON when Gate = LOW    turns ON when Gate = HIGH
  turns OFF when Gate = HIGH  turns OFF when Gate = LOW
  like a normally-closed      like a normally-open
  switch                      switch
```

Wiring one PMOS and one NMOS together allows a gate to drive output both HIGH
and LOW without burning power (only one is on at a time, so no direct
Vdd→GND path).

---

## 2. Logic Gates — Teaching Switches to Think

A **logic gate** is a small circuit built from a handful of transistors that
implements a basic logical operation. Once you have gates, you never think about
transistors again — gates become your building block.

### NOT Gate (Inverter) — 2 transistors

Input = 0 → Output = 1. Input = 1 → Output = 0.

```
    Vdd (power)
     │
  [PMOS]  ← Gate = A (PMOS: ON when A=0, OFF when A=1)
     │
     ├──── Output Y
     │
  [NMOS]  ← Gate = A (NMOS: ON when A=1, OFF when A=0)
     │
    GND
```

```
When A = 0:  PMOS ON,  NMOS OFF  → Y connected to Vdd  → Y = 1
When A = 1:  PMOS OFF, NMOS ON   → Y connected to GND  → Y = 0
```

Truth table:

| A | Y = NOT A |
|---|---|
| 0 | 1 |
| 1 | 0 |

Symbol: `Y = A̅` or `Y = ¬A` or `Y = ~A`

---

### NAND Gate — 4 transistors

Output is 0 only when BOTH inputs are 1. Otherwise output is 1.
(NOT AND = NAND)

```
    Vdd
   / \
[P] [P]       ← Two PMOS in PARALLEL (if either A or B = 0, top path ON)
 A   B
  \ /
   ├──── Y
   │
  [N]         ← Two NMOS in SERIES
   A
   │
  [N]         ← (both must be ON for bottom path to pull Y low)
   B
   │
  GND
```

Truth table:

| A | B | AND | NAND (Y) |
|---|---|-----|----------|
| 0 | 0 |  0  |    1     |
| 0 | 1 |  0  |    1     |
| 1 | 0 |  0  |    1     |
| 1 | 1 |  1  |    0     |

> **NAND is universal** — every other logic gate can be built from NAND gates
> alone. This is why real chips are designed using NAND-heavy standard cell libraries.

---

### NOR Gate — 4 transistors

Output is 1 only when BOTH inputs are 0.

| A | B | NOR (Y) |
|---|---|---------|
| 0 | 0 |    1    |
| 0 | 1 |    0    |
| 1 | 0 |    0    |
| 1 | 1 |    0    |

---

### AND, OR, XOR — derived from NAND/NOR

```
AND  = NAND + NOT   (4 + 2 = 6 transistors)
OR   = NOR  + NOT   (4 + 2 = 6 transistors)
XOR  = 4× NAND gates (12 transistors)
       Y = A⊕B = 1 when A and B differ
```

XOR truth table — the key gate for addition:

| A | B | XOR (Y) |
|---|---|---------|
| 0 | 0 |    0    |
| 0 | 1 |    1    |
| 1 | 0 |    1    |
| 1 | 1 |    0    |

Gate symbols used in circuit diagrams:

```
NOT:    A ──[>o]── Y         (bubble = inversion)

AND:    A ──┐
            [D]── Y
        B ──┘

OR:     A ──┐
            [≥1]── Y
        B ──┘

XOR:    A ──┐
            [=1]── Y         (exclusive OR)
        B ──┘

NAND:   A ──┐
            [D]o── Y         (AND + bubble)
        B ──┘
```

---

## 3. Boolean Algebra — The Math of Logic

Logic gates obey a mathematical system called **Boolean algebra** where every
variable is either 0 or 1. Understanding these rules lets you simplify circuits
— fewer gates = smaller chip, faster circuit.

### Basic Rules

```
AND laws:          OR laws:           NOT laws:
  A · 0 = 0          A + 0 = A          ¬(¬A) = A
  A · 1 = A          A + 1 = 1
  A · A = A          A + A = A
  A · ¬A = 0         A + ¬A = 1
```

### Key Identities

```
Commutative:   A · B = B · A           A + B = B + A
Associative:   (A·B)·C = A·(B·C)       (A+B)+C = A+(B+C)
Distributive:  A·(B+C) = A·B + A·C     A+(B·C) = (A+B)·(A+C)
```

### De Morgan's Laws — the most important identities

```
¬(A · B) = ¬A + ¬B        NOT(AND) = OR of NOTs
¬(A + B) = ¬A · ¬B        NOT(OR)  = AND of NOTs
```

This means: NAND gate output = NOT(A AND B) = (NOT A) OR (NOT B).

In practice De Morgan's lets you convert between AND/OR/NAND/NOR forms to
pick whichever is cheapest to implement.

### Why Binary?

Computers use binary (base-2) because transistors have two reliable states:
on and off. A transistor cannot stably hold a voltage of exactly 1.7V (base-3
would need three stable states). With binary, you only need to distinguish
"clearly high" from "clearly low" — noise tolerance is built in.

```
Voltage range interpretation (3.3V logic):
  0.0V – 0.8V  →  logic 0  (LOW)
  2.0V – 3.3V  →  logic 1  (HIGH)
  0.8V – 2.0V  →  undefined (forbidden zone)
```

---

## 4. Combinational Circuits — Logic That Computes

A **combinational circuit** is a network of logic gates where the output depends
**only on the current inputs** — no memory, no history. Give it the same inputs
twice and you always get the same output.

### Multiplexer (MUX) — the data selector

A MUX selects one of several inputs and passes it to the output, based on a
"select" control signal. Think of it as a data switchboard.

```
2-to-1 MUX:

  A ──────┐
          [AND] ──┐
  ¬S ─────┘       │
                  [OR]── Y
  S ──────┐       │
          [AND] ──┘
  B ──────┘

  When S=0: Y = A
  When S=1: Y = B
```

MUXes are everywhere in CPUs — they select which register value goes into the
ALU, which result gets written back, which instruction path is taken.

### Decoder — address to one-hot

A decoder takes an N-bit binary number and activates exactly one of 2^N output
lines. A 3-bit decoder has 8 outputs; exactly one is HIGH at any time.

```
2-to-4 decoder:

  Inputs: A, B (2 bits → 4 possible combinations)
  Outputs: Y0, Y1, Y2, Y3 (exactly one HIGH)

  Y0 = ¬A · ¬B    (active when AB = 00)
  Y1 = ¬A ·  B    (active when AB = 01)
  Y2 =  A · ¬B    (active when AB = 10)
  Y3 =  A ·  B    (active when AB = 11)
```

Decoders are used in register files and memory to select which register or
memory cell to read/write.

---

## 5. Binary Arithmetic — Adding with Gates

Computers represent numbers in binary. Let's see how addition is implemented
purely with logic gates.

### Binary Counting

```
Decimal:  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
Binary:   0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
```

Each bit position is a power of 2:

```
  Bit 3   Bit 2   Bit 1   Bit 0
   2³=8    2²=4    2¹=2    2⁰=1

Example: binary 1011 = 8 + 0 + 2 + 1 = decimal 11
```

### Half Adder — adding two 1-bit numbers

When you add two single bits A and B, you get a Sum bit and a Carry bit:

```
  0 + 0 = 0  (sum=0, carry=0)
  0 + 1 = 1  (sum=1, carry=0)
  1 + 0 = 1  (sum=1, carry=0)
  1 + 1 = 10 (sum=0, carry=1)  ← "10" in binary = decimal 2
```

Truth table:

| A | B | Sum | Carry |
|---|---|-----|-------|
| 0 | 0 |  0  |   0   |
| 0 | 1 |  1  |   0   |
| 1 | 0 |  1  |   0   |
| 1 | 1 |  0  |   1   |

Notice:
- **Sum = A XOR B** (differ → 1, same → 0)
- **Carry = A AND B** (both 1 → carry 1)

Circuit:

```
  A ──┬──[XOR]── Sum
      │
  B ──┴──[AND]── Carry
```

Just **2 gates** (6+6 = ~12 transistors) implement 1-bit addition. This is remarkable.

### Full Adder — adding with a carry-in

When adding multi-bit numbers, each bit position also receives a **carry-in**
from the position to its right. A full adder has three inputs: A, B, Cin.

```
  A + B + Cin  →  Sum, Cout
```

| A | B | Cin | Sum | Cout |
|---|---|-----|-----|------|
| 0 | 0 |  0  |  0  |  0   |
| 0 | 0 |  1  |  1  |  0   |
| 0 | 1 |  0  |  1  |  0   |
| 0 | 1 |  1  |  0  |  1   |
| 1 | 0 |  0  |  1  |  0   |
| 1 | 0 |  1  |  0  |  1   |
| 1 | 1 |  0  |  0  |  1   |
| 1 | 1 |  1  |  1  |  1   |

Implementation: **two half adders + one OR gate**

```
  A, B  ──[Half Adder 1]──► S1, C1
  S1, Cin─[Half Adder 2]──► Sum, C2
  C1, C2 ─[OR]────────────► Cout
```

### 4-bit Ripple Carry Adder — adding 4-bit numbers

Chain four full adders, feeding the Cout of each into the Cin of the next:

```
  A3 A2 A1 A0     (4-bit number A)
  B3 B2 B1 B0     (4-bit number B)

  Cin=0 ─► [FA0] ─Cout─► [FA1] ─Cout─► [FA2] ─Cout─► [FA3] ─► Cout (overflow)
            │              │              │              │
            S0             S1             S2             S3

  Result: S3 S2 S1 S0  (4-bit sum)
```

Each FA is ~28 transistors. Four of them add two 4-bit numbers: ~112 transistors.
A 32-bit adder chains 32 full adders: ~900 transistors. One small part of a CPU.

---

## 6. Sequential Circuits — Logic That Remembers

Combinational circuits have no memory. But a CPU needs to store intermediate
results, track state, and hold the program counter. This requires **sequential
circuits** — circuits whose output depends on both current input AND previous state.

### The SR Latch — storing one bit

Two cross-coupled NOR gates create a circuit that "remembers" which input was
last set to 1:

```
       S ──┐
           [NOR]──── Q        Q = stored bit
       ┌───┘
       │
       └───┐
           [NOR]──── Q̄ = NOT Q
       R ──┘
       └───────────── (fed back to first gate)
```

Behaviour:

| S | R | Q (next) | Meaning |
|---|---|----------|---------|
| 1 | 0 |    1     | Set Q to 1 |
| 0 | 1 |    0     | Reset Q to 0 |
| 0 | 0 | unchanged | Hold (memory!) |
| 1 | 1 | invalid  | Forbidden |

The SR latch is the **most primitive memory element** — two gates holding each
other in a stable state. The feedback loops are what create memory: the output
feeds back to reinforce itself.

### D Flip-Flop — the practical memory cell

The SR latch has a forbidden state (S=R=1). The **D flip-flop** solves this by
having a single data input D and a **clock** input. It captures the value of D
at the rising edge of the clock and holds it until the next clock edge.

```
       D ──────┐
               [Logic]──── Q    (captures D on ↑ clock edge)
  Clock ───────┘            │
                            ├── Q̄
                            │
  (Q feeds back ────────────┘  to maintain state between clock edges)
```

```
                 ___     ___
  Clock:     ___|   |___|   |___

  D:         _________|‾‾‾‾‾‾‾‾
                       ↑
                       D captured here
  Q:         __________|‾‾‾‾‾‾‾   (Q follows D after rising edge)
```

Key properties:
- **Q only changes on the clock edge** — the rest of the time it holds its value
- One D flip-flop stores **one bit**
- Built from ~12–20 transistors (NOR-based SR latches + logic)

---

## 7. Registers — Storing a Word

A **register** is simply N flip-flops sharing a common clock and enable signal,
storing N bits (one word) in parallel.

### 4-bit register

```
  D3 ─[DFF]─ Q3 ─┐
  D2 ─[DFF]─ Q2 ─┤
  D1 ─[DFF]─ Q1 ─┤── 4-bit output
  D0 ─[DFF]─ Q0 ─┘

  Clock ─────────────── all four DFFs share the same clock
  Enable ────────────── gates whether data is loaded
```

On each rising clock edge, if Enable=1, the register captures the 4-bit input
and holds it until the next enabled clock edge.

A 32-bit register = 32 D flip-flops ≈ 32 × 20 = ~640 transistors.

### Register File — the CPU's scratch pad

A CPU has many registers (ARM: 16, RISC-V: 32). These are arranged in a
**register file** — an array of registers with read/write ports controlled by
address decoders.

```
  ┌─────────────────────────────────────────────────────┐
  │                  Register File (32 × 32-bit)        │
  │                                                     │
  │  Write port:  Write addr (5-bit) → decoder → which │
  │               Write data (32-bit) → selected reg   │
  │               Write enable                         │
  │                                                     │
  │  Read port A: Read addr A (5-bit) → decoder → MUX  │
  │               → 32-bit data out A                  │
  │                                                     │
  │  Read port B: Read addr B (5-bit) → decoder → MUX  │
  │               → 32-bit data out B                  │
  └─────────────────────────────────────────────────────┘
```

The register file can output two register values simultaneously (for the ALU's
two input operands) and accept one write-back per clock cycle.

---

## 8. The ALU — Arithmetic Logic Unit

The **ALU** is the combinational circuit that performs arithmetic and logical
operations. It takes two N-bit inputs (A, B) and an operation selector, and
produces an N-bit result plus flags.

```
                    ┌────────────────────────────────────┐
  A (32-bit) ──────►│                                    │
                    │              ALU                   ├──► Result (32-bit)
  B (32-bit) ──────►│                                    │
                    │  Op: ADD / SUB / AND / OR / XOR /  ├──► Zero flag (Z)
  Operation ───────►│       SHL / SHR / ...              ├──► Carry flag (C)
  (4-bit code)      │                                    ├──► Negative flag (N)
                    └────────────────────────────────────┘    Overflow flag (V)
```

### What's inside an ALU

An ALU is a **bank of operation circuits** with a big MUX at the output:

```
  A, B ──►[32-bit Adder    ]──► add_result    ─┐
  A, B ──►[Subtractor      ]──► sub_result    ─┤
  A, B ──►[AND array       ]──► and_result    ─┤
  A, B ──►[OR array        ]──► or_result     ─┤── [32-bit MUX] ──► Result
  A, B ──►[XOR array       ]──► xor_result    ─┤      ▲
  A    ──►[Left shifter    ]──► shl_result    ─┘      │
                                                  Operation code
                                               (selects which result passes)
```

All operations run **simultaneously** in parallel hardware. The MUX at the end
simply selects which result to output based on the operation code. This is why
the ALU is fast — it doesn't decide "what to compute", it computes everything
and then picks.

### Flag generation

After computing the result, flag logic checks:

```
  Zero flag  Z = (result == 0)    → all 32 bits NORed together
  Carry flag C = carry out of bit 31 of adder
  Negative N = result[31]          → top bit (sign bit in two's complement)
  Overflow V = carry[31] XOR carry[30]  → signed overflow detection
```

These flags feed directly into the **branch logic** — conditional jumps read
these flags to decide whether to branch.

---

## 9. The Clock — Heartbeat of the CPU

Everything in a synchronous digital circuit is coordinated by a **clock** — a
square wave signal that alternates between 0 and 1 at a fixed frequency.

```
       ___     ___     ___     ___
      |   |   |   |   |   |   |   |
 _____|   |___|   |___|   |___|   |___

  ↑       ↑       ↑       ↑
  Rising edges — when flip-flops capture new values
  ← one period → = 1 / frequency
```

A 1 GHz clock has a period of 1 nanosecond (0.000000001 seconds).
In that 1 ns, electrons travel only ~20 cm at the speed of light — so chip
designers must ensure signals can propagate from one flip-flop through all the
logic gates to the next flip-flop **within one clock period**.

### Why the clock frequency is limited

The clock period must be long enough for the **longest combinational path**
(called the **critical path**) to settle. If you clock too fast, a signal
hasn't finished propagating and you capture garbage.

```
  [FF] ──gate──gate──gate──gate──gate──gate──[FF]
         ← critical path delay: 0.8 ns →

  Max clock frequency = 1 / 0.8 ns = 1.25 GHz

  If you clock at 2 GHz (0.5 ns period), the last few gates haven't settled → wrong answer
```

Modern CPUs (3–5 GHz) achieve this by making the critical path through logic
extremely short — often just 8–15 gate delays per clock cycle.

---

## 10. The Datapath — Moving Data Through the CPU

The **datapath** is the collection of registers, ALU, buses, and MUXes that
data flows through. It is the hardware that carries out the actual computation.

```
┌─────────────────────────────────────────────────────────────────┐
│                         DATAPATH                                │
│                                                                 │
│  ┌─────────────┐    Read addr A ──►─────────────────────────┐  │
│  │  Instruction│    Read addr B ──►  Register File (32 regs)│  │
│  │  Register   │    Write addr  ──►  (32 × 32-bit DFFs)     │  │
│  │  (holds     │    Write data  ──►                         │  │
│  │   current   │                    Data A out ─────────────┼──┼──► ALU input A
│  │  instruction│                    Data B out ─────────────┼──┼──► ALU input B (or imm)
│  │  word)      │                                            │  │
│  └──────┬──────┘                                            │  │
│         │ decode                 ┌──────────┐               │  │
│         ▼                        │   ALU    │               │  │
│  ┌──────────────┐  Op select ──► │          │◄──────────────┘  │
│  │ Control Unit │                │          ├──► Result         │
│  │  (FSM)       │  Branch ◄───── │          ├──► Flags          │
│  └──────┬───────┘                └────┬─────┘                  │
│         │                             │ Result                  │
│         │ PC control                  ▼                        │
│  ┌──────▼──────┐             ┌────────────────┐                │
│  │  Program    │             │  Data Memory   │                │
│  │  Counter    │             │  (RAM interface│                │
│  │  (PC)       │             │   load/store)  │                │
│  └──────┬──────┘             └────────────────┘                │
│         │ address                                               │
│         ▼                                                       │
│  ┌─────────────┐                                               │
│  │ Instruction │                                               │
│  │   Memory    │                                               │
│  │  (ROM/Flash)│                                               │
│  └─────────────┘                                               │
└─────────────────────────────────────────────────────────────────┘
```

### The key busses

```
A bus is just a bundle of wires carrying N bits in parallel.

  32-bit data bus  — carries one 32-bit value per cycle
  32-bit address bus — carries a memory address
  Control bus — carries enable, read/write, operation signals
```

---

## 11. The Control Unit — The CPU's Brain

The **control unit** is a circuit that reads the current instruction word and
generates all the control signals that tell every other component what to do
this cycle.

It is implemented as a **Finite State Machine (FSM)** — a circuit that moves
through a sequence of states, with each state producing different control outputs.

### The states of instruction execution

```
   ┌─────────────┐
   │             │
   ▼             │
  [FETCH]        │  • PC → instruction memory address
   │             │  • instruction memory outputs instruction word
   ▼             │  • instruction word loaded into Instruction Register
  [DECODE]       │  • control unit reads opcode field
   │             │  • determines what type of instruction this is
   ▼             │  • sets up register file read addresses
  [EXECUTE]      │  • ALU performs operation
   │             │  (for load/store: computes address)
   ▼             │
  [MEMORY]       │  • (only for LDR/STR) access data memory
   │             │
   ▼             │
  [WRITEBACK]    │  • write result back to register file
   │             │
   └─────────────┘  • update PC (PC = PC+4, or branch target)
```

Each state lasts one clock cycle in a simple CPU. A 5-stage pipeline CPU
executes all 5 stages simultaneously for different instructions.

### What "control signals" look like

For a given instruction, the control unit asserts/deasserts wires:

```
  RegWrite     = 1   (allow writing to register file)
  ALUSrc       = 0   (ALU B input comes from register, not immediate)
  ALUOp        = 010 (ALU should perform ADD)
  MemRead      = 0   (don't read memory)
  MemWrite     = 0   (don't write memory)
  MemToReg     = 0   (write ALU result to register, not memory data)
  Branch       = 0   (no branch)
  PCWrite      = 1   (update PC)
```

Each of these 1-bit signals connects to the enable or select input of a MUX,
register, or memory component in the datapath. The control unit is
essentially a **truth table decoder** — instruction bits in, control signals out.

---

## 12. Instruction Encoding — Numbers as Commands

Every instruction a CPU executes is stored in memory as a fixed-width binary
number. The CPU doesn't understand text like "ADD" — it reads a pattern of bits
and the hardware is wired to respond to that pattern.

### RISC-V 32-bit R-type instruction format

```
 31      25 24   20 19  15 14  12 11    7 6      0
 ┌─────────┬───────┬──────┬──────┬───────┬────────┐
 │ funct7  │  rs2  │ rs1  │funct3│  rd   │ opcode │
 │  7 bits │ 5 bits│5 bits│3 bits│ 5 bits│ 7 bits │
 └─────────┴───────┴──────┴──────┴───────┴────────┘

  opcode  = major operation class (e.g. 0110011 = register-register op)
  funct3  = sub-operation (e.g. 000 = add/sub)
  funct7  = further qualifier (e.g. 0000000 = add, 0100000 = sub)
  rd      = destination register number (0–31)
  rs1     = source register 1 number (0–31)
  rs2     = source register 2 number (0–31)
```

### Example: ADD x5, x6, x7

```
  ADD x5, x6, x7   means:  x5 = x6 + x7

  rd  = 5  = 00101
  rs1 = 6  = 00110
  rs2 = 7  = 00111
  funct3 = 000
  funct7 = 0000000
  opcode = 0110011

  Binary: 0000000  00111  00110  000  00101  0110011
          funct7   rs2    rs1   f3    rd     opcode

  Hex: 0x00730333
```

That 32-bit hex number `0x00730333` sitting in Flash memory **is** the
instruction `ADD x5, x6, x7`. The hardware reads those bit patterns and routes
them through gates that implement exactly the right operation.

### How the bit fields connect to hardware

```
  Instruction bits [6:0]   →  opcode decoder  →  selects major operation
  Instruction bits [11:7]  →  register file write address (rd)
  Instruction bits [19:15] →  register file read address A (rs1)
  Instruction bits [24:20] →  register file read address B (rs2)
  Instruction bits [14:12] →  ALU operation MUX (funct3)
  Instruction bits [31:25] →  ALU operation MUX (funct7)

  These bit fields are literally wired directly to the inputs of the
  relevant circuits — no computation needed to "parse" the instruction.
  The decoding IS the wiring.
```

---

## 13. Full Trace: ADD R0, R1, R2 Through Hardware

Let's trace exactly what happens — gate by gate, cycle by cycle — when the CPU
executes `ADD x0, x1, x2` (RISC-V: x0 = x1 + x2).

Assume:
- x1 contains value **5**  (binary: 0000...0101)
- x2 contains value **3**  (binary: 0000...0011)
- The instruction is at memory address **0x0000_0010**
- PC currently holds **0x0000_0010**

---

### Cycle 1 — FETCH

```
  1. PC value (0x10) is placed on the address bus
  2. Instruction memory (Flash/ROM) receives address 0x10
  3. Instruction memory outputs the 32-bit word stored at 0x10:
         0x00208033  = ADD x0, x1, x2  (in binary)
  4. This word is loaded into the Instruction Register (IR)
  5. PC is incremented: PC ← PC + 4 = 0x14
     (done by a dedicated adder in the datapath — not the ALU)
```

Hardware active this cycle:
- PC register outputs its value
- Address MUX routes PC to instruction memory
- Instruction memory does a read (combinational)
- IR flip-flop captures the instruction on the clock edge

---

### Cycle 2 — DECODE

```
  1. The 32-bit instruction word in IR is split by bit field:
       bits [6:0]   = 0110011  → opcode = register-register
       bits [11:7]  = 00000    → rd  = x0
       bits [19:15] = 00001    → rs1 = x1
       bits [24:20] = 00010    → rs2 = x2
       bits [14:12] = 000      → funct3 = add
       bits [31:25] = 0000000  → funct7 = add (not sub)

  2. Control unit reads opcode + funct3 + funct7:
       → Asserts: RegWrite=1, ALUSrc=0, ALUOp=ADD, MemRead=0,
                  MemWrite=0, MemToReg=ALU, Branch=0

  3. Register file read addresses are set:
       Read addr A = rs1 bits = 00001  → read x1
       Read addr B = rs2 bits = 00010  → read x2

  4. Register file outputs:
       Data A = contents of x1 = 5
       Data B = contents of x2 = 3
```

Hardware active this cycle:
- Instruction bit fields are routed by wires (no gates needed — it's just routing)
- Opcode decoder (combinational) produces control signals
- Register file read ports activated by decoder circuits
- D flip-flops in x1 and x2 drive their Q outputs onto the data buses

---

### Cycle 3 — EXECUTE

```
  1. ALU receives:
       Input A = 5  (from register x1)
       Input B = 3  (from register x2, via MUX — ALUSrc=0 means register)
       Operation = ADD  (from control unit)

  2. Inside the ALU:
       All 32-bit operations run simultaneously:
         Adder computes:    5 + 3 = 8
         AND computes:      5 & 3 = 1  (0101 & 0011 = 0001)
         OR computes:       5 | 3 = 7  (0101 | 0011 = 0111)
         XOR computes:      5 ^ 3 = 6  (0101 ^ 0011 = 0110)
         ... all others too ...

       The output MUX selects the ADD result: 8

  3. How the adder works (32-bit ripple carry):
       Bit 0:  1+1 = 0, carry=1    (1 XOR 1 = 0, 1 AND 1 = 1)
       Bit 1:  0+1+carry1 = 0, carry=1   (full adder: 0+1+1=10)
       Bit 2:  1+0+carry1 = 0, carry=1
       Bit 3:  0+0+carry1 = 1, carry=0
       Bits 4-31: all 0+0+0 = 0
       Result: 0000...01000 = 8  ✓

  4. Flag logic evaluates the result:
       Z = (result == 0) = 0   (8 ≠ 0)
       N = result[31] = 0       (positive)
       C = carry_out = 0        (no overflow)
       V = overflow = 0
```

Hardware active this cycle:
- All 32 full adders compute in parallel (propagating carry bit by bit)
- All other operation circuits also compute in parallel
- Output MUX selects the adder result
- Flag circuits evaluate result bits

The total delay through the adder is ~32 gate delays (one per carry ripple) —
this is actually the critical path. Modern CPUs use a **carry-lookahead adder**
to compute carry in O(log N) stages instead of O(N), reducing this to ~6 gate delays.

---

### Cycle 4 — WRITEBACK

```
  1. ALU result (8) is on the result bus
  2. MemToReg MUX selects ALU result (not memory data — this isn't a load)
  3. Write address = rd bits = 00000 → write to x0
  4. RegWrite = 1 → register file write port is enabled
  5. On the rising clock edge:
       The D flip-flop for x0 captures value 8
       x0 now holds 8

  Done. The instruction is complete.
```

### Summary of the full trace

```
  Clock edge 1:  PC=0x10 → fetch instruction word from memory → IR = 0x00208033
  Clock edge 2:  Decode bits → read x1=5 and x2=3 from register file
  Clock edge 3:  ALU computes 5+3=8 with carry ripple through 32 adders
  Clock edge 4:  Write 8 into x0 register

  4 clock cycles × (1/1GHz) = 4 nanoseconds to add two numbers.

  Meanwhile the program is already fetching the NEXT instruction
  (pipelining overlaps these stages for different instructions).
```

---

## 14. Memory Hierarchy — Why Registers Exist

You might wonder: if memory can store billions of values, why bother with
registers at all? The answer is speed and power:

```
  Storage type    | Capacity | Access time | Cost per bit | Technology
  ────────────────┼──────────┼─────────────┼──────────────┼───────────
  Register        |  32 words| 1 cycle     | very high    | D flip-flops
  L1 Cache        |  32 KB   | 4 cycles    | high         | 6-transistor SRAM
  L2 Cache        | 256 KB   | 12 cycles   | medium       | 6-transistor SRAM
  L3 Cache        |  8 MB    | 40 cycles   | lower        | 6-transistor SRAM
  RAM (DRAM)      | 16 GB    | 200 cycles  | very low     | 1 transistor + capacitor
  Flash/SSD       |  1 TB    | 100,000 cy  | cheapest     | floating-gate transistors
```

### Why is SRAM faster than DRAM?

**SRAM** (Static RAM) stores each bit with 6 transistors arranged in two
cross-coupled inverters — the same structure as an SR latch. The bit is always
"alive" in a stable state and can be read in nanoseconds.

```
  SRAM cell (6 transistors):

       Vdd        Vdd
        │          │
      [P1]       [P2]
        │          │
   ─────┤ Q      Q̄ ├─────
        │          │
      [N1]       [N2]   ← cross-coupled inverters (the storage)
        │          │
       GND        GND

  + 2 access transistors (N3, N4) controlled by Word Line
  + Bit lines (BL, BL̄) for reading and writing
```

**DRAM** stores each bit as charge on a tiny capacitor with only 1 transistor.
Much denser (more bits per area) but:
- Capacitor leaks → must be refreshed every 64ms
- Reading destroys the charge → must be rewritten after every read
- Slower access due to charge/discharge time

### Why the hierarchy exists

```
  If every memory access needed to go to DRAM (200 cycle delay):
    A simple 5-instruction loop would take 200 × 5 = 1000 cycles
    Actual performance: 5 cycles  (200× slower without cache)

  Caches exploit "locality":
    Temporal locality:  recently used data will likely be used again soon
    Spatial locality:   data near recently used data will likely be used soon

  The cache hierarchy keeps frequently-used data close to the CPU
  in fast-but-small SRAM, hiding the slowness of large-but-cheap DRAM.
```

---

## 15. The Complete Picture

Now we can draw the full chain from silicon to running code:

```
PHYSICAL LAYER
  Silicon wafer + dopants
  → transistors (switches controlled by voltage)

GATE LAYER
  2–20 transistors per gate
  → NOT, AND, OR, NAND, NOR, XOR gates
  → Boolean algebra: the math of 0s and 1s

COMBINATIONAL LAYER
  Hundreds of gates
  → Half adder, full adder, MUX, decoder, comparator
  → No memory — output depends only on current inputs

SEQUENTIAL LAYER
  Gates + feedback loops
  → SR latch, D flip-flop (stores 1 bit, updates on clock edge)

REGISTER LAYER
  32 flip-flops sharing a clock
  → Register (stores 32 bits)
  → Register file (32 registers, dual read ports, one write port)

FUNCTIONAL UNIT LAYER
  Thousands of gates
  → ALU (adder + logic + MUX at output)
  → Barrel shifter (shift by N positions in 1 cycle)

DATAPATH LAYER
  Tens of thousands of gates
  → PC, instruction register, register file, ALU, memory interface
  → Connected by buses, MUXes controlled by control signals

CONTROL LAYER
  Finite state machine
  → Reads instruction bits
  → Asserts control signals for each pipeline stage
  → Sequences fetch → decode → execute → memory → writeback

INSTRUCTION SET LAYER
  32-bit binary words in memory
  → Instruction encoding (opcode + register fields + immediates)
  → The instruction set architecture (ISA) is the contract between
    software and hardware

ASSEMBLY LAYER
  Human-readable mnemonics
  → "ADD x0, x1, x2" assembled to 0x00208033
  → Assembler translates text → machine code binary

COMPILER LAYER
  High-level language → assembly → machine code
  → C, Rust, Go → GCC/LLVM → binary

APPLICATION LAYER
  Your C program
  → Compiled to thousands of machine code instructions
  → Loaded into Flash, executed by the CPU you now understand
```

---

### A single transistor → the entire computer

```
  1 transistor = 1 switch                         (one gate: ~0.000001 mm²)
  6 transistors = 1 SRAM bit (memory cell)
  ~20 transistors = 1 D flip-flop (register bit)
  ~100 transistors = 1 full adder (1-bit addition with carry)
  ~3,200 transistors = 32-bit adder (core of the ALU)
  ~640 transistors = 32-bit register
  ~20,000 transistors = 32-register file
  ~500,000 transistors = complete simple 32-bit CPU core
  ~25,000,000,000 transistors = Apple M3 (25 billion)
                                 (multiple cores, caches, GPU, neural engine)
```

Every one of those 25 billion transistors is still just a voltage-controlled
switch. The mind-bending complexity of a modern CPU is entirely emergent from
that one simple primitive, repeated, wired together cleverly.

When you write:

```c
int result = a + b;
```

The compiler turns this into something like:

```asm
lw   a0, 0(sp)         # load 'a' from stack into register a0
lw   a1, 4(sp)         # load 'b' from stack into register a1
add  a2, a0, a1        # a2 = a0 + a1
```

And that `add` instruction becomes `0x00A50633` in Flash memory. The CPU reads
those 32 bits, routes them through decoders, register file, and ALU, and 4
nanoseconds later the result is sitting in register a2 — a journey through
billions of transistors that you now understand from the first principles up.

---

*Next: [Assembly Programming Guide](05_assembly_programming_guide.md) — now that
you understand the hardware, learn to write assembly that runs directly on it.*

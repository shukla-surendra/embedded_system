# CPU vs Microcontroller — And Ways of Handling Tasks
## A Beginner's Deep-Dive Guide

---

## The One-Line Difference

> **CPU:** Just the brain. Needs organs, bones, and skin added separately.
> **Microcontroller:** A complete tiny body — brain, memory, and organs all in one chip.

Everything below unpacks that sentence.

---

## Part 1 — What Is a CPU?

A **CPU (Central Processing Unit)** is a chip that can only do one thing: execute instructions (add numbers, compare values, move data). It has no memory of its own, no way to talk to the outside world, nothing.

To be useful, a CPU needs a supporting team of chips around it:

```
  A CPU-Based System (like inside your laptop):

  ┌─────────────────────────────────────────────────────────┐
  │                    MOTHERBOARD                          │
  │                                                         │
  │   ┌──────────┐    ┌──────────┐    ┌──────────────────┐  │
  │   │   CPU    │◄──►│   RAM    │    │  Storage (SSD)   │  │
  │   │ (brain)  │    │(fast mem)│    │  (slow memory)   │  │
  │   └────┬─────┘    └──────────┘    └──────────────────┘  │
  │        │                                                 │
  │        │  (via chipset / buses)                          │
  │        ▼                                                 │
  │   ┌──────────┐    ┌──────────┐    ┌──────────┐          │
  │   │  USB     │    │  GPU     │    │ Network  │          │
  │   │ Controller    │(graphics)│    │   Card   │          │
  │   └──────────┘    └──────────┘    └──────────┘          │
  └─────────────────────────────────────────────────────────┘

  The CPU itself is a tiny rectangle of silicon.
  All those other chips are what make a COMPUTER.
```

### Real CPU Examples

| CPU | Used In | Core Count | Clock Speed |
|-----|---------|-----------|-------------|
| Intel Core i7 | Laptops, desktops | 8-24 cores | 3-5 GHz |
| AMD Ryzen 9 | Gaming PCs, workstations | 12-32 cores | 3-5 GHz |
| Apple M4 | Mac, iPad | 10-12 cores | ~3.9 GHz |
| ARM Cortex-A53 | Raspberry Pi 3 | 4 cores | 1.4 GHz |

Notice: all these need external RAM chips, storage chips, USB controllers, etc. bolted on separately.

---

## Part 2 — What Is a Microcontroller?

A **Microcontroller (MCU)** is a CPU plus all the essential peripherals **baked into a single chip**:

```
  A Microcontroller Chip (one small black chip):

  ┌─────────────────────────────────────────────────────────┐
  │                 STM32F405 Microcontroller                │
  │                                                         │
  │  ┌───────────┐  ┌───────────┐  ┌───────────────────┐   │
  │  │  CPU Core │  │  Flash    │  │  SRAM (192 KB)    │   │
  │  │ Cortex-M4 │  │ (1 MB)    │  │  (your program's  │   │
  │  │  168 MHz  │  │(your code)│  │   variables live) │   │
  │  └─────┬─────┘  └───────────┘  └───────────────────┘   │
  │        │                                                 │
  │        │  Internal buses connect everything              │
  │        ▼                                                 │
  │  ┌──────────────────────────────────────────────────┐   │
  │  │               Built-in Peripherals               │   │
  │  │                                                  │   │
  │  │  [GPIO]  [UART×3]  [SPI×3]  [I2C×3]  [CAN]     │   │
  │  │  [ADC×3] [DAC×2]   [Timers×14] [USB OTG]        │   │
  │  │  [DMA×2] [RTC]     [Watchdog]  [CRC unit]       │   │
  │  └──────────────────────────────────────────────────┘   │
  │                                                         │
  │  All of this is on ONE chip, about the size of a        │
  │  postage stamp (14mm × 14mm)                            │
  └─────────────────────────────────────────────────────────┘
```

### Real Microcontroller Examples

| MCU | Flash | RAM | Peripherals | Used In |
|-----|-------|-----|-------------|---------|
| ATmega328P | 32 KB | 2 KB | Basic GPIO, UART | Arduino Uno |
| ESP32 | 4 MB ext | 520 KB | WiFi, BT, I2C, SPI | IoT projects |
| STM32F103 | 64 KB | 20 KB | Many timers, USB | Blue Pill boards |
| STM32F405 | 1 MB | 192 KB | FPU, USB OTG, DMA | Flight controllers |
| STM32H743 | 2 MB | 1 MB | FPU, DSP, Ethernet | Pro flight controllers |

---

## Part 3 — CPU vs Microcontroller Side by Side

```
  ┌──────────────────────────────────────────────────────────────┐
  │                    CPU vs MCU Comparison                     │
  ├───────────────────────┬──────────────────────────────────────┤
  │         CPU           │         Microcontroller              │
  ├───────────────────────┼──────────────────────────────────────┤
  │ Just the processor    │ Processor + memory + peripherals     │
  │ Needs external RAM    │ Has RAM built-in (KB range)          │
  │ Needs storage chip    │ Has Flash built-in (KB-MB range)     │
  │ Needs USB controller  │ USB built-in (most STM32s)           │
  │ Runs full OS (Linux)  │ Runs bare-metal or RTOS              │
  │ GHz clock speeds      │ MHz clock speeds (1-500 MHz)         │
  │ Watts of power        │ Milliwatts of power                  │
  │ Complex, expensive    │ Simple, cheap ($1-$15)               │
  │ Boot time: seconds    │ Boot time: milliseconds              │
  │ Hard real-time: NO    │ Hard real-time: YES                  │
  ├───────────────────────┼──────────────────────────────────────┤
  │ Good for:             │ Good for:                            │
  │ Video processing      │ Reading sensors                      │
  │ Machine learning      │ Controlling motors                   │
  │ Running web servers   │ Timing-critical control              │
  │ UI applications       │ Low power devices                    │
  └───────────────────────┴──────────────────────────────────────┘
```

### The "Real-Time" Difference — Why It Matters for Drones

A CPU running Linux has an OS that decides what runs when. The OS can pause your program at any moment to do its own housekeeping. You cannot guarantee that a specific task runs at exactly 1ms intervals.

```
  CPU (Linux) — Non-deterministic timing:

  time ─────────────────────────────────────────────►
        Task A  │OS│  Task A  │OS│ Task A  │GC│ Task A
                └─┘          └─┘          └─┘
                 ^ OS paused             ^ Garbage
                   your task               collector ran
  
  Your 1ms loop might actually take: 0.8ms, 1.2ms, 0.9ms, 5ms ← OOPS!
  
  ─────────────────────────────────────────────────────────
  
  MCU (STM32 bare metal) — Deterministic timing:

  time ─────────────────────────────────────────────►
        Loop   Loop   Loop   Loop   Loop   Loop
        1.0ms  1.0ms  1.0ms  1.0ms  1.0ms  1.0ms ← perfectly regular
  
  Nothing can interrupt this unless YOU explicitly allow it.
  This is "hard real-time."
```

### Where Does Each Fit in a Drone?

```
  Professional/Advanced Drone System:

  ┌──────────────────────────────────────────────────────────┐
  │                                                          │
  │  ┌───────────────────────┐  ┌──────────────────────────┐ │
  │  │   Companion Computer  │  │    Flight Controller     │ │
  │  │  (CPU-based)          │  │    (MCU-based)           │ │
  │  │                       │  │                          │ │
  │  │  Raspberry Pi / Jetson│  │  STM32F405 + Betaflight  │ │
  │  │  ─────────────────    │  │  ──────────────────────  │ │
  │  │  ✓ Camera processing  │  │  ✓ 1kHz PID loop         │ │
  │  │  ✓ Object detection   │  │  ✓ IMU sensor fusion     │ │
  │  │  ✓ Path planning      │  │  ✓ Motor PWM output      │ │
  │  │  ✓ WiFi/4G comms      │  │  ✓ RC input decode       │ │
  │  │  ✓ Logs to SD card    │  │                          │ │
  │  │                       │  │                          │ │
  │  │  NOT good for:        │  │  NOT good for:           │ │
  │  │  ✗ Hard real-time     │  │  ✗ Complex AI/ML         │ │
  │  │  ✗ Tight timing loops │  │  ✗ Running Linux         │ │
  │  └───────────┬───────────┘  └──────────────────────────┘ │
  │              │ MAVLink / UART                  ▲          │
  │              └─────────────────────────────────┘          │
  │              High-level commands flow down ───►           │
  │              Status/telemetry flows up   ◄───            │
  └──────────────────────────────────────────────────────────┘
```

---

## Part 4 — Ways of Handling Tasks

This is the most practically important section. When your microcontroller needs to do multiple things — read a sensor, blink an LED, listen for RC input, drive motors — how does it handle all of that?

There are **5 fundamental approaches**, each with different trade-offs.

---

### Method 1: Polling (Busy-Wait)

**The concept:** The CPU constantly checks if something has happened, in a loop, over and over.

**Analogy:** You're waiting for a friend to text you. You stare at your phone screen every second checking. You can't do anything else while waiting.

```
  Polling in code:

  while (1) {
      if (button_is_pressed()) {   // ← CPU checks every loop
          do_something();
      }
      if (uart_has_data()) {       // ← CPU checks every loop
          read_uart();
      }
      if (sensor_ready()) {        // ← CPU checks every loop
          read_sensor();
      }
      // repeat forever
  }
```

```
  CPU Timeline — Polling:

  time ──────────────────────────────────────────────────────►
  
  [Check btn?NO][Check uart?NO][Check sensor?NO][Check btn?NO]...
  [Check btn?NO][Check uart?NO][Check sensor?YES!][Process]...
  [Check btn?NO][Check uart?NO][Check sensor?NO]...
  
  CPU is busy 100% of the time — mostly doing useless checking.
```

**Pros:**
- Dead simple to understand and write
- Predictable execution order
- No setup needed

**Cons:**
- Wastes CPU cycles doing nothing useful
- Response time depends on where in the loop you are (latency is variable)
- Can miss fast events (if checking too slowly)

**When to use:** Small, simple programs. When you're learning. When only one thing needs attention.

**In drone context:** Acceptable for slow things like checking battery voltage every second. Never acceptable for motor control timing.

---

### Method 2: Interrupts

**The concept:** Instead of the CPU checking, the hardware itself taps the CPU on the shoulder when something happens. The CPU stops what it's doing, handles the event, then resumes.

**Analogy:** You put your phone down and work normally. When your friend texts, your phone buzzes. You stop, read the message, reply, then go back to work. The phone (hardware) notified you — you didn't have to keep checking.

```
  Interrupt-Driven Flow:

  Normal program:      ──────────────────────────────────────►
                             ↑ IRQ fires here
                             │
  Interrupt fires ───────────┘
  CPU saves its work (registers pushed to stack)
  CPU jumps to ISR (Interrupt Service Routine)
  
  ISR executes:                [~~~fast code~~~]
                                               │
                                               └── ISR returns
  
  Normal program resumes: ────────────────────────────────────►
  (picks up exactly where it stopped)
```

```c
// Example: ESP32 button interrupt

volatile bool button_pressed = false;   // volatile! Changed in ISR

// This function is called automatically by hardware:
void IRAM_ATTR button_isr(void) {
    button_pressed = true;    // Just set a flag — keep ISRs SHORT
}

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                    button_isr,
                    FALLING);   // trigger on button press
}

void loop() {
    if (button_pressed) {
        button_pressed = false;
        Serial.println("Button was pressed!");
        // do your processing here, outside the ISR
    }
    // CPU is free to do other things while waiting
}
```

#### ISR Golden Rules

```
  ┌─────────────────────────────────────────────────────────┐
  │              ISR RULES — NEVER BREAK THESE              │
  ├─────────────────────────────────────────────────────────┤
  │                                                         │
  │  1. Keep ISRs SHORT                                     │
  │     Set a flag. Do the real work in main loop.          │
  │     ISRs block all other processing while running.      │
  │                                                         │
  │  2. NO blocking calls inside an ISR                     │
  │     ✗ delay(), HAL_Delay(), vTaskDelay()               │
  │     ✗ printf(), Serial.print()                          │
  │     ✗ malloc(), new                                     │
  │                                                         │
  │  3. Mark shared variables as volatile                   │
  │     volatile uint8_t flag;                              │
  │     Tells compiler: don't cache this in a register,     │
  │     always read from memory (ISR may have changed it)   │
  │                                                         │
  │  4. Protect multi-byte variables                        │
  │     Reading a 32-bit value across 2 bus cycles          │
  │     can be interrupted halfway → corrupt data           │
  │     Solution: disable interrupts while reading          │
  │                                                         │
  └─────────────────────────────────────────────────────────┘
```

#### Interrupt Priority (STM32 NVIC)

STM32 has a **Nested Vectored Interrupt Controller (NVIC)** — it manages which interrupt wins when two fire at the same time.

```
  Priority levels (lower number = higher priority):

  Priority 0  ←── HIGHEST (reserved for critical system use)
  Priority 1  ←── IMU data ready interrupt (must respond fast)
  Priority 2  ←── Timer 1ms flight control loop tick
  Priority 3  ←── UART receive (RC input)
  Priority 4  ←── SPI transaction complete
  ...
  Priority 15 ←── LOWEST (background, non-critical)

  If Priority-2 ISR is running and Priority-1 fires:
  → Priority-1 preempts Priority-2 (runs immediately)
  → After Priority-1 finishes, Priority-2 resumes
  
  This is "nested" interrupts — hence "Nested VIC"
```

**Pros:**
- Extremely fast response to events
- CPU is free between events (can sleep to save power)
- Response time is predictable and bounded

**Cons:**
- More complex to write correctly
- Shared data between ISR and main code needs careful handling
- Hard to debug (ISR can fire at any time, hard to reproduce bugs)

**In drone context:** Everything time-critical uses interrupts — IMU data ready, timer for control loop, RC signal decoding, ESC signal generation.

---

### Method 3: DMA (Direct Memory Access)

**The concept:** A special hardware unit that moves data between peripherals and memory **without CPU involvement at all**. The CPU sets it up, starts it, and goes off to do other work. DMA fires an interrupt only when done.

**Analogy:** You need 500 documents moved from Room A to Room B. 

- **Without DMA:** You carry each document yourself. You're busy the entire time.
- **With DMA:** You hire a courier service. You hand them the task ("move 500 docs"), go back to your desk, and they interrupt you only when done.

```
  Without DMA — CPU does all the work:

  time ──────────────────────────────────────────────────────►
  
  [Read byte 1 from SPI][Store to RAM][Read byte 2][Store]...
  [Read byte 14 from SPI][Store to RAM] ← CPU busy all 14 bytes
  
  CPU is occupied for the entire SPI transfer.
  
  ─────────────────────────────────────────────────────────────
  
  With DMA — CPU sets it up and walks away:

  CPU:  [Configure DMA][Start DMA][──────FREE to do PID math──]
                                                               ↑ DMA interrupt: "done!"
  DMA:               [b1][b2][b3][b4]...[b14] → RAM directly
  
  CPU gets 14 bytes of SPI data while doing other useful work.
```

```
  DMA in a flight controller — reading IMU via SPI:

  1ms Loop Start
      │
      ├── CPU tells DMA: "Read 14 bytes from SPI1, store at imu_buf[]"
      │   DMA starts running in background ──────────────────────────┐
      │                                                               │
      ├── CPU runs PID math (using LAST loop's sensor data)          │
      │                                                               │
      ├── CPU updates motor PWM output                               │
      │                                                               │
      └── DMA interrupt fires: "IMU data is ready in imu_buf[]" ◄────┘
  
  1ms Loop End → use fresh imu_buf[] data next loop
  
  Net result: SPI + PID math happen in PARALLEL. 
  If SPI takes 0.1ms, that 0.1ms is NOT wasted CPU time.
```

```c
// STM32 HAL: SPI + DMA (simplified)

uint8_t imu_rx_buf[14];   // DMA writes here directly

void start_imu_read(void) {
    HAL_SPI_Receive_DMA(&hspi1, imu_rx_buf, 14);
    // CPU is now FREE. DMA handles the transfer.
}

// This ISR fires automatically when DMA finishes:
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    // imu_rx_buf[] now has fresh data
    parse_imu_data(imu_rx_buf);
}
```

#### What DMA Can Move

```
  DMA Transfer Types:

  Memory → Memory:    memcpy() style, but done by hardware
  Peripheral → Memory: SPI RX, UART RX, ADC readings → buffer
  Memory → Peripheral: SPI TX, UART TX, DAC output ← buffer

  DMA channels on STM32F405:
  ┌──────────────────────────────────────────────┐
  │  DMA1: 7 streams, handles low-speed peripherals│
  │  DMA2: 8 streams, handles SPI1, ADC, SDIO    │
  │                                               │
  │  Each stream has 4 channels (which peripheral)│
  │  Configured by you in CubeMX or by hand       │
  └──────────────────────────────────────────────┘
```

**Pros:**
- CPU does zero work during data transfer
- Transfers continue even while CPU sleeps
- Essential for high-speed data (SPI at 10MHz, ADC at high sample rates)

**Cons:**
- Setup is complex (configure channels, priorities, circular mode, etc.)
- Cache coherency issues on some chips (data in cache vs memory differ)
- Debugging DMA problems is hard

**In drone context:** IMU reads via SPI-DMA, motor signal generation via timer-DMA, UART telemetry transmit via DMA. All professional flight controller firmware uses DMA extensively.

---

### Method 4: Bare-Metal Superloop

**The concept:** No OS, no scheduler, no tasks. One big `while(1)` loop runs forever. You — the programmer — decide the order and timing of everything.

```
  Bare-Metal Superloop Architecture:

  main()
    │
    ├── initialize everything
    │
    └── while(1) {                 ← runs forever
            read_sensors();        ← ~0.1ms
            run_pid();             ← ~0.2ms
            update_motors();       ← ~0.05ms
            check_battery();       ← ~0.01ms (every Nth loop)
            handle_telemetry();    ← ~0.1ms (every Nth loop)
        }                          ← total: ~0.5ms per loop
  
  Interrupts fire in the background and set flags.
  The main loop reads those flags and acts on them.
```

```c
// Bare-metal superloop example:

volatile bool imu_ready = false;    // Set by SPI DMA interrupt
volatile uint32_t tick_1ms = 0;     // Set by 1ms Timer interrupt

void main(void) {
    hardware_init();
    
    while (1) {
        // Wait for 1ms timer tick (set by timer ISR)
        if (tick_1ms) {
            tick_1ms = 0;
            
            // All flight-critical work here:
            if (imu_ready) {
                imu_ready = false;
                read_imu();
                sensor_fusion();
                pid_update();
                motor_output();
            }
        }
        
        // Non-critical work (runs whenever CPU is free):
        if (telemetry_timer_expired()) {
            send_telemetry();
        }
    }
}
```

**Pros:**
- Maximum control — you know exactly when everything runs
- Deterministic — great for hard real-time
- No OS overhead (every CPU cycle belongs to your code)
- Easiest to reason about for simple systems

**Cons:**
- Manually manage priorities ("which thing runs first?")
- Hard to add features later (everything is tangled together)
- No concurrency — one thing runs at a time
- Timing gets complex when many tasks need different rates

**In drone context:** Betaflight's inner flight loop is essentially a bare-metal superloop timed by a hardware interrupt. The fastest, most critical code lives here.

---

### Method 5: RTOS (Real-Time Operating System)

**The concept:** A lightweight OS designed for microcontrollers. You write multiple **tasks** (like threads), and the RTOS scheduler runs them in a controlled, time-sliced fashion.

**Analogy:** A restaurant kitchen.
- **Bare-metal:** One chef who does everything sequentially: chops, cooks, plates. Simple but slow.
- **RTOS:** Multiple chefs, each with their specialty. A head chef (scheduler) tells everyone when to work. They share equipment (resources) without stepping on each other.

```
  RTOS Task Model (FreeRTOS on ESP32/STM32):

  ┌──────────────────────────────────────────────────────────┐
  │                      RTOS Scheduler                      │
  │               (runs every 1ms by default)                │
  ├──────────────────────────────────────────────────────────┤
  │                                                          │
  │  Task 1: Flight Control       Priority: 5 (HIGHEST)     │
  │  ┌────────────────────────┐   Runs every: 1ms           │
  │  │ IMU → PID → PWM output │   Stack: 1024 bytes         │
  │  └────────────────────────┘                             │
  │                                                          │
  │  Task 2: Telemetry            Priority: 3               │
  │  ┌────────────────────────┐   Runs every: 100ms         │
  │  │ Serialize → UART send  │   Stack: 512 bytes          │
  │  └────────────────────────┘                             │
  │                                                          │
  │  Task 3: Battery Monitor      Priority: 2               │
  │  ┌────────────────────────┐   Runs every: 1000ms        │
  │  │ ADC read → alarm check │   Stack: 256 bytes          │
  │  └────────────────────────┘                             │
  │                                                          │
  │  Task 4: LED Blinky           Priority: 1 (LOWEST)      │
  │  ┌────────────────────────┐   Runs: when nothing else   │
  │  │ Toggle status LED      │   Stack: 128 bytes          │
  │  └────────────────────────┘                             │
  └──────────────────────────────────────────────────────────┘
```

#### RTOS Scheduling — How It Works

```
  Preemptive scheduling timeline:

  time ──────────────────────────────────────────────────────►

  Task1 (P5): [runs]          [runs]          [runs]
              │    │          │    │          │    │
  Task2 (P3): │    [ru─PREEMPTED by T1]       │    │
              │    │                          │    │
  Task3 (P2): │    │    [runs─────────]       │    │
              │    │                          │    │
  Idle  (P0): │    │                   [idle] │    │

  "Preempted" means: T1's timer fires, it has higher priority,
  so it immediately interrupts T2. T2 resumes after T1 is done.
```

#### RTOS Communication Tools

Tasks need to share data without corrupting each other. RTOS provides:

```
  ┌─────────────────────────────────────────────────────────┐
  │                RTOS Communication Primitives            │
  ├────────────────┬────────────────────────────────────────┤
  │  Queues        │ Pass data between tasks safely         │
  │                │ Like a mailbox: Task1 writes,          │
  │                │ Task2 reads when ready                  │
  ├────────────────┼────────────────────────────────────────┤
  │  Semaphores    │ Protect shared resources               │
  │                │ "Only 1 task can use SPI at a time"    │
  │                │ Like a bathroom key — take it, use it, │
  │                │ return it                               │
  ├────────────────┼────────────────────────────────────────┤
  │  Mutexes       │ Like semaphore but with priority        │
  │                │ inheritance (prevents priority          │
  │                │ inversion deadlocks)                    │
  ├────────────────┼────────────────────────────────────────┤
  │  Event Groups  │ Signal multiple tasks at once           │
  │                │ "IMU ready" bit → triggers sensor       │
  │                │ fusion task                             │
  ├────────────────┼────────────────────────────────────────┤
  │  Task Notify   │ Lightweight signal to one specific task │
  │                │ Faster than semaphore for simple cases  │
  └────────────────┴────────────────────────────────────────┘
```

```c
// FreeRTOS example on ESP32 — 2 tasks sharing data via queue

QueueHandle_t sensor_queue;

// Task 1: Reads IMU, sends data to queue
void imu_task(void *pvParams) {
    imu_data_t data;
    while (1) {
        read_imu(&data);
        xQueueSend(sensor_queue, &data, 0);    // send to queue
        vTaskDelayUntil(..., pdMS_TO_TICKS(1)); // wait for next 1ms tick
    }
}

// Task 2: Receives from queue, runs PID
void pid_task(void *pvParams) {
    imu_data_t data;
    while (1) {
        // Block here until imu_task sends data:
        if (xQueueReceive(sensor_queue, &data, portMAX_DELAY)) {
            run_pid(&data);
            update_motors();
        }
    }
}

void app_main(void) {
    sensor_queue = xQueueCreate(1, sizeof(imu_data_t));
    xTaskCreate(imu_task, "imu",  2048, NULL, 5, NULL); // priority 5
    xTaskCreate(pid_task, "pid",  2048, NULL, 5, NULL); // priority 5
}
```

**Pros:**
- Clean separation of concerns (each task does one thing)
- Easy to add features (add a new task, set its priority)
- RTOS handles timing, priority, blocking — you focus on logic
- Scales well as complexity grows

**Cons:**
- RTOS has overhead (~1-5% CPU for scheduler, RAM for each task's stack)
- Concurrency bugs (race conditions, deadlocks) are subtle and hard to find
- More complex than a simple loop
- Non-deterministic if priorities are wrong

**In drone context:** ESP32 companion computer code (WiFi telemetry, GPS, logging) benefits hugely from RTOS. For the STM32 flight loop itself, most firmware (Betaflight) uses a bare-metal superloop for the critical inner loop, and RTOS-inspired patterns for lower-priority work.

---

## Part 5 — All Five Methods Compared

```
  ┌─────────────────────────────────────────────────────────────────┐
  │                  Task Handling Method Comparison                │
  ├──────────────┬──────────┬───────────┬──────────────┬───────────┤
  │  Method      │Complexity│ CPU Usage │ Response Time│ Use When  │
  ├──────────────┼──────────┼───────────┼──────────────┼───────────┤
  │ Polling      │ Very Low  │  100%     │ Variable     │ Learning  │
  │              │           │ (wasteful)│ (loop-bound) │ Simple IO │
  ├──────────────┼──────────┼───────────┼──────────────┼───────────┤
  │ Interrupts   │ Medium    │  Low      │ Microseconds │ Events,   │
  │              │           │ (efficient│ (very fast)  │ Timing,   │
  │              │           │ when idle)│              │ Always!   │
  ├──────────────┼──────────┼───────────┼──────────────┼───────────┤
  │ DMA          │ High      │  Minimal  │ Zero (async) │ High-speed│
  │              │           │ (hardware │              │ data: SPI │
  │              │           │ does work)│              │ UART, ADC │
  ├──────────────┼──────────┼───────────┼──────────────┼───────────┤
  │ Superloop    │ Low-Med   │  ~100%    │ Predictable  │ Hard RT   │
  │ (bare-metal) │           │ (busy but │ (deterministic│ Flight   │
  │              │           │ useful)   │              │ control   │
  ├──────────────┼──────────┼───────────┼──────────────┼───────────┤
  │ RTOS         │ High      │  Low-Med  │ Priority-    │ Multiple  │
  │              │           │ (overhead │ based        │ features  │
  │              │           │ for sched)│              │ IoT, apps │
  └──────────────┴──────────┴───────────┴──────────────┴───────────┘
```

---

## Part 6 — Real Flight Controller Architecture (Putting It Together)

Here's how a professional flight controller (like Betaflight on STM32F405) combines all these methods:

```
  ┌──────────────────────────────────────────────────────────────┐
  │          STM32F405 Flight Controller — Full Architecture     │
  ├──────────────────────────────────────────────────────────────┤
  │                                                              │
  │  HARDWARE EVENTS (Interrupts):                              │
  │  ┌─────────────────────────────────────────────────────┐   │
  │  │ 1kHz Timer ISR ──────────────────────────────────┐  │   │
  │  │ (sets "run loop" flag)                           │  │   │
  │  │                                                  │  │   │
  │  │ SPI DMA Complete ISR → IMU data in buffer ───────┤  │   │
  │  │ (DMA read IMU data without CPU help)             │  │   │
  │  │                                                  │  │   │
  │  │ UART ISR → RC receiver data (SBUS) ──────────────┤  │   │
  │  └──────────────────────────────────────────────────┘  │   │
  │                                   │ flags set           │   │
  │                                   ▼                     │   │
  │  MAIN SUPERLOOP (bare-metal, no RTOS):                  │   │
  │  ┌─────────────────────────────────────────────────┐   │   │
  │  │ while(1) {                                      │   │   │
  │  │   if (loop_flag) {      // 1ms timer fired      │   │   │
  │  │     loop_flag = false;                          │   │   │
  │  │                                                 │   │   │
  │  │     // DMA already filled imu_buf[]             │   │   │
  │  │     gyro = parse_gyro(imu_buf);    // POLLING   │   │   │
  │  │     accel = parse_accel(imu_buf);  // the flag  │   │   │
  │  │                                                 │   │   │
  │  │     attitude = mahony_filter(gyro, accel, dt);  │   │   │
  │  │                                                 │   │   │
  │  │     rc = get_rc_channels();   // from UART ISR  │   │   │
  │  │                                                 │   │   │
  │  │     roll_out  = pid(&roll_pid,  ...);           │   │   │
  │  │     pitch_out = pid(&pitch_pid, ...);           │   │   │
  │  │     yaw_out   = pid(&yaw_pid,   ...);           │   │   │
  │  │                                                 │   │   │
  │  │     motor_mix(rc.throttle, roll_out, ...);      │   │   │
  │  │     write_dshot(motor_values);   // DMA output  │   │   │
  │  │                                                 │   │   │
  │  │     // Trigger NEXT DMA read for next loop:     │   │   │
  │  │     start_imu_dma_read();                       │   │   │
  │  │   }                                             │   │   │
  │  │                                                 │   │   │
  │  │   // Background (non-critical, runs when free): │   │   │
  │  │   handle_msp_telemetry();   // config protocol  │   │   │
  │  │   write_blackbox_log();     // data logging     │   │   │
  │  │   update_osd();             // on-screen display│   │   │
  │  │ }                                               │   │   │
  │  └─────────────────────────────────────────────────┘   │   │
  │                                                         │   │
  │  Used in combination:                                   │   │
  │  ✓ Interrupts — timer tick, UART receive, SPI done     │   │
  │  ✓ DMA — SPI IMU reads, DShot motor output, UART TX   │   │
  │  ✓ Superloop — main flight control logic               │   │
  │  ✓ Polling — check flags set by ISRs                   │   │
  └──────────────────────────────────────────────────────────┘
```

---

## Part 7 — Practical Exercises

### Exercise 1: Polling vs Interrupt (ESP32)

Write the same program two ways — blink an LED when a button is pressed:

```c
// WAY 1: Polling (simple, CPU-hungry)
void loop() {
    if (digitalRead(BUTTON) == LOW) {
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
    }
    // What else could you do here while waiting? Nothing — you're stuck checking.
}

// WAY 2: Interrupt (efficient)
volatile bool pressed = false;

void IRAM_ATTR btn_isr() { pressed = true; }

void setup() {
    attachInterrupt(digitalPinToInterrupt(BUTTON), btn_isr, FALLING);
}

void loop() {
    if (pressed) {
        pressed = false;
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
    }
    // CPU can do OTHER things here between button presses
    do_other_work();
}
```

**Question:** In Way 2, if the button is pressed during `do_other_work()`, will it be detected?
**Answer:** Yes — the interrupt fires regardless of what the main loop is doing, sets the flag, and `loop()` will catch it on the next iteration.

### Exercise 2: Calculate DMA Benefit

Your flight controller reads 14 bytes from the IMU via SPI at 10 MHz.

```
  SPI transfer time = (14 bytes × 8 bits) / 10,000,000 bits/sec
                    = 112 / 10,000,000
                    = 0.0000112 seconds
                    = 11.2 microseconds

  Without DMA: CPU is occupied for 11.2μs per IMU read
  With DMA:    CPU is free during those 11.2μs

  In a 1000Hz loop (1ms per loop):
  Without DMA: 11.2μs / 1000μs = 1.12% of CPU time wasted on SPI
  
  Doesn't sound like much? At 1kHz with 4 IMU axes:
  11.2μs × 1000 loops/sec = 11,200μs = 11.2ms/sec of CPU wasted on SPI alone
  
  That's 1.12% of your CPU. On a 168MHz MCU, that's 1,881,600 wasted instructions/sec.
  DMA reclaims all of them.
```

---

## Quick Reference Summary

```
  ┌─────────────────────────────────────────────────────────────┐
  │               CPU vs MCU — Quick Reference                  │
  ├───────────────────┬─────────────────────────────────────────┤
  │  CPU              │ Brain only. Needs RAM, storage, IO      │
  │                   │ chips separately. Runs full OS. GHz.    │
  ├───────────────────┼─────────────────────────────────────────┤
  │  Microcontroller  │ Brain + memory + peripherals in one.    │
  │                   │ MHz. Hard real-time. $1-$15. Tiny.      │
  └───────────────────┴─────────────────────────────────────────┘

  ┌─────────────────────────────────────────────────────────────┐
  │               5 Ways to Handle Tasks                        │
  ├────────────────┬────────────────────────────────────────────┤
  │ 1. Polling     │ CPU keeps checking. Simple. Wasteful.      │
  │ 2. Interrupts  │ Hardware notifies CPU. Fast. Efficient.    │
  │ 3. DMA         │ Hardware moves data. CPU free. Fast.       │
  │ 4. Superloop   │ One loop, you control order. Predictable.  │
  │ 5. RTOS        │ Multiple tasks, scheduler. Scalable.       │
  └────────────────┴────────────────────────────────────────────┘

  Real systems combine all five:
  Interrupt → sets flag → DMA reads data → superloop processes it
  → RTOS task sends result over WiFi
```

> **Next step:** Module 03 — ESP32 deep dive. You'll use interrupts and timers to build a precise PWM generator for controlling a servo motor without `delay()`.

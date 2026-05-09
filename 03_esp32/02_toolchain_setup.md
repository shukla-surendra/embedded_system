# 02 — Toolchain Setup

You need two things to program the ESP32:
1. A way to **write code** (the Arduino IDE)
2. A way to **upload code** to the board (USB driver + board support package)

---

## Option A — Arduino IDE (Recommended for Beginners)

The Arduino IDE is the easiest way to get started. It handles compilation and uploading in one click.

### Step 1 — Install Arduino IDE 2

Download from: https://www.arduino.cc/en/software

Choose the installer for your OS (Windows/Linux/macOS). Install it normally.

### Step 2 — Add ESP32 Board Support

The Arduino IDE doesn't support ESP32 out of the box. You add it via the Board Manager:

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Find "Additional boards manager URLs" and paste:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click OK
5. Go to **Tools → Board → Boards Manager**
6. Search for "esp32" — find the one by **Espressif Systems**
7. Click **Install** (downloads ~200MB — takes a few minutes)

### Step 3 — Install the USB Driver

Your ESP32 DevKit uses one of two USB-Serial chips. Check your board or try both:

**CP2102 driver (most common):**
- Download from Silicon Labs: search "CP210x USB to UART Bridge VCP Drivers"

**CH340 driver (common on cheap boards):**
- Download from WCH: search "CH340 driver"

**On Linux:** Usually no driver needed. Add yourself to the `dialout` group:
```bash
sudo usermod -aG dialout $USER
```
Then log out and back in.

**On macOS (M1/M2):** The CP2102 driver may need Rosetta. If the CH340 driver doesn't work, try the Silicon Labs driver.

### Step 4 — Select Your Board and Port

1. Plug in your ESP32 via USB
2. In Arduino IDE: **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
3. **Tools → Port** — select the port that appeared when you plugged in
   - Windows: something like `COM3` or `COM5`
   - Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`
   - macOS: `/dev/cu.SLAB_USBtoUART` or `/dev/cu.usbserial-...`

If no port appears after plugging in:
- Try a different USB cable (many are charge-only — no data wires)
- Try a different USB port
- Check Device Manager (Windows) or `dmesg | tail -20` (Linux) for the device

### Step 5 — Configure Upload Settings

In **Tools** menu, set:
```
Board:           ESP32 Dev Module
Upload Speed:    921600
CPU Frequency:   240MHz (WiFi/BT)
Flash Size:      4MB (32Mb)
Partition Scheme: Default 4MB with spiffs
```

---

## Option B — VS Code + PlatformIO (Intermediate)

If you're comfortable with a terminal and want a better coding experience:

1. Install **VS Code** from https://code.visualstudio.com
2. Install the **PlatformIO IDE** extension (search in VS Code extensions)
3. Create a new project → select board "Espressif ESP32 Dev Module"
4. PlatformIO automatically downloads the toolchain

Benefits over Arduino IDE:
- Better code completion and error checking
- Handles library dependencies cleanly
- Supports multiple projects/configurations

---

## Your First Upload Test

Before writing any code, test that the toolchain works:

### Open the Blink example:
**File → Examples → 01.Basics → Blink**

This opens a program that blinks the built-in LED.

### Change the LED pin (important for ESP32):
The example uses `LED_BUILTIN`. On most ESP32 DevKit boards this is GPIO2. Confirm by changing the code to:

```cpp
#define LED_PIN 2   // GPIO2 = onboard blue LED on most DevKit boards
```

### Upload:
Click the **→ Upload** button (right arrow) or press `Ctrl+U`.

You'll see:
```
Connecting....
Chip is ESP32-D0WD-V3 (revision 3)
...
Writing at 0x00001000... (8 %)
Writing at 0x00008000... (16 %)
...
Hash of data verified.
Leaving...
Hard resetting via RTS pin...
```

After upload completes, the onboard LED should start blinking once per second.

### Troubleshooting upload failures

| Error | Fix |
|-------|-----|
| `Failed to connect to ESP32: Timed out` | Hold BOOT button while uploading starts, release after "Connecting..." appears |
| `A fatal error occurred: Could not open /dev/ttyUSB0` | Wrong port selected, or no permission (Linux: add to dialout group) |
| `No module named serial` | Install pyserial: `pip install pyserial` |
| Upload succeeds but LED doesn't blink | Wrong LED pin — try GPIO2 and GPIO22 |

---

## Understanding the Upload Process

When you click Upload, the Arduino IDE:
1. **Compiles** your C++ code into machine code (binary)
2. Puts the ESP32 into **download mode** (GPIO0 → LOW, then reset)
3. Sends the binary over USB-Serial at high baud rate
4. The ESP32's ROM bootloader writes it to Flash
5. Releases GPIO0 and resets — your program starts running

---

## Serial Monitor

The Serial Monitor is your window into the ESP32's mind — it shows text your program sends via `Serial.println()`.

Open it: **Tools → Serial Monitor** (or `Ctrl+Shift+M`)

Set baud rate to **115200** (must match what your code uses).

You'll use Serial Monitor constantly for debugging — it's your `print()` for hardware.

---

## Practice

1. Install the Arduino IDE and ESP32 board support.
2. Upload the Blink example. Confirm the LED blinks.
3. Change the blink speed to 100ms on, 100ms off. Upload again. Observe the difference.
4. Open the Serial Monitor. Add `Serial.begin(115200);` in setup() and `Serial.println("Hello!");` in loop(). Upload and confirm you see "Hello!" repeating.

---

**Next:** [03 — Hello World: Blink](03_blink.md)

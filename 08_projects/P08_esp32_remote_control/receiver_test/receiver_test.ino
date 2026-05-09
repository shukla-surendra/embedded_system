/*
  ESP32 Drone Remote Control — Receiver / Drone Side
  ────────────────────────────────────────────────────
  Purpose:
    Receives DronePacket from the transmitter via NRF24L01.
    Prints values to Serial Monitor so you can verify the
    link is working before connecting to motors/ESCs.

  Wiring (same NRF24L01 pinout as transmitter):
    CE=4  CSN=5  SCK=18  MOSI=23  MISO=19  VCC=3.3V  GND

  Libraries needed:
    • RF24 by TMRh20

  Failsafe:
    If no packet arrives for 500ms, motors are commanded to zero
    and the drone disarms. This prevents a runaway drone if
    the radio link is lost.

  Next step:
    Replace the Serial.printf at the bottom of loop() with calls
    to your PID flight controller using the received values.
*/

#include <SPI.h>
#include <RF24.h>

#define PIN_NRF_CE     4
#define PIN_NRF_CSN    5
#define RADIO_CHANNEL 108     // Must match transmitter exactly
#define FAILSAFE_MS   500     // Disarm if no packet for this long

// Must be identical to the struct in transmitter.ino
struct DronePacket {
  uint16_t throttle;
  int16_t  roll;
  int16_t  pitch;
  int16_t  yaw;
  uint8_t  armed;
  uint8_t  mode;
};

RF24 radio(PIN_NRF_CE, PIN_NRF_CSN);
const byte PIPE_ADDR[6] = "DRONE";

DronePacket pkt        = {0, 0, 0, 0, 0, 0};
unsigned long lastPktTime = 0;
bool failsafe          = true;
uint32_t packetsRx     = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP32 Drone Receiver ===");

  if (!radio.begin()) {
    Serial.println("NRF24L01 not found — check wiring and power supply");
    while (true) { delay(500); }
  }

  radio.openReadingPipe(1, PIPE_ADDR);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(RADIO_CHANNEL);
  radio.startListening();

  Serial.println("Receiver ready — waiting for transmitter...");
  Serial.println("Columns: THR  ROLL  PITCH  YAW  ARMED  MODE");
}

void loop() {
  // Drain all available packets (use the newest one)
  while (radio.available()) {
    radio.read(&pkt, sizeof(pkt));
    lastPktTime = millis();
    failsafe    = false;
    packetsRx++;
  }

  // Trigger failsafe if signal lost
  if (millis() - lastPktTime > FAILSAFE_MS) {
    if (!failsafe) {
      Serial.println("!!! FAILSAFE — signal lost !!!");
    }
    failsafe      = true;
    pkt.armed     = 0;
    pkt.throttle  = 0;
    pkt.roll = pkt.pitch = pkt.yaw = 0;
  }

  // Print at ~20 Hz so Serial Monitor is readable
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 50) {
    lastPrint = millis();

    if (failsafe) {
      Serial.print("FAILSAFE  ");
    } else {
      Serial.printf("RX#%lu  ", packetsRx);
    }

    Serial.printf("T:%4u  R:%4d  P:%4d  Y:%4d  ARM:%d  MODE:%d\n",
      pkt.throttle, pkt.roll, pkt.pitch, pkt.yaw,
      pkt.armed, pkt.mode);
  }

  // ─────────────────────────────────────────────────────────────────
  // TODO: Replace the Serial output above with your flight controller
  //
  //   if (pkt.armed && !failsafe) {
  //     pidUpdate(pkt.throttle, pkt.roll, pkt.pitch, pkt.yaw);
  //   } else {
  //     motorsOff();
  //   }
  // ─────────────────────────────────────────────────────────────────
}

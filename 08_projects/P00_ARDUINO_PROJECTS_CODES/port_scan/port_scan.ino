#include <Wire.h>

#define SDA_PIN 18
#define SCL_PIN 19

void setup() {
  Serial.begin(115200);

  delay(1000);

  Serial.println("\nESP32 I2C Scanner");

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.print("SDA: ");
  Serial.println(SDA_PIN);

  Serial.print("SCL: ");
  Serial.println(SCL_PIN);
  while (true){
    scanI2C();
    delay(3000);
  }
  
}

void loop() {
}

void scanI2C() {
  byte error, address;
  int devices = 0;

  Serial.println("Scanning...");

  for(address = 1; address < 127; address++) {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if(error == 0) {
      Serial.print("I2C device found at 0x");

      if(address < 16)
        Serial.print("0");

      Serial.println(address, HEX);

      devices++;
    }
    else if(error == 4) {
      Serial.print("Unknown error at 0x");

      if(address < 16)
        Serial.print("0");

      Serial.println(address, HEX);
    }
  }

  if(devices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.println("Done");
  }
}
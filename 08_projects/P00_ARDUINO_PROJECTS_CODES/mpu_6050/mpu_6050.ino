#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// ======================================================
// SENSOR OBJECTS
// ======================================================

// MPU6050 and BMP280 use Adafruit libraries.
// LSM303DLHC is driven directly via Wire — no extra library needed.

Adafruit_MPU6050 mpu;
Adafruit_BMP280  bmp;

// ======================================================
// I2C ADDRESSES
// ======================================================

#define MPU_ADDR       0x68
#define BMP_ADDR       0x76   // 0x77 if SDO is HIGH
#define LSM_ACCEL_ADDR 0x19
#define LSM_MAG_ADDR   0x1E

// ======================================================
// CONSTANTS
// ======================================================

#define SEA_LEVEL_HPA   1013.25
#define GYRO_INTERVAL   10      // gyro integration: every 10ms = 100Hz
#define DISPLAY_INTERVAL 1000   // screen refresh: every 1 second

// ======================================================
// MPU6050 VARIABLES
// ======================================================

float gyroAngleX    = 0;
float gyroAngleY    = 0;
unsigned long lastGyroTime    = 0;
unsigned long lastDisplayTime = 0;

// Raw ADC values — registers 0x3B to 0x48
// AccelX, AccelY, AccelZ, Temp, GyroX, GyroY, GyroZ — 2 bytes each, high first
int16_t rawMpuAx, rawMpuAy, rawMpuAz;
int16_t rawMpuGx, rawMpuGy, rawMpuGz;
int16_t rawMpuTemp;

// ======================================================
// BMP280 VARIABLES
// ======================================================

float baseAltitude = 0;
float prevAltitude = 0;
bool  baseSet      = false;

// Raw ADC — registers 0xF7-0xFC (20-bit pressure then 20-bit temp)
// These are pre-compensation counts; chip applies trim data to get hPa / C
int32_t rawBmpPressure = 0;
int32_t rawBmpTemp     = 0;

// ======================================================
// LSM303DLHC VARIABLES
// ======================================================

// Accelerometer raw — registers 0x28-0x2D (auto-increment via 0xA8)
// 16-bit, left-aligned; right-shift 4 gives 12-bit value
int16_t rawLsmAx, rawLsmAy, rawLsmAz;

// Magnetometer raw — registers 0x03-0x08
// IMPORTANT: register order is X, Z, Y (not X, Y, Z!)
int16_t rawMagX, rawMagY, rawMagZ;

// ======================================================
// READ MPU6050 RAW REGISTERS
// ======================================================

void readMpuRaw()
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  rawMpuAx   = (Wire.read() << 8) | Wire.read();
  rawMpuAy   = (Wire.read() << 8) | Wire.read();
  rawMpuAz   = (Wire.read() << 8) | Wire.read();
  rawMpuTemp = (Wire.read() << 8) | Wire.read();
  rawMpuGx   = (Wire.read() << 8) | Wire.read();
  rawMpuGy   = (Wire.read() << 8) | Wire.read();
  rawMpuGz   = (Wire.read() << 8) | Wire.read();
}

// ======================================================
// READ BMP280 RAW REGISTERS
// ======================================================

void readBmpRaw()
{
  Wire.beginTransmission(BMP_ADDR);
  Wire.write(0xF7);
  Wire.endTransmission(false);
  Wire.requestFrom(BMP_ADDR, 6, true);

  uint8_t p_msb  = Wire.read(), p_lsb  = Wire.read(), p_xlsb = Wire.read();
  uint8_t t_msb  = Wire.read(), t_lsb  = Wire.read(), t_xlsb = Wire.read();

  rawBmpPressure = ((int32_t)p_msb << 12) | ((int32_t)p_lsb << 4) | (p_xlsb >> 4);
  rawBmpTemp     = ((int32_t)t_msb << 12) | ((int32_t)t_lsb << 4) | (t_xlsb >> 4);
}

// ======================================================
// INITIALISE LSM303DLHC  (direct I2C — no library needed)
// ======================================================
// Accelerometer (0x19):
//   CTRL_REG1_A (0x20) = 0x57  -> 100 Hz, XYZ on
//   CTRL_REG4_A (0x23) = 0x08  -> high-res, +/-2g
// Magnetometer (0x1E):
//   CRA_REG_M   (0x00) = 0x10  -> 15 Hz data rate
//   CRB_REG_M   (0x01) = 0x20  -> +/-1.3 gauss gain
//   MR_REG_M    (0x02) = 0x00  -> continuous conversion

void initLSM303()
{
  Wire.beginTransmission(LSM_ACCEL_ADDR);
  Wire.write(0x20); Wire.write(0x57);
  Wire.endTransmission();

  Wire.beginTransmission(LSM_ACCEL_ADDR);
  Wire.write(0x23); Wire.write(0x08);
  Wire.endTransmission();

  Wire.beginTransmission(LSM_MAG_ADDR);
  Wire.write(0x00); Wire.write(0x10);
  Wire.endTransmission();

  Wire.beginTransmission(LSM_MAG_ADDR);
  Wire.write(0x01); Wire.write(0x20);
  Wire.endTransmission();

  Wire.beginTransmission(LSM_MAG_ADDR);
  Wire.write(0x02); Wire.write(0x00);
  Wire.endTransmission();
}

// ======================================================
// READ LSM303 ACCELEROMETER RAW REGISTERS
// ======================================================
// Registers 0x28-0x2D via auto-increment (0x28 | 0x80 = 0xA8).
// Data is 16-bit left-aligned; right-shift 4 gives 12-bit counts.

void readLsmAccelRaw()
{
  Wire.beginTransmission(LSM_ACCEL_ADDR);
  Wire.write(0xA8);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM_ACCEL_ADDR, 6, true);

  uint8_t xl = Wire.read(), xh = Wire.read();
  uint8_t yl = Wire.read(), yh = Wire.read();
  uint8_t zl = Wire.read(), zh = Wire.read();

  rawLsmAx = (int16_t)((xh << 8) | xl) >> 4;
  rawLsmAy = (int16_t)((yh << 8) | yl) >> 4;
  rawLsmAz = (int16_t)((zh << 8) | zl) >> 4;
}

// ======================================================
// READ LSM303 MAGNETOMETER RAW REGISTERS
// ======================================================
// Registers 0x03-0x08.
// IMPORTANT: chip register order is X, Z, Y — NOT X, Y, Z.

void readLsmMagRaw()
{
  Wire.beginTransmission(LSM_MAG_ADDR);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM_MAG_ADDR, 6, true);

  rawMagX = (int16_t)((Wire.read() << 8) | Wire.read());
  rawMagZ = (int16_t)((Wire.read() << 8) | Wire.read());
  rawMagY = (int16_t)((Wire.read() << 8) | Wire.read());
}

// ======================================================
// CONVERT LSM303 RAW TO PHYSICAL UNITS
// ======================================================
// Accel: +/-2g high-res mode -> 1 mg/LSB (datasheet table 3)
//   m/s² = raw * 0.001 * 9.81
// Mag: +/-1.3 gauss gain (CRB=0x20):
//   XY: 1100 LSB/Gauss,  Z: 980 LSB/Gauss
//   uT  = gauss * 100

void convertLsmValues(float &ax, float &ay, float &az,
                      float &mx, float &my, float &mz)
{
  ax = rawLsmAx * 0.001f * 9.81f;
  ay = rawLsmAy * 0.001f * 9.81f;
  az = rawLsmAz * 0.001f * 9.81f;

  mx = (rawMagX / 1100.0f) * 100.0f;
  my = (rawMagY / 1100.0f) * 100.0f;
  mz = (rawMagZ /  980.0f) * 100.0f;
}

// ======================================================
// TILT-COMPENSATED COMPASS HEADING
// ======================================================
// Simple heading only works when sensor is flat.
// Using MPU6050 roll/pitch here corrects for tilt.

float computeHeading(float magX, float magY, float magZ,
                     float rollDeg, float pitchDeg)
{
  float roll_r  = rollDeg  * PI / 180.0;
  float pitch_r = pitchDeg * PI / 180.0;

  float xh = magX * cos(pitch_r) + magZ * sin(pitch_r);
  float yh = magX * sin(roll_r)  * sin(pitch_r)
           + magY * cos(roll_r)
           - magZ * sin(roll_r)  * cos(pitch_r);

  float heading = atan2(yh, xh) * 180.0 / PI;
  if (heading < 0) heading += 360.0;
  return heading;
}

// ======================================================
// CARDINAL DIRECTION NAME
// ======================================================

const char* getCardinal(float heading)
{
  if      (heading <  22.5 || heading >= 337.5) return "NORTH";
  else if (heading <  67.5)                      return "NORTH-EAST";
  else if (heading < 112.5)                      return "EAST";
  else if (heading < 157.5)                      return "SOUTH-EAST";
  else if (heading < 202.5)                      return "SOUTH";
  else if (heading < 247.5)                      return "SOUTH-WEST";
  else if (heading < 292.5)                      return "WEST";
  else                                           return "NORTH-WEST";
}

// ======================================================
// BAR INDICATOR — symmetric around zero
// ======================================================
// LEFTLABEL [......#.....|...........] RIGHTLABEL  val unit

void drawBar(Print      &out,
             const char *leftLabel,
             const char *rightLabel,
             float       value,
             float       maxVal,
             const char *unit)
{
  const int barW   = 41;
  const int center = barW / 2;

  float clamped = constrain(value, -maxVal, maxVal);
  int   pos     = center + (int)((clamped / maxVal) * (float)center);
  pos = constrain(pos, 0, barW - 1);

  out.print(leftLabel);
  out.print(" [");
  for (int i = 0; i < barW; i++)
  {
    if      (i == pos && i == center) out.print('O');
    else if (i == center)             out.print('|');
    else if (i == pos)                out.print('#');
    else                              out.print('.');
  }
  out.print("] ");
  out.print(rightLabel);
  out.print("  ");
  out.print(value, 1);
  out.print(" ");
  out.println(unit);
}

// ======================================================
// BAR INDICATOR — 0 to maxVal range (for heading 0-360)
// ======================================================

void drawBarFull(Print      &out,
                 const char *leftLabel,
                 const char *rightLabel,
                 float       value,
                 float       maxVal,
                 const char *unit)
{
  const int barW = 41;
  int pos = (int)((value / maxVal) * (float)barW);
  pos = constrain(pos, 0, barW - 1);

  out.print(leftLabel);
  out.print(" [");
  for (int i = 0; i < barW; i++)
  {
    if (i == pos) out.print('#');
    else          out.print('.');
  }
  out.print("] ");
  out.print(rightLabel);
  out.print("  ");
  out.print(value, 1);
  out.print(" ");
  out.println(unit);
}

// ======================================================
// COMPASS SECTION  (LSM303DLHC)
// ======================================================

void drawCompassSection(Print &out,
                        float heading,
                        float magX,  float magY,  float magZ,
                        float lsmAx, float lsmAy, float lsmAz)
{
  const char* cardinal = getCardinal(heading);

  // Octant: 0=N, 1=NE, 2=E, 3=SE, 4=S, 5=SW, 6=W, 7=NW
  int octant = (int)((heading + 22.5) / 45.0) % 8;

  out.println();
  out.println("------------------------------------------");
  out.println("COMPASS  (LSM303DLHC Magnetometer)");
  out.println("------------------------------------------");
  out.println();

  out.print("  Heading   : "); out.print(heading, 1);
  out.print(" deg  ->  "); out.println(cardinal);
  out.println("  (tilt-compensated using MPU6050 roll/pitch)");
  out.println();

  // ---- Heading bar 0-360 ----
  out.println("  Heading Bar  (0 = North, 90 = East, 180 = South, 270 = West):");
  out.print("  ");
  drawBarFull(out, "N(0)", "N(360)", heading, 360.0, "deg");

  // ---- 3x3 Compass Rose ----
  // Grid layout:
  //  [NW]  [N]  [NE]
  //   [W]  [+]   [E]
  //  [SW]  [S]  [SE]
  // Active octant is highlighted with * markers.

  out.println();
  out.println("  Compass Rose  (* = current direction):");
  out.println();

  // octant mapping for 3x3 grid (row, col):
  // (0,0)=NW=7  (0,1)=N=0   (0,2)=NE=1
  // (1,0)=W=6   (1,1)=ctr   (1,2)=E=2
  // (2,0)=SW=5  (2,1)=S=4   (2,2)=SE=3

  const int grid[3][3] = {{7, 0, 1}, {6, -1, 2}, {5, 4, 3}};
  const char* label[8] = {"N ", "NE", "E ", "SE", "S ", "SW", "W ", "NW"};

  for (int row = 0; row < 3; row++)
  {
    out.print("    ");
    for (int col = 0; col < 3; col++)
    {
      int idx = grid[row][col];
      if (idx == -1)
      {
        out.print(" [+] ");   // fixed center
      }
      else if (idx == octant)
      {
        out.print("*");
        out.print(label[idx]);
        out.print("* ");      // active direction
      }
      else
      {
        out.print(" ");
        out.print(label[idx]);
        out.print("  ");
      }
    }
    out.println();
  }

  out.println();
  out.print("  Pointing: "); out.println(cardinal);

  // ---- Magnetic field values ----
  out.println();
  out.println("  Magnetic Field (raw magnetometer data):");
  out.print("  Mag X : "); out.print(magX, 2); out.println(" uT");
  out.print("  Mag Y : "); out.print(magY, 2); out.println(" uT");
  out.print("  Mag Z : "); out.print(magZ, 2); out.println(" uT");

  // ---- LSM303 accelerometer ----
  out.println();
  out.println("  LSM303 Accelerometer (compare with MPU6050 above):");
  out.print("  Accel X : "); out.print(lsmAx, 2); out.println(" m/s^2");
  out.print("  Accel Y : "); out.print(lsmAy, 2); out.println(" m/s^2");
  out.print("  Accel Z : "); out.print(lsmAz, 2); out.println(" m/s^2");

  // ---- Raw ADC values ----
  out.println();
  out.println("  LSM303 Raw ADC counts (before library conversion):");
  out.println("    Accel (+/-2G, 12-bit left-aligned, reg 0x28-0x2D):");
  out.print  ("    Raw AX : "); out.println(rawLsmAx);
  out.print  ("    Raw AY : "); out.println(rawLsmAy);
  out.print  ("    Raw AZ : "); out.println(rawLsmAz);
  out.println("    Mag (regs 0x03-0x08, order X,Z,Y not X,Y,Z):");
  out.print  ("    Raw Mag X : "); out.println(rawMagX);
  out.print  ("    Raw Mag Y : "); out.println(rawMagY);
  out.print  ("    Raw Mag Z : "); out.println(rawMagZ);
}

// ======================================================
// ALTITUDE SECTION  (BMP280)
// ======================================================

void drawAltitudeSection(Print &out,
                         float  altitude,
                         float  relAlt,
                         float  pressure,
                         float  bmpTemp)
{
  float altChange = altitude - prevAltitude;

  out.println();
  out.println("------------------------------------------");
  out.println("ALTITUDE  (BMP280 Barometer)");
  out.println("------------------------------------------");
  out.println();

  out.print("  Absolute : "); out.print(altitude, 1); out.println(" m  (above sea level)");

  if (baseSet)
  {
    out.print("  Home     : "); out.print(baseAltitude, 1); out.println(" m");
    out.print("  Relative : ");
    if (relAlt >= 0) out.print("+");
    out.print(relAlt, 1);
    out.println(" m  (from home)");

    out.println();
    out.println("  Relative Altitude Bar  (+/- 50 m from home):");
    out.print  ("  ");
    drawBar(out, "DOWN -50m", "+50m UP", relAlt, 50.0, "m");
  }
  else
  {
    out.println("  Home : not set  (type B to set ground level)");
  }

  out.println();
  out.print("  Pressure : "); out.print(pressure, 2); out.println(" hPa");
  out.print("  BMP Temp : "); out.print(bmpTemp, 1);  out.println(" C");

  out.println();
  out.print("  Trend : ");
  if      (altChange >  0.3) { out.print("CLIMBING   ^^^ (+"); out.print(altChange, 1); out.println(" m)"); }
  else if (altChange < -0.3) { out.print("DESCENDING vvv ("); out.print(altChange, 1); out.println(" m)"); }
  else                         out.println("STABLE   ---");

  // ASCII altimeter
  out.println();
  if (baseSet)
  {
    if (relAlt > 10)
    {
      out.print  ("    "); out.print(altitude, 0); out.println(" m  <- HERE");
      out.println("    |");
      out.print  ("    "); out.print(baseAltitude, 0); out.println(" m  [HOME]");
    }
    else if (relAlt < -10)
    {
      out.print  ("    "); out.print(baseAltitude, 0); out.println(" m  [HOME]");
      out.println("    |");
      out.print  ("    "); out.print(altitude, 0); out.println(" m  <- HERE (below home)");
    }
    else
    {
      out.print("    "); out.print(altitude, 0); out.println(" m  <- HERE  (near home level)");
    }
  }
  else
  {
    out.print("    "); out.print(altitude, 0); out.println(" m  (type B to set home)");
  }

  // BMP280 raw values
  out.println();
  out.println("  BMP280 Raw ADC (20-bit, before calibration trim):");
  out.print  ("    Raw Pressure : "); out.println(rawBmpPressure);
  out.print  ("    Raw Temp     : "); out.println(rawBmpTemp);
}

// ======================================================
// FULL SCHEMATIC DISPLAY
// ======================================================
// Uses Print& so the output can be redirected to any stream.

void drawSchematic(Print &out,
                   float roll,    float pitch,   float yawRate,
                   float altitude, float relAlt,
                   float pressure, float bmpTemp,
                   float heading,
                   float magX,    float magY,    float magZ,
                   float lsmAx,   float lsmAy,   float lsmAz)
{
  out.println();
  out.println("+==========================================+");
  out.println("|   MPU6050 + BMP280 + LSM303  DASHBOARD  |");
  out.println("+==========================================+");

  // ---- MPU6050 orientation bars ----

  out.println();
  out.println("MPU6050 ORIENTATION   (# = sensor,  | = level)");
  out.println();
  drawBar(out, "ROLL  LEFT", "RIGHT", roll,    90.0,  "deg");
  drawBar(out, "PITCH BACK", "FRONT", pitch,   90.0,  "deg");
  drawBar(out, "YAW    CCW", "CW   ", yawRate, 200.0, "deg/s");

  // ---- Front view: Roll ----

  out.println();
  out.println("------------------------------------------");
  out.println("FRONT VIEW  (Roll = Left/Right tilt)");
  out.println("------------------------------------------");
  out.println();

  if (roll > 30)
  {
    out.println("       UP");
    out.println("        \\");
    out.println("   ______\\______");
    out.println("  | SENSOR     |    <- EXTREME RIGHT");
    out.print  ("     "); out.print((int)roll); out.println(" deg");
  }
  else if (roll > 10)
  {
    out.println("      UP");
    out.println("       \\");
    out.println("  ______\\_____");
    out.println("  | SENSOR   |    <- Tilted RIGHT");
    out.print  ("    "); out.print((int)roll); out.println(" deg");
  }
  else if (roll < -30)
  {
    out.println("             UP");
    out.println("            /");
    out.println("   _________/___");
    out.println("  |     SENSOR |    <- EXTREME LEFT");
    out.print  ("     "); out.print((int)roll); out.println(" deg");
  }
  else if (roll < -10)
  {
    out.println("           UP");
    out.println("          /");
    out.println("  ________/____");
    out.println("  |   SENSOR  |    <- Tilted LEFT");
    out.print  ("    "); out.print((int)roll); out.println(" deg");
  }
  else
  {
    out.println("       UP");
    out.println("       |");
    out.println("  _____|_____");
    out.println("  | SENSOR  |    <- LEVEL");
    out.println("  |_________|");
    out.println("       |");
    out.println("     DOWN");
    out.print  ("    "); out.print((int)roll); out.println(" deg");
  }

  // ---- Side view: Pitch ----

  out.println();
  out.println("------------------------------------------");
  out.println("SIDE VIEW  (Pitch = Forward/Backward tilt)");
  out.println("------------------------------------------");
  out.println();

  if (pitch > 30)
  {
    out.println("  UP (nose up)");
    out.println("  /");
    out.println("  /__________");
    out.println(" |  SENSOR  |    <- EXTREME FORWARD");
    out.print  ("    "); out.print((int)pitch); out.println(" deg");
  }
  else if (pitch > 10)
  {
    out.println("  UP");
    out.println("  /");
    out.println("  /_________");
    out.println(" | SENSOR  |    <- Tilted FORWARD");
    out.print  ("    "); out.print((int)pitch); out.println(" deg");
  }
  else if (pitch < -30)
  {
    out.println("  ___________");
    out.println(" |  SENSOR  |    <- EXTREME BACKWARD");
    out.println("             \\");
    out.println("              \\");
    out.println("             DOWN (nose down)");
    out.print  ("    "); out.print((int)pitch); out.println(" deg");
  }
  else if (pitch < -10)
  {
    out.println("  __________");
    out.println(" | SENSOR  |    <- Tilted BACKWARD");
    out.println("             \\");
    out.println("            DOWN");
    out.print  ("    "); out.print((int)pitch); out.println(" deg");
  }
  else
  {
    out.println("  ___________");
    out.println(" |  SENSOR  |    <- LEVEL");
    out.println(" |__________|");
    out.print  ("    "); out.print((int)pitch); out.println(" deg");
  }

  // ---- Top view: Yaw ----

  out.println();
  out.println("------------------------------------------");
  out.println("TOP VIEW  (Yaw = spinning flat)");
  out.println("------------------------------------------");
  out.println();
  out.println("      NORTH");
  out.println("        ^");
  out.println("        |");
  out.println(" WEST <-+-> EAST");
  out.println("        |");
  out.println("        v");
  out.println("      SOUTH");
  out.println();

  if      (yawRate >  10) { out.print("  Spinning CLOCKWISE  (");         out.print(yawRate, 1);  out.println(" deg/s)"); }
  else if (yawRate < -10) { out.print("  Spinning COUNTER-CLOCKWISE  ("); out.print(-yawRate, 1); out.println(" deg/s)"); }
  else                      out.println("  Yaw STABLE");

  // ---- MPU6050 raw values ----

  out.println();
  out.println("------------------------------------------");
  out.println("MPU6050 RAW ADC VALUES");
  out.println("------------------------------------------");
  out.println("  Accel (regs 0x3B-0x40)  +/-8G = 4096 LSB per G:");
  out.print  ("  AX="); out.print(rawMpuAx);
  out.print  ("  AY="); out.print(rawMpuAy);
  out.print  ("  AZ="); out.println(rawMpuAz);
  out.println("  Gyro  (regs 0x43-0x48)  +/-500 deg/s = 65.5 LSB per deg/s:");
  out.print  ("  GX="); out.print(rawMpuGx);
  out.print  ("  GY="); out.print(rawMpuGy);
  out.print  ("  GZ="); out.println(rawMpuGz);
  out.print  ("  MPU Temp raw="); out.print(rawMpuTemp);
  float mpuTempC = (rawMpuTemp / 340.0) + 36.53;
  out.print  ("  ("); out.print(mpuTempC, 1); out.println(" C)");
  out.print  ("  Gyro integrated: Roll="); out.print(gyroAngleX, 1);
  out.print  (" deg   Pitch="); out.print(gyroAngleY, 1); out.println(" deg");

  // ---- Compass section ----
  drawCompassSection(out, heading, magX, magY, magZ, lsmAx, lsmAy, lsmAz);

  // ---- Altitude section ----
  drawAltitudeSection(out, altitude, relAlt, pressure, bmpTemp);

  out.println();
  out.println("+==========================================+");
  out.println("  B = reset home altitude");
  out.println("+==========================================+");
}

// ======================================================
// SETUP
// ======================================================

void setup()
{
  Serial.begin(115200);

  Serial.println("\n====================================");
  Serial.println("MPU6050 + BMP280 + LSM303 DASHBOARD");
  Serial.println("====================================");

  // All three sensors share one I2C bus: SDA=18, SCL=19
  Wire.begin(18, 19);

  // ---- MPU6050 ----
  if (!mpu.begin())
  {
    Serial.println("MPU6050 NOT DETECTED - check wiring!");
    while (1);
  }
  Serial.println("MPU6050  OK  (0x68)");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // ---- BMP280 ----
  if (!bmp.begin(BMP_ADDR))
  {
    Serial.println("BMP280 NOT DETECTED - try #define BMP_ADDR 0x77");
    while (1);
  }
  Serial.println("BMP280   OK  (0x76)");
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  baseAltitude = bmp.readAltitude(SEA_LEVEL_HPA);
  prevAltitude = baseAltitude;
  baseSet      = true;
  Serial.print("Home altitude: "); Serial.print(baseAltitude, 1); Serial.println(" m");

  // ---- LSM303DLHC (direct I2C, no Adafruit library) ----
  initLSM303();

  // Verify accel is responding by checking its WHO_AM_I-equivalent:
  // CTRL_REG1_A should read back 0x57 after we wrote it.
  Wire.beginTransmission(LSM_ACCEL_ADDR);
  Wire.write(0x20);
  Wire.endTransmission(false);
  Wire.requestFrom(LSM_ACCEL_ADDR, 1, true);
  uint8_t accelCheck = Wire.read();
  if (accelCheck != 0x57)
  {
    Serial.print("LSM303 ACCEL NOT RESPONDING (0x19) - got 0x");
    Serial.println(accelCheck, HEX);
    Serial.println("Check wiring!");
    while (1);
  }
  Serial.println("LSM303 Accel OK  (0x19)");
  Serial.println("LSM303 Mag   OK  (0x1E)  (direct I2C)");

  // ---- Startup reference ----
  Serial.println();
  Serial.println("====================================");
  Serial.println("  SENSOR REFERENCE");
  Serial.println("====================================");
  Serial.println();
  Serial.println("  MPU6050 (orientation) addr 0x68:");
  Serial.println("    ROLL  = tips Left/Right");
  Serial.println("    PITCH = tips Forward/Back");
  Serial.println("    YAW   = spins flat");
  Serial.println();
  Serial.println("  BMP280 (altitude) addr 0x76:");
  Serial.println("    Reads air pressure -> altitude");
  Serial.println("    ~1 m accuracy in stable air");
  Serial.println();
  Serial.println("  LSM303DLHC (compass + accel) - direct I2C:");
  Serial.println("    Accel addr: 0x19  (no library, raw Wire reads)");
  Serial.println("    Mag   addr: 0x1E");
  Serial.println("    Heading = tilt-compensated using MPU6050");
  Serial.println();
  Serial.println("  WIRING (all on same I2C bus):");
  Serial.println("    SDA -> GPIO 18");
  Serial.println("    SCL -> GPIO 19");
  Serial.println();
  Serial.println("  COMMAND:  B = reset home altitude");
  Serial.println("  Refresh every 1 second.");
  Serial.println("====================================");
  Serial.println();

  lastGyroTime    = millis();
  lastDisplayTime = millis();
}

// ======================================================
// LOOP
// ======================================================

void loop()
{
  unsigned long now = millis();

  // ---- Serial commands ----
  if (Serial.available())
  {
    char cmd = toupper(Serial.read());
    if (cmd == 'B')
    {
      baseAltitude = bmp.readAltitude(SEA_LEVEL_HPA);
      baseSet      = true;
      Serial.print("[ALT] Home reset to ");
      Serial.print(baseAltitude, 1);
      Serial.println(" m");
    }
  }

  // ---- Gyro integration at 100Hz ----
  // Runs continuously and separately from the 1-sec display.

  if (now - lastGyroTime >= GYRO_INTERVAL)
  {
    float dt     = (now - lastGyroTime) / 1000.0;
    lastGyroTime = now;

    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);

    gyroAngleX += g.gyro.x * 57.2958 * dt;
    gyroAngleY += g.gyro.y * 57.2958 * dt;
  }

  // ---- Full display every 1 second ----

  if (now - lastDisplayTime >= DISPLAY_INTERVAL)
  {
    lastDisplayTime = now;

    // --- MPU6050 ---
    sensors_event_t mpuAccel, mpuGyro, mpuTemp;
    mpu.getEvent(&mpuAccel, &mpuGyro, &mpuTemp);
    readMpuRaw();

    float gz = mpuGyro.gyro.z * 57.2958;

    float accelRoll  = atan2(mpuAccel.acceleration.y,
                             mpuAccel.acceleration.z) * 57.2958;
    float accelPitch = atan2(-mpuAccel.acceleration.x,
                              mpuAccel.acceleration.z) * 57.2958;

    // --- BMP280 ---
    float altitude = bmp.readAltitude(SEA_LEVEL_HPA);
    float pressure = bmp.readPressure() / 100.0;
    float bmpTemp  = bmp.readTemperature();
    float relAlt   = altitude - baseAltitude;
    readBmpRaw();

    // --- LSM303DLHC ---
    readLsmAccelRaw();
    readLsmMagRaw();

    float lsmAx, lsmAy, lsmAz;
    float magX,  magY,  magZ;
    convertLsmValues(lsmAx, lsmAy, lsmAz, magX, magY, magZ);

    float heading = computeHeading(magX, magY, magZ, accelRoll, accelPitch);

    // --- Draw everything to Serial ---
    drawSchematic(Serial,
                  accelRoll, accelPitch, gz,
                  altitude, relAlt, pressure, bmpTemp,
                  heading, magX, magY, magZ,
                  lsmAx, lsmAy, lsmAz);

    // Update altitude trend reference
    prevAltitude = altitude;
  }
}

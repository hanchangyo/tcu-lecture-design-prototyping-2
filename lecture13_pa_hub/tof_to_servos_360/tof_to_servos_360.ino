// 第13回: Unit PaHub 経由で ToF → Servo Kit 360° (CH0/CH1)
//
// ハードウェア構成:
//   M5Stack Core S3
//     └ Port A (I2C, SDA=G2 / SCL=G1)
//         └ Unit PaHub (I2C アドレス 0x70)
//             ├ PaHub CH0 ← Unit 8Servos
//             │              ├ CH0: Servo Kit 360°
//             │              └ CH1: Servo Kit 360°
//             └ PaHub CH1 ← Unit ToF (VL53L0X)
//
// 動作:
//   - ToF で測った距離に応じて CH0 / CH1 の連続回転サーボの速度を変える
//   - 物体が近い   → 回転速度 大 (パルス 500 µs 付近)
//   - 物体が遠い   → 停止       (パルス 1500 µs)
//   - 測定エラー時 → 停止
//
// 必要ライブラリ (Arduino IDE → ライブラリマネージャ):
//   - M5Unified
//   - M5Unit-8Servo
//   - VL53L0X (Pololu)
//
// 配線メモ:
//   - PaHub の実際の CH 番号が異なる場合は下記 PAHUB_CH_* を変更
//   - PaHub v2.1 で DIP スイッチを変えた場合は PAHUB_ADDR も変更

#include <M5Unified.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "M5_UNIT_8SERVO.h"

// --- Core S3 Port A (I2C) ---
static constexpr int I2C_SDA = 2;
static constexpr int I2C_SCL = 1;

// --- Unit PaHub (PCA9548A) ---
static constexpr uint8_t PAHUB_ADDR = 0x70;
static constexpr uint8_t PAHUB_CH_SERVOS = 0;  // 8Servos Unit が刺さっている CH
static constexpr uint8_t PAHUB_CH_TOF    = 1;  // ToF Unit が刺さっている CH

// --- 8Servos Unit ---
static constexpr uint8_t SERVO_CH0 = 0;
static constexpr uint8_t SERVO_CH1 = 1;

// --- Servo Kit 360° のパルス幅 (µs) ---
static constexpr uint16_t PULSE_MIN   = 500;   // 時計回り 最大速
static constexpr uint16_t PULSE_STOP  = 1500;  // 停止
static constexpr uint16_t PULSE_MAX   = 2500;  // 反時計回り 最大速
static constexpr uint16_t PULSE_DEADBAND = 25;

// --- ToF → 速度マップ (mm) ---
static constexpr uint16_t DIST_NEAR_MM = 50;   // これ以下 → 最大速
static constexpr uint16_t DIST_STOP_MM = 400; // これ以上 → 停止

M5_UNIT_8SERVO unit_8servo;
VL53L0X tof;

// PaHub の指定チャンネルに I2C バスを切り替える
void selectPaHubChannel(uint8_t channel) {
  Wire.beginTransmission(PAHUB_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

bool isValidDistance(uint16_t mm) {
  return mm > 20 && mm < 2000;
}

// 距離が近いほど速く回転 (時計回り方向)。遠いほど停止へ。
uint16_t distanceToPulse(uint16_t mm) {
  if (!isValidDistance(mm)) {
    return PULSE_STOP;
  }
  if (mm >= DIST_STOP_MM) {
    return PULSE_STOP;
  }
  if (mm <= DIST_NEAR_MM) {
    return PULSE_MIN;
  }
  uint16_t pulse = map(mm, DIST_NEAR_MM, DIST_STOP_MM, PULSE_MIN, PULSE_STOP);
  if (abs((int)pulse - (int)PULSE_STOP) <= (int)PULSE_DEADBAND) {
    pulse = PULSE_STOP;
  }
  return pulse;
}

int speedPercent(uint16_t pulse) {
  if (pulse == PULSE_STOP) return 0;
  if (pulse < PULSE_STOP) {
    return map(pulse, PULSE_MIN, PULSE_STOP - PULSE_DEADBAND, 100, 0);
  }
  return map(pulse, PULSE_STOP + PULSE_DEADBAND, PULSE_MAX, 0, 100);
}

const char* directionLabel(uint16_t pulse) {
  if (pulse == PULSE_STOP) return "STOP";
  if (pulse < PULSE_STOP)  return "CW";
  return "CCW";
}

bool init8Servos() {
  selectPaHubChannel(PAHUB_CH_SERVOS);
  if (!unit_8servo.begin(&Wire, I2C_SDA, I2C_SCL, M5_UNIT_8SERVO_DEFAULT_ADDR)) {
    return false;
  }
  unit_8servo.setAllPinMode(SERVO_CTL_MODE);
  unit_8servo.setServoPulse(SERVO_CH0, PULSE_STOP);
  unit_8servo.setServoPulse(SERVO_CH1, PULSE_STOP);
  return true;
}

bool initToF() {
  selectPaHubChannel(PAHUB_CH_TOF);
  tof.setTimeout(500);
  if (!tof.init()) {
    return false;
  }
  tof.startContinuous();
  return true;
}

uint16_t readDistanceMm() {
  selectPaHubChannel(PAHUB_CH_TOF);
  return tof.readRangeContinuousMillimeters();
}

void setServoPulseBoth(uint16_t pulse) {
  selectPaHubChannel(PAHUB_CH_SERVOS);
  unit_8servo.setServoPulse(SERVO_CH0, pulse);
  unit_8servo.setServoPulse(SERVO_CH1, pulse);
}

void setup() {
  M5.begin();
  M5.Display.setTextSize(2);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.println("PaHub + ToF");
  M5.Display.println("  + 8Servos");

  Wire.begin(I2C_SDA, I2C_SCL);

  if (!init8Servos()) {
    M5.Display.fillScreen(TFT_RED);
    M5.Display.setCursor(10, 10);
    M5.Display.println("8Servos init NG");
    M5.Display.printf("PaHub CH%d?", PAHUB_CH_SERVOS);
    while (true) delay(500);
  }

  if (!initToF()) {
    M5.Display.fillScreen(TFT_RED);
    M5.Display.setCursor(10, 10);
    M5.Display.println("ToF init NG");
    M5.Display.printf("PaHub CH%d?", PAHUB_CH_TOF);
    while (true) delay(500);
  }

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.println("Ready");
}

void loop() {
  M5.update();

  uint16_t distance = readDistanceMm();
  uint16_t pulse    = distanceToPulse(distance);
  int speed_pct     = speedPercent(pulse);

  setServoPulseBoth(pulse);

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.printf("PaHub ToF->360\n");
  M5.Display.printf("  dist : %4d mm\n", distance);
  M5.Display.printf("  valid: %s\n", isValidDistance(distance) ? "yes" : "no");
  M5.Display.printf("Servo CH0/CH1\n");
  M5.Display.printf("  pulse: %4d us\n", pulse);
  M5.Display.printf("  dir  : %s\n", directionLabel(pulse));
  M5.Display.printf("  speed: %3d %%\n", speed_pct);

  delay(50);
}

// 第10回 拡張例: Unit Angle で 8Servos Unit の CH0 / CH1 を制御
//            (連続回転サーボ Servo Kit 360° 向け)
//
// 配線:
//   - Port A (赤, I2C)  ← Unit 8Servos  (SDA=G2, SCL=G1)
//     ・Servo Kit 360° を CH0 と CH1 に接続
//   - Port B (黒, GPIO) ← Unit Angle    (白線=G8 / アナログ入力)
//
// 動作:
//   - Unit Angle (可変抵抗) の回転量を 0〜360° として読み取る
//   - その値に比例して連続回転サーボの速度・方向を変える
//       0°   付近 → 時計回り 最大速 (パルス 500 µs)
//       180° 付近 → 停止           (パルス 1500 µs)
//       360° 付近 → 反時計回り 最大速 (パルス 2500 µs)
//
// 必要ライブラリ (Arduino IDE → ライブラリマネージャ):
//   - M5Unified
//   - M5Unit-8Servo
//
// 参考: M5Stack Servo Kit 360°
//   https://docs.m5stack.com/en/accessory/servo_kit_360

#include <M5Unified.h>
#include <Wire.h>
#include "M5_UNIT_8SERVO.h"

// --- Port B (Core S3): 白線=G8 がアナログ入力 ---
static constexpr int ANGLE_PIN = 8;

// --- 制御するサーボのチャンネル ---
static constexpr uint8_t SERVO_CH0 = 0;
static constexpr uint8_t SERVO_CH1 = 1;

// --- Servo Kit 360° のパルス幅 (µs) ---
static constexpr uint16_t PULSE_MIN   = 500;   // 時計回り 最大速
static constexpr uint16_t PULSE_STOP  = 1500;  // 停止
static constexpr uint16_t PULSE_MAX   = 2500;  // 反時計回り 最大速
static constexpr uint16_t PULSE_DEADBAND = 25; // 停止付近の揺れ防止 (±µs)

M5_UNIT_8SERVO unit_8servo;

// Unit Angle の生値 (0-4095) → 連続回転サーボ用パルス幅 (500-2500 µs)
uint16_t angleToPulse(uint16_t raw) {
  uint16_t pulse = map(raw, 0, 4095, PULSE_MIN, PULSE_MAX);
  if (abs((int)pulse - (int)PULSE_STOP) <= (int)PULSE_DEADBAND) {
    pulse = PULSE_STOP;
  }
  return pulse;
}

// 停止位置からのずれ (%) を表示用に計算 (0 = 停止, 100 = 最大速)
int speedPercent(uint16_t pulse) {
  if (pulse == PULSE_STOP) return 0;
  if (pulse < PULSE_STOP) {
    return map(pulse, PULSE_MIN, PULSE_STOP - PULSE_DEADBAND, 100, 0);
  }
  return map(pulse, PULSE_STOP + PULSE_DEADBAND, PULSE_MAX, 0, 100);
}

const char* directionLabel(uint16_t pulse) {
  if (pulse == PULSE_STOP) return "STOP";
  if (pulse < PULSE_STOP)  return "CW";   // Clockwise
  return "CCW";                           // Counter-clockwise
}

void setup() {
  M5.begin();
  M5.Display.setTextSize(2);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.println("Init 8Servos...");

  // Port A の I2C を G2(SDA) / G1(SCL) で初期化
  while (!unit_8servo.begin(&Wire, 2, 1, M5_UNIT_8SERVO_DEFAULT_ADDR)) {
    M5.Display.fillScreen(TFT_RED);
    M5.Display.setCursor(10, 10);
    M5.Display.println("8Servos not found");
    Serial.println("8Servos not found");
    delay(500);
  }

  unit_8servo.setAllPinMode(SERVO_CTL_MODE);

  // 起動時は停止
  unit_8servo.setServoPulse(SERVO_CH0, PULSE_STOP);
  unit_8servo.setServoPulse(SERVO_CH1, PULSE_STOP);

  // Unit Angle のアナログ入力設定 (ESP32-S3 の ADC を 12bit に)
  pinMode(ANGLE_PIN, INPUT);
  analogReadResolution(12);  // 0 〜 4095

  M5.Display.fillScreen(TFT_BLACK);
}

void loop() {
  M5.update();

  // 1) Unit Angle を読み取る (0 〜 4095)
  uint16_t raw = analogRead(ANGLE_PIN);

  // 2) 入力角度 0 〜 360° (表示用)
  int input_deg = map(raw, 0, 4095, 0, 360);

  // 3) 連続回転サーボ用パルス幅へ変換
  uint16_t pulse = angleToPulse(raw);
  int speed_pct  = speedPercent(pulse);

  // 4) CH0 / CH1 に同じパルス幅を送る
  unit_8servo.setServoPulse(SERVO_CH0, pulse);
  unit_8servo.setServoPulse(SERVO_CH1, pulse);

  // 5) 画面に状態を表示
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.printf("Angle Unit\n");
  M5.Display.printf("  raw : %4d\n", raw);
  M5.Display.printf("  in  : %3d deg\n", input_deg);
  M5.Display.printf("Servo 360 CH0/CH1\n");
  M5.Display.printf("  pulse: %4d us\n", pulse);
  M5.Display.printf("  dir  : %s\n", directionLabel(pulse));
  M5.Display.printf("  speed: %3d %%\n", speed_pct);

  delay(20);  // 50Hz 程度で更新
}

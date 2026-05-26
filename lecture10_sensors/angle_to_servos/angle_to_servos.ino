// 第10回 拡張例: Unit Angle で 8Servos Unit の CH0 / CH1 を制御
//
// 配線:
//   - Port A (赤, I2C)  ← Unit 8Servos  (SDA=G2, SCL=G1)
//     ・サーボは CH0 と CH1 に接続
//   - Port B (黒, GPIO) ← Unit Angle    (白線=G8 / アナログ入力)
//
// 動作:
//   - Unit Angle (可変抵抗) の回転量を ADC で読み取り、概念的に 0〜360°に
//     マップして画面表示する。
//   - 標準サーボの可動域は 0〜180°なので、サーボ角度は 0〜180°に縮めて
//     CH0 / CH1 に同じ値を送る。
//
// 必要ライブラリ (Arduino IDE → ライブラリマネージャ):
//   - M5Unified
//   - M5Unit-8Servo
//
// 補足:
//   - 入力 0〜360°をそのまま反映したい場合は、360°対応サーボ
//     (連続回転サーボなど) を使うか、CH0 で前半 0〜180°、CH1 で後半
//     180〜360°を分担するように loop() を改造してください。

#include <M5Unified.h>
#include <Wire.h>
#include "M5_UNIT_8SERVO.h"

// --- Port B (Core S3): 白線=G8 がアナログ入力 ---
static constexpr int ANGLE_PIN = 8;

// --- 制御するサーボのチャンネル ---
static constexpr uint8_t SERVO_CH0 = 0;
static constexpr uint8_t SERVO_CH1 = 1;

M5_UNIT_8SERVO unit_8servo;

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

  // Unit Angle のアナログ入力設定 (ESP32-S3 の ADC を 12bit に)
  pinMode(ANGLE_PIN, INPUT);
  analogReadResolution(12);  // 0 〜 4095

  M5.Display.fillScreen(TFT_BLACK);
}

void loop() {
  M5.update();

  // 1) Unit Angle を読み取る (0 〜 4095)
  uint16_t raw = analogRead(ANGLE_PIN);

  // 2) 概念上の入力角度 0 〜 360° にマップ (表示用)
  int input_deg = map(raw, 0, 4095, 0, 360);

  // 3) サーボに送る角度 0 〜 180° (ハードウェア上限)
  int servo_deg = map(raw, 0, 4095, 0, 180);
  servo_deg = constrain(servo_deg, 0, 180);

  // 4) CH0 / CH1 に同じ角度を送る
  unit_8servo.setServoAngle(SERVO_CH0, servo_deg);
  unit_8servo.setServoAngle(SERVO_CH1, servo_deg);

  // 5) 画面に状態を表示
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.printf("Angle Unit\n");
  M5.Display.printf("  raw : %4d\n", raw);
  M5.Display.printf("  in  : %3d deg\n", input_deg);
  M5.Display.printf("Servo CH0/CH1\n");
  M5.Display.printf("  out : %3d deg\n", servo_deg);

  delay(20);  // 50Hz 程度で更新
}

// 第10回: ToF 距離センサ + アクチュエータ連携
// 接続: Grove Port A (I2C) に Unit ToF を接続
// 動作:
//   distance < 100mm  → 画面を赤、警告音(2000Hz)
//   distance < 300mm  → 画面を黄色
//   それ以外           → 画面を青

#include <M5Unified.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X tof;

void setup() {
  M5.begin();
  Wire.begin();
  tof.setTimeout(500);
  tof.init();
  tof.startContinuous();
  M5.Display.setTextSize(2);
}

void loop() {
  M5.update();

  uint16_t distance = tof.readRangeContinuousMillimeters();

  if (distance < 100) {
    M5.Display.fillScreen(TFT_RED);
    M5.Speaker.tone(2000, 100);
  } else if (distance < 300) {
    M5.Display.fillScreen(TFT_YELLOW);
  } else {
    M5.Display.fillScreen(TFT_BLUE);
  }

  delay(200);
}

// 第10回: ToF 距離センサ(Unit ToF / VL53L0X)で距離を計測
// 接続: Grove Port A (I2C)
// 必要ライブラリ: Pololu の "VL53L0X"
//   Arduino IDE → ツール → ライブラリを管理 → "VL53L0X" で検索

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
  uint16_t distance = tof.readRangeContinuousMillimeters();

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.printf("Distance:\n");
  M5.Display.printf("%d mm\n", distance);

  delay(200);
}

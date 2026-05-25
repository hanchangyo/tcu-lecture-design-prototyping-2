// 第9回: シリアルモニタの基本
// 動作: 起動時に "Hello, M5Stack!" を 1 回出力し、その後 1 秒ごとに経過秒数を出力
// 確認方法: Arduino IDE → ツール → シリアルモニタ（115200 baud）

#include <M5Unified.h>

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.println("Hello, M5Stack!");
}

void loop() {
  Serial.printf("uptime: %lu s\n", millis() / 1000);
  delay(1000);
}

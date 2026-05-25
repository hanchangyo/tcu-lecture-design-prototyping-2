// 第9回: 「Lチカ」相当 — 画面の色を交互に切り替える
// 動作: 画面が赤と黒で 0.5 秒ごとに切り替わる

#include <M5Unified.h>

void setup() {
  M5.begin();
}

void loop() {
  M5.Display.fillScreen(TFT_RED);
  delay(500);
  M5.Display.fillScreen(TFT_BLACK);
  delay(500);
}

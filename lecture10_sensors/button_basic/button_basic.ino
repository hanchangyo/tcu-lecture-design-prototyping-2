// 第10回: ボタン入力の基本
// 動作: M5ロゴ部分(BtnA)を押すと画面にメッセージが表示される
// 注意: Core S3 には物理 BtnB/BtnC は存在しない

#include <M5Unified.h>

void setup() {
  M5.begin();
  M5.Display.setTextSize(2);
  M5.Display.println("Press the M5 button");
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Button A pressed!");
  }
}

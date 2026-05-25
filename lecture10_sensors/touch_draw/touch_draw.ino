// 第10回: タッチ位置に円を描く簡易お絵描き
// 動作: 画面をタッチした位置に白い小さな円が描かれる

#include <M5Unified.h>

void setup() {
  M5.begin();
  M5.Display.fillScreen(TFT_BLACK);
}

void loop() {
  M5.update();

  auto touch = M5.Touch.getDetail();
  if (touch.wasPressed()) {
    int x = touch.x;
    int y = touch.y;
    M5.Display.fillCircle(x, y, 5, TFT_WHITE);
  }
}

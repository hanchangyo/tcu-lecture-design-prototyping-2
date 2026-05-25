// 第10回: ボタン + 画面タッチで複数入力
// 動作:
//   - BtnA(M5ロゴ): 画面を赤く
//   - 画面左半分タッチ: 画面を緑に
//   - 画面右半分タッチ: 画面を青に

#include <M5Unified.h>

void setup() {
  M5.begin();
  M5.Display.fillScreen(TFT_BLACK);
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    M5.Display.fillScreen(TFT_RED);
  }

  auto t = M5.Touch.getDetail();
  if (t.wasPressed()) {
    if (t.x < 160) {
      M5.Display.fillScreen(TFT_GREEN);
    } else {
      M5.Display.fillScreen(TFT_BLUE);
    }
  }
}

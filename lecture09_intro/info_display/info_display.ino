// 第9回: 情報表示画面の総合例
// テキストサイズ・色・図形描画を組み合わせた静的な情報表示

#include <M5Unified.h>

void setup() {
  M5.begin();
  M5.Display.fillScreen(TFT_BLACK);

  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_CYAN);
  M5.Display.setCursor(60, 10);
  M5.Display.println("My M5Stack");

  M5.Display.drawFastHLine(0, 40, 320, TFT_WHITE);

  M5.Display.setTextSize(1);
  M5.Display.setTextColor(TFT_WHITE);
  M5.Display.setCursor(10, 60);
  M5.Display.println("Name: Taro Yamada");
  M5.Display.setCursor(10, 80);
  M5.Display.println("Date: 2026-04-01");
}

void loop() {
  M5.update();
}

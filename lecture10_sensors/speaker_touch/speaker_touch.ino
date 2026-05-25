// 第10回: タッチ位置で周波数と音量を変化させる簡易テルミン
// 動作: 画面をタッチしている間、X 座標で音程・Y 座標で音量が変わる

#include <M5Unified.h>

void setup() {
  M5.begin();
}

void loop() {
  M5.update();

  auto t = M5.Touch.getDetail();
  if (t.isPressed()) {
    int freq = map(t.x, 0, 320, 200, 2000);
    int vol  = map(t.y, 0, 240, 255, 0);

    M5.Speaker.setVolume(vol);
    M5.Speaker.tone(freq, 100);
  }
}

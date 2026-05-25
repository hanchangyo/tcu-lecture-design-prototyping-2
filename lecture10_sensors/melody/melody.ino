// 第10回: ドレミファソラシド のメロディ再生
// 動作: BtnA(M5ロゴ) を押すたびにメロディを 1 回再生

#include <M5Unified.h>

int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};

void playMelody() {
  for (int i = 0; i < 8; i++) {
    M5.Speaker.tone(melody[i], 300);
    delay(400);
  }
}

void setup() {
  M5.begin();
  M5.Speaker.setVolume(128);
  M5.Display.setTextSize(2);
  M5.Display.println("Press BtnA");
}

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    playMelody();
  }
}

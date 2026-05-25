// 第11回: M5Stack を BLE ペリフェラル化
// 必要ライブラリ: "NimBLE-Arduino"
//   Arduino IDE → ツール → ライブラリを管理 → "NimBLE" で検索
// 動作:
//   - "M5-Sensor" という名前でアドバタイズ
//   - 1 秒ごとに経過秒数 (= millis()/1000) を Notify で送信
//   - 受信側: ../web_bluetooth_receiver/ もしくは nRF Connect アプリで確認

#include <M5Unified.h>
#include <NimBLEDevice.h>

#define SERVICE_UUID "12345678-1234-5678-1234-56789abcdef0"
#define CHAR_UUID    "12345678-1234-5678-1234-56789abcdef1"

NimBLECharacteristic* pChar;

void setup() {
  M5.begin();
  M5.Display.setTextSize(2);

  NimBLEDevice::init("M5-Sensor");
  auto pServer  = NimBLEDevice::createServer();
  auto pService = pServer->createService(SERVICE_UUID);
  pChar = pService->createCharacteristic(
    CHAR_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
  );
  pService->start();
  NimBLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID);
  NimBLEDevice::getAdvertising()->start();

  M5.Display.println("BLE Ready");
  M5.Display.println("Name: M5-Sensor");
}

void loop() {
  M5.update();

  uint32_t value = millis() / 1000;
  String str = String(value);

  pChar->setValue(str.c_str());
  pChar->notify();

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.printf("Send: %s", str.c_str());

  delay(1000);
}

// 第12回: チーム制作のためのプログラム骨格サンプル
//
// 構成:
//   - センサ読み取り(ToF: Grove Port A) → 処理 → 出力(画面表示 + BLE 送信)
//   - チームごとにセンサ・処理・出力を差し替えていく
//
// 必要ライブラリ:
//   - M5Unified
//   - NimBLE-Arduino
//   - VL53L0X (Pololu)
//
// 注意:
//   - ToF を使わない場合は SETUP/LOOP 内の tof 関連をコメントアウトし、
//     readSensors() 内で IMU・ボタンなど別の値に差し替えるとよい。

#include <M5Unified.h>
#include <Wire.h>
#include <NimBLEDevice.h>
#include <VL53L0X.h>

// === BLE 設定 ===
#define SERVICE_UUID "12345678-1234-5678-1234-56789abcdef0"
#define CHAR_UUID    "12345678-1234-5678-1234-56789abcdef1"
NimBLECharacteristic* pChar = nullptr;

// === センサ ===
VL53L0X tof;
uint16_t g_distance_mm = 0;

// === 関数プロトタイプ ===
void setupBLE();
void setupSensors();
void setupDisplay();
void readSensors();
void processData();
void updateDisplay();
void sendBLE();

void setup() {
  M5.begin();
  setupBLE();
  setupSensors();
  setupDisplay();
}

void loop() {
  M5.update();
  readSensors();
  processData();
  updateDisplay();
  sendBLE();
  delay(100);
}

// ---------------------------------------------------------------------------
// NimBLE 初期化・アドバタイズ開始
// ---------------------------------------------------------------------------
void setupBLE() {
  NimBLEDevice::init("M5-Team");
  auto pServer  = NimBLEDevice::createServer();
  auto pService = pServer->createService(SERVICE_UUID);
  pChar = pService->createCharacteristic(
    CHAR_UUID,
    NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
  );
  pService->start();
  NimBLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID);
  NimBLEDevice::getAdvertising()->start();
}

// ---------------------------------------------------------------------------
// センサ初期化 (Grove Port A の I2C 経由で ToF を初期化)
// ---------------------------------------------------------------------------
void setupSensors() {
  Wire.begin();
  tof.setTimeout(500);
  tof.init();
  tof.startContinuous();
}

void setupDisplay() {
  M5.Display.setTextSize(2);
  M5.Display.fillScreen(TFT_BLACK);
}

// ---------------------------------------------------------------------------
// センサ値読み取り
// ---------------------------------------------------------------------------
void readSensors() {
  g_distance_mm = tof.readRangeContinuousMillimeters();
}

// ---------------------------------------------------------------------------
// データ処理・判定 (チームごとにロジックを書く)
// ---------------------------------------------------------------------------
void processData() {
  // 例: 距離に応じて警告フラグを立てる、移動平均を取る、など
}

// ---------------------------------------------------------------------------
// M5 画面表示更新
// ---------------------------------------------------------------------------
void updateDisplay() {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(10, 10);
  M5.Display.printf("Dist: %d mm\n", g_distance_mm);
}

// ---------------------------------------------------------------------------
// BLE で値を送信 (PC 側は ../lecture11_ble/web_bluetooth_receiver/ で受信)
// ---------------------------------------------------------------------------
void sendBLE() {
  if (pChar == nullptr) return;
  String str = String(g_distance_mm);
  pChar->setValue(str.c_str());
  pChar->notify();
}

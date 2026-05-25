# デザインプロトタイピング(2) サンプルコード集

東京都市大学「デザインプロトタイピング(2)」後半パート(第9〜12回)のスライドに登場する Arduino / Web Bluetooth のサンプルコードをまとめたものです。

## 対象ハードウェア

- M5Stack Core S3 (ESP32-S3)
- Grove 接続ユニット (例: Unit ToF / VL53L0X)

## 開発環境

- Arduino IDE 2.x
- 追加ボードマネージャ URL:
  `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json`
- ボード: `M5Stack` → `M5CoreS3`

## 必要ライブラリ

ライブラリマネージャからインストールしてください。

| ライブラリ | 用途 | 使用するサンプル |
|---|---|---|
| `M5Unified` | M5Stack 統合 | すべて |
| `VL53L0X` (Pololu) | ToF 距離センサ | `tof_basic`, `tof_actuator`, `program_skeleton` |
| `NimBLE-Arduino` | BLE ペリフェラル | `m5stack_ble_peripheral`, `program_skeleton` |

## ディレクトリ構成

```
Examples/
├── lecture09_intro/            第9回: 環境構築・Lチカ・画面描画
│   ├── blink_screen/           画面色を交互に切り替える「Lチカ」
│   ├── serial_hello/           シリアルモニタの基本
│   └── info_display/           テキスト + 図形の総合例
│
├── lecture10_sensors/          第10回: センサ・ボタン入力・アクチュエータ
│   ├── button_basic/           BtnA の wasPressed()
│   ├── button_and_touch/       ボタン + 画面タッチ
│   ├── touch_draw/             タッチでお絵描き
│   ├── imu_basic/              内蔵 IMU の値を表示
│   ├── tof_basic/              ToF 距離センサ (Grove Port A)
│   ├── speaker_touch/          タッチでテルミン風
│   ├── melody/                 ドレミファソラシド
│   └── tof_actuator/           ToF → 画面色 + 警告音
│
├── lecture11_ble/              第11回: BLE 通信
│   ├── m5stack_ble_peripheral/ M5 側: BLE ペリフェラル(NimBLE)
│   └── web_bluetooth_receiver/ PC 側: ブラウザで受信
│       ├── index.html
│       └── app.js
│
└── lecture12_team_project/     第12回: チーム制作の骨格
    └── program_skeleton/       センサ → 処理 → 表示 + BLE 送信の雛形
```

## Web Bluetooth 受信の動かし方

1. M5Stack に `lecture11_ble/m5stack_ble_peripheral` を書き込む
2. `lecture11_ble/web_bluetooth_receiver` で簡易サーバーを起動

   ```sh
   cd lecture11_ble/web_bluetooth_receiver
   python3 -m http.server 8000
   ```

3. Chrome / Edge で <http://localhost:8000/> を開く
4. 「M5に接続」→ 一覧から **M5-Sensor** を選択
5. 値が表示されれば成功

> Web Bluetooth は HTTPS または localhost からしか動作しません。
> Safari / Firefox は未対応です。

## .ino ファイル名の注意

Arduino IDE では `.ino` ファイル名と親フォルダ名が一致している必要があります。フォルダ名を変えるときは中の `.ino` も同じ名前に変更してください。

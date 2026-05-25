// 第11回: Web Bluetooth API による受信
// 動作: 「M5に接続」ボタンを押すと M5-Sensor を選択するダイアログが開き、
//       接続後に Notify で送られてくる値を画面に表示する。
//
// 注意:
//   - Chrome / Edge のみ対応 (Safari, Firefox 非対応)
//   - HTTPS または localhost からのみ動作する
//     ローカル確認例:  python3 -m http.server 8000
//                      → http://localhost:8000/ にアクセス

const SERVICE = "12345678-1234-5678-1234-56789abcdef0";
const CHAR    = "12345678-1234-5678-1234-56789abcdef1";

const valueEl  = document.getElementById("value");
const statusEl = document.getElementById("status");

document.getElementById("connect").onclick = async () => {
  try {
    statusEl.textContent = "デバイスを選択してください...";
    const device = await navigator.bluetooth.requestDevice({
      filters: [{ services: [SERVICE] }],
    });

    statusEl.textContent = `接続中: ${device.name}`;
    const server  = await device.gatt.connect();
    const service = await server.getPrimaryService(SERVICE);
    const ch      = await service.getCharacteristic(CHAR);

    await ch.startNotifications();
    ch.addEventListener("characteristicvaluechanged", (e) => {
      const text = new TextDecoder().decode(e.target.value);
      valueEl.textContent = text;
    });

    device.addEventListener("gattserverdisconnected", () => {
      statusEl.textContent = "切断されました";
    });

    statusEl.textContent = `接続済み: ${device.name}`;
  } catch (err) {
    statusEl.textContent = `エラー: ${err.message}`;
    console.error(err);
  }
};

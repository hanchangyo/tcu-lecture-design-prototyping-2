// 第10回: 内蔵 IMU(BMI270) の値を画面に表示
// 動作: 加速度 (ax, ay, az) とジャイロ (gx, gy, gz) を 100ms ごとに更新

#include <M5Unified.h>

void setup() {
  M5.begin();
  M5.Display.setTextSize(2);
}

void loop() {
  auto imu_update = M5.Imu.update();
  if (imu_update) {
    M5.Display.setCursor(0, 20);
    M5.Display.clear();

    auto data = M5.Imu.getImuData();

    M5.Display.printf("IMU:\r\n");
    M5.Display.printf("ax:%f\nay:%f\naz:%f\r\n",
                      data.accel.x, data.accel.y, data.accel.z);
    M5.Display.printf("gx:%f\ngy:%f\ngz:%f\r\n",
                      data.gyro.x, data.gyro.y, data.gyro.z);
  }
  delay(100);
}

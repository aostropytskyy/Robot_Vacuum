#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // ESP32 SDA, SCL

  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found. Check power and SDA/SCL wiring.");
    while (true) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU-6050 ready. Keep it still, then tilt or rotate it.");
}

void loop() {
  sensors_event_t accel, gyro, temperature;
  mpu.getEvent(&accel, &gyro, &temperature);

  Serial.print("Accel (m/s^2): ");
  Serial.print(accel.acceleration.x); Serial.print(", ");
  Serial.print(accel.acceleration.y); Serial.print(", ");
  Serial.println(accel.acceleration.z);

  Serial.print("Gyro (rad/s): ");
  Serial.print(gyro.gyro.x); Serial.print(", ");
  Serial.print(gyro.gyro.y); Serial.print(", ");
  Serial.println(gyro.gyro.z);

  Serial.println();
  delay(250);
}                                                       
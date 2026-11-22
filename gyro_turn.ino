#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

const int motor1In1 = 19;  // Left motor
const int motor1In2 = 18;
const int motor2In1 = 23;  // Right motor
const int motor2In2 = 4;
const int bumperPin = 27;

const int high=255;

float gyroZBias = 0;
bool turnDone = false;

void setMotor1Forward(int speed) {
  analogWrite(motor1In1, speed);
  analogWrite(motor1In2, 0);
}

void setMotor1Reverse(int speed) {
  analogWrite(motor1In1, 0);
  analogWrite(motor1In2, speed);
}

void setMotor2Forward(int speed) {
  analogWrite(motor2In1, speed);
  analogWrite(motor2In2, 0);
}

void setMotor2Reverse(int speed) {
  analogWrite(motor2In1, 0);
  analogWrite(motor2In2, speed);
}

void stopMotors() {
  // Both inputs HIGH selects brake on the DRV8833
  analogWrite(motor1In1, high);
  analogWrite(motor1In2, high);
  analogWrite(motor2In1, high);
  analogWrite(motor2In2, high);
}

void calibrateGyro() {
  Serial.println("Keep robot still: calibrating gyro...");
  float sum = 0;

  for (int i = 0; i < 500; i++) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    sum += gyro.gyro.z;
    delay(2);
  }

  gyroZBias = sum / 500.0;
  Serial.println("Calibration done.");
}

bool turnRight90() {
  float turnedDegrees = 0;
  unsigned long lastUs = micros();
  unsigned long startMs = millis();

  while (turnedDegrees < 90.0) {
    // With NC bumper wiring, HIGH means pressed
    if (digitalRead(bumperPin) == HIGH) {
      stopMotors();
      Serial.println("Bumper pressed; turn stopped.");
      return false;
    }

    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    unsigned long nowUs = micros();
    float dt = (nowUs - lastUs) / 1000000.0;
    lastUs = nowUs;

    float rateZ = gyro.gyro.z - gyroZBias;  // radians/second
    turnedDegrees += fabs(rateZ * dt * 57.2958);

    // Pivot right: left wheel forward, right wheel backward
    int speed = (turnedDegrees < 65.0) ? 170 : 120;
    setMotor1Forward(speed);
    setMotor2Reverse(speed);

    if (millis() - startMs > 5000) {
      stopMotors();
      Serial.println("Turn timed out; check gyro and motor directions.");
      return false;
    }
  }

  stopMotors();
  Serial.print("Turn complete; measured about ");
  Serial.print(turnedDegrees);
  Serial.println(" degrees.");
  return true;
}

void setup() {
  Serial.begin(115200);

  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);
  pinMode(bumperPin, INPUT_PULLUP);

  stopMotors();

  Wire.begin(21, 22);  // SDA, SCL

  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found; check its wiring.");
    while (true) delay(10);
  }

  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  calibrateGyro();
}

void loop() {
  if (!turnDone) {
    turnDone = true;  // Run once after startup
    delay(1000);      // Let the robot settle after calibration
    turnRight90();
  }
}
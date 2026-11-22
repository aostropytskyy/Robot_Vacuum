#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
Adafruit_MPU6050 mpu;

//Variables
//Motor pins
const int encoderA = 32;  // Motor 1, yellow
const int encoderB = 33;  // Motor 1, green
const int encoderC = 16;  // Motor 2, yellow
const int encoderD = 17;  // Motor 2, green
const int motor1In1 = 19;
const int motor1In2 = 18;
const int motor2In1 = 23;
const int motor2In2 = 4;
//button pins
const int bumperPin = 27;
//ultrasonic pins
const int trigPin = 26;
const int echoPin = 25;
//Gyro uses I2C pins 21 (SDA) and 22 (SCL)

volatile long encoderCount1 = 0;
volatile long encoderCount2 = 0;
long startCount1 = 0;
long startCount2 = 0;
const long ticksPerTurn = 194;
bool motor1Moving = false;
bool motor2Moving = false;

void IRAM_ATTR onEncoderA() {
  if (digitalRead(encoderA) == digitalRead(encoderB)) {
    encoderCount1++;
  } else {
    encoderCount1--;
  }
}

void IRAM_ATTR onEncoderC() {
  if (digitalRead(encoderC) == digitalRead(encoderD)) {
    encoderCount2++;
  } else {
    encoderCount2--;
  }
}

void forward(){
  digitalWrite(motor1In1, HIGH);
  digitalWrite(motor1In2, LOW);
  digitalWrite(motor2In1, HIGH);
  digitalWrite(motor2In2, LOW);

  motor1Moving = true;
  motor2Moving = true;
}

void coaststop(){
  digitalWrite(motor1In1, LOW);
  digitalWrite(motor1In2, LOW);
  digitalWrite(motor2In1, LOW);
  digitalWrite(motor2In2, LOW);
  motor1Moving = false;
  motor2Moving = false;
}

void motor1Forward() {
  digitalWrite(motor1In1, HIGH);
  digitalWrite(motor1In2, LOW);
}

void motor1Reverse() {
  digitalWrite(motor1In1, LOW);
  digitalWrite(motor1In2, HIGH);
}

void motor2Forward() {
  digitalWrite(motor2In1, HIGH);
  digitalWrite(motor2In2, LOW);
}

void motor2Reverse() {
  digitalWrite(motor2In1, LOW);
  digitalWrite(motor2In2, HIGH);
}

void ultradistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int count = 0;

  // Timeout after 30 ms, about 5 m maximum
  unsigned long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No echo");
  } else {
    float distanceCm = duration * 0.0343 / 2.0;
    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.println(" cm");
    if(distanceCm<5){
      //turn right then if obstacle turn left
      motor1Reverse();
      motor2Reverse();
      delay(500);
      motor1Forward();
      motor2Reverse();
      delay(500);
      count++;
    }
  }

  delay(100);
}

void gyro_accel(){
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

void setup() {
  Serial.begin(115200);

  pinMode(motor1In1, OUTPUT);
  pinMode(motor1In2, OUTPUT);
  pinMode(motor2In1, OUTPUT);
  pinMode(motor2In2, OUTPUT);

  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  pinMode(encoderC, INPUT_PULLUP);
  pinMode(encoderD, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderA), onEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderC), onEncoderC, CHANGE);

  noInterrupts();
  startCount1 = encoderCount1;
  startCount2 = encoderCount2;
  interrupts();

  pinMode(bumperPin, INPUT_PULLUP);
  // Initialize MPU6050
  Wire.begin(21, 22);  // ESP32 SDA, SCL

  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found. Check power and SDA/SCL wiring.");
    while (true) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (digitalRead(bumperPin) == HIGH) {
    coaststop();
    return;
  }

  forward();
  ultradistance();
  
  noInterrupts();
  long currentCount1 = encoderCount1;
  long currentCount2 = encoderCount2;
  interrupts();

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 250) {
    lastPrint = millis();
    Serial.print("Motor 1 count: ");
    Serial.println(currentCount1);
    Serial.print("Motor 2 count: ");
    Serial.println(currentCount2);
  }
}
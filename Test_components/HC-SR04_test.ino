const int trigPin = 26;
const int echoPin = 25;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Timeout after 30 ms, about 5 m maximum
  unsigned long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    Serial.println("No echo");
  } else {
    float distanceCm = duration * 0.0343 / 2.0;
    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.println(" cm");
  }

  delay(100);
}
const int motorIn1 = 18;
const int motorIn2 = 19;

void setup() {
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
}

void loop() {
  // Forward
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  delay(2000);

  // Coast stop
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  delay(1000);

  // Reverse
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  delay(2000);

  // Force stop
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, HIGH);
  delay(1000);
}
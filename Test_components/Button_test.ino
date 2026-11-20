const int bumperPin = 27;

void setup() {
  Serial.begin(115200);
  pinMode(bumperPin, INPUT_PULLUP);
}

void loop() {
  // LOW means bumper untouched; HIGH means pressed or circuit opened
  if (digitalRead(bumperPin) == HIGH) {
    Serial.println("Bumper pressed");
    
  }


}
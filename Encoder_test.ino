const int encoderA = 32;  // Yellow
const int encoderB = 33;  // Green

const int motorIn1 = 18;
const int motorIn2 = 19;

volatile long encoderCount = 0;
long startCount = 0;
const long ticksPerTurn = 195;  // Your measured target
bool moving = false;

void IRAM_ATTR onEncoderA() {
  if (digitalRead(encoderB) == digitalRead(encoderA)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderA), onEncoderA, CHANGE);

  noInterrupts();
  startCount = encoderCount;
  interrupts();

  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  moving = true;
}

void loop() {
  noInterrupts();
  long currentCount = encoderCount;
  interrupts();

  if (moving && abs(currentCount - startCount) >= ticksPerTurn) {
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, HIGH);
    moving = false;
    Serial.println("Target reached; motor stopped.");
  }

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 250) {
    lastPrint = millis();
    Serial.print("Encoder count: ");
    Serial.println(currentCount);
  }
}
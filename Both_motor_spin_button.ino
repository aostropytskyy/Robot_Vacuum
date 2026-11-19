//Variables
const int encoderA = 32;  // Motor 1, yellow
const int encoderB = 33;  // Motor 1, green
const int encoderC = 16;  // Motor 2, yellow
const int encoderD = 17;  // Motor 2, green

const int motor1In1 = 19;
const int motor1In2 = 18;
const int motor2In1 = 23;
const int motor2In2 = 4;

volatile long encoderCount1 = 0;
volatile long encoderCount2 = 0;

long startCount1 = 0;
long startCount2 = 0;

const long ticksPerTurn = 194;
bool motor1Moving = false;
bool motor2Moving = false;

const int bumperPin = 27;

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

void stop(){
  digitalWrite(motor1In1, LOW);
  digitalWrite(motor1In2, LOW);
  digitalWrite(motor2In1, LOW);
  digitalWrite(motor2In2, LOW);
  motor1Moving = false;
  motor2Moving = false;
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
}

void loop() {
  if (digitalRead(bumperPin) == HIGH) {
    stop();
    return;
  }

  forward();

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
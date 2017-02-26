// Shift Register
int dataPin   = D2;
int clockPin  = D0;
int latchPin  = D1;
int clearPin  = D3;

int sensorPin = D5;
int ledPin    = D7;

#define COUNT 8
int count = COUNT;

int status[COUNT];

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
  digitalWrite(clearPin, HIGH);

  pinMode(ledPin,    OUTPUT);
  pinMode(sensorPin, INPUT);
}

void loop() {
  fetchSensorData();
  printStatus();
  delay(500);
}

void fetchSensorData() {
  clear();

  for (int i = 0; i < count; i++) {
    // Set first memory value HIGH, then push LOWs in.
    // This moves the HIGH bit through every register pin
    // which we then read to determine if that sensor is on.
    digitalWrite(dataPin, i == 0);

    advanceClock();
    latch();

    status[i] = digitalRead(sensorPin);
  }
}

void clear() {
  digitalWrite(clearPin, LOW);
  digitalWrite(clearPin, HIGH);
}

void advanceClock() {
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
}

void latch() {
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

void printStatus() {
  for (int i = 0; i < count; i++) {
    Serial.print(status[i]);
    if (i != count-1) Serial.print(",");
  }
  Serial.println("");
}

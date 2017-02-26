// Shift Register
int dataPin   = D2;
int clockPin  = D0;
int latchPin  = D1;
int clearPin  = D3;

int sensorPin = D5;
int ledPin    = D7;

const int count = 8;

class Position {
  public:
    String position;
    int    index;
    int    value;
    long   lastChange;
    Position(String newPosition, int newIndex) {
      position   = newPosition;
      index      = newIndex;
      value      = 0;
      lastChange = 0;
    }

    void setValue(int newValue, long currentTime) {
      value      = newValue;
      lastChange = currentTime;
    }
};

Position positions[count] = {
  Position("c1", 0),
  Position("c2", 1),
  Position("c3", 2),
  Position("c4", 3),
  Position("c5", 4),
  Position("c6", 5),
  Position("c7", 6),
  Position("c8", 7)
};

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
    digitalWrite(dataPin, i == 0);
    advanceClock();
    latch();

    int sensorValue = digitalRead(sensorPin);
    Position &position = positions[i];

    if (sensorValue != position.value) {
      position.setValue(sensorValue, millis());
    }
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
    Serial.print(positions[i].position);
    Serial.print(":");
    Serial.print(positions[i].value);

    if (i == count - 1) {
      Serial.println("");
    } else {
      Serial.print(",");
    }
  }
}

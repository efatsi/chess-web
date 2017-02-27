int dataPin   = D2;
int clockPin  = D0;
int latchPin  = D1;
int clearPin  = D3;

int sensorPin = D5;

const int count = 8;

class Position {
  #define UNSTABLE_OFF 0
  #define UNSTABLE_ON  1
  #define STABLE_OFF   2
  #define STABLE_ON    3
  #define CONFIRMED    4

  public:
    String position;
    int    state;
    int    index;
    int    value;
    long   lastChange;
    Position(String newPosition, int newIndex) {
      position   = newPosition;
      state      = CONFIRMED;
      index      = newIndex;
      value      = 0;
      lastChange = 0;
    }

    void setValue(int newValue, long currentTime) {
      value      = newValue;
      lastChange = currentTime;

      if (newValue) {
        if (state == UNSTABLE_OFF || state == STABLE_OFF) {
          // changed their mind ... or TODO: check if user's piece of taking other's
          confirmState();
        } else {
          state = UNSTABLE_ON;
        }
      } else {
        if (state == UNSTABLE_ON) {
          // on blip - likely just a passing piece;
          confirmState();
        } else {
          state = UNSTABLE_OFF;
        }
      }
    }

    void checkStability() {
      if (_unstable() && _changeIsOld()) {
        switch (state) {
          case UNSTABLE_OFF:
            state = STABLE_OFF;
            break;
          case UNSTABLE_ON:
            state = STABLE_ON;
            break;
        }
      }
    }

    void confirmState() {
      state = CONFIRMED;
    }

    String printState() {
      switch (state) {
        case UNSTABLE_OFF:
          return "unstable_off ";
        case UNSTABLE_ON:
          return "unstable_on  ";
        case STABLE_OFF:
          return "stable_off   ";
        case STABLE_ON:
          return "stable_on    ";
        case CONFIRMED:
          return "confirmed    ";
      }
    }

  private:
    bool _unstable() {
      return state == UNSTABLE_ON || state == UNSTABLE_OFF;
    }

    bool _changeIsOld() {
      return millis() - lastChange > 1000;
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

  pinMode(sensorPin, INPUT);

  fetchSensorData();
  confirmChanges();
}

void loop() {
  fetchSensorData();
  checkForMove();
  printStatus();
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
    } else {
      position.checkStability();
    }
  }
}

void checkForMove() {
  String stableOff = "";
  String stableOn  = "";

  for (int i = 0; i < count; i++) {
    Position &position = positions[i];

    if (position.state == STABLE_OFF) {
      stableOff = position.position;
    } else if (position.state == STABLE_ON) {
      stableOn  = position.position;
    }
  }

  if (stableOff != "" && stableOn != "") {
    Serial.print(stableOff);
    Serial.print("-");
    Serial.println(stableOn);
    confirmChanges();
  }
}

void printStatus() {
  // delay(500);

  // Position &position = positions[6];
  // Serial.print("1: ");
  // Serial.print(position.value);
  // Serial.print(" ");
  // Serial.print(position.printState());
  //
  // Position &position2 = positions[7];
  // Serial.print("2: ");
  // Serial.print(position2.value);
  // Serial.print(" ");
  // Serial.print(position2.printState());
  // Serial.println("");

  // for (int i = 0; i < count; i++) {
  //   Serial.print(positions[i].position);
  //   Serial.print(":");
  //   Serial.print(positions[i].value);
  //
  //   if (i == count - 1) {
  //     Serial.println("");
  //   } else {
  //     Serial.print(",");
  //   }
  // }
}

void confirmChanges() {
  for (int i = 0; i < count; i++) {
    Position &position = positions[i];
    position.confirmState();
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

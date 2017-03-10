int dataPin   = D6;
int clockPin  = D4;
int latchPin  = D5;
int clearPin  = D3;

int sensorPin = A5;

class Position {
  #define UNSTABLE_OFF    0
  #define UNSTABLE_ON     1
  #define UNSTABLE_OFF_ON 2
  #define STABLE_OFF      3
  #define STABLE_ON       4
  #define STABLE_OFF_ON   5
  #define CONFIRMED       6

  #define EMPTY 0
  #define WHITE 1
  #define BLACK 2

  #define CONFIRM_TIME 200

  public:
    String position;
    int    occupied;
    int    state;
    int    index;
    int    value;
    long   lastChange;
    Position(String newPosition, int newIndex, int newOccupied) {
      position   = newPosition;
      state      = CONFIRMED;
      index      = newIndex;
      occupied   = newOccupied;
      value      = occupied ? 1 : 0;
      lastChange = 0;
    }

    void setValue(int newValue, int player, long currentTime) {
      value      = newValue;
      lastChange = currentTime;

      if (newValue) {
        if (state == UNSTABLE_OFF || state == STABLE_OFF) {
          if (occupied == player) {
            // changed their mind
            state = CONFIRMED;
          } else {
            // player's piece of taking other's
            state = UNSTABLE_OFF_ON;
          }
        } else {
          state = UNSTABLE_ON;
        }
      } else {
        if (state == UNSTABLE_ON) {
          // on blip - likely just a passing piece;
          state = CONFIRMED;
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
          case UNSTABLE_OFF_ON:
            state = STABLE_OFF_ON;
            break;
        }
      }
    }

    void confirmState(int player) {
      switch (state) {
        case STABLE_OFF:
          occupied = EMPTY;
          break;
        case STABLE_ON:
          occupied = player;
          break;
        case STABLE_OFF_ON:
          occupied = player;
          break;
      }

      state = CONFIRMED;
    }

  private:
    bool _unstable() {
      return state == UNSTABLE_ON || state == UNSTABLE_OFF || state == UNSTABLE_OFF_ON;
    }

    bool _changeIsOld() {
      return millis() - lastChange > CONFIRM_TIME;
    }
};

const int count = 64;

int currentPlayer = WHITE;

Position positions[count] = {

  Position("a8", 0, EMPTY),
  Position("a7", 1, EMPTY),
  Position("a6", 2, EMPTY),
  Position("a5", 3, EMPTY),
  Position("a4", 4, EMPTY),
  Position("a3", 5, BLACK),
  Position("a2", 6, EMPTY),
  Position("a1", 7, EMPTY),

  Position("b8", 8, EMPTY),
  Position("b7", 9, EMPTY),
  Position("b6", 10, EMPTY),
  Position("b5", 11, EMPTY),
  Position("b4", 12, EMPTY),
  Position("b3", 13, EMPTY),
  Position("b2", 14, EMPTY),
  Position("b1", 15, EMPTY),

  Position("c8", 16, EMPTY),
  Position("c7", 17, WHITE),
  Position("c6", 18, EMPTY),
  Position("c5", 19, EMPTY),
  Position("c4", 12, EMPTY),
  Position("c3", 13, BLACK),
  Position("c2", 22, EMPTY),
  Position("c1", 23, EMPTY),

  Position("d8", 24, EMPTY),
  Position("d7", 25, WHITE),
  Position("d6", 26, EMPTY),
  Position("d5", 27, EMPTY),
  Position("d4", 28, EMPTY),
  Position("d3", 29, BLACK),
  Position("d2", 30, EMPTY),
  Position("d1", 31, EMPTY),

  Position("e8", 32, EMPTY),
  Position("e7", 33, WHITE),
  Position("e6", 34, EMPTY),
  Position("e5", 35, EMPTY),
  Position("e4", 36, EMPTY),
  Position("e3", 37, BLACK),
  Position("e2", 38, EMPTY),
  Position("e1", 39, EMPTY),

  Position("f8", 40, EMPTY),
  Position("f7", 41, WHITE),
  Position("f6", 42, EMPTY),
  Position("f5", 43, EMPTY),
  Position("f4", 44, EMPTY),
  Position("f3", 45, BLACK),
  Position("f2", 46, EMPTY),
  Position("f1", 47, EMPTY),

  Position("g8", 48, EMPTY),
  Position("g7", 49, WHITE),
  Position("g6", 50, EMPTY),
  Position("g5", 51, EMPTY),
  Position("g4", 52, EMPTY),
  Position("g3", 53, BLACK),
  Position("g2", 54, EMPTY),
  Position("g1", 55, EMPTY),

  Position("h8", 56, EMPTY),
  Position("h7", 57, EMPTY),
  Position("h6", 58, EMPTY),
  Position("h5", 59, EMPTY),
  Position("h4", 60, EMPTY),
  Position("h3", 61, EMPTY),
  Position("h2", 62, EMPTY),
  Position("h1", 63, EMPTY)

};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
  digitalWrite(clearPin, HIGH);

  pinMode(sensorPin, INPUT);
}

void loop() {
  fetchSensorData();
  checkForMove();
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
      position.setValue(sensorValue, currentPlayer, millis());
    } else {
      position.checkStability();
    }
  }
}

void checkForMove() {
  String stableOff   = "";
  String stableOn    = "";
  String stableOffOn = "";

  for (int i = 0; i < count; i++) {
    Position &position = positions[i];

    if (position.state == STABLE_OFF) {
      stableOff = position.position;
    } else if (position.state == STABLE_ON) {
      stableOn = position.position;
    } else if (position.state == STABLE_OFF_ON) {
      stableOffOn = position.position;
    }
  }

  if (stableOff != "") {
    if (stableOn != "" || stableOffOn != "") {
      Serial.print(stableOff);
      Serial.print("-");
      Serial.print(stableOn);
      Serial.print(stableOffOn);
      Serial.println("");
      confirmChanges();
    }
  }
}

void confirmChanges() {
  for (int i = 0; i < count; i++) {
    Position &position = positions[i];
    position.confirmState(currentPlayer);
  }

  currentPlayer = WHITE + BLACK - currentPlayer;
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

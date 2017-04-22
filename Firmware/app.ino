#include "Position.h"

int dataPin   = D6;
int clockPin  = D4;
int latchPin  = D5;
int clearPin  = D3;

int sensorPin = A5;

const int count = 64;

int currentPlayer = WHITE;
int otherPlayer   = BLACK;

Position positions[count] = {
  Position("a8", BLACK),
  Position("a7", BLACK),
  Position("a6", EMPTY),
  Position("a5", EMPTY),
  Position("a4", EMPTY),
  Position("a3", EMPTY),
  Position("a2", WHITE),
  Position("a1", WHITE),

  Position("b8", BLACK),
  Position("b7", BLACK),
  Position("b6", EMPTY),
  Position("b5", EMPTY),
  Position("b4", EMPTY),
  Position("b3", EMPTY),
  Position("b2", WHITE),
  Position("b1", WHITE),

  Position("c8", BLACK),
  Position("c7", BLACK),
  Position("c6", EMPTY),
  Position("c5", EMPTY),
  Position("c4", EMPTY),
  Position("c3", EMPTY),
  Position("c2", WHITE),
  Position("c1", WHITE),

  Position("d8", BLACK),
  Position("d7", BLACK),
  Position("d6", EMPTY),
  Position("d5", EMPTY),
  Position("d4", EMPTY),
  Position("d3", EMPTY),
  Position("d2", WHITE),
  Position("d1", WHITE),

  Position("e8", BLACK),
  Position("e7", BLACK),
  Position("e6", EMPTY),
  Position("e5", EMPTY),
  Position("e4", EMPTY),
  Position("e3", EMPTY),
  Position("e2", WHITE),
  Position("e1", WHITE),

  Position("f8", BLACK),
  Position("f7", BLACK),
  Position("f6", EMPTY),
  Position("f5", EMPTY),
  Position("f4", EMPTY),
  Position("f3", EMPTY),
  Position("f2", WHITE),
  Position("f1", WHITE),

  Position("g8", BLACK),
  Position("g7", BLACK),
  Position("g6", EMPTY),
  Position("g5", EMPTY),
  Position("g4", EMPTY),
  Position("g3", EMPTY),
  Position("g2", WHITE),
  Position("g1", WHITE),

  Position("h8", BLACK),
  Position("h7", BLACK),
  Position("h6", EMPTY),
  Position("h5", EMPTY),
  Position("h4", EMPTY),
  Position("h3", EMPTY),
  Position("h2", WHITE),
  Position("h1", WHITE)
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
  printStatus();
}

void fetchSensorData() {
  // collect data
  int data[count];
  clear();
  for (int i = 0; i < count; i++) {
    // Set first memory value HIGH, then push LOWs in.
    writeToRegister(i == 0);
    data[i] = digitalRead(sensorPin);
  }

  // count ups and downs
  int downCount    = 0;
  int upCount      = 0;
  for (int i = 0; i < count; i++) {
    Position &position = positions[i];
    int sensorValue = data[i];

    if (sensorValue != position.value) {
      sensorValue ? downCount += 1 : upCount += 1;
    } else {
      if (position.up())   upCount   += 1;
      if (position.down()) downCount += 1;
    }
  }

  // set statuses
  long currentTime = millis();
  for (int i = 0; i < count; i++) {
    Position &position = positions[i];
    int sensorValue = data[i];

    if (sensorValue != position.value) {
      position.setStatus(sensorValue, upCount, downCount, currentTime);
    } else {
      position.checkStability(currentTime);
    }
  }
}

void checkForMove() {
  class ChangedPosition {
  public:
    String position;
    int    wasOccupiedBy;

    ChangedPosition() {
    }

    ChangedPosition(String _position, int _wasOccupiedBy) {
      position      = _position;
      wasOccupiedBy = _wasOccupiedBy;
    }
  };

  ChangedPosition up[count];
  ChangedPosition down[count];
  int upCount   = 0;
  int downCount = 0;

  for (int i = 0; i < count; i++) {
    Position &position = positions[i];

    if (position.status == STABLE_UP) {
      up[upCount] = ChangedPosition(position.position, position.occupiedBy);
      upCount += 1;
    }

    if (position.status == STABLE_DOWN) {
      down[downCount] = ChangedPosition(position.position, position.occupiedBy);
      downCount += 1;
    }
  }

  if (upCount == 1 && downCount == 1) {
    if (up[0].wasOccupiedBy == currentPlayer && down[0].wasOccupiedBy == otherPlayer) {
      // white captured opponent
      Serial.print(up[0].position);
      Serial.print("-");
      Serial.println(down[0].position);
      confirmChanges();
    } else if (up[0].wasOccupiedBy == currentPlayer && down[0].wasOccupiedBy == EMPTY) {
      // white moved to empty space
      Serial.print(up[0].position);
      Serial.print("-");
      Serial.println(down[0].position);
      confirmChanges();
    } else {
      // something's wrong
    }
  }
}

void confirmChanges() {
  for (int i = 0; i < count; i++) {
    Position &position = positions[i];
    position.confirmState(currentPlayer);
  }

  otherPlayer   = currentPlayer;
  currentPlayer = WHITE + BLACK - currentPlayer;
}

void clear() {
  digitalWrite(clearPin, LOW);
  digitalWrite(clearPin, HIGH);
}

void writeToRegister(int value) {
  digitalWrite(dataPin, value);
  advanceClock();
  latch();
}

void advanceClock() {
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
}

void latch() {
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delayMicroseconds(15);
}

void printStatus() {
  // for (int i = 0; i < 8; i++) {
  //   for (int j = 0; j < 8; j++) {
  //     int index = i + (j * 8);
  //     Serial.print(positions[index].value);
  //     Serial.print(" ");
  //   }
  //
  //   Serial.println();
  // }
  // Serial.println();
  // delay(200);
}

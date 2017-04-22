#include <LiquidCrystal.h>
#include "Position.h"

// Shift Register pins
int sensorPin = D0;
int clearPin  = D1;
int clockPin  = D2;
int latchPin  = D3;
int dataPin   = D4;

int ledPin    = D5;
int playerPin = D6;

LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

const int count = 64;

int currentPlayer = WHITE;
int waitingPlayer = BLACK;

int homePlayer;
int awayPlayer;

SYSTEM_MODE(MANUAL);

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
  Particle.function("screen", updateScreen);
  Particle.function("light", updateLight);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(clearPin, HIGH);

  pinMode(ledPin, OUTPUT);
  pinMode(playerPin, INPUT);

  if (digitalRead(playerPin)) {
    homePlayer = WHITE;
    awayPlayer = BLACK;
  } else {
    homePlayer = BLACK;
    awayPlayer = WHITE;
  }

  lcd.begin(16, 2);
  introSequence();
}

void loop() {
  fetchSensorData();
  checkForMove();
  printStatus();
}

void introSequence() {
  lcd.print("   Yay Chess!");
  lcd.setCursor(0, 1);
  lcd.print("       - fat$$");

  delay(2000);

  lcd.setCursor(0, 0);
  for (int i = 0; i < 32; i++) {
    if (i == 16) lcd.setCursor(0, 1);

    lcd.print(" ");
    if (!(i >= 16 && i < 23)) delay(100);
  }
}

int updateScreen(String player) {
  lcd.clear();

  lcd.print(" " + player + "'s Move:");
  lcd.setCursor(0, 1);
  lcd.print("    C2 - C4");

  return 1;
}

int updateLight(String command) {
  if (command == "on") {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  return 1;
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
    String output;
    if (up[0].wasOccupiedBy == currentPlayer && down[0].wasOccupiedBy == waitingPlayer) {
      // white captured opponent
      output = up[0].position + " - " + down[0].position;
      confirmChanges(output);
    } else if (up[0].wasOccupiedBy == currentPlayer && down[0].wasOccupiedBy == EMPTY) {
      // white moved to empty space
      output = up[0].position + " - " + down[0].position;
      confirmChanges(output);
    } else {
      // something's wrong
    }
  }
}

void printOutput(String output) {
  lcd.clear();

  String player = currentPlayer == WHITE ? "White" : "Black";

  lcd.print(" " + player + "'s Move:");
  lcd.setCursor(0, 1);
  lcd.print("    " + output);
}

void rawPrint(String output) {
  lcd.clear();
  lcd.print(output);
}

void confirmChanges(String output) {
  if (homePlayer == currentPlayer) {
    printOutput(output);
  } else {
    rawPrint("   satisfied");
  }

  for (int i = 0; i < count; i++) {
    Position &position = positions[i];
    position.confirmState(currentPlayer);
  }

  waitingPlayer = currentPlayer;
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
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int index = i + (j * 8);
      Serial.print(positions[index].value);
      Serial.print(" ");
    }

    Serial.println();
  }
  Serial.println();
  delay(200);
}

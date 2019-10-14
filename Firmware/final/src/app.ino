#include <LiquidCrystal.h>
#include "Position.h"
#include "utils.h"

#define ROWS 8
#define COLUMNS 8
#define TOTAL (ROWS * COLUMNS)

int readings[ROWS][COLUMNS];
int rawReadings[TOTAL];
int upperNorm;

int inputPin  = A6;
int ledPin    = D7;
int playerPin = D6;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

int currentPlayer = WHITE;
int waitingPlayer = BLACK;

int homePlayer;
int awayPlayer;

SYSTEM_MODE(MANUAL);

Position positions[TOTAL] = {
  Position("a8", BLACK),
  Position("b8", BLACK),
  Position("c8", BLACK),
  Position("d8", BLACK),
  Position("e8", BLACK),
  Position("f8", BLACK),
  Position("g8", BLACK),
  Position("h8", BLACK),

  Position("a7", BLACK),
  Position("b7", BLACK),
  Position("c7", BLACK),
  Position("d7", BLACK),
  Position("e7", BLACK),
  Position("f7", BLACK),
  Position("g7", BLACK),
  Position("h7", BLACK),

  Position("a6", EMPTY),
  Position("b6", EMPTY),
  Position("c6", EMPTY),
  Position("d6", EMPTY),
  Position("e6", EMPTY),
  Position("f6", EMPTY),
  Position("g6", EMPTY),
  Position("h6", EMPTY),

  Position("a5", EMPTY),
  Position("b5", EMPTY),
  Position("c5", EMPTY),
  Position("d5", EMPTY),
  Position("e5", EMPTY),
  Position("f5", EMPTY),
  Position("g5", EMPTY),
  Position("h5", EMPTY),

  Position("a4", EMPTY),
  Position("b4", EMPTY),
  Position("c4", EMPTY),
  Position("d4", EMPTY),
  Position("e4", EMPTY),
  Position("f4", EMPTY),
  Position("g4", EMPTY),
  Position("h4", EMPTY),

  Position("a3", EMPTY),
  Position("b3", EMPTY),
  Position("c3", EMPTY),
  Position("d3", EMPTY),
  Position("e3", EMPTY),
  Position("f3", EMPTY),
  Position("g3", EMPTY),
  Position("h3", EMPTY),

  Position("a2", WHITE),
  Position("b2", WHITE),
  Position("c2", WHITE),
  Position("d2", WHITE),
  Position("e2", WHITE),
  Position("f2", WHITE),
  Position("g2", WHITE),
  Position("h2", WHITE),

  Position("a1", WHITE),
  Position("b1", WHITE),
  Position("c1", WHITE),
  Position("d1", WHITE),
  Position("e1", WHITE),
  Position("f1", WHITE),
  Position("g1", WHITE),
  Position("h1", WHITE)
};

void setup() {
  Particle.function("screen", updateScreen);
  Particle.function("light", updateLight);

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);

  pinMode(inputPin, INPUT);
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
  countUpsAndDowns();
  checkForMove();
  // printBinaryReadings();
  // printUpdDownStatus();
  printOccupieds();
}

void fetchSensorData() {
  // Collect data from sensors
  for (int i = 0; i < ROWS; i++) {
    selectSlavePin(i);

    for (int j = 0; j < COLUMNS; j++) {
      selectMasterPin(j);

      int cleanI = j >= 4 ? COLUMNS - 1 - i : i;

      int reading = analogRead(inputPin);
      readings[cleanI][j] = reading;
      rawReadings[(i * COLUMNS) + j] = reading;
    }
  }

  // Calculate upper norm
  sort(rawReadings, TOTAL);
  int sum = 0;
  int half = TOTAL / 2;
  for (int i = 0; i < half; i++) {
    sum += rawReadings[TOTAL - 1 - i];
  }
  upperNorm = sum / half;
}

void countUpsAndDowns() {
  // TOTAL ups and downs
  int downCount    = 0;
  int upCount      = 0;

  for (int i = 0; i < TOTAL; i++) {
    Position &position = positions[i];
    bool sensorValue = getReading(i);

    if (sensorValue != position.value) {
      sensorValue ? downCount += 1 : upCount += 1;
    } else {
      if (position.up())   upCount   += 1;
      if (position.down()) downCount += 1;
    }
  }

  // set statuses
  long currentTime = millis();
  for (int i = 0; i < TOTAL; i++) {
    Position &position = positions[i];
    bool sensorValue = getReading(i);

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

  ChangedPosition up[TOTAL];
  ChangedPosition down[TOTAL];
  int upCount   = 0;
  int downCount = 0;

  for (int i = 0; i < TOTAL; i++) {
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

void confirmChanges(String output) {
  if (homePlayer == currentPlayer) {
    printOutput(output);
  } else {
    rawPrint("   satisfied");
  }

  for (int i = 0; i < TOTAL; i++) {
    Position &position = positions[i];
    position.confirmState(currentPlayer);
  }

  waitingPlayer = currentPlayer;
  currentPlayer = WHITE + BLACK - currentPlayer;
}

bool getReading(int i) {
  int x = i / 8;
  int y = i % 8;

  return (readings[x][y] * 100 / upperNorm) < 35;
}

// Debugging
void printReadings() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      Serial.print((readings[i][j] * 100 / upperNorm) < 35);
      Serial.print(" ");
    }

    Serial.println();
  }
  Serial.println();
  delay(200);
}

void printOccupieds() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      int index = (i * 8) + j;
      Position &position = positions[index];

      switch (position.occupiedBy) {
        case EMPTY:
          Serial.print(". ");
          break;
        case WHITE:
          Serial.print("W ");
          break;
        case BLACK:
          Serial.print("B ");
          break;
      }
    }

    Serial.println();
  }
  Serial.println();
  delay(200);
}

void printUpdDownStatus() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int index = i + (j * 8);
      Position &position = positions[index];

      if (position.status != CONFIRMED) {
        Serial.print(position.position);
        Serial.print(": ");

        switch (position.status) {
          case UNSTABLE_DOWN:
            switch (position.occupiedBy) {
              case WHITE:
                Serial.println("U_down W  ");
                break;
              case BLACK:
                Serial.println("U_down B  ");
                break;
              case EMPTY:
                Serial.println("U_down _  ");
                break;
            }
            break;
          case STABLE_DOWN:
            switch (position.occupiedBy) {
              case WHITE:
                Serial.println("S_down W  ");
                break;
              case BLACK:
                Serial.println("S_down B  ");
                break;
              case EMPTY:
                Serial.println("S_down _  ");
                break;
            }
            break;
          case UNSTABLE_UP:
            switch (position.occupiedBy) {
              case WHITE:
                Serial.println("U_up   W  ");
                break;
              case BLACK:
                Serial.println("U_up   B  ");
                break;
              case EMPTY:
                Serial.println("U_up   _  ");
                break;
            }
            break;
          case STABLE_UP:
            switch (position.occupiedBy) {
              case WHITE:
                Serial.println("S_up   W  ");
                break;
              case BLACK:
                Serial.println("S_up   B  ");
                break;
              case EMPTY:
                Serial.println("S_up   _  ");
                break;
            }
            break;
        }
      }
    }
  }
  delay(500);
}

// Visuals
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

void printOutput(String output) {
  lcd.clear();

  String player = currentPlayer == WHITE ? "White" : "Black";

  lcd.print(" " + player + "'s Move:");
  lcd.setCursor(0, 1);
  lcd.print("    " + output);

  Serial.println(" " + player + "'s Move: " + output);
}

void rawPrint(String output) {
  lcd.clear();
  lcd.print(output);
}

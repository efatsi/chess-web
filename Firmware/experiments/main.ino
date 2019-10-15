SYSTEM_MODE(MANUAL);

#include "utils.h"

const int inputPin = A6;

#define ROWS 8
#define COLUMNS 8
#define TOTAL (ROWS * COLUMNS)

int readings[ROWS][COLUMNS];
int rawReadings[TOTAL];

int upperNorm;

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);

  pinMode(inputPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  collectData();
  calculateNorms();

  display();
}

void collectData() {
  for (int i = 0; i < ROWS; i++) {
    selectSlavePin(i);

    for (int j = 0; j < COLUMNS; j++) {
      selectMasterPin(j);

      int cleanI = j >= 4 ? COLUMNS - 1 - i : i;

      readings[cleanI][j] = analogRead(inputPin);
      rawReadings[(cleanI * COLUMNS) + j] = analogRead(inputPin);
    }
  }
}

void calculateNorms() {
  sort(rawReadings, TOTAL);

  int sum = 0;
  int half = TOTAL / 2;
  for (int i = 0; i < half; i++) {
    sum += rawReadings[TOTAL - 1 - i];
  }
  upperNorm = sum / half;
}

void display() {
  if (upperNorm < 100) {
    Serial.print("\rNot enough light");
  } else {
    printStatus();
  }
}

void printStatus() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      // Serial.print((readings[i][j] * 100 / upperNorm) < 35);
      Serial.print(readings[i][j]);
      Serial.print(" ");
    }

    Serial.println();
  }
  Serial.println();
  delay(200);
}

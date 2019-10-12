SYSTEM_MODE(MANUAL);

#include "utils.h"

const int inputPin = A6;

#define ROWS 1
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
    selectMasterPin(i);

    for (int j = 0; j < COLUMNS; j++) {
      selectSlavePin(j);

      readings[i][j] = analogRead(inputPin);
      rawReadings[(i * COLUMNS) + j] = analogRead(inputPin);
    }
  }
}

void calculateNorms() {
  sort(rawReadings, TOTAL);

  upperNorm = (rawReadings[TOTAL - 1] + rawReadings[TOTAL - 2]) / 2;
}

void display() {
  if (upperNorm < 100) {
    Serial.print("\rNot enough light");
  } else {
    // showPercentages();
    showOnOffs();
  }
}

void showOnOffs() {
  printOnOff(0, 0);
  printOnOff(0, 1);
  printOnOff(0, 2);
  printOnOff(0, 3);
  printOnOff(0, 4);
  printOnOff(0, 5);
  printOnOff(0, 6);
  printOnOff(0, 7);

  Serial.println();
}

void printOnOff(int x, int y) {
  Serial.print(readings[x][y]);
  Serial.print("\t");
  Serial.print((readings[x][y] * 100 / upperNorm));
  Serial.print("%");
  Serial.print("\t");
  if ((readings[x][y] * 100 / upperNorm) < 35) {
    Serial.print(1);
  } else {
    Serial.print(0);
  }
  Serial.print("\t");
}

void showPercentages() {
  for (size_t i = 0; i < TOTAL; i++) {
    Serial.print(rawReadings[i] * 100 / upperNorm);
    Serial.print("%\t");
  }
  Serial.print("Light: ");
  Serial.println(upperNorm);
}

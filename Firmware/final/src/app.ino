#include "Screen.h"
#include "Board.h"

Screen screen;
Board board;

int ledPin    = D7;
int playerPin = D6;

int currentPlayer = WHITE;
int waitingPlayer = BLACK;

int homePlayer;
int awayPlayer;

SYSTEM_MODE(MANUAL);

void setup() {
  Particle.function("screen", updateScreen);
  Particle.function("light", updateLight);

  screen.init();
  board.init();

  pinMode(ledPin, OUTPUT);
  pinMode(playerPin, INPUT);

  if (digitalRead(playerPin)) {
    homePlayer = WHITE;
    awayPlayer = BLACK;
  } else {
    homePlayer = BLACK;
    awayPlayer = WHITE;
  }
}

void loop() {
  board.determineState();

  if (board.moveDetected(currentPlayer, waitingPlayer)) {
    confirmChanges(board.moveString);
  }

  // board.printBinary();
  // board.printReadings();
  board.printFullStatus();
  delay(50);
}

void confirmChanges(String move) {
  // Print move locally
  String player = currentPlayer == WHITE ? "White" : "Black";
  if (homePlayer == currentPlayer) {
    screen.printMove(player, move);
  } else {
    screen.rawPrint("   satisfied");
  }
  screen.debugPrintMove(player, move);

  // Reset board
  board.confirmChanges(currentPlayer);

  // Switch players
  waitingPlayer = currentPlayer;
  currentPlayer = WHITE + BLACK - currentPlayer;
}

// Particle Functions
int updateLight(String command) {
  if (command == "on") {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  return 1;
}

int updateScreen(String player) {
  screen.printPlayerMove(player);
  return 1;
}

#include <LiquidCrystal.h>

class Screen {
public:
    LiquidCrystal *lcd;

    Screen() {
      lcd = new LiquidCrystal(A0, A1, A2, A3, A4, A5);
    }

    void init() {
      lcd->begin(16, 2);
      _introSequence();
    }

    void printPlayerMove(String player) {
      lcd->clear();

      lcd->print(" " + player + "'s Move:");
      lcd->setCursor(0, 1);
      lcd->print("    C2 - C4");
    }

    void printMove(String player, String move) {
      lcd->clear();

      lcd->print(" " + player + "'s Move:");
      lcd->setCursor(0, 1);
      lcd->print("    " + move);
    }

    void debugPrintMove(String player, String output) {
      Serial.println(" " + player + "'s Move: " + output);
    }

    void rawPrint(String output) {
      lcd->clear();
      lcd->print(output);
    }

  private:

    void _introSequence() {
      lcd->print("   Yay Chess!");
      lcd->setCursor(0, 1);
      lcd->print("       - fat$$");

      delay(2000);

      lcd->setCursor(0, 0);
      for (int i = 0; i < 32; i++) {
        if (i == 16) lcd->setCursor(0, 1);

        lcd->print(" ");
        if (!(i >= 16 && i < 23)) delay(100);
      }
    }
};

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


void printStatus() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      int index = i + (j * 8);
      Position &position = positions[index];

      if (position.state != CONFIRMED) {
        Serial.print(position.position);
        Serial.print(": ");

        switch (position.state) {
          case UNSTABLE_OFF:
            switch (position.occupied) {
              case WHITE:
                Serial.println("unstable_off W  ");
                break;
              case BLACK:
                Serial.println("unstable_off B  ");
                break;
              case EMPTY:
                Serial.println("unstable_off _  ");
                break;
            }
            break;
          case UNSTABLE_ON:
            switch (position.occupied) {
              case WHITE:
                Serial.println("unstable_on  W  ");
                break;
              case BLACK:
                Serial.println("unstable_on  B  ");
                break;
              case EMPTY:
                Serial.println("unstable_on  _  ");
                break;
            }
            break;
          case UNSTABLE_OFF_ON:
            switch (position.occupied) {
              case WHITE:
                Serial.println("unstable_ofn W  ");
                break;
              case BLACK:
                Serial.println("unstable_ofn B  ");
                break;
              case EMPTY:
                Serial.println("unstable_ofn _  ");
                break;
            }
            break;
          case STABLE_OFF_ON:
            switch (position.occupied) {
              case WHITE:
                Serial.println("stable_ofn   W  ");
                break;
              case BLACK:
                Serial.println("stable_ofn   B  ");
                break;
              case EMPTY:
                Serial.println("stable_ofn   _  ");
                break;
            }
            break;
          case STABLE_OFF:
            switch (position.occupied) {
              case WHITE:
                Serial.println("stable_off   W  ");
                break;
              case BLACK:
                Serial.println("stable_off   B  ");
                break;
              case EMPTY:
                Serial.println("stable_off   _  ");
                break;
            }
            break;
          case STABLE_ON:
            switch (position.occupied) {
              case WHITE:
                Serial.println("stable_on    W  ");
                break;
              case BLACK:
                Serial.println("stable_on    B  ");
                break;
              case EMPTY:
                Serial.println("stable_on    _  ");
                break;
            }
            break;
        }
      }
    }
  }
  delay(500);
}

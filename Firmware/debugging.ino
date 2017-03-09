// call printStatus in loop()
void printStatus() {
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

  for (int i = 0; i < count; i++) {
    Serial.print(positions[i].position);
    Serial.print(":");
    Serial.print(positions[i].value);

    if (i == count - 1) {
      Serial.println("");
    } else {
      Serial.print(",");
    }
  }

  delay(500);
}


// Position#printState()
String printState() {
  switch (state) {
    case UNSTABLE_OFF:
      switch (occupied) {
        case WHITE:
          return "unstable_off W  ";
        case BLACK:
          return "unstable_off B  ";
        case EMPTY:
          return "unstable_off _  ";
      }
    case UNSTABLE_ON:
      switch (occupied) {
        case WHITE:
          return "unstable_on  W  ";
        case BLACK:
          return "unstable_on  B  ";
        case EMPTY:
          return "unstable_on  _  ";
      }
    case UNSTABLE_OFF_ON:
      switch (occupied) {
        case WHITE:
          return "unstable_ofn W  ";
        case BLACK:
          return "unstable_ofn B  ";
        case EMPTY:
          return "unstable_ofn _  ";
      }
    case STABLE_OFF_ON:
      switch (occupied) {
        case WHITE:
          return "stable_ofn   W  ";
        case BLACK:
          return "stable_ofn   B  ";
        case EMPTY:
          return "stable_ofn   _  ";
      }
    case STABLE_OFF:
      switch (occupied) {
        case WHITE:
          return "stable_off   W  ";
        case BLACK:
          return "stable_off   B  ";
        case EMPTY:
          return "stable_off   _  ";
      }
    case STABLE_ON:
      switch (occupied) {
        case WHITE:
          return "stable_on    W  ";
        case BLACK:
          return "stable_on    B  ";
        case EMPTY:
          return "stable_on    _  ";
      }
    case CONFIRMED:
      switch (occupied) {
        case WHITE:
          return "confirmed    W  ";
        case BLACK:
          return "confirmed    B  ";
        case EMPTY:
          return "confirmed    _  ";
      }
  }
}

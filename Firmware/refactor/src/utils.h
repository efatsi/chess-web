const int masterPins[3] = {D0, D1, D2};
const int slavePins[3]  = {D3, D4, D5};

void selectMasterPin(byte pin) {
  if (pin > 7) return;
  for (int i=0; i<3; i++) {
    if (pin & (1<<i))
      digitalWrite(masterPins[i], HIGH);
    else
      digitalWrite(masterPins[i], LOW);
  }
}

void selectSlavePin(byte pin) {
  if (pin < 0 || pin > 7) return;
  for (int i=0; i<3; i++) {
    if (pin & (1<<i))
      digitalWrite(slavePins[i], HIGH);
    else
      digitalWrite(slavePins[i], LOW);
  }
}

void sort(int a[], int size) {
  for(int i=0; i<(size-1); i++) {
    for(int j=0; j<(size-(i+1)); j++) {
      if(a[j] > a[j+1]) {
        int t = a[j];
        a[j] = a[j+1];
        a[j+1] = t;
      }
    }
  }
}

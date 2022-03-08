#include <math.h>

const int pins[5] = {14, 15, 16, 17, 18};

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], INPUT);
  }
}

void loop() {
  Serial.printf("PINS:  ");
  for (int i = 0; i < 5; i++) {
    float curPinVal = analogRead(pins[i]);
    float val = map(curPinVal, 0.0, 1024.0, 0.0, 3.3);
    Serial.printf("%d: %.2f    ", i, val);
  }
  Serial.printf("\n");
  delay(200);
}

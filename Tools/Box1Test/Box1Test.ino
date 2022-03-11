#include <math.h>

const int PIN = 14;
const int curResistorVal = 100000;


void setup() {
  // put your setup code here, to run once:
  pinMode(PIN, INPUT);
  
}

void loop() {
  float pinVal = analogRead(PIN);
  float voltVal = map(pinVal, 0, 1024, 0, 3.3);
  float resistance = voltVal*curResistorVal / (3.3-voltVal);
  Serial.printf("Val: %.2f      Res: %.2f\n", voltVal, resistance);
  delay(200);
}

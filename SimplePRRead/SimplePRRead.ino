
const int RESISTOR_BOX_YELLOW = 19;
const int RESISTOR_BOX_GREEN = 20;
const int RESISTOR_BOX_GRAY = 17;
const int RESISTOR_BOX_BLUE = 22;
const int RESISTOR_BOX_ORANGE = 23;

void setup() {
  // put your setup code here, to run once:
  pinMode(RESISTOR_BOX_YELLOW, INPUT);
  pinMode(RESISTOR_BOX_GREEN, INPUT);
  pinMode(RESISTOR_BOX_GRAY, INPUT);
  pinMode(RESISTOR_BOX_BLUE, INPUT);
  pinMode(RESISTOR_BOX_ORANGE, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("%d    ", analogRead(RESISTOR_BOX_YELLOW));
  Serial.printf("%d    ", analogRead(RESISTOR_BOX_GREEN));
  Serial.printf("%d    ", analogRead(RESISTOR_BOX_GRAY));
  Serial.printf("%d    ", analogRead(RESISTOR_BOX_BLUE));
  Serial.printf("%d    ", analogRead(RESISTOR_BOX_ORANGE));
  Serial.printf("\n");
  delay(5);

}

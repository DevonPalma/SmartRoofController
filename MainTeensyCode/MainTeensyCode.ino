#include "PRBoxController.h"
#include "Timer.h"

PRBoxController yellowBox(17);
PRBoxController greenBox(20);
PRBoxController grayBox(21);
PRBoxController blueBox(22);
PRBoxController orangeBox(23);

void setup() {
  Serial.printf("Raw, sample\n");
}

void loop() {
//  static Timer timer(1000/4);
  greenBox.tick();
  Serial.printf("%d,%d\n", greenBox.getRawValue(), greenBox.getValue());
  delay(10);
}

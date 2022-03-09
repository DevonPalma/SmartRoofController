#include "PhotoresistorBox.h"
#include "PINS.h"
#include <OneButton.h>
#include <Timer.h>

PhotoresistorBox orangeBox(RESISTOR_BOX_ORANGE);
PhotoresistorBox blueBox(RESISTOR_BOX_BLUE);
PhotoresistorBox grayBox(RESISTOR_BOX_GRAY);
PhotoresistorBox greenBox(RESISTOR_BOX_GREEN);
PhotoresistorBox yellowBox(RESISTOR_BOX_YELLOW);

OneButton myButton(POTENTIOMETER_BUTTON, true, true);

Timer displayTimer;

void setup() {
  displayTimer.setDuration(250);
  displayTimer.start();
}


void loop() {
  myButton.tick();

  orangeBox.tick();
  blueBox.tick();
  grayBox.tick();
  greenBox.tick();
  yellowBox.tick();

  if (displayTimer.isDone()) {
    displayTimer.start();
    Serial.printf("%03d    %03d    %03d    %03d    %03d\n", yellowBox.getSample(),
                  greenBox.getSample(), grayBox.getSample(), blueBox.getSample(), orangeBox.getSample());
  }
}

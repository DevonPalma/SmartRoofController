#include "PINS.h"
#include "Looper.h"
#include "EncoderLooper.h"
#include "PhotoresistorBox.h"

EncoderLooper myEncoder(ENCODER_A, ENCODER_B, ENCODER_BUTTON, ENCODER_RED, ENCODER_GREEN);
PhotoresistorBox yellowBox(RESISTOR_BOX_YELLOW);
PhotoresistorBox greenBox(RESISTOR_BOX_GREEN);
PhotoresistorBox grayBox(RESISTOR_BOX_GRAY);
PhotoresistorBox blueBox(RESISTOR_BOX_BLUE);
PhotoresistorBox orangeBox(RESISTOR_BOX_ORANGE);

void setup() {

  addLooper(myEncoder);
  myEncoder.attachClick(encoderClick);
  addLooper(yellowBox);
  addLooper(greenBox);
  addLooper(grayBox);
  addLooper(blueBox);
  addLooper(orangeBox);
}

void loop() {
  runLoopers();
}

void encoderClick() {
  static bool redHigh = false;

  redHigh = !redHigh;
  myEncoder.setRed(redHigh);
  myEncoder.setGreen(!redHigh);
}

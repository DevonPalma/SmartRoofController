
#include "Menu.h"
#include "EncoderData.h"
#include "ScreenData.h"
#include "PRBoxData.h"


void setup() {
  setupEncoder();
  setupScreen();
  setupMenu();
}

void loop() {
  loopEncoder();
  loopPRBoxes();
  loopMenu();
  loopScreen();

  
  static String lastYellowStateChange = yellowBox.getLastStateChange();

  String curYellowStateChange = yellowBox.getLastStateChange();

  if (lastYellowStateChange != curYellowStateChange) {
    Serial.printf("%s\n", curYellowStateChange.c_str());
    lastYellowStateChange = curYellowStateChange;
  }
}

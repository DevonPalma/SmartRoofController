#ifndef _PRBOX_DATA_H_
#define _PRBOX_DATA_H_

#include "Menu.h"
#include "PRBoxController.h"
#include "Presentation.h"



PresentationController presenter;

bool allBoxesCalibrated() {
  for (int i = 0; i < 5; i ++) {
    if (!boxControllers[i]->isCalibrated()) {
      return false;
    }
  }
  return true;
}

void stateWatcher() {
  if (!allBoxesCalibrated || !calibrationState == G_ACTIVE) {
    return;
  }
  static int lastStates[5];

  for (int i = 0; i < 5; i++) {
    int curState = boxControllers[i]->getLastStateChange();
    if (curState != lastStates[i]) {
      Serial.printf("New State %d\n", curState);
      switch (curState) {
        case OFF_INACTIVE:
          int clickCount = boxControllers[i]->getClicks();
          if (clickCount == 1) {
            Serial.printf("Clicked %d\n", i);
            presenter.clicked(i);
          } else {
            Serial.printf("MultiClicked %d %d times\n", i, clickCount);
            presenter.multiClicked(i, clickCount);
          }
          break;
      }
      lastStates[i] = curState;
    }
  }
}

void loopPRBoxes() {
  for (int i = 0; i < 5; i++) {
    boxControllers[i]->tick();
  }
  stateWatcher();
}

#endif

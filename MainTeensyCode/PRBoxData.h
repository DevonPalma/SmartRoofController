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
}

void loopPRBoxes() {
  for (int i = 0; i < 5; i++) {
    boxControllers[i]->tick();
  }
  stateWatcher();
}

#endif

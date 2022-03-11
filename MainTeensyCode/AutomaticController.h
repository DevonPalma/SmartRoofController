#ifndef _AUTO_CONTROLLER_H_
#define _AUTO_CONTROLLER_H_

#include "PRBoxController.h"
#include <math.h>
#include <hue.h>
#include "Timer.h"

// range = laserOff - laserOn
// Light brightness = map(curBrightness, laserOff, laserOff + range, 0, 100);

class AutomaticController {
    boolean isEnabled;
    PRBoxController* controller;
    Timer displayDelay;

  public:
    AutomaticController(PRBoxController& connectedController) {
      controller = &connectedController;
      displayDelay.setDuration(2500);
      displayDelay.start();
    }


    void enable() {
      isEnabled = true;
    }

    void disable() {
      isEnabled = false;
    }

    void toggle() {
      isEnabled = !isEnabled;
      Serial.printf("Toggled automatic mode %s\n", isEnabled ? "on" : "off");
    }

    bool isOn() {
      return isEnabled;
    }


    void tick() {
      if (isEnabled && controller->isCalibrated() && displayDelay.isDone()) {
        displayDelay.start();
        int laserOff = controller->getLaserOffValue();
        int range = laserOff - controller->getLaserOnValue();
        int brightness = map(controller->getValue(), laserOff, laserOff + range, 0, 100);

        Serial.printf("AUTO\n");
        for (int i = 0; i < 6; i++) {
          setHue(i, true, 22500, brightness, 0);
        }
      }
    }

};

#endif

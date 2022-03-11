#ifndef _AUTO_CONTROLLER_H_
#define _AUTO_CONTROLLER_H_

#include "PRBoxController.h"
#include <math.h>
#include <hue.h>

// range = laserOff - laserOn
// Light brightness = map(curBrightness, laserOff, laserOff + range, 0, 100);

class AutomaticController {
    boolean isEnabled;
    PRBoxController* controller;

  public:
    AutomaticController(PRBoxController& connectedController) {
      controller = &connectedController;
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


    void tick() {
      if (isEnabled && controller->isCalibrated()) {
        int laserOff = controller->getLaserOffValue();
        int range = laserOff - controller->getLaserOnValue();
        int brightness = map(controller->getValue(), laserOff, laserOff + range, 0, 100);

        for (int i = 0; i < 6; i++) {
          setHue(i, true, 22500, brightness, 0);
        }
      }
    }

};

#endif

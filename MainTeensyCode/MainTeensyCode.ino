#include "EncoderButton.h"
#include "Menu.h"
#include "PRBoxController.h"
#include "Presentation.h"
#include <Adafruit_BME280.h>
#include "AutomaticController.h"
#include "Timer.h"


const int ENCODER_PIN_A = 3;
const int ENCODER_PIN_B = 2;
const int ENCODER_PIN_BUTTON = 6;
const int ENCODER_PIN_RED = 4;
const int ENCODER_PIN_GREEN = 5;

const int PRBOX_PIN_YELLOW = 17;
const int PRBOX_PIN_GREEN = 20;
const int PRBOX_PIN_GRAY = 21;
const int PRBOX_PIN_BLUE = 22;
const int PRBOX_PIN_ORANGE = 23;

const int BME_ADDRESS = 0x76;

// Every bloody thing

EncoderButton myEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BUTTON, ENCODER_PIN_RED, ENCODER_PIN_GREEN);

// Setup Photoresistor boxes
PRBoxController prBoxA("Avery", 'A', PRBOX_PIN_YELLOW);
PRBoxController prBoxB("Brian", 'B', PRBOX_PIN_GREEN);
PRBoxController prBoxC("Chris", 'C', PRBOX_PIN_GRAY);
PRBoxController prBoxD("Devon", 'D', PRBOX_PIN_BLUE);
PRBoxController prBoxE("Edith", 'E', PRBOX_PIN_ORANGE);

// Put photoresistor boxes in a nice lil array
PRBoxController *boxControllers[5] = {&prBoxA, &prBoxB, &prBoxC, &prBoxD, &prBoxE};

AwfulMenu myMenu(boxControllers);

AutomaticController automode(prBoxA);

PresentationController presenter(automode);

Adafruit_BME280 myBME;

Timer delayTimer;

void setup() {
  setupEthernet();
  myEncoder.attachClick(encoderButtonClicked);
  myEncoder.attachDoubleClick(encoderButtonDoubleClicked);
  myEncoder.attachDuringLongPress(encoderButtonHeld);

  if (!myBME.begin(BME_ADDRESS)) {
    Serial.printf("BME Did not initialize on address 0x02X\n", BME_ADDRESS);
    while (true);
  }

  delayTimer.setDuration(3000);
}

void loop() {
  controllerLoop();
  myEncoder.tick();
  myMenu.handleRotationInput(myEncoder.read());
  myMenu.tick();
  automode.tick();
}

// This is in charge of looping over each boxConrtoller and ticking them
// If they are setup correctly and I am in the right menu, I go ahead and send
// the code to the presentation.h which controls sub-functions
void controllerLoop() {
  bool isPrimed = myMenu.getActiveState() == EXPLORER ;
  for (int i = 0; i < 5; i++) {
    boxControllers[i]->tick();
    if (!boxControllers[i]->isCalibrated()) {
      isPrimed = false;
    }
  }
  // if the menu isn't in explorer mode or one of the buttons isn't callibrated, go ahead and skip over the last part of the loop
  if (!isPrimed) {
   return; 
  } 
  
  static int lastStates[5];
  if (!delayTimer.isRunning()) {
    delayTimer.start();
    for (int i = 0; i < 5; i++) {
      lastStates[i] = OFF_INACTIVE;
    }
    return;
  } 
  
  if (!delayTimer.isDone()) {
    return;
  }


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


void encoderButtonClicked() {
  myMenu.handleClickInput();
}

void encoderButtonDoubleClicked() {
  myMenu.handleDoubleClickInput();
}

void encoderButtonHeld() {
  myMenu.handleLongPressInput();
}

#include "EncoderButton.h"
#include "WemoWrapper.h"
#include "Menu.h"
#include "PRBoxController.h"

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

// Every bloody thing

EncoderButton myEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_PIN_BUTTON, ENCODER_PIN_RED, ENCODER_PIN_GREEN);
WemoWrapper wemo();

// Setup Photoresistor boxes
PRBoxController prBoxA("Avery", 'A', PRBOX_PIN_YELLOW);
PRBoxController prBoxB("Brian", 'B', PRBOX_PIN_GREEN);
PRBoxController prBoxC("Chris", 'C', PRBOX_PIN_GRAY);
PRBoxController prBoxD("Devon", 'D', PRBOX_PIN_BLUE);
PRBoxController prBoxE("Edith", 'E', PRBOX_PIN_ORANGE);

// Put photoresistor boxes in a nice lil array
PRBoxController *boxControllers[5] = {&prBoxA, &prBoxB, &prBoxC, &prBoxD, &prBoxE};

AwfulMenu myMenu(boxControllers);


void setup() {
  myEncoder.attachClick(encoderButtonClicked);
  myEncoder.attachDoubleClick(encoderButtonDoubleClicked);
  myEncoder.attachDuringLongPress(encoderButtonHeld);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    boxControllers[i]->tick();
  }
  myEncoder.tick();
  myMenu.handleRotationInput(myEncoder.read());
  myMenu.tick();
  
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

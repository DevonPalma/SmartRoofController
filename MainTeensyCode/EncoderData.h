#ifndef _ENCODER_DATA_H_
#define _ENCODER_DATA_H_

#include <Encoder.h>
#include <OneButton.h>

const int ENCODER_A = 3;
const int ENCODER_B = 2;
const int ENCODER_BUTTON = 6;
const int ENCODER_RED = 4;
const int ENCODER_GREEN = 5;

Encoder myEncoder(ENCODER_A, ENCODER_B);
OneButton myEncoderButton(ENCODER_BUTTON);


enum ENCODER_COLOR {
  BLACK,
  RED,
  GREEN,
  YELLOW,
};


void setEncoderColor(int color) {
  switch (color) {
    case BLACK:
      digitalWrite(ENCODER_RED, LOW);
      digitalWrite(ENCODER_GREEN, LOW);
      break;
    case RED:
      digitalWrite(ENCODER_RED, HIGH);
      digitalWrite(ENCODER_GREEN, LOW);
      break;
    case GREEN:
      digitalWrite(ENCODER_RED, LOW);
      digitalWrite(ENCODER_GREEN, HIGH);
      break;
    case YELLOW:
      digitalWrite(ENCODER_RED, HIGH);
      digitalWrite(ENCODER_GREEN, HIGH);
      break;
  }
}


void clampEncoder(int min, int max) {
  int curValue = myEncoder.read();
  if (curValue < min) {
    curValue = min;
  } else if (curValue > max) {
    curValue = max;
  }
  myEncoder.write(curValue);
}


void setupEncoder() {
  pinMode(ENCODER_RED, OUTPUT);
  pinMode(ENCODER_GREEN, OUTPUT);
  setEncoderColor(RED);
}

void loopEncoder() {
  myEncoderButton.tick();
}

#endif

#ifndef _ENCODER_DATA_H_
#define _ENCODER_DATA_H_

#include "PINS.h"
#include "Looper.h"
#include <Encoder.h>
#include <OneButton.h>


class EncoderLooper : public Looper, public Encoder, public OneButton {
    int _pinR;
    bool _pinRState;
    int _pinG;
    bool _pinGState;


  public:
    EncoderLooper(int pinA, int pinB, int buttonPin, int pinR, int pinG) : Encoder(pinA, pinB), OneButton(buttonPin, true, true) {
      pinMode(pinR, OUTPUT);
      _pinR = pinR;
      pinMode(pinG, OUTPUT);
      _pinG = pinG;
    }

    void tick() {
      OneButton::tick();
    }
    void update() {
      Serial.printf("\n");
    }
    void draw() {
      digitalWrite(_pinR, _pinRState);
      digitalWrite(_pinG, _pinGState);
    }

    void setRed(bool state) {
      _pinRState = state;
    }

    void setGreen(bool state) {
      _pinGState = state;
    }
};


#endif

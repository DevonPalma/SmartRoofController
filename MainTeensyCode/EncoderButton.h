#ifndef _ENCODER_DATA_H_
#define _ENCODER_DATA_H_

#include <Encoder.h>
#include <OneButton.h>

class EncoderButton : public Encoder, public OneButton {
  private:
    int _pinRed;
    int _pinGreen;
  public:
    EncoderButton(int pinA, int pinB, int pinButton, int pinRed, int pinGreen) : Encoder(pinA, pinB), OneButton(pinButton) {
      _pinRed = pinRed;
      pinMode(_pinRed, OUTPUT);
      _pinGreen = pinGreen;
      pinMode(_pinGreen, OUTPUT);
    }

    void clamp(int min, int max) {
      int curValue = Encoder::read();
      if (curValue < min) {
        curValue = min;
      } else if (curValue > max) {
        curValue = max;
      }
      Encoder::write(curValue);
    }
};
#endif

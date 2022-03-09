#include "Sampler.h"
#include "Looper.h"


class PhotoresistorBox : private Sampler, public Looper {

    int _pin;

    int sampledMax;
    int sampledMin;

    int lastSample;

  public:
    PhotoresistorBox(int pin) {
      _pin = pin;
      pinMode(_pin, INPUT);
    }

    void tick() {
      addSample(analogRead(_pin));
    }

    void update() {
      lastSample = getSample();
      Serial.printf("%d:%d    ", _pin, lastSample); 
    }

    void draw() {}

    void setLaserOff() {
      sampledMax = lastSample;
    }

    void setLaserOn() {
      sampledMin = lastSample;
    }

    bool isOn() {
      int mid = (sampledMax + sampledMin) / 2;
      return lastSample > mid;
    }


};

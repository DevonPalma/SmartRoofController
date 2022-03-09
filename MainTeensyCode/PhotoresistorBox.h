#include "Sampler.h"

const int SAMPLE_COUNT = 50;

class PhotoresistorBox : public Sampler{

    int _pin;

    int sampledMax;
    int sampledMin;

  public:
    PhotoresistorBox(int pin) : Sampler(SAMPLE_COUNT) {
      _pin = pin;
      pinMode(_pin, INPUT);
    }

    void tick() {
      addSample(analogRead(_pin));
    }

    void setLaserOff() {
      sampleMax = getSample();
    }

    void setLaserOn() {
      sampleMin = getSample();
    }

    bool isOn() {
      int mid = (sampleMax + sampleMin) / 2;
      return getSample() > mid;
    }


};



/*
   PRBoxController

   has two main components the sampling and the state machine


   Sampling:
      Every tick a sample is taken from the getValue function
      this is then added to a total sample value
      after a certain number of samples are taken, the total sample value
        is devided by the number of samples taken, returning the average sample
      The average sample will then be used as the "main sample" for the state
        machine and any other functions which need it

   State Machine
      INACTIVE
        if laser goes on, switch to ON
      ON
        if laser goes off
           if totalTime < 20ms
              switch to INACTIVE -- debouncing
           else
              switch to OFF
        else if startTime + pressTimeout < now
          switch to LONG_ON
      OFF
        add 1 to click count
        if laser goes on
            switch to MULTI_ON
        if startTime + multiClickTimeout < now
          switch to INACTIVE
      MULTI_ON
        if laser goes off
          if totalTime < 20ms
            subtract 1 from click count -- debouncing
          switch to OFF
      LONG_ON
        if laser goes off
          switchToState(INACTIVE)


*/


class PRBoxController {
    // Handle all sampling information
    int _lastValue;
    int _sampleValue;
    int _sampleCount;
    int _sampleMax;
    int _lowSetting;
    int _highSetting;
    int _pin;

    // collects a sample every tick
    // After _sampleMax samples are collected
    // set the lastValue to the average of the last set samples
    // Clear all sample data
    void _tickSampler() {
      // handle sampler
      _sampleCount += 1;
      _sampleValue += getRawValue();
      if (_sampleCount >= _sampleMax) {
        _lastValue = (float)_sampleValue / _sampleCount;
        _sampleValue = 0;
        _sampleCount = 0;
      }
    }


    //    enum STATE {
    //      PR_INACTIVE,
    //      PR_ON,
    //      PR_LONG_ON,
    //      PR_MULTI_ON,
    //      PR_OFF
    //    };
    //    STATE _currentState = PR_INACTIVE;
    //    int _clickCount = 0;
    //    int _startTime;
    //    int _pressTimeout;
    //    int _multiClickTimeout;
    //
    //
    //
    //    void _enterInactive() {
    //      Serial.printf("Entered Inactive\n");
    //      _currentState = PR_INACTIVE;
    //    }
    //    void _tickInactive() {
    //      if (isActive()) {
    //        _leaveInactive();
    //        _enterOn();
    //      }
    //    }
    //    void _leaveInactive() {
    //      Serial.printf("Left INACTIVE ");
    //      _clickCount = 0; //So the click count can be retrieved in case I need it
    //    }
    //
    //
    //    void _enterOn() {
    //      Serial.printf("Entered ON\n");
    //      _currentState = PR_ON;
    //      _startTime = millis();
    //    }
    //    void _tickOn() {
    //      if (!isActive()) {
    //        if (_startTime + 20 < millis()) { // debouncing in case its needed
    //          _leaveOn();
    //          _enterInactive();
    //        } else {
    //          _leaveOn();
    //          _enterOff();
    //        }
    //      } else if (_startTime + _multiClickTimeout < millis()) {
    //        _leaveOn();
    //        _enterLongOn();
    //      }
    //    }
    //    void _leaveOn() {
    //      Serial.printf("Left ON ");
    //    }
    //
    //
    //    void _enterLongOn() {
    //      Serial.printf("Entered LONG_ON\n");
    //      _currentState = PR_LONG_ON;
    //      _startTime = millis();
    //    }
    //    void _tickLongOn() {
    //      if (!isActive()) {
    //        _leaveLongOn();
    //        _enterInactive();
    //      }
    //    }
    //    void _leaveLongOn() {
    //      Serial.printf("Left LONG_ON ");
    //    }
    //
    //
    //
    //    void _enterMultiOn() {
    //      Serial.printf("Entered MULTI_ON\n");
    //      _currentState = PR_MULTI_ON;
    //      _startTime = millis();
    //    }
    //    void _tickMultiOn() {
    //      if (!isActive()) {
    //        if (_startTime + 20 > millis()) {
    //          _clickCount--; // debouncing
    //        }
    //        _leaveMultiOn();
    //        _enterOff();
    //      }
    //    }
    //    void _leaveMultiOn() {
    //      Serial.printf("Left MULTI_ON ");
    //    }
    //
    //
    //
    //    void _enterOff() {
    //      Serial.printf("Enter OFF\n");
    //      _currentState = PR_OFF;
    //      _startTime = millis();
    //      _clickCount++;
    //    }
    //    void _tickOff() {
    //      if (isActive()) {
    //        _leaveOff();
    //        _enterMultiOn();
    //      } else if (_startTime + _multiClickTimeout < millis()) {
    //        _leaveOff();
    //        _enterInactive();
    //      }
    //    }
    //    void _leaveOff() {
    //      Serial.printf("Left OFF ");
    //    }

  public:
    PRBoxController(int PRBoxPin) {
      _pin = PRBoxPin;
      pinMode(_pin, INPUT);
      _sampleMax = 1;
      //      _pressTimeout = 1000;
      //      _multiClickTimeout = 1000;
    }

    int getRawValue() {
      return analogRead(_pin);
    }

    int getValue() {
      return _lastValue;
    }

    int isActive() { // TODO: recheck this IDK if I have the right comparison
      return (_lowSetting + _highSetting) / 2 > _lastValue;
    }

    void tick() {
      _tickSampler();
      //      switch (_currentState) {
      //
      //        case PR_INACTIVE:
      //          _tickInactive();
      //          break;
      //        case PR_ON:
      //          _tickOn();
      //          break;
      //        case PR_LONG_ON:
      //          _tickLongOn();
      //          break;
      //        case PR_MULTI_ON:
      //          _tickMultiOn();
      //          break;
      //        case PR_OFF:
      //          _tickOff();
      //          break;
      //      }
    }

    void setHighToCurrent() {
      _highSetting = _lastValue;
    }

    void setLowToCurrent() {
      _lowSetting = _lastValue;
    }
};

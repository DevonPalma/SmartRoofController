#ifndef _PR_BOX_CONTROLLER_H_
#define _PR_BOX_CONTROLLER_H_
#include <math.h>
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


enum STATE_CHANGES {
  INACTIVE_ON,
  DEBOUNCED_ON_INACTIVE,
  ON_OFF,
  ON_LONGON,
  LONGON_INACTIVE,
  MULTION_OFF,
  DEBOUNCED_MULTION_OFF,
  OFF_MULTION,
  OFF_INACTIVE,
};

enum STATE {
  PR_INACTIVE,
  PR_ON,
  PR_LONG_ON,
  PR_MULTI_ON,
  PR_OFF
};

const int LASER_OFF_DEFAULT_VALUE = 1024;
const int LASER_ON_DEFAULT_VALUE = 0;

/*
 * Basic required logic
 * When there is no laser on the PR box, its value will increase towards 1024
 * When there is a laser on the PR box, its value will decrease towards 0
 */
class PRBoxController {

    //==============================
    //    Main Variables
    //==============================
    String _name;
    char _symbol;
    int _laserOffValue;
    int _laserOnValue;
    int _lastValue;

    //==============================
    //      Sampling Variables
    //==============================
    int _sampleValue;
    int _sampleCount;
    int _sampleMax;
    int _pin;

    //==============================
    //      State Variables
    //==============================

    STATE _currentState = PR_INACTIVE;
    int _clickCount = 0;
    int _startTime;
    int _pressTimeout;
    int _multiClickTimeout;
    STATE_CHANGES _lastStateChange;


    //==============================
    //      Sampling Functions
    //==============================

    // collects a sample every tick
    // After _sampleMax samples are collected
    // set the lastValue to the average of the last set samples
    // Clear all sample data
    void _tickSampler() {
      // handle sampler
      _sampleCount += 1;
      _sampleValue += getRawValue();
      if (_sampleCount >= _sampleMax) {
        _lastValue = _sampleValue / _sampleCount;
        _sampleValue = 0;
        _sampleCount = 1;
      }
    }



    //==============================
    //      State Functions
    //==============================

    // State functions follow the format
    // _enterSTATENAME() -- called whenever the state is entered
    // _tickSTATENAME() -- called each loop if this is the current state
    // _leaveSTATENAME() -- called whenever the state is left


    // INACTIVE State functions
    void _enterInactive() {
      _currentState = PR_INACTIVE;
    }

    void _tickInactive() {
      if (isActive()) {
        _leaveInactive();
        _enterOn();
        _lastStateChange = INACTIVE_ON;
      }
    }

    void _leaveInactive() {
      _clickCount = 0;
    }



    // ON state functions
    void _enterOn() {
      _currentState = PR_ON;
      _startTime = millis();
    }

    void _tickOn() {
      if (!isActive()) { // Check if the pr box controller is still active
        if (_startTime + 20 > millis()) { // If I haven't been in state for 20ms, consider it a debounce
          _leaveOn();
          _enterInactive();
          _lastStateChange = DEBOUNCED_ON_INACTIVE;
        } else {
          _leaveOn();
          _enterOff();
          _lastStateChange = ON_OFF;
        }
      } else if (_startTime + _pressTimeout < millis()) { // check if we have been in the state for the _pressTimeout duration
        _leaveOn();
        _enterLongOn();
        _lastStateChange = ON_LONGON;
      }
    }

    void _leaveOn() {}


    // LONG_ON state functions
    void _enterLongOn() {
      _currentState = PR_LONG_ON;
      _startTime = millis();
    }

    void _tickLongOn() {
      if (!isActive()) { // Check if the PR box is no longer active
        _leaveLongOn();
        _enterInactive();
        _lastStateChange = LONGON_INACTIVE;
      }
    }

    void _leaveLongOn() {}


    // MULTI_ON State functions
    void _enterMultiOn() {
      _currentState = PR_MULTI_ON;
      _startTime = millis();
    }

    void _tickMultiOn() {
      if (!isActive()) { // check if the PR box is no longer on
        _lastStateChange = MULTION_OFF;
        if (_startTime + 20 > millis()) { // if I haven't been in the MultiOn state for more than 20ms, consider it a debounce
          _clickCount--; // debouncing
          _lastStateChange = DEBOUNCED_MULTION_OFF;
        }
        _leaveMultiOn();
        _enterOff();
      }
    }

    void _leaveMultiOn() {}


    // OFF state functions
    void _enterOff() {
      _currentState = PR_OFF;
      _startTime = millis();
      _clickCount++;
    }

    void _tickOff() {
      if (isActive()) { // check if pr box has gone high
        _lastStateChange = OFF_MULTION;
        _leaveOff();
        _enterMultiOn();
      } else if (_startTime + _multiClickTimeout < millis()) { // check if the pr box has remained off for _multiClickTimeout ms
        _lastStateChange = OFF_INACTIVE;
        _leaveOff();
        _enterInactive();
      }
    }
    void _leaveOff() {}



  public:
    // constructor
    PRBoxController(String PRBoxName, char PRBoxSymbol, int PRBoxPin) {
      _name = PRBoxName;
      _symbol = PRBoxSymbol;
      _pin = PRBoxPin;

      pinMode(_pin, INPUT);
      // set defaults for sampling
      _sampleMax = 7;
      // set defauls for state machine
      _pressTimeout = 500;
      _multiClickTimeout = 1000;
      setToDefault();
    }

    // should be called every loop()
    void tick() {
      _tickSampler();
      switch (_currentState) {

        case PR_INACTIVE:
          _tickInactive();
          break;
        case PR_ON:
          _tickOn();
          break;
        case PR_LONG_ON:
          _tickLongOn();
          break;
        case PR_MULTI_ON:
          _tickMultiOn();
          break;
        case PR_OFF:
          _tickOff();
          break;
      }
    }

    // utility functions
    
    // checks if the PR box would be considered "ACTIVE"
    bool isActive() { 
      int distFromOn =  abs(_laserOnValue - _lastValue);
      int distFromOff = abs(_laserOffValue - _lastValue);
       return distFromOn < distFromOff;
    }

    void calibrateLaserOn() {
      setLaserOn(getValue());
    }
    
    void calibrateLaserOff() {
      setLaserOff(getValue());
    }
    
    bool isCalibrated() {
      return getLaserOnValue() != 0 && getLaserOffValue() != 1024;
    }


    // general variable getters
    int getRawValue() {
      return analogRead(_pin);
    }

    int getValue() {
      return _lastValue;
    }
    
    int getLaserOnValue() {
      return _laserOnValue;
    }
    
    int getLaserOffValue() {
      return _laserOffValue;
    }

    char getSymbol() {
      return _symbol;
    }

    String * getName() {
      return &_name;
    }

    int getClicks() {
      return _clickCount;
    }

    STATE_CHANGES getLastStateChange() {
      return _lastStateChange;
    }


    // Setter functions
    void setLaserOn(int val) {
      _laserOnValue = val;
    }

    void setLaserOff(int val) {
      _laserOffValue = val;
    }

    void setToDefault() {
      _laserOffValue = LASER_OFF_DEFAULT_VALUE;
      _laserOnValue = LASER_ON_DEFAULT_VALUE;
    }

    
};

#endif

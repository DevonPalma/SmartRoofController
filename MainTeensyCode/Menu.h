#ifndef _MENU_H_
#define _MENU_H_

#include "PRBoxController.h"

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Timer.h"

/*
   main menu:
    display each box attached to the controller
    when I spen encoder, look through them
    When I click encoder, open the corresponding box;s sub menu

   sub menu for each box:
    display the settings for the specified box on second screen
      - current value
      - Current Max Brightness
      - current Min Brightness
      - Is considered "on"
    display the setting changes I can do
      - Wipe the current settings
      - Set the max brightness to the current value
      - set the min brightness to the current value

*/

enum MENU_STATE {
  EXPLORER,
  GENERAL_SETTINGS,
  SUPER_SETTINGS,
};


const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int SCREEN1_ADDRESS = 0x3C;
const int SCREEN2_ADDRESS = 0x3D;
const int SCREEN_REFRESH_RATE = 20; // How many times per second should I update the screens

class AwfulMenu {
    String _title = "Settings";

    static const int explorerCount = 6;
    String explorerItems[explorerCount] = {"All", "Avery", "Brian", "Chris", "Devon", "Edith"};
    int explorerCursor = 0;

    static const int generalSettingCount = 2;
    String generalSettingItems[generalSettingCount] = {"Set Laser Off", "Set Laser On"};
    int generalSettingCursor = 0;

    static const int superSettingCount = 3;
    String superSettingItems[superSettingCount] = {"All lasers off", "Scale to A", "Reset"};
    int superSettingCursor = 0;

    int explorerGeneralSplit = 40;

    MENU_STATE currentState;

    Timer displayTimer;

    Adafruit_SSD1306 *display1;
    Adafruit_SSD1306 *display2;

    PRBoxController **boxes;




    int clamp(int val, int min, int max) {
      if (val < min) {
        return min;
      } else if (val > max) {
        return max;
      }
      return val;
    }




    void _setLaserToOff(int i) {
      boxes[i]->calibrateLaserOff();
    }

    void _setLaserToOn(int i) {
      boxes[i]->calibrateLaserOn();
    }

    void _setAllLasersOff() {
      for (int i = 0; i < 5; i++ ) {
        boxes[i]->calibrateLaserOff();
      }
    }

    void _scaleToA() {
      int diff = boxes[0]->getLaserOffValue() - boxes[0]->getValue();
      for (int i = 0; i < 5; i++ ) {
        boxes[i]->setLaserOn(boxes[i]->getLaserOffValue() - diff);
      }
    }

    void _resetAllLasers() {
      for (int i = 0; i < 5; i++ ) {
        boxes[i]->setToDefault();
      }
    }


    void _resetScreen(Adafruit_SSD1306 *disp) {
      disp->clearDisplay();
      disp->setCursor(0, 0);
      disp->setTextColor(SSD1306_WHITE);
      disp->setTextSize(1);
    }

    void _pushScreen(Adafruit_SSD1306 *disp) {
      disp->display();
    }

    void _drawScreen1() {
      _resetScreen(display1);

      // Draw the menu title
      display1->printf("   %s\n", _title.c_str());
      display1->drawFastHLine(0, 7, SCREEN_WIDTH, SSD1306_WHITE);
      display1->drawFastVLine(explorerGeneralSplit, 7, SCREEN_HEIGHT, SSD1306_WHITE);

      // Draw the explorer options
      for (int i = 0; i < explorerCount; i++) {
        if (explorerCursor == i) { // if the current option is selected, invert it
          display1->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        } else {
          display1->setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        }
        display1->printf("%s\n", explorerItems[i].c_str());
      }


      if (currentState == GENERAL_SETTINGS) {
        for (int i = 0; i < generalSettingCount; i++) {
          if (generalSettingCursor == i) {
            display1->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          } else {
            display1->setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          }
          display1->setCursor(explorerGeneralSplit + 2, 8 + 8 * i);
          display1->printf("%s\n", generalSettingItems[i].c_str());
        }
      }

      if (currentState == SUPER_SETTINGS) {
        for (int i = 0; i < superSettingCount; i++) {
          if (superSettingCursor == i) {
            display1->setTextColor(SSD1306_BLACK, SSD1306_WHITE);
          } else {
            display1->setTextColor(SSD1306_WHITE, SSD1306_BLACK);
          }
          display1->setCursor(explorerGeneralSplit + 2, 8 + 8 * i);
          display1->printf("%s\n", superSettingItems[i].c_str());
        }
      }

      _pushScreen(display1);
    }



    void _drawScreen2() {
      _resetScreen(display2);

      switch (currentState) {
        case EXPLORER:
        case GENERAL_SETTINGS:
          switch (explorerCursor) {
            case 0:
              display2->printf("   ON  CUR  OFF\n");
              for (int i = 0; i < 5; i++) {
                bool isBoxActive = boxes[i]->isActive();
                display2->printf("%c: %d %c %d %c %d\n", boxes[i]->getSymbol(), boxes[i]->getLaserOnValue(),
                                 isBoxActive ? '<' : ' ', boxes[i]->getValue(), isBoxActive ? ' ' : '>', boxes[i]->getLaserOffValue());
              }
              break;
            default:
              PRBoxController* activeBox = boxes[explorerCursor - 1];
              display2->printf("Current  : %d\n", activeBox->getValue());
              display2->printf("Laser Off: %d\n", activeBox->getLaserOffValue());
              display2->printf("Laser On : %d\n", activeBox->getLaserOnValue());
              display2->printf("State    : %s\n", activeBox->isActive() ? "on" : "off");
              break;
          }
          break;
        case SUPER_SETTINGS:
          switch (superSettingCursor) {
            case 2:
              display2->printf("   ON  CUR  OFF\n");
              for (int i = 0; i < 5; i++) {
                bool isBoxActive = boxes[i]->isActive();
                display2->printf("%c: %d %c %d %c %d\n", boxes[i]->getSymbol(), boxes[i]->getLaserOnValue(),
                                 isBoxActive ? '<' : ' ', boxes[i]->getValue(), isBoxActive ? ' ' : '>', boxes[i]->getLaserOffValue());
              }
              break;
            case 0:
              for (int i = 0; i < 5; i++) {
                display2->printf("%c: %d -> Off(%d)\n", boxes[i]->getSymbol(), boxes[i]->getValue(), boxes[i]->getLaserOffValue());
              }
              break;
            case 1:
              int diff = boxes[0]->getLaserOffValue() - boxes[0]->getValue();
              display2->printf("%c: off - cur = diff\n", boxes[0]->getSymbol());
              display2->printf("  %d - %d = %d\n\n", boxes[0]->getLaserOffValue(), boxes[0]->getValue(), diff);
              for (int i = 0; i < 5; i++) {
                display2->printf("%c: %d -> ON(%d)\n", boxes[i]->getSymbol(), boxes[i]->getLaserOffValue() - diff, boxes[i]->getLaserOnValue());
              }

              break;
          }
          break;
      }

      _pushScreen(display2);
    }

  public:

    AwfulMenu(PRBoxController **allBoxes) {
      boxes = allBoxes;
      display1 = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
      if (!display1->begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDRESS)) {
        Serial.printf("SSD1306 allocation failed");
        while (true);
      }
      display2 = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
      if (!display2->begin(SSD1306_SWITCHCAPVCC, SCREEN2_ADDRESS)) {
        Serial.printf("SSD1306 allocation failed");
        while (true);
      }

      display1->cp437(true);
      display2->cp437(true);

      displayTimer.setDuration(1000 / SCREEN_REFRESH_RATE);
      displayTimer.start();
    }

    void handleClickInput() {
      switch (currentState) {
        case EXPLORER:
          switch (explorerCursor) {
            case 0: // ALL
              currentState = SUPER_SETTINGS;
              break;
            default:
              currentState = GENERAL_SETTINGS;
              break;
          }
          break;
      }
    }

    void handleDoubleClickInput() {
      switch (currentState) {
        case EXPLORER:
          explorerCursor = 0;
          break;
        case GENERAL_SETTINGS:
          currentState = EXPLORER;
          generalSettingCursor = 0;
          break;
        case SUPER_SETTINGS:
          currentState = EXPLORER;
          superSettingCursor = 0;
          break;
      }
    }

    void handleLongPressInput() {
      switch (currentState) {
        case GENERAL_SETTINGS:
          switch (generalSettingCursor) {
            case 0: // "Set laser off" instruction
              _setLaserToOff(explorerCursor - 1);
              break;
            case 1: // "Set laser on" instruction
              _setLaserToOn(explorerCursor - 1);
              break;
          }
          break;
        case SUPER_SETTINGS:
          switch (superSettingCursor) {
            case 0:
              _setAllLasersOff();
              break;
            case 1:
              _scaleToA();
              break;
            case 2:
              _resetAllLasers();
              break;
          }
      }
    }

    void handleRotationInput(int newRotation) {
      static int lastRotation = newRotation;


      int rotationOffset = 0;
      if (lastRotation < newRotation - 2) {
        rotationOffset = -1;
      } else if (lastRotation > newRotation + 2) {
        rotationOffset = 1;
      }

      if (rotationOffset != 0) { // no point in updating the variables and doing the switch, if we aren't adding
        switch (currentState) {
          case EXPLORER:
            explorerCursor = clamp(explorerCursor + rotationOffset, 0, explorerCount - 1);
            break;
          case GENERAL_SETTINGS:
            generalSettingCursor = clamp(generalSettingCursor + rotationOffset, 0, generalSettingCount - 1);
            break;
          case SUPER_SETTINGS:
            superSettingCursor = clamp(superSettingCursor + rotationOffset, 0, superSettingCount - 1);
            break;
        }
        // inside if statement otherwise if the encoder would be span really slow, it would never actually move up
        lastRotation = newRotation;
      }
    }

    void tick() {
      if (displayTimer.isDone()) {
        displayTimer.start();
        _drawScreen1();
        _drawScreen2();
      }
    }

    MENU_STATE getActiveState() {
      return currentState;
    }
};



#endif

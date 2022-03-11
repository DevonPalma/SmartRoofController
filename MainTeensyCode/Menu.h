#ifndef _MENU_H_
#define _MENU_H_

//#include "GlobalStateMachine.h"
//#include "ScreenData.h"
//#include "EncoderData.h"
//#include "PRBoxData.h"
//#include "Timer.h"


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


    int clamp(int val, int min, int max) {
      if (val < min) {
        return min;
      } else if (val > max) {
        return max;
      }
      return val;
    }




    void _setLaserToOff(int i) {
      Serial.printf("SET LASER %d OFF\n", i);
    }

    void _setLaserToOn(int i) {
      Serial.printf("SET LASER %d ON\n", i);
    }

    void _setAllLasersOff() {
      Serial.printf("SET ALL LASER OFF\n");
    }

    void _scaleToA() {
      Serial.printf("Scale to A\n");
    }

    void _resetAllLasers() {
      Serial.printf("Reset all lasers\n");

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
      _pushScreen(display2);
    }

  public:

    AwfulMenu() {
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
};




//const String MENU_TITLE = "PR Box Settings";
//
//const int PRB_EXPLORER_COUNT = 6;
//const String PRB_EXPLORER_ITEMS[] = {"All", "Yellow", "Green", "Gray", "Blue", "Orange"};
//
//enum PRB_EXPLORER_IDS {
//  EXPLORER_ALL,
//  EXPLORER_YELLOW,
//  EXPLORER_GREEN,
//  EXPLORER_GRAY,
//  EXPLORER_BLUE,
//  EXPLORER_ORANGE
//};
//
//const int PRB_SETTINGS_COUNT = 2;
//const String PRB_SETTINGS_ITEMS[] = {"Set laser off", "Set laser on"};
//
//enum PRB_SETTINGS_IDS {
//  SETTINGS_SET_LASER_OFF,
//  SETTINGS_SET_LASER_ON
//};
//
//const int PRB_ALL_SETTINGS_COUNT = 3;
//const String PRB_ALL_SETTINGS_ITEMS[] = {"All lasers off", "Scale to yello", "reset"};
//
//enum PRB_ALL_SETTINGS_IDS {
//  ALL_SETTINGS_LASER_OFF,
//  ALL_SETTINGS_LASER_YELLOW,
//  ALL_SETTINGS_RESET
//};

//const int MENU_SPLIT = 40;
//
//enum MENU_STATES {
//  EXPLORER,
//  SETTINGS,
//  ALL_SETTINGS,
//};
//MENU_STATES currentMenuState = EXPLORER;

//Timer menuDisplayTimer;

//int prbExplorerIndex = 0;
//int prbSettingsIndex = 0;
//int prbAllSettingsIndex = 0;

//int controllerOffset;
//bool controllerClicked;
//bool controllerDoubleClicked;
//bool controllerLongClicked;
//
//void menuClick() {
//  controllerClicked = true;
//}
//void menuDoubleClick() {
//  controllerDoubleClicked = true;
//}
//void menuLongPressLoop() {
//  controllerLongClicked = true;
//}





//void tickController() {
//  static int lastEncoderVal = myEncoder.read();
//  int curEncoderVal = myEncoder.read();
//
//  if (curEncoderVal < lastEncoderVal) {
//    controllerOffset = 1;
//  } else if (curEncoderVal > lastEncoderVal) {
//    controllerOffset = -1;
//  } else {
//    controllerOffset = 0;
//  }
//  lastEncoderVal = curEncoderVal;
//}


//void tickExplorer() {
//  prbExplorerIndex += controllerOffset;
//
//  if (prbExplorerIndex < 0) {
//    prbExplorerIndex = 0;
//  } else if (prbExplorerIndex >= PRB_EXPLORER_COUNT) {
//    prbExplorerIndex = PRB_EXPLORER_COUNT - 1;
//  }
//
//  if (controllerClicked) {
//    switch (prbExplorerIndex) {
//      case EXPLORER_ALL:
//        prbAllSettingsIndex = 0;
//        currentMenuState = ALL_SETTINGS;
//        activeGlobalState = G_CALLIBRATING;
//        break;
//      default:
//        prbSettingsIndex = 0;
//        currentMenuState = SETTINGS;
//        activeGlobalState = G_CALLIBRATING;
//        break;
//    }
//  }
//}


//PRBoxController* _getCurrentSelectedBox() {
//  switch (prbExplorerIndex) {
//    case EXPLORER_ALL:
//      Serial.printf("_getCurrentSelectedLaser shouldn't have been called\n");
//      break;
//    case EXPLORER_YELLOW:
//      return &yellowBox;
//    case EXPLORER_GREEN:
//      return &greenBox;
//    case EXPLORER_GRAY:
//      return &grayBox;
//    case EXPLORER_BLUE:
//      return &blueBox;
//    case EXPLORER_ORANGE:
//      return &orangeBox;
//  }
//  return;
//}


//void tickSettings() {
//  // Handle encoder rotation in menu
//  prbSettingsIndex += controllerOffset;
//  if (prbSettingsIndex < 0) {
//    prbSettingsIndex = 0;
//  } else if (prbSettingsIndex >= PRB_SETTINGS_COUNT) {
//    prbSettingsIndex = PRB_SETTINGS_COUNT - 1;
//  }
//
//  // return to EXPLORER state if the button is double clicked
//  if (controllerDoubleClicked) {
//    currentMenuState = EXPLORER;
//    activeGlobalState = G_ACTIVE;
//    return;
//  }
//
//  // Handle long click (Choosing a setting)
//  if (controllerLongClicked) {
//    // activate the corresponding setting
//    PRBoxController* activeBox = _getCurrentSelectedBox();
//    switch (prbSettingsIndex) {
//      case SETTINGS_SET_LASER_OFF:
//        activeBox->setHighToCurrent();
//        break;
//      case SETTINGS_SET_LASER_ON:
//        activeBox->setLowToCurrent();
//        break;
//    }
//  }
//}

//void tickAllSettings() {
//  prbAllSettingsIndex += controllerOffset;
//
//  if (prbAllSettingsIndex < 0) {
//    prbAllSettingsIndex = 0;
//  } else if (prbAllSettingsIndex >= PRB_ALL_SETTINGS_COUNT) {
//    prbAllSettingsIndex = PRB_ALL_SETTINGS_COUNT - 1;
//  }
//
//
//
//  // return to EXPLORER state if the button is double clicked
//  if (controllerDoubleClicked) {
//    currentMenuState = EXPLORER;
//    activeGlobalState = G_ACTIVE;
//    return;
//  }
//
//  if (controllerLongClicked) {
//
//    switch (prbAllSettingsIndex) {
//      case ALL_SETTINGS_LASER_OFF:
//        yellowBox.setHighToCurrent();
//        greenBox.setHighToCurrent();
//        grayBox.setHighToCurrent();
//        blueBox.setHighToCurrent();
//        orangeBox.setHighToCurrent();
//        break;
//      case ALL_SETTINGS_LASER_YELLOW:
//        int yellowDif = yellowBox.getHigh() - yellowBox.getValue();
//        yellowBox.setLowToCurrent();
//        greenBox.setLow(greenBox.getHigh() - yellowDif);
//        grayBox.setLow(grayBox.getHigh() - yellowDif);
//        blueBox.setLow(blueBox.getHigh() - yellowDif);
//        orangeBox.setLow(orangeBox.getHigh() - yellowDif);
//        break;
//      case ALL_SETTINGS_RESET:
//        yellowBox.setToDefault();
//        greenBox.setToDefault();
//        grayBox.setToDefault();
//        blueBox.setToDefault();
//        orangeBox.setToDefault();
//        break;
//    }
//  }
//}




//const int PRB_EXPLORER_COUNT = 5;
//const String PRB_EXPLORER_ITEMS[] = {"All", "Yellow", "Green", "Gray", "Blue", "Orange"};
//
//enum PRB_EXPLORER_IDS {
//  EXPLORER_ALL,
//  EXPLORER_YELLOW,
//  EXPLORER_GREEN,
//  EXPLORER_GRAY,
//  EXPLORER_BLUE,
//  EXPLORER_ORANGE
//};
//int prbExplorerIndex = 0;

//void _drawMenuTitle() {
//  display1.printf("   %s\n", MENU_TITLE.c_str());
//  display1.drawFastHLine(0, 7, SCREEN_WIDTH, SSD1306_WHITE);
//  display1.drawFastVLine(MENU_SPLIT, 7, SCREEN_HEIGHT, SSD1306_WHITE);
//}

//void _drawExplorerOptions() {
//  for (int i = 0; i < PRB_EXPLORER_COUNT; i++) {
//    if (prbExplorerIndex == i) {
//      display1.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
//    } else {
//      display1.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
//    }
//    display1.printf("%s\n", PRB_EXPLORER_ITEMS[i].c_str());
//  }
//}

//void _drawSettingsOptions() {
//  for (int i = 0; i < PRB_SETTINGS_COUNT; i++) {
//    if (prbSettingsIndex == i) {
//      display1.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
//    } else {
//      display1.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
//    }
//    display1.setCursor(MENU_SPLIT + 2, 8 + 8 * i);
//    display1.printf("%s\n", PRB_SETTINGS_ITEMS[i].c_str());
//  }
//}

//void _drawAllSettingsOptions() {
//  for (int i = 0; i < PRB_ALL_SETTINGS_COUNT; i++) {
//    if (prbAllSettingsIndex == i) {
//      display1.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
//    } else {
//      display1.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
//    }
//    display1.setCursor(MENU_SPLIT + 2, 8 + 8 * i);
//    display1.printf("%s\n", PRB_ALL_SETTINGS_ITEMS[i].c_str());
//  }
//}

//void _resetDisplay() {
//  display1.clearDisplay();
//  display1.setCursor(0, 0);
//  display1.setTextColor(SSD1306_WHITE);
//  display1.setTextSize(1);
//
//  display2.clearDisplay();
//  display2.setCursor(0, 0);
//  display2.setTextColor(SSD1306_WHITE);
//  display2.setTextSize(1);
//}



//void _draw2QuickSettings() {
//  switch (prbExplorerIndex) {
//    case EXPLORER_ALL:
//      display2.printf(" Y: %d < %d = %s\n", yellowBox.getValue(), yellowBox.getMid(), yellowBox.isActive() ? "ON" : "OFF");
//      display2.printf("Gn: %d < %d = %s\n", greenBox.getValue(), greenBox.getMid(), greenBox.isActive() ? "ON" : "OFF");
//      display2.printf("Gy: %d < %d = %s\n", grayBox.getValue(), grayBox.getMid(), grayBox.isActive() ? "ON" : "OFF");
//      display2.printf(" B: %d < %d = %s\n", blueBox.getValue(), blueBox.getMid(), blueBox.isActive() ? "ON" : "OFF");
//      display2.printf(" O: %d < %d = %s\n", orangeBox.getValue(), orangeBox.getMid(), orangeBox.isActive() ? "ON" : "OFF");
//      break;
//    default:
//      PRBoxController* activeBox = _getCurrentSelectedBox();
//      display2.printf("Current  : %d\n", activeBox->getValue());
//      display2.printf("Laser Off: %d\n", activeBox->getHigh());
//      display2.printf("Laser On : %d\n", activeBox->getLow());
//      display2.printf("State    : %s\n", activeBox->isActive() ? "on" : "off");
//      break;
//  }
//}

//void _draw2DetailedAllSettings() {
//
//  switch (prbAllSettingsIndex) {
//    case ALL_SETTINGS_LASER_OFF:
//      display2.printf(" Y: %d -> High(%d)\n", yellowBox.getValue(), yellowBox.getHigh());
//      display2.printf("Gn: %d -> High(%d)\n", greenBox.getValue(), greenBox.getHigh());
//      display2.printf("Gy: %d -> High(%d)\n", grayBox.getValue(), grayBox.getHigh());
//      display2.printf(" B: %d -> High(%d)\n", blueBox.getValue(), blueBox.getHigh());
//      display2.printf(" O: %d -> High(%d)\n", orangeBox.getValue(), orangeBox.getHigh());
//      break;
//    case ALL_SETTINGS_LASER_YELLOW:
//      int diff = yellowBox.getHigh() - yellowBox.getValue();
//      display2.printf("Y: off - cur = diff\n");
//      display2.printf("  %d - %d = %d\n\n", yellowBox.getHigh(), yellowBox.getValue(), diff);
//
//      display2.printf(" Y: %d -> ON(%d) \n", yellowBox.getValue(), yellowBox.getLow());
//      display2.printf("Gn: %d -> ON(%d) \n", greenBox.getHigh() - diff, greenBox.getLow());
//      display2.printf("Gy: %d -> ON(%d) \n", grayBox.getHigh() - diff, grayBox.getLow());
//      display2.printf(" B: %d -> ON(%d) \n", blueBox.getHigh() - diff, blueBox.getLow());
//      display2.printf(" O: %d -> ON(%d) \n", orangeBox.getHigh() - diff, orangeBox.getLow());
//      break;
//    case ALL_SETTINGS_RESET:
//      display2.printf("NOT IMPLEMENTED\n");
//      break;
//  }
//}


//void drawExplorer() {
//  _resetDisplay();
//  _drawMenuTitle();
//  _drawExplorerOptions();
//  _draw2QuickSettings();
//}
//
//void drawSettings() {
//  _resetDisplay();
//  _drawMenuTitle();
//  _drawExplorerOptions();
//  _drawSettingsOptions();
//  _draw2QuickSettings();
//}
//
//void drawAllSettings() {
//  _resetDisplay();
//  _drawMenuTitle();
//  _drawExplorerOptions();
//  _drawAllSettingsOptions();
//  _draw2DetailedAllSettings();
//}



//void setupMenu() {
//  calibrationState = G_ACTIVE;
//
//  myEncoderButton.attachClick(menuClick);
//  myEncoderButton.attachDoubleClick(menuDoubleClick);
//  myEncoderButton.attachDuringLongPress(menuLongPressLoop);
//
//  menuDisplayTimer.setDuration(1000 / 40);
//}
//
//void loopMenu() {
//  if (menuDisplayTimer.isDone()) {
//    menuDisplayTimer.start();
//    tickController();
//    switch (currentMenuState) {
//      case EXPLORER:
//        tickExplorer();
//        drawExplorer();
//        break;
//      case SETTINGS:
//        tickSettings();
//        drawSettings();
//        break;
//      case ALL_SETTINGS:
//        tickAllSettings();
//        drawAllSettings();
//    }
//    controllerClicked = false;
//    controllerDoubleClicked = false;
//    controllerLongClicked = false;
//  }
//}



#endif

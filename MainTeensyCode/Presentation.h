#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>
#include <wemo.h>
#include "AutomaticController.h"

EthernetClient client;

void setupEthernet() {
  Serial.printf("Setting up wemo, please wait\n");
  if (!Ethernet.begin(mac)) {
    Serial.printf("Failed to configure ethernet using DHCP\n");
    while (true);
  }

  Serial.print("My IP address: ");
  byte thisbyte;
  for (thisbyte = 0; thisbyte < 3; thisbyte++) {
    Serial.printf("%i.", Ethernet.localIP()[thisbyte]);
  }
  Serial.printf("%i\n", Ethernet.localIP()[thisbyte]);
}

enum PRESENTATION_STATES {
  P_INTRO,
  P_PRESENTING,
  P_PLAYING,
};




class PresentationController {
    PRESENTATION_STATES currentState;
    AutomaticController* automode;
    bool teaTime;
    int activeHue;

    void sendNextSlide() {
      Serial.printf("Sending next slide\n");
      Keyboard.press(KEY_RIGHT_ALT);
      Keyboard.press('l');
      Keyboard.release('l');
      Keyboard.release(KEY_RIGHT_ALT);
      // alt + k
    }

    void sendPrevSlide() {
      Serial.printf("Sending prev slide\n");
      Keyboard.press(KEY_RIGHT_ALT);
      Keyboard.press('k');
      Keyboard.release('k');
      Keyboard.release(KEY_RIGHT_ALT);
      // alt + l
    }

    void sendOpenSlides() {
      for (int i = 0; i < 6; i++) {
        setHue(i, true, 22500, 100, 0);
      }
      Serial.printf("Sending open slide\n");
      Keyboard.press(KEY_RIGHT_ALT);
      Keyboard.press('j');
      Keyboard.release('j');
      Keyboard.release(KEY_RIGHT_ALT);
      // alt + j
    }

    void sendOpenCalmMusic() {
      for (int i = 0; i < 6; i++) {
        setHue(i, true, 22500, 100, 100);
      }
      Serial.printf("Sending open calm\n");
      Keyboard.press(KEY_RIGHT_ALT);
      Keyboard.press('p');
      Keyboard.release('p');
      Keyboard.release(KEY_RIGHT_ALT);
      // alt + p
    }

    void sendOpenRockMusic() {
      for (int i = 0; i < 6; i++) {
        setHue(i, true, 0, 100, 100);
      }
      Serial.printf("Sending open rock\n");
      Keyboard.press(KEY_RIGHT_ALT);
      Keyboard.press('o');
      Keyboard.release('o');
      Keyboard.release(KEY_RIGHT_ALT);
      // alt + o
    }

    int curState = 0;

    void cycleNextState() {
      curState = (curState + 1) % 3;

      switch (curState) {
        case 0: // normal mode
          sendOpenSlides();
          break;
        case 1: // Rock mode
          sendOpenRockMusic();
          break;
        case 2: // Calm Mode
          sendOpenCalmMusic();
          break;
      }
    }

  public:
    PresentationController(AutomaticController &_automode) {
      currentState = P_PLAYING;
      automode = &_automode;
    }

    void clicked(int buttonIndex) {
      switch (buttonIndex) {
        case 0: //yellow
          yellowButtonClicked();
          break;
        case 1: // green
          greenButtonClicked();
          break;
        case 2: // gray
          grayButtonClicked();
          break;
        case 3: // blue
          blueButtonClicked();
          break;
        case 4: // orange
          orangeButtonClicked();
          break;
      }
    }
    void longClickStarted(int buttonIndex) {}
    void longClickDuring(int buttonIndex, int durationMS) {}
    void longClickStop(int buttonIndex, int durationMS) {}
    void multiClicked(int buttonIndex, int clickCount) {}


    void yellowButtonClicked() {
      switch (currentState) {
        case P_PLAYING:
          Serial.printf("Turn on all wemo outlets\n");
          for (int i = 0; i < 5; i++) {
            switchON(i);
          }
          break;
        case P_PRESENTING:
          if (!automode->isOn()) {
            sendPrevSlide();
          }
          break;
      }
    }
    //    void yellowButtonLongClickedStart() {}
    //    void yellowButtonLongClickedDuring(int durationMS) {}
    //    void yellowButtonLongClickedStop(int durationMS) {}
    //    void yellowButtonMultiClicked(int clickCount) {}


    void greenButtonClicked() {
      switch (currentState) {
        case P_PLAYING:
          Serial.printf("Turn off all hue lights\n");
          for (int i = 0; i < 5; i++) {
            switchOFF(i);
          }
          break;
        case P_PRESENTING:
          automode->toggle();
          break;
      }
    }
    //    void greenButtonLongClickedStart() {}
    //    void greenButtonLongClickedDuring(int durationMS) {}
    //    void greenButtonLongClickedStop(int durationMS) {}
    //    void greenButtonMultiClicked(int clickCount) {}


    void grayButtonClicked() {
      switch (currentState) {
        case P_PLAYING:
          Serial.printf("Turn off all hue lights\n");
          for (int i = 0; i < 6; i++) {
            setHue(i, false, 0, 0, 0);
          }
          break;
        case P_PRESENTING:
          cycleNextState();
          break;
      }
    }
    //    void grayButtonLongClickedStart() {}
    //    void grayButtonLongClickedDuring(int durationMS) {}
    //    void grayButtonLongClickedStop(int durationMS) {}
    //    void grayButtonMultiClicked(int clickCount) {}


    void blueButtonClicked() {
      switch (currentState) {
        case P_PLAYING:
          Serial.printf("Turn on all hue lights\n");
          for (int i = 0; i < 6; i++) {
            setHue(i, true, activeHue, 255, 255);
          }
          break;
        case P_PRESENTING:
          if (teaTime) {
            Serial.printf("No more tea time\n");
            teaTime = false;
            switchOFF(2);
          } else {
            Serial.printf("Tea time\n");
            teaTime = true;
            switchON(2);
          }
          break;
      }
    }
    //    void blueButtonLongClickedStart() {}
    //    void blueButtonLongClickedDuring(int durationMS) {}
    //    void blueButtonLongClickedStop(int durationMS) {}
    //    void blueButtonMultiClicked(int clickCount) {}


    void orangeButtonClicked() {
      switch (currentState) {
        case P_PLAYING:
          activeHue = (activeHue + 5000) % 65000;
          for (int i = 0; i < 6; i++) {
            setHue(i, true, activeHue, 255, 255);
          }
          Serial.printf("New Hue: %d\n", activeHue);
          break;
        case P_PRESENTING:
          sendNextSlide();
          break;
      }
    }
    //    void orangeButtonLongClickedStart() {}
    //    void orangeButtonLongClickedDuring(int durationMS) {}
    //    void orangeButtonLongClickedStop(int durationMS) {}
    //    void orangeButtonMultiClicked(int clickCount) {}
};

#include "WemoData.h"

enum PRESENTATION_STATES {
  P_INTRO,
  P_PRESENTING,
  P_PLAYING,
};



class PresentationController {
    PRESENTATION_STATES currentState;

  public:
    PresentationController() {
      currentState = P_PLAYING;
    }

    void clicked(int buttonIndex) {
      switch (buttonIndex) {
        case 0: //yellow
          yellowButtonClicked();
          break;
        case 1: // green
          greenButtonClicked();
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
          turnOnAllWemo();
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
          Serial.printf("Turn off all wemo outlets\n");
          turnOffAllWemo();
          break;
      }
    }
    //    void greenButtonLongClickedStart() {}
    //    void greenButtonLongClickedDuring(int durationMS) {}
    //    void greenButtonLongClickedStop(int durationMS) {}
    //    void greenButtonMultiClicked(int clickCount) {}


    //    void grayButtonClicked() {}
    //    void grayButtonLongClickedStart() {}
    //    void grayButtonLongClickedDuring(int durationMS) {}
    //    void grayButtonLongClickedStop(int durationMS) {}
    //    void grayButtonMultiClicked(int clickCount) {}


    //    void blueButtonClicked() {}
    //    void blueButtonLongClickedStart() {}
    //    void blueButtonLongClickedDuring(int durationMS) {}
    //    void blueButtonLongClickedStop(int durationMS) {}
    //    void blueButtonMultiClicked(int clickCount) {}


    //    void orangeButtonClicked() {}
    //    void orangeButtonLongClickedStart() {}
    //    void orangeButtonLongClickedDuring(int durationMS) {}
    //    void orangeButtonLongClickedStop(int durationMS) {}
    //    void orangeButtonMultiClicked(int clickCount) {}
};

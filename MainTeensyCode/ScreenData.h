#ifndef _SCREEN_DATA_H_
#define _SCREEN_DATA_H_

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Timer.h"

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int SCREEN1_ADDRESS = 0x3C;
const int SCREEN2_ADDRESS = 0x3D;
const int SCREEN_REFRESH_RATE = 20; // How many times per second

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

Timer screenDisplayTimer;


void setupScreen() {
  if (!display1.begin(SSD1306_SWITCHCAPVCC, SCREEN1_ADDRESS)) {
    Serial.printf("SSD1306 allocation failed");
    while (true);
  }
  if (!display2.begin(SSD1306_SWITCHCAPVCC, SCREEN2_ADDRESS)) {
    Serial.printf("SSD1306 allocation failed");
    while (true);
  }


  display1.cp437(true);
  display2.cp437(true);

  screenDisplayTimer.setDuration(1000/SCREEN_REFRESH_RATE); // 1
  screenDisplayTimer.start();
}

void loopScreen() {
  if (screenDisplayTimer.isDone()) {
    screenDisplayTimer.start();
    display1.display();
    display2.display();
  }
}


#endif

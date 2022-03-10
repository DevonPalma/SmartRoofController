#ifndef _MENU_H_
#define _MENU_H_

#include "ScreenData.h"
#include "EncoderData.h"

/*
 * main menu:
 *  display each box attached to the controller
 *  when I spen encoder, look through them
 *  When I click encoder, open the corresponding box;s sub menu
 *  
 * sub menu for each box:
 *  display the settings for the specified box on second screen
 *    - current value
 *    - Current Max Brightness
 *    - current Min Brightness
 *    - Is considered "on"
 *  display the setting changes I can do
 *    - Wipe the current settings
 *    - Set the max brightness to the current value
 *    - set the min brightness to the current value
 *  
 */

const int TOTAL_LINES = 5;

String title = "PR Box Settings";
String lines[TOTAL_LINES] = {"orange", "blue", "gray", "green", "yellow"};

bool inSubMenu;

void normalColor() {
  display1.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
}
void inverseColor() {
  display1.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
}

void menuClick() {
  if (!inSubMenu) {
    inSubMenu = true;
  }
}

void menuDoubleClick() {
  if (inSubMenu) {
    inSubMenu = false;
  }
}


void drawMainMenu() {

  display1.setCursor(0, 0);
  display1.setTextColor(SSD1306_WHITE);
  display1.setTextSize(1);
  display1.printf("%s\n", title.c_str());
  display1.drawFastHLine(0, 7, SCREEN_WIDTH, SSD1306_WHITE);

  display1.setTextColor(SSD1306_BLACK, SSD1306_WHITE);

  
  normalColor();
  if (selectedIndex == 0) {
    display1.printf("\n");
  } else {
    display1.printf("%s\n", lines[selectedIndex - 1].c_str());
  }

  inverseColor();
  display1.printf("%s\n", lines[selectedIndex].c_str());

  normalColor();
  if (selectedIndex == TOTAL_LINES - 1) {
    display1.printf("\n");
  } else {
    display1.printf("%s\n", lines[selectedIndex + 1].c_str());
  }
}

void drawSubMenu() {

}

void setupMenu() {
  myEncoderButton.attachClick(menuClick);
}
void loopMenu() {
  display1.clearDisplay();
  drawMainMenu();
}



#endif

#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C displaySensor;

class Display {
public:
  void init();
  void displayString(const char* text, int textSize = 1, unsigned int durationMs = 0);
  void manageDisplayState();
  void menu();
private:
  static unsigned long lastDisplayTime;
  static unsigned long displayStartTime;
  static unsigned long currentDuration;
  static const uint8_t OLED_ADDR = 0x3C;
};

extern Display display;

#endif
#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C displaySensor;

class Display {
public:
  void init();
  void displayString(const char* text, int textSize = 1, unsigned int durationS = 0);
  void drawDisplayLine(unsigned int& y_pos, const char* label, const char* state = "");
  void manageDisplayState();
  void menu();

  template<typename... Args>
  void updateScreen(unsigned int& y_pos, bool sendBuffer, Args... lines) {
    displaySensor.clearBuffer();
    _updateScreenRecursive(y_pos, sendBuffer, lines...);
  }

private:
  static unsigned long displayStartTime;
  static unsigned long currentDuration;

  void _updateScreenRecursive(unsigned int& y_pos, bool sendBuffer) {
    if (sendBuffer) {
      displaySensor.sendBuffer();
    }
  }

  void _updateScreenRecursive(unsigned int& y_pos, bool sendBuffer, const char* label) {
    drawDisplayLine(y_pos, label, "");
    _updateScreenRecursive(y_pos, sendBuffer); 
  }

  template<typename... Args>
  void _updateScreenRecursive(unsigned int& y_pos, bool sendBuffer, const char* label, const char* state, Args... rest) {
    drawDisplayLine(y_pos, label, state);
    _updateScreenRecursive(y_pos, sendBuffer, rest...);
  }
};

extern Display display;

#endif
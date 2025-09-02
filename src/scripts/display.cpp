#include <Wire.h>

#include "headers/display.h"
#include "headers/states.h"
#include "headers/globals.h"
#include "headers/config.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
U8G2_SSD1306_128X64_NONAME_F_HW_I2C displaySensor(U8G2_R0, U8X8_PIN_NONE);

Display display;

const unsigned long timeoutDisplay = DISPLAY_DURATION_DEFAULT * 1000;
const unsigned long defaultDuration = TOUCH_COOLDOWN_S * 1000;

unsigned long Display::displayStartTime = 0;
unsigned long Display::currentDuration = defaultDuration;

unsigned long Display::lastDisplayTime = 0;

void Display::init() {
  Wire.begin(OLED_PIN_SDA, OLED_PIN_SCL);

  lastDisplayTime = millis();
  displaySensor.begin();
  displaySensor.setFont(u8g2_font_6x13_tf);
  displaySensor.setFontPosTop();
  displaySensor.drawStr(0,10, "Se initializeaza...");
  displaySensor.sendBuffer();
};

void Display::displayString(const char* text, int textSize, unsigned int durationS) {
    lastDisplayTime = millis();
    displaySensor.clearBuffer();
    displaySensor.drawStr(0,10, text);
    displaySensor.sendBuffer();

    if (durationS > 0) {
        displayStartTime = millis();
        currentDuration = durationS * 1000;
        showingTemporary = true;
    }
};

void drawDisplayLine(unsigned int& y_pos, const char* label, const char* state = "") {
  int line_height = displaySensor.getFontAscent() + 2; 

  displaySensor.drawStr(0, y_pos, label);
  int stateXPos = displaySensor.getStrWidth(label) + 2;
  displaySensor.drawStr(stateXPos, y_pos, state);
  
  y_pos += line_height;
}

void Display::menu() {
  activeMenu = true;
  displaySensor.clearBuffer();

  unsigned int line_y = 0;

  switch (currentPage)
  {
    case CONTROL_PAGE: {
      const char* tapoState = getEntityState(Entity::tapo_0);
      const char* roborockState = getEntityState(Entity::roborock);
      
      drawDisplayLine(line_y, "PAGINA CONTROL", "");
      drawDisplayLine(line_y, "1.Priza:", tapoState);
      drawDisplayLine(line_y, "2.Aspirator:", roborockState);
      break;
    }
    case FORECAST_PAGE: {
      drawDisplayLine(line_y, "VREMEA AFARA", "");
      drawDisplayLine(line_y, "1.Vreme:", getEntityState(Entity::weather_temp));
      drawDisplayLine(line_y, "2.Humidity:", getEntityState(Entity::weather_hum));
      break;
    }
    case ROOM_PAGE: {
      drawDisplayLine(line_y, "STARE CAMERA", "");
      drawDisplayLine(line_y, "1.Temperatura:", roomTemp);
      drawDisplayLine(line_y, "2.Humiditate:", roomHum);
      break;
    }
    default:
      break;
  }

  lastDisplayTime = millis();
  displaySensor.sendBuffer();
}

void Display::manageDisplayState() {
  if (showingTemporary && millis() - displayStartTime > defaultDuration) {
    showingTemporary = false;
    display.menu();
    currentDuration = defaultDuration;
    lastDisplayTime = millis();
  }

  if (millis() - lastDisplayTime > timeoutDisplay) {
    activeMenu = false;
    displaySensor.clearBuffer();
    displaySensor.sendBuffer();
  }

  if(activeMQTT && needsDisplayMenuUpdate && !showingTemporary) {
    display.menu();
    activeMQTT = false;
    needsDisplayMenuUpdate = false;
  }
}
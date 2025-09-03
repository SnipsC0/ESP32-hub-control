#include <Wire.h>

#include "headers/display.h"
#include "headers/states.h"
#include "headers/globals.h"
#include "headers/config.h"
#include "headers/configManager.h"

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
  displaySensor.clearBuffer();
  unsigned int line_y = 0;
  
  // Găsește pagina curentă în configurație
  const Page* page = configManager.getPage(currentPage);

  if (page) {
    // Afișează titlul paginii
    drawDisplayLine(line_y, page->label.c_str());

    // Iterează prin liniile paginii și le afișează
    for (const auto& line : page->lines) {
        String stateStr = "";
        if (!line.entity.isEmpty()) {
            stateStr = getEntityState(line.entity.c_str());
        } 
        else if (line.variable == "roomTemp") {
            stateStr = roomTemp;
        } else if (line.variable == "roomHum") {
            stateStr = roomHum;
        }
        
        drawDisplayLine(line_y, line.label.c_str(), stateStr.c_str());
    }
  } else {
    // Pagina nu a fost găsită în configurație
    drawDisplayLine(line_y, "Pagina invalida:", currentPage.c_str());
  }
  
  activeMenu = true;
  if(currentPage != "ROOM_PAGE") {
    displayStartTime = millis();
    lastDisplayTime = millis();
  };
  displaySensor.sendBuffer();
}


void Display::manageDisplayState() {
  if (showingTemporary && millis() - displayStartTime > defaultDuration) {
    showingTemporary = false;
    display.menu();
    currentDuration = defaultDuration;
    lastDisplayTime = millis();
  }
  
  if (activeMenu && millis() - lastDisplayTime > timeoutDisplay) {
    activeMenu = false;
    displaySensor.clearBuffer();
    displaySensor.sendBuffer();
  }
  
  if(!activeMQTT && needsDisplayMenuUpdate && !showingTemporary) {
    display.menu();
    needsDisplayMenuUpdate = false;
  }
}
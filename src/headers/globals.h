#ifndef GLOBALS_H
#define GLOBALS_H

#include "headers/config.h"

#include <Arduino.h>
#include <Wire.h>

enum MenuPage {
  CONTROL_PAGE,
  ROBOROCK_PAGE,
  FORECAST_PAGE,
  ROOM_PAGE
};

extern MenuPage currentPage;
extern bool activeMenu;

extern bool showingTemporary;
extern bool needsDisplayMenuUpdate;
extern bool activeMQTT;

extern unsigned long lastTouchTime;
extern int activeButton;

extern int OLEDpinSDA;
extern int OLEDpinSCL;

extern char roomTemp[5];
extern char roomHum[5];

#endif

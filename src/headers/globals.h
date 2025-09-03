#ifndef GLOBALS_H
#define GLOBALS_H

#include "headers/config.h"

#include <Arduino.h>
#include <Wire.h>

extern String currentPage;

extern bool activeMenu;

extern bool showingTemporary;
extern bool needsDisplayMenuUpdate;
extern bool activeMQTT;

extern unsigned long lastTouchTime;
extern int activeButton;

extern int OLEDpinSDA;
extern int OLEDpinSCL;

extern char roomTemp[10];
extern char roomHum[10];
extern char heatIndex[10];

extern char mqtt_server[40];
extern char mqtt_port[6];

#endif

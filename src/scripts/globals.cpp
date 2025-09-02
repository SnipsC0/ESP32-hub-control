#include "headers/globals.h"

MenuPage currentPage = CONTROL_PAGE;

bool activeMenu = false;
bool showingTemporary = false;
bool activeMQTT = false;
bool needsDisplayMenuUpdate = false;

int activeButton = -1;

char roomTemp[5];
char roomHum[5];

unsigned long lastTouchTime = 0;
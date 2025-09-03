#include "headers/globals.h"

String currentPage = "CONTROL_PAGE";

bool activeMenu = false;
bool showingTemporary = false;
bool activeMQTT = false;
bool needsDisplayMenuUpdate = false;

int activeButton = -1;

char roomTemp[10];
char roomHum[10];

unsigned long lastTouchTime = 0;
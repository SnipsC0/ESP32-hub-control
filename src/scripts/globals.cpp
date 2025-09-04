#include "headers/globals.h"

String currentPage = "CONTROL_PAGE";

bool activeMenu = false;
bool showingTemporary = false;
bool activeMQTT = false;
bool needsDisplayMenuUpdate = false;

unsigned long lastDisplayTime = 0;
int activeButton = -1;

char roomTemp[10];
char roomHum[10];
char heatIndex[10];
float rawHumidity = 0.0;

char mqtt_server[40] = "192.168.0.240";
char mqtt_port[6] = "1883";

unsigned long displayUpdateRaceStartTime = 0;
bool isDisplayUpdateRaceActive = false;

unsigned long lastTouchTime = 0;
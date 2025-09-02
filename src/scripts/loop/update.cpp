#include <Arduino.h>

#include "headers/update.h"
#include "headers/config.h"
#include "headers/globals.h"
#include "headers/deepSleep.h"
#include "headers/display.h"

void update() {
  // deep sleep
  enterDeepSleep();

  // display timeout etc
  display.manageDisplayState();
}
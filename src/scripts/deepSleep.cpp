#include "headers/deepSleep.h"
#include "headers/display.h"
#include <Arduino.h>

const int deepSleepTimeout = DEEP_SLEEP_TIMEOUT_S * 1000;

void enterDeepSleep() {
    if(DEEP_SLEEP == 1) {
        if(millis() - lastTouchTime > deepSleepTimeout) {
            Serial.println("Intrare in deep sleep...");
            delay(100);
            esp_deep_sleep_start();
        }
    }
}
#define USE_SIMULATION

#include <Arduino.h>
#include "headers/led_control.h"
#include "headers/globals.h"

#ifdef USE_SIMULATION
#include <PubSubClient.h>

#include "headers/handleTouch.h"


uint16_t currtouched = 0;
uint16_t lasttouched = 0;

unsigned long lastReleaseTime[12] = {0};

void setup_touch() {
  Serial.println("Simulare MPR121 - taste 0-9 simuleaza touch pe electrozi");
}

void use_touch() {
  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c >= '0' && c <= '9') {
      uint8_t idx = c - '0';
      currtouched |= (1 << idx);
      lastReleaseTime[idx] = millis();
     
      if(activeButton == -1 || activeButton == idx) {
        currtouched |= (1 << idx);
        lastReleaseTime[idx] = millis();
      }
    }
  }

  for (uint8_t i = 0; i < 12; i++) {
    bool touchedNow = (currtouched & (1 << i));
    bool touchedBefore = (lasttouched & (1 << i));

    if (touchedNow && !touchedBefore) {
      if(activeButton == -1 && !needsDisplayMenuUpdate) {
        activeButton = i;
        
        Serial.print("Electrod "); Serial.print(i); Serial.println(" touched!");
        
        handleTouch(i);
      }
    }
    
    if ((touchedNow && (millis() - lastReleaseTime[i] > 1000)) && !needsDisplayMenuUpdate) {
      currtouched &= ~(1 << i);
      touchedNow = false;
    }

    if (!touchedNow && touchedBefore) {
      if(activeButton == i) {

        Serial.print("Electrod "); Serial.print(i); Serial.println(" released");
        setPinState(2, false);
        
        activeButton = -1;
      }
    }
  }

  lasttouched = currtouched;
}

#else

#include <Wire.h>
#include <Adafruit_MPR121.h>
#include "headers/handleTouch.h"

const int TOUCH_THRESHOLD = 3;

uint16_t baselineValues[12] = {0};
bool isCalibrated = false;

const unsigned int touchCooldown = 1000;

Adafruit_MPR121 cap = Adafruit_MPR121();

void setup_touch() {
  // Wire1.begin(MPR_PIN_SDA, MPR_PIN_SCL);

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 nu a fost gasit, verificati cablajul!");
    while (1);
  }
  Serial.println("MPR121 initializat!");
}

void calibrate_touch() {
  for (uint8_t i = 0; i < 12; i++) {
    baselineValues[i] = cap.filteredData(i);
  }
  isCalibrated = true;
  Serial.println("Calibrare touch completa.");
}

bool ledState = false;

void use_touch() {
  if (!isCalibrated) {
    calibrate_touch();
    return;
  }

  bool isCooldownOver = (millis() - lastTouchTime > touchCooldown);

  if(isCooldownOver) {
    if(ledState) {
      setPinState(2, false);
    }

    for (uint8_t i = 0; i < 12; i++) {
    uint16_t currentData = cap.filteredData(i);
      if (baselineValues[i] - currentData > TOUCH_THRESHOLD) {
       Serial.print("Electrod ");
        Serial.print(i);
        Serial.println(" atins!");

        lastTouchTime = millis();
      
        baselineValues[i] = currentData;
      
        ledState = true;

        handleTouch(i);

        break;
      }
    }
  }

  for (uint8_t i = 0; i < 12; i++) {
    uint16_t currentData = cap.filteredData(i);
    baselineValues[i] = (baselineValues[i] * 7 + currentData) / 8;
  }
}

#endif
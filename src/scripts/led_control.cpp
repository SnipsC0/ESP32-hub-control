#include "headers/led_control.h"

bool pinInitialized[40] = {false};
bool pinState[40] = {false};

void setPinState(uint8_t pin, bool state) {
  if(pin>=40) return;

  if(!pinInitialized[pin]) {
    pinMode(pin, OUTPUT);
    pinInitialized[pin] = true;
  }
  
  pinState[pin] = state;
  digitalWrite(pin, state ? HIGH : LOW);
}
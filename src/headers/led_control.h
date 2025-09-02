#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

extern bool pinState[40];

void setPinState(uint8_t pin, bool state);

#endif
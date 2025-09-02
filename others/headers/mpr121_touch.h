#ifndef MPR121_TOUCH_H
#define MPR121_TOUCH_H

#include <Wire.h>
#include <Arduino.h>

class MPR121_Touch {
public:
    void begin();
    bool checkTouch();
    void checkBaseline();
    
private:
    TwoWire* wire;
    
    void setupMPR121();
    static void IRAM_ATTR handleDeepTouch();
    
    static volatile bool touchDetected;
    static const uint8_t MPR121_ADDR = 0x5A;
};

extern MPR121_Touch touchPad;

#endif
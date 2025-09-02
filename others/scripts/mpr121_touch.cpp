#include "headers/mpr121_Touch.h"
#include "headers/led_control.h"
#include "headers/globals.h"
#include "headers/handleTouch.h"

MPR121_Touch touchPad;

const unsigned long touchCooldown = TOUCH_COOLDOWN_S;

volatile bool MPR121_Touch::touchDetected = false;


void IRAM_ATTR MPR121_Touch::handleDeepTouch() {
    touchDetected = true;
}

void MPR121_Touch::setupMPR121() {
    TwoWire* wire = getWireMPR121();
    if(wire == nullptr) return;

    wire->beginTransmission(MPR121_ADDR);
    wire->write(0x5E);
    wire->write(0x00);
    wire->endTransmission();
    delay(10);
    
    for (int i = 0; i < 12; i++) {
        wire->beginTransmission(MPR121_ADDR);
        wire->write(0x41 + i);
        wire->write(25);
        wire->endTransmission();

        wire->beginTransmission(MPR121_ADDR);
        wire->write(0x51 + i);
        wire->write(15);   // Release threshold (jumătate din touch)
        wire->endTransmission();
    }
    
    wire->beginTransmission(MPR121_ADDR);
    wire->write(0x5E);
    wire->write(0x8F);
    wire->endTransmission();
    delay(100);
}

void MPR121_Touch::begin() {
    // Configurează interrupt
    initWireMPR121();
    
    TwoWire* wire = getWireMPR121();
    if (wire == nullptr) return;

    pinMode(MPR_PIN_IRQ, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MPR_PIN_IRQ), handleDeepTouch, FALLING);
    
    // Configurează wake-up pentru deep sleep
    esp_sleep_enable_ext0_wakeup((gpio_num_t)MPR_PIN_IRQ, 0);
    
    setupMPR121();
    
    // Verifică dacă s-a trezit din deep sleep
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
        Serial.println("S-a trezit din deep sleep!");
    }
    
    Serial.println("Sistem ready!");
    lastTouchTime = millis();
}

bool MPR121_Touch::checkTouch() {
    TwoWire* wire = getWireMPR121();
    if (wire == nullptr) return false;

    bool isCooldownOver = (millis() - lastTouchTime > touchCooldown );
    
    if(isCooldownOver) {
        if(pinState[2]) {
            setPinState(2, false);
            Serial.println(pinState[2]);
        }

        if (touchDetected) {
            touchDetected = false;

            wire->beginTransmission(MPR121_ADDR);
            wire->write(0x00);
            wire->endTransmission(false);
            wire->requestFrom(MPR121_ADDR, static_cast<uint8_t>(2));
            uint16_t touched = wire->read() | (wire->read() << 8);
            
            for (int i = 0; i < 12; i++) {
                if (touched & (1 << i)) {
                    Serial.print("Touch: ");
                    Serial.println(i);
                    lastTouchTime = millis();

                    setPinState(2, true);
                    handleTouch(i);
                    
                    Serial.println(pinState[2]);
                    return true;
                }
            }
        }
    }
    return false;
}

void MPR121_Touch::checkBaseline() {
    TwoWire* wire = getWireMPR121();
    if (wire == nullptr) return;

    Serial.println("Baseline curent:");
    for (int i = 0; i < 12; i++) {
        wire->beginTransmission(MPR121_ADDR);
        wire->write(0x1E + i);
        wire->endTransmission(false);
        wire->requestFrom(MPR121_ADDR, static_cast<uint8_t>(1));
        byte baseline = wire->read();
        
        Serial.print("E");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(baseline);
        Serial.print("  ");
        
        if (i == 5 || i == 11) Serial.println();
    }
    Serial.println();
}


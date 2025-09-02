#include <Arduino.h>


#include "headers/globals.h"
#include "headers/wifi.h"
#include "headers/mqtt.h"
#include "headers/display.h"
#include "headers/update.h"
#include "headers/use_touch.h"
#include "headers/temphum.h"


void setup() {
  Serial.begin(115200);
  
  display.init();
  setup_touch();

  wifi.setup();
  mqttClient.setup();
  tempHumSensor.setup();
}

void loop() {
  mqttClient.loop();
  use_touch();
  tempHumSensor.check();
  update();
}
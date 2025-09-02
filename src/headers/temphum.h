#ifndef DHT22_H
#define DHT22_H

#include <DHT.h>
#include "headers/globals.h"

class TempHumSensor {
  public:
    void setup();
    void check();
};

extern DHT dht;
extern TempHumSensor tempHumSensor;

#endif
#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include "config.h"
#include <Wifi.h>

class Wifi {
public:
  void setup();
};

extern Wifi wifi;

extern WiFiClient wifiClient;

#endif
#include <WiFi.h>
#include <WiFiManager.h>
#include <Arduino.h>

#include "headers/wifi.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PSW;

Wifi wifi;

WiFiClient wifiClient;

void Wifi::setup() {
  delay(10);
  WiFi.begin(ssid, password);
  Serial.print("Conectare la WIFI...");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectat la WiFi!");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}
#include <WiFi.h>
#include <WiFiManager.h>
#include <Arduino.h>

#include "headers/wifi.h"
#include "headers/display.h"
#include "headers/globals.h"
#include "headers/helpers.h"

Wifi wifi;

WiFiClient wifiClient;

bool wifi_connect_failed = false;

void connectFailedCallback() {
  Serial.println("Eroare la conectare. Credentiale gresite?");
  wifi_connect_failed = true;
}

void Wifi::setup() {
  delay(2000);
  
  WiFiManager wm;

  // wm.resetSettings();

  // String password = generateRa1ndomPassword(8);
  String password = "hubparola";

  WiFiManagerParameter custom_mqtt_server("server", "HomeAssistant MQTT", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "HomeAssistant Port MQTT", mqtt_port, 6);
  
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.setTitle("Room Hub ESP32");
    
  unsigned int line_y = 0;
  display.updateScreen(line_y, true,
    "Configureaza WiFi...", "",
    "SSID:", "",
    "Room Hub ESP32", "",
    "PWD:", "",
    password.c_str(), ""
  );

  if (!wm.autoConnect("Room Hub ESP32", password.c_str())) {
    line_y = 0;
    Serial.println("Eroare la conectare, timeout.");
    display.updateScreen(line_y, true,
    "Eroare conectare...", "",
    "Restart...", ""
    );
    delay(3000);
    ESP.restart();
  }

  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
}
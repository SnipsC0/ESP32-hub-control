#include <WiFi.h>
#include <WiFiManager.h>
#include <Arduino.h>

#include "headers/wifi.h"
#include "headers/display.h"
#include "headers/globals.h"
#include "headers/helpers.h"

Wifi wifi;

WiFiClient wifiClient;

void Wifi::setup() {
  delay(10);
  
  WiFiManager wm;

  // wm.resetSettings();

  // String password = generateRandomPassword(8);
  String password = "hubparola";

  WiFiManagerParameter custom_mqtt_server("server", "Server MQTT", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "Port MQTT", mqtt_port, 6);
  
  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.setTitle("Room Hub ESP32");
  
  char password_display[20];
  snprintf(password_display, sizeof(password_display), "PWD: %s", password.c_str());
  
  unsigned int line_y = 0;
  display.updateScreen(line_y, true,
    "Configureaza WiFi...", "",
    "SSID: Room Hub ESP32", "",
    password_display, ""
  );

  if (!wm.autoConnect("Room Hub ESP32", password.c_str())) {
    Serial.println("Eroare la conectare, timeout.");
    display.updateScreen(line_y, true,
    "Eroare la conectare...", "",
    password_display, ""
    );
    delay(2000);
    ESP.restart();
  } else {
    Serial.println("Conectat la Wi-Fi!");
  }

  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
}
#include "headers/temphum.h"
#include "headers/states.h"
#include "headers/mqtt.h"
#include "headers/display.h"
#include "headers/configManager.h"

TempHumSensor tempHumSensor;
DHT dht(DHT_PIN, DHT22);

unsigned long previousMillis = 0;

const long interval = 2000;

void TempHumSensor::setup(){
  dht.begin();
  Serial.print("Senzor temperatura si umiditate initializat.");
}

void TempHumSensor::check(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      return;
    }
    
    rawHumidity = humidity;
    
    float heat_index = dht.computeHeatIndex(temperature, humidity, false);

    snprintf(roomTemp, sizeof(roomTemp), "%.1f C", temperature);
    snprintf(roomHum, sizeof(roomHum), "%.1f %%", humidity);
    snprintf(heatIndex, sizeof(heatIndex), "%.1f C", heat_index);

    if(activeMenu && currentPage == "ROOM_PAGE") {
      display.menu();
    }

    const Page* page = configManager.getPage(currentPage);
    if (page && page->dehumidifier.enabled) {
        const char* dehumidifierState = getEntityState(page->dehumidifier.entity.c_str());

        const Page* page = configManager.getPage(currentPage);

        if (page && page->dehumidifier.enabled && !page->dehumidifier.command_topic.isEmpty()) {
          const char* dehumidifierState = getEntityState(page->dehumidifier.entity.c_str());
          
          if (rawHumidity - 5 > page->dehumidifier.threshold && strcmp(dehumidifierState, "ON") != 0) {
              mqttClient.publish(page->dehumidifier.command_topic.c_str(), "ON");
          } else if (rawHumidity + 5 <= page->dehumidifier.threshold && strcmp(dehumidifierState, "OFF") != 0) {
              mqttClient.publish(page->dehumidifier.command_topic.c_str(), "OFF");
          }
      }
    }
  }
}
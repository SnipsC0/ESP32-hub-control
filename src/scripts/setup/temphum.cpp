#include "headers/temphum.h"
#include "headers/states.h"
#include "headers/mqtt.h"
#include "headers/display.h"

TempHumSensor tempHumSensor;
DHT dht(DHT_PIN, DHT22);

unsigned long previousMillis = 0;

const long interval = 2000;

void TempHumSensor::setup(){
  dht.begin();
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
    
    float heat_index = dht.computeHeatIndex(temperature, humidity, false);

    snprintf(roomTemp, sizeof(roomTemp), "%.1f C", temperature);
    snprintf(roomHum, sizeof(roomHum), "%.1f %%", humidity);
    snprintf(heatIndex, sizeof(heatIndex), "%.1f C", heat_index);

    if(activeMenu && currentPage == "ROOM_PAGE") {
      display.menu();
    }
  }
}
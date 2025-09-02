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

  // Verificam daca a trecut intervalul de timp dorit
  if (currentMillis - previousMillis >= interval) {
    // Salvam timpul curent ca punct de referinta
    previousMillis = currentMillis;

    // Citim umiditatea
    float humidity = dht.readHumidity();
    // Citim temperatura in Celsius
    float temperature = dht.readTemperature();

    // Verificam daca citirea a esuat
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Eroare la citirea de la senzorul DHT!");
      return;
    }
    
    // char topic[32];
    // char payload[11];

    // Calculam indicele de caldura
    float heat_index = dht.computeHeatIndex(temperature, humidity, false);

    snprintf(roomTemp, sizeof(roomTemp), "%.1f", temperature);
    snprintf(roomHum, sizeof(roomHum), "%.1f", humidity);

    // Serial.print("Umiditate: ");
    // Serial.print(humidity);
    // Serial.print(" %\t");
    // Serial.print("Temperatura: ");
    // Serial.print(temperature);
    // Serial.print(" *C\t");
    // Serial.print("Indice de caldura: ");
    // Serial.print(heat_index);
    // Serial.println(" *C");

    if(activeMenu && currentPage == ROOM_PAGE) {
      display.menu();
    }
  }
}
#include "headers/config.h"
#include "headers/globals.h"
#include "headers/display.h"
#include "headers/states.h"
#include "headers/mqtt.h"
#include "headers/wifi.h"
#include <ArduinoJson.h>

Mqtt mqttClient;
PubSubClient mqtt(wifiClient);

const char* mqttServer = MQTT_IP;
const int mqttPort = MQTT_PORT;

// Functie pentru a gestiona cazurile speciale (logica aditionala)
void handleSpecialCases(Entity entity, const char* payload) {
    if (entity == Entity::roborock && strcmp(payload, "Idle") == 0) {
        mqttClient.publish("home/roborock", "Paused");
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    if (length > 0) {
        char payload_str[length + 1];
        memcpy(payload_str, payload, length);
        payload_str[length] = '\0';

        // Corectare avertisment: utilizare JsonDocument
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload_str);
        
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }
        
        // Iteram prin toate entitatile pentru a le actualiza starea
        for (const auto& [entity, config] : entityConfigs) {
            if (strcmp(topic, config.topic) == 0) {
                if (doc[config.json_key].is<JsonVariant>()) {
                    std::string state_value = doc[config.json_key].as<std::string>();
                    // Serial.println(state_value.c_str());
                    Serial.println("Ajuns");
                    updateEntityState(entity, state_value.c_str());
                
                    handleSpecialCases(entity, state_value.c_str());
                    activeMQTT = false;
                    needsDisplayMenuUpdate = true;
                }
            }
        }
    }
}

void Mqtt::subscribe() {
    for (auto const& [entity, config] : entityConfigs) {
        mqtt.subscribe(config.topic, 1);
    }
    mqtt.publish("home/states", "");
    mqtt.publish("home/weather/state", "");
}

void Mqtt::reconnect() {
    while (!mqtt.connected()) {
        Serial.print("Conectare la MQTT...");
        if (mqtt.connect("ESP32Client")) {
            Serial.println("\nConectat!");
            mqttClient.subscribe();
        } else {
            Serial.print("Eroare, rc=");
            Serial.print(mqtt.state());
            Serial.println("Reincercare in 5 sec...");
            delay(5000);
        }
    }
}

void Mqtt::setup() {
    mqtt.setServer(mqttServer, mqttPort);
    delay(1000);
    mqtt.setCallback(callback);
}

void Mqtt::loop() {
    if (!mqtt.connected()) {
        mqttClient.reconnect();
    }
    mqtt.loop();
}

void Mqtt::publish(const char* topic, const char* payload) {
    display.displayString("...", 1, 2);
    mqtt.publish(topic, payload);

    activeMQTT = true;
    needsDisplayMenuUpdate = true;
}
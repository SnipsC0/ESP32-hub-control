#include "headers/config.h"
#include "headers/globals.h"
#include "headers/display.h"
#include "headers/states.h"
#include "headers/mqtt.h"
#include "headers/wifi.h"
#include "headers/configManager.h"
#include <ArduinoJson.h>
#include <WiFiManager.h>

Mqtt mqttClient;
PubSubClient mqtt(wifiClient);

const char* configTopic = "home/config";

void callback(char* topic, byte* payload, unsigned int length) {
    if (strcmp(topic, configTopic) == 0) {

        char payload_str[length + 1];
        memcpy(payload_str, payload, length);
        payload_str[length] = '\0';
        
        if (configManager.parseConfig(payload_str)) {
            Serial.println();
            mqttClient.subscribeToEntities();
            needsDisplayMenuUpdate = true;
        }
        return;
    }

    const EntityConfig* entityConf = configManager.getEntityConfigByTopic(topic);
    if (entityConf) {
        char payload_str[length + 1];
        memcpy(payload_str, payload, length);
        payload_str[length] = '\0';

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload_str);
        
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }
        
        for (const auto& pair : configManager.entities) {
            const EntityConfig& entityConf = pair.second;

            if (strcmp(topic, entityConf.topic.c_str()) == 0) {
            
                if (!doc[entityConf.json_key.c_str()].isNull()) {
                    String state_value = doc[entityConf.json_key.c_str()].as<String>();
                    
                    updateEntityState(entityConf.name.c_str(), state_value.c_str());
                    
                    if (entityConf.name == "roborock" && state_value == "Idle") {
                        mqttClient.publish("home/roborock", "Paused");
                    }
                }
            }
        }
        activeMQTT = false;
        needsDisplayMenuUpdate = true;
    }
}

void Mqtt::subscribe() {
    Serial.println("Ma abonez la topicul de configuratie...");
    mqtt.subscribe(configTopic, 1);
    
    Serial.println("Trimit cerere de configuratie...");
    mqtt.publish("home/config/request", "");
}

void Mqtt::subscribeToEntities() {
    for (auto const& [name, config] : configManager.entities) {
        mqtt.subscribe(config.topic.c_str(), 1);
    }
    mqtt.publish("home/states", "");
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
    int mqtt_port_int = atoi(mqtt_port);

    mqtt.setServer(mqtt_server, mqtt_port_int);
    mqtt.setBufferSize(4000);
    mqtt.setCallback(callback);
    delay(1000);
}

void Mqtt::loop() {
    if (!mqtt.connected()) {
        mqttClient.reconnect();
    }
    
    if (isDisplayUpdateRaceActive) { 
        if (!needsDisplayMenuUpdate) {
            isDisplayUpdateRaceActive = false;
        } else if (millis() - displayUpdateRaceStartTime > (RACE_MQTT_TOUCH_COOLDOWN_S * 1000)) {
            needsDisplayMenuUpdate = false;
            isDisplayUpdateRaceActive = false;
        }
    }

    mqtt.loop();
}

void Mqtt::publish(const char* topic, const char* payload) {
    display.displayString("...", 1, 2);
    mqtt.publish(topic, payload);

    activeMQTT = true;
    needsDisplayMenuUpdate = true;

    isDisplayUpdateRaceActive = true;
    displayUpdateRaceStartTime = millis();
}
#include "headers/config.h"
#include "headers/globals.h"
#include "headers/display.h"
#include "headers/states.h"
#include "headers/mqtt.h"
#include "headers/wifi.h"
#include "headers/configManager.h"
#include <ArduinoJson.h>

Mqtt mqttClient;
PubSubClient mqtt(wifiClient);

const char* mqttServer = MQTT_IP;
const int mqttPort = MQTT_PORT;

const char* configTopic = "home/config";

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.printf("Mesaj primit pe topicul: %s\n", topic);
    if (strcmp(topic, configTopic) == 0) {

        char payload_str[length + 1];
    memcpy(payload_str, payload, length);
    payload_str[length] = '\0';
    
    Serial.println("--- Incep parsarea configuratiei ---");
    Serial.println(payload_str); // <-- ADAUGĂ ASTA pentru a vedea JSON-ul brut
    
    if (configManager.parseConfig(payload_str)) {
        Serial.println("✅ Configuratie parsata cu SUCCES!"); // <-- ADAUGĂ ASTA
        mqttClient.subscribeToEntities();
        needsDisplayMenuUpdate = true;
    } else {
        Serial.println("❌ EROARE la parsarea configuratiei!"); // <-- ADAUGĂ ASTA
    }
        return;
    }

    // Logica existentă, dar adaptată
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

        // Verificăm dacă entitatea curentă ascultă pe topicul pe care a venit mesajul
            if (strcmp(topic, entityConf.topic.c_str()) == 0) {
            
            // Dacă da, verificăm dacă JSON-ul conține cheia de care are nevoie
                if (!doc[entityConf.json_key.c_str()].isNull()) {
                    String state_value = doc[entityConf.json_key.c_str()].as<String>();
                    
                    Serial.printf("Actualizare pentru entitatea '%s': noua stare este '%s'\n", entityConf.name.c_str(), state_value.c_str());
                    updateEntityState(entityConf.name.c_str(), state_value.c_str());
                    
                    // Logica specială pentru Roborock
                    if (entityConf.name == "roborock" && state_value == "Idle") {
                        mqttClient.publish("home/roborock", "Paused");
                    }
                }
            }
        }
    // La final, marcăm că este necesară o actualizare a afișajului
        activeMQTT = false;
        needsDisplayMenuUpdate = true;
    }
}

void Mqtt::subscribe() {
    // La conectare, ne abonăm întâi la topic-ul de configurare
    Serial.println("Ma abonez la topicul de configuratie...");
    mqtt.subscribe(configTopic, 1);
    
    // Apoi, cerem configuratia
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
    mqtt.setServer(mqttServer, mqttPort);
    mqtt.setBufferSize(2048);
    mqtt.setCallback(callback);
    delay(1000);
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
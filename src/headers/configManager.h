#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <vector>
#include <map>

// Structura pentru o acțiune / linie de pe display
struct ActionLine {
    uint8_t electrode;
    String label;
    String type; // "MQTT", "CHANGE_PAGE", "INFO"
    
    // Campuri specifice tipului
    String topic;
    String payload;
    String entity;
    String forbiddenState;
    String nextPage;
    String variable; // pentru variabile locale precum roomTemp
};

// Structura pentru o pagină a meniului
struct Page {
    String name;
    String label;
    std::vector<ActionLine> lines;
};

// Structura pentru configurarea unei entități (pentru recepția stărilor)
struct EntityConfig {
    String name;
    String topic;
    String json_key;
};

class ConfigManager {
public:
    // Container pentru toată configurația
    std::vector<Page> pages;
    std::map<String, EntityConfig> entities;

    // Functia de parsing
    bool parseConfig(const char* json);

    // Functii helper pentru a găsi pagini și acțiuni
    const Page* getPage(const String& pageName);
    const ActionLine* getAction(const String& pageName, uint8_t electrode);
    const EntityConfig* getEntityConfigByTopic(const char* topic);
    const String getEntityTopic(const String& entityName);

};

// Declaram o instanță globală
extern ConfigManager configManager;

#endif
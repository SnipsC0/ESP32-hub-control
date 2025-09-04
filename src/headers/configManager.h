#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <vector>
#include <map>

struct ActionLine {
    uint8_t electrode;
    String label;
    String type; // "MQTT", "CHANGE_PAGE", "INFO"
    
    String topic;
    String payload;
    String entity;
    String forbiddenState;
    String nextPage;
    String variable; // pentru variabile locale precum roomTemp
    bool showState;
};

struct DehumidifierConfig {
    bool enabled;
    String entity;
    int threshold;
    String command_topic;
};

struct Page {
    String name;
    String label;
    std::vector<ActionLine> lines;
    DehumidifierConfig dehumidifier;
};

struct EntityConfig {
    String name;
    String topic;
    String json_key;
};

class ConfigManager {
public:
    std::vector<Page> pages;
    std::map<String, EntityConfig> entities;

    bool parseConfig(const char* json);

    const Page* getPage(const String& pageName);
    const ActionLine* getAction(const String& pageName, uint8_t electrode);
    const EntityConfig* getEntityConfigByTopic(const char* topic);
    const String getEntityTopic(const String& entityName);

};

extern ConfigManager configManager;

#endif
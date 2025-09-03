#include "headers/configManager.h"
#include <ArduinoJson.h>

ConfigManager configManager;

bool ConfigManager::parseConfig(const char* json) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return false;
    }

    pages.clear();
    entities.clear();

    JsonArray pagesArray = doc["pages"].as<JsonArray>();
    for (JsonObject pageObj : pagesArray) {
        Page newPage;
        newPage.name = pageObj["name"].as<String>();
        newPage.label = pageObj["label"].as<String>();

        JsonArray linesArray = pageObj["lines"].as<JsonArray>();
        for (JsonObject lineObj : linesArray) {
            ActionLine newLine;
            newLine.electrode = lineObj["electrode"].as<uint8_t>();
            newLine.label = lineObj["label"].as<String>();
            newLine.type = lineObj["type"].as<String>();
            if (!lineObj["showState"].isNull()) {
                if (lineObj["showState"].is<bool>()) {
                    newLine.showState = lineObj["showState"].as<bool>();
                } 
                else if (lineObj["showState"].is<const char*>() && strcmp(lineObj["showState"], "false") == 0) {
                    newLine.showState = false;
                }
                else {
                    newLine.showState = true;
                }
            } else {
                newLine.showState = true;
            }
            
            if (!lineObj["topic"].isNull()) newLine.topic = lineObj["topic"].as<String>();
            if (!lineObj["payload"].isNull()) newLine.payload = lineObj["payload"].as<String>();
            if (!lineObj["entity"].isNull()) newLine.entity = lineObj["entity"].as<String>();
            if (!lineObj["forbiddenState"].isNull()) newLine.forbiddenState = lineObj["forbiddenState"].as<String>();
            if (!lineObj["nextPage"].isNull()) newLine.nextPage = lineObj["nextPage"].as<String>();
            if (!lineObj["variable"].isNull()) newLine.variable = lineObj["variable"].as<String>();


            newPage.lines.push_back(newLine);
        }
        pages.push_back(newPage);
    }
    
    JsonArray entitiesArray = doc["entities"].as<JsonArray>();
    for (JsonObject entityObj : entitiesArray) {
        EntityConfig newEntity;
        String entityName = entityObj["name"].as<String>();
        newEntity.name = entityName;
        newEntity.topic = entityObj["topic"].as<String>();
        newEntity.json_key = entityObj["json_key"].as<String>();
        entities[entityName] = newEntity;
    }

    return true;
}

const Page* ConfigManager::getPage(const String& pageName) {
    for (const auto& page : pages) {
        if (page.name == pageName) {
            return &page;
        }
    }
    return nullptr;
}

const ActionLine* ConfigManager::getAction(const String& pageName, uint8_t electrode) {
    const Page* page = getPage(pageName);
    if (page) {
        for (const auto& line : page->lines) {
            if (line.electrode == electrode) {
                return &line;
            }
        }
    }
    return nullptr;
}

const EntityConfig* ConfigManager::getEntityConfigByTopic(const char* topic) {
    for (const auto& pair : entities) {
        if (pair.second.topic == topic) {
            return &pair.second;
        }
    }
    return nullptr;
}
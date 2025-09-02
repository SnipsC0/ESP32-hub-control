#ifndef ENTITY_STATES_H
#define ENTITY_STATES_H

#include <map>
#include <string>
#include "headers/config.h"

// Structura EntityConfig contine topic-ul si cheia JSON
struct EntityConfig {
    const char* topic;
    const char* json_key;
};

// Define the enum using the macro
#define X_ENTITY(name, topic, json_key) name,
enum class Entity {
    ENTITY_LIST
};
#undef X_ENTITY

extern std::map<Entity, std::string> states;
extern const std::map<Entity, EntityConfig> entityConfigs;

void updateEntityState(Entity entity, const char* newState);
const char* getEntityState(Entity entity);

#endif
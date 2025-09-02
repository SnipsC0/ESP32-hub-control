#include "headers/states.h"
#include <map>
#include <string>

// Aici se defineste variabila globala 'states'
std::map<Entity, std::string> states;

// Aici se definesc functiile 'updateEntityState' si 'getEntityState'
void updateEntityState(Entity entity, const char* newState) {
    states[entity] = newState;
}

const char* getEntityState(Entity entity) {
    if (states.find(entity) != states.end()) {
        return states[entity].c_str();
    }
    return "";
}

// Aici se defineste si harta de configurare
#define X_ENTITY(name, topic, json_key) {Entity::name, {topic, json_key}},
const std::map<Entity, EntityConfig> entityConfigs = {
    ENTITY_LIST
};
#undef X_ENTITY
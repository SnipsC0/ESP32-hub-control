#include "headers/states.h"
#include <string>
#include <map>

// Harta de stări. Cheia este numele entității (ex: "tapo_0")
std::map<std::string, std::string> states;

void updateEntityState(const char* entityName, const char* newState) {
    states[entityName] = newState;
}

const char* getEntityState(const char* entityName) {
    if (states.count(entityName)) {
        return states[entityName].c_str();
    }
    return "N/A"; // Returnam un text implicit
}
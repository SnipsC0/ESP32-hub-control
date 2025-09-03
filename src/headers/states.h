#ifndef ENTITY_STATES_H
#define ENTITY_STATES_H

#include <map>
#include <string>

extern std::map<std::string, std::string> states;

void updateEntityState(const char* entityName, const char* newState);
const char* getEntityState(const char* entityName);

#endif
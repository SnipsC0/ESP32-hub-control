#include "headers/handleTouch.h"
#include "headers/globals.h"
#include "headers/states.h"
#include "headers/display.h"
#include "headers/mqtt.h"
#include "headers/led_control.h"
#include "headers/configManager.h"

void handleTouch(uint8_t electrode) {
  setPinState(2, true);

  if(!activeMenu) {
    display.menu();
    return;
  }

  const ActionLine* action = configManager.getAction(currentPage, electrode);

  if (action) {
    if (action->type == "CHANGE_PAGE") {
        currentPage = action->nextPage;
        display.menu();
    } 
    else if (action->type == "MQTT") {
        String currentState = getEntityState(action->entity.c_str());
        
        if (!action->forbiddenState.isEmpty() && currentState == action->forbiddenState) {
            return;
        }

        String payload = action->payload;
        if (action->payload == "TOGGLE") {
            if (currentState == "ON") {
                payload = "OFF";
            } else {
                payload = "ON";
            }
        }
        
        mqttClient.publish(action->topic.c_str(), payload.c_str());
    } else if(action->type == "RESTART") {
      unsigned int line_y = 0;
      display.updateScreen(line_y, true, "Repornire...", "");
      delay(2000);
      ESP.restart();
    }
  }
}
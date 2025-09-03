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

  // Caută acțiunea corespunzătoare electrodului apăsat pe pagina curentă
  const ActionLine* action = configManager.getAction(currentPage, electrode);

  if (action) {
    // Verifică tipul acțiunii
    if (action->type == "CHANGE_PAGE") {
        currentPage = action->nextPage;
        display.menu();
    } 
    else if (action->type == "MQTT") {
        String currentState = getEntityState(action->entity.c_str());
        
        // Verifică dacă acțiunea este interzisă de starea curentă
        if (!action->forbiddenState.isEmpty() && currentState == action->forbiddenState) {
            return; // Nu face nimic
        }

        String payload = action->payload;
        // Gestionează cazul special "TOGGLE"
        if (action->payload == "TOGGLE") {
            if (currentState == "ON") {
                payload = "OFF";
            } else {
                payload = "ON";
            }
        }
        
        mqttClient.publish(action->topic.c_str(), payload.c_str());
    }
    // Tipul "INFO" nu face nimic la atingere
  }
}
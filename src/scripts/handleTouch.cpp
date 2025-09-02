#include "headers/handleTouch.h"
#include "headers/globals.h"
#include "headers/states.h"
#include "headers/display.h"
#include "headers/mqtt.h"
#include "headers/led_control.h"

void handleTouch(uint8_t electrode) {
  char topic[32];
  char payload[11];

  setPinState(2, true);

  if(activeMenu) {
    Serial.println("ActiveMenu");
    switch (currentPage)
    {
      case CONTROL_PAGE: {
        Serial.println("Control Page");
        switch (electrode) {
          case 0:
            Serial.println("case 1");
            if(strcmp(getEntityState(Entity::tapo_0), "ON") == 0) {
              snprintf(topic, sizeof(topic), "home/tapo/0");
              snprintf(payload, sizeof(payload), "OFF");
            }
            if(strcmp(getEntityState(Entity::tapo_0), "OFF") == 0){
              snprintf(topic, sizeof(topic), "home/tapo/0");
              snprintf(payload, sizeof(payload), "ON");
            }
            Serial.println("Publish: "); Serial.print("OFF");
            break;
          case 1:
            Serial.println(getEntityState(Entity::roborock));
            if(strcmp(getEntityState(Entity::roborock), "Cleaning") == 0) return;
            snprintf(topic, sizeof(topic), "home/roborock");
            snprintf(payload, sizeof(payload), "CleanFull");
            break;
          case 2:
            if(strcmp(getEntityState(Entity::roborock), "Docking") == 0) return;
            snprintf(topic, sizeof(topic), "home/roborock");
            snprintf(payload, sizeof(payload), "Docking");
            break;
          case 3:
            if(strcmp(getEntityState(Entity::roborock), "Paused") == 0) return;
            snprintf(topic, sizeof(topic), "home/roborock");
            snprintf(payload, sizeof(payload), "Paused");
            break;
          case 9: {
            currentPage = FORECAST_PAGE;
            display.menu();
            return;
          }
          default:
            return;
        }

      mqttClient.publish(topic, payload);      
      break;
    }
    case FORECAST_PAGE:
      switch (electrode) {
        case 9:
          currentPage = ROOM_PAGE;
          display.menu();
          return;
        default:
          break;
      }
      break;
    case ROOM_PAGE:
      switch (electrode) {
        case 9:
          currentPage = CONTROL_PAGE;
          display.menu();
          return;
        default:
          break;
      }
      break;
    }
  } else {
    display.menu();
  }

}
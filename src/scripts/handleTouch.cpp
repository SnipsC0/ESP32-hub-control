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

  Serial.println("Intra");
  if(activeMenu) {
    switch (currentPage)
    {
      case CONTROL_PAGE: {
        switch (electrode) {
          case 1: {
            if(strcmp(getEntityState(Entity::tapo_0), "ON") == 0) {
              snprintf(topic, sizeof(topic), "home/tapo/0");
              snprintf(payload, sizeof(payload), "OFF");
            } else if(strcmp(getEntityState(Entity::tapo_0), "OFF") == 0){
              snprintf(topic, sizeof(topic), "home/tapo/0");
              snprintf(payload, sizeof(payload), "ON");
            } else return;
            break;
          }
          case 2:
            currentPage = ROBOROCK_PAGE;
            display.menu();
            return;
          case 9: 
            currentPage = FORECAST_PAGE;
            display.menu();
            return;
          default:
           return;
        }
        
        mqttClient.publish(topic, payload);      
        break;
      }
      case ROBOROCK_PAGE: {
        switch(electrode) {
          case 1:
          Serial.println(getEntityState(Entity::roborock));
          if(strcmp(getEntityState(Entity::roborock), "Cleaning") == 0) return;
          snprintf(topic, sizeof(topic), "home/roborock");
          snprintf(payload, sizeof(payload), "Cleaning");
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
          case 4:
          currentPage = CONTROL_PAGE;
          display.menu();
          return;
          default:
          return;
        }
        
        currentPage = CONTROL_PAGE;
        mqttClient.publish(topic, payload);      
        display.menu();
        return;
      }
      case FORECAST_PAGE: {
        switch (electrode) {
          case 9:
          currentPage = ROOM_PAGE;
          display.menu();
          return;
          default:
          return;
        }
      }
      case ROOM_PAGE:{
        switch (electrode) {
          case 9:
          currentPage = CONTROL_PAGE;
          display.menu();
          return;
          default:
          return;
        }
      }
    }
  } else {
    Serial.println("Intra else");
    display.menu();
  }
  
}
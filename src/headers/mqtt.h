#ifndef MQTT_SETUP
#define MQTT_SETUP

#include <Arduino.h>
#include <PubSubClient.h>

class Mqtt {
  public:
    void setup();
    void loop();
    void publish(const char *topic, const char *payload);
  private:
    void subscribe();
    void reconnect();
};

extern Mqtt mqttClient;
extern PubSubClient mqtt;

#endif
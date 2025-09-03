#ifndef _CONFIG_H
#define _CONFIG_H

#define DEEP_SLEEP 0 // 0 || 1
#define DEEP_SLEEP_TIMEOUT_S 30

#define MPR_PIN_SDA 21
#define MPR_PIN_SCL 22
#define MPR_PIN_IRQ 4

#define OLED_PIN_SDA 18
#define OLED_PIN_SCL 19

#define DHT_TYPE DHT22
#define DHT_PIN 23

#define TOUCH_COOLDOWN_S 1
#define RACE_MQTT_TOUCH_COOLDOWN_S 1

#define DISPLAY_DURATION_DEFAULT 30

#endif
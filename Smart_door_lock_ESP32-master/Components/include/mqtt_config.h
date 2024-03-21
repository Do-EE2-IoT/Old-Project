#pragma once

#include <stdint.h>     
#define MQTT_BROKER         "mqtt://46d844e9ca324787ae7359f1fe4bd6a5.s1.eu.hivemq.cloud:1883"
#define TOPIC_SUB           "v1/devices/me/telemetry"
//#define TOPIC_PUB           "VB/DMP/VBEEON/CUSTOM/SMH/DeviceID/Command"
typedef void (*mqtt_data_handle_t) (char *data, uint16_t len);
typedef void (*mqtt_publish_handle_t) (char *topic);
void app_mqtt_init(void);
void app_mqtt_start(void);
void app_mqtt_stop(void);
void app_mqtt_publish(char * topic, char *data, uint16_t len);
void app_mqtt_subscribe(char * topic);
void app_mqtt_set_data_callback(void *cb);
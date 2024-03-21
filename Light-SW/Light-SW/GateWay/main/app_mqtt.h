#ifndef __APP_MQTT_H
#define __APP_MQTT_H
#include <stdint.h>     
#define MQTT_BROKER     "mqtt://45.77.243.33:1883"
#define TOPIC           "v1/devices/me/rpc/request/+"
#define TOPIC_PUB       "v1/devices/me/attributes"
typedef void (*mqtt_data_handle_t) (char *data, uint16_t len);
void mqtt_app_start(void);
void app_mqtt_publish(char * topic, char *data, int len);
void app_mqtt_subscribe(char * topic);
void app_mqtt_set_data_callback(void *cb);
#endif
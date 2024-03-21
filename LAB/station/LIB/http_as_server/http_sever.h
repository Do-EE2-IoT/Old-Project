#ifndef __HTTP_SEVER_H_
#define __HTTP_SEVER_H_

#include "esp_err.h"
#include "esp_http_server.h"
#include "stdint.h"
void start_webserver(void);
void stop_webserver(void);

typedef void (*http_post_slider_t) (char *, int);
typedef void (*http_post_callback_t) (char * , int);
typedef void (*http_get_callback_t)  (void);
typedef void (*http_post_wifi_callback_t) (char *, int);
void http_set_post_switch_callback(void *cb);
void http_set_get_dht_callback(void *cb);
void http_set_post_slider_callback(void *cb);
void take_data_of_get(char *data_of_humidity, int leng);
void http_set_wifi_callback(void *cb);


#endif
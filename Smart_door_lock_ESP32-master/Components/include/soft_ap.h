#pragma once
#include "esp_netif.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"

// Callback typedef
typedef void (*http_post_wifi_callback_t) (char *, char *);

/**
 * Wifi softAP init function.
 */
void wifi_init_soft_ap();

/**
 * Wifi softAP stop function.
 */
void wifi_stop_soft_ap();

/**
 * Sets the callback function.
 */
void http_set_wifi_callback(void *cb);
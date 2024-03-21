#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"
#include "output_iot.h"
#include "wifi_application.h"
#include "BLE_app.h"
#include "http_sever_ble_app.h"

extern char copy_blu_message[20];
#define MAC_ADDR_SIZE 6


static void get_mac_address()
{
    uint8_t mac[MAC_ADDR_SIZE];
    esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);
    ESP_LOGI("MAC address", "MAC address: %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void app_main()
{
    esp_err_t err = nvs_flash_init(); 
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NOT_FOUND){
       nvs_flash_erase();
       nvs_flash_init();
    }
    BLE_start();
    wifi_init_sta();
    get_mac_address();
    mqtt_app_start();
    
                           
    
}

idf_component_register(SRCS "key_pad.c" "soft_ap.c" "jm101.c" "wifi_provision.c" "mqtt_config.c"
                    REQUIRES driver esp_wifi nvs_flash esp_http_server wifi_provisioning mqtt
                    INCLUDE_DIRS ".")

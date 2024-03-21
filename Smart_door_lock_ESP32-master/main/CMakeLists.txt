idf_component_register(SRCS "Smart_door_lock_ESP32.c"
					REQUIRES driver nvs_flash esp_http_server Components
                    INCLUDE_DIRS ".")

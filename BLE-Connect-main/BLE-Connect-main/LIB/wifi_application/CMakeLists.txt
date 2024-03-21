set(pri_req nvs_flash)
idf_component_register(SRCS "wifi_application.c"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash)
set(pri_req nvs_flash esp_http_server)

idf_component_register(SRCS  "http_serve.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})
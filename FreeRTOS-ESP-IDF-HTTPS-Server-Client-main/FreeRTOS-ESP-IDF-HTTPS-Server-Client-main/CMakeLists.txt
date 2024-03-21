set(pri_req esp_https_server esp_http_client)
idf_component_register(SRCS "proj4.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES "crt.pem"
                    REQUIRES ${pri_req})

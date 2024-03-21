set(pri_req bt)
idf_component_register(SRCS "BLE_app.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})
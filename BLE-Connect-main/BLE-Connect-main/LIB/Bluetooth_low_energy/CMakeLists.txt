set(pri_req bt output_iot mqtt)
idf_component_register(SRCS "BLE_app.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})
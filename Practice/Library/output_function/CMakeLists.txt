set(pri_req nvs_flash)
idf_component_register(SRCS "output.c" 
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})
set(pre_req nvs_flash)
idf_component_register(SRCS "input.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
                    
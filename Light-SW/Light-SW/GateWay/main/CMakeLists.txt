set(srcs "main.c" "app_mqtt.c" 
        "board.c"
        "app_config.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS  ".")

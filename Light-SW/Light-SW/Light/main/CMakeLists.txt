set(srcs "main.c"
    "board.c"
    "bee_app_nvs.c")

idf_component_register(SRCS "bee_app_nvs.c" "${srcs}"
                    INCLUDE_DIRS  ".")

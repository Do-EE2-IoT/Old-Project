/*****************************************************************************
 *
 * @file 	bee_app_nvs.c
 * @author 	biennd
 * @date 	13 Jun 2023
 * @brief	All functions relate to init and exculate flash 
 *
 ***************************************************************************/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "bee_app_nvs.h"


static nvs_handle_t my_handle;


/********************Sending int data to flash************************/
void app_nvs_set_value(char *keyWord, int value)
{
    int err;
    err = nvs_open(USER_NEWSPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error\n");
    } else {
        printf("Done\n");
    }
    err = nvs_set_i32(my_handle, keyWord, value);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
    printf("Committing updates in NVS ... ");
    err = nvs_commit(my_handle);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
    nvs_close(my_handle);
}


/********************Sending string data to flash************************/
void app_nvs_set_string(char *keyWord, char *str)
{
    int err;
    err = nvs_open(USER_NEWSPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error\n");
    } else {
        printf("Done\n");
    }
    err = nvs_set_str(my_handle, keyWord, str);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
    printf("Committing updates in NVS ... ");
    err = nvs_commit(my_handle);
    printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
        // Close
    nvs_close(my_handle);
}



/********************Getting int data to flash************************/
void app_nvs_get_value(char *keyWord, int *value)
{
    int err;
    err = nvs_open(USER_NEWSPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error\n");
    } else {
        printf("Done\n");
    }
    
    err = nvs_get_i32(my_handle, keyWord, value);
        switch (err) {
            case ESP_OK:
                printf("Done\n");
                printf("%s: %d\n", keyWord, *value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
        nvs_close(my_handle);
}



/********************Getting string data to flash************************/
void app_nvs_get_string(char *keyWord, char *value)
{
    int err;
    size_t length = 0;
    err = nvs_open(USER_NEWSPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error\n");
    } else {
        printf("Done\n");
    }
    
    err = nvs_get_str(my_handle, keyWord, value, &length);
    err =0;
    err = nvs_get_str(my_handle, keyWord, value, &length);
        switch (err) {
            case ESP_OK:
                printf("Done\n");
                printf("%s - %s, length= %d\n", keyWord, value, length);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
        nvs_close(my_handle);
}
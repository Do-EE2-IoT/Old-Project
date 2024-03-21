#include <stdio.h>
#include <string.h>
#include "jm101.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>
#include "freertos/event_groups.h"
#include <keypad.h>
#include "soft_ap.h"
#include "driver/uart.h"
#include "esp_sleep.h"
#include "wifi_provision.h"
#include "mqtt_config.h"


/// @brief Task for keypad input
TaskHandle_t keypad_input_task_handle;
void Task_keypad_input(void* parameter);

/// @brief Task for Fingerprint UART
TaskHandle_t fingerprint_task_handle;
void Task_for_fingerprint(void *parameter);

/// @brief Task for Wifi Provision
TaskHandle_t wifi_provision_task_handle;
void Task_for_wifi_provision(void *parameter);

/// @brief Task for MQTT Communicate
TaskHandle_t mqtt_communicate_task_handle;
void Task_for_mqtt_communicate(void *parameter);

#define ON_CHANGE 1
#define DISABLE 0

EventGroupHandle_t KEYPAD_CHANGE;   // Keypad condition

char default_address[4] = {0xFF, 0xFF, 0xFF, 0xFF};         //++ Default Module Address is FF:FF:FF:FF
char default_password[4] = {0x00, 0x00, 0x00, 0x00};        //++ Default Module Password is 00:00:00:00

char password[16];
char key[6] = "123456";
int the_position = 0;

char topic[] = "SmartDoorLock"; 
char data[] = "Openned" ;

void callback()
{
    gpio_set_level(2, 1);
    app_mqtt_publish(topic, data, strlen(data));
}

void app_main() {

    esp_sleep_enable_gpio_wakeup();
    
    /// Flash memory init
    esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

    /// keypad pinout
    ///                     R1  R2  R3  R4  C1  C2  C3  C4 
    gpio_num_t keypad[8] = {13, 12, 14, 27, 26, 25, 33, 32};
    /// Initialize keyboard
    keypad_initalize(keypad);
    gpio_intr_disable(2);
    gpio_set_direction(2, GPIO_MODE_OUTPUT);

    /// Initialized fingerprint sensor
    jm101_init();
    app_mqtt_init();

    xTaskCreate(Task_keypad_input, "Input",4096,NULL,8,&keypad_input_task_handle);
    xTaskCreate(Task_for_fingerprint, "Fingerprint",4096,NULL,7,&fingerprint_task_handle);
    xTaskCreate(Task_for_wifi_provision, "Wifi prov",4096,NULL,7,&wifi_provision_task_handle);
    xTaskCreate(Task_for_mqtt_communicate, "Mqtt com",4096,NULL,7,&mqtt_communicate_task_handle);
}

void Task_keypad_input(void* parameter){
    int enable_door_lock = 0;
    while(true)
    {
        char keypressed = keypad_getkey();  /// gets from key queue
        if(keypressed != '\0')
        {
            // xEventGroupSetBits(KEYPAD_CHANGE,ON_CHANGE); 
            password[the_position%16] = keypressed;
            the_position++;
            printf("password: %s\n", password);
            for(int i = 0; i < the_position - 5; i++){
                if (password[i] == key[0] && password[i+1] == key[1] && password[i+2] == key[2] && password[i+3] == key[3] && password[i+4] == key[4] && password[i+5] == key[5])
                {
                    printf("OK");
                    enable_door_lock = 1;
                }
            }
            if (keypressed == '#')
            {
                if (enable_door_lock)
                {
                    enable_door_lock = 0;
                    the_position = 0;
                    gpio_set_level(2, 1);
                }
                else{
                    the_position = 0;
                    gpio_set_level(2, 0);
                }                
            }       
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void Task_for_fingerprint(void *parameter){
    uint8_t confirmation_code = 0x01;
    confirmation_code = VfyPwd(default_address, default_password);      //++ Performs Password Verification with Fingerprint Module
    if(confirmation_code == 0x00)
    {
        set_fingerprint_confirm_callback(&callback);
        printf("R307 FINGERPRINT MODULE DETECTED\n");
        TempleteNum(default_address);
        while (1)
        {
            GR_Auto(default_address);
            vTaskDelay(100);
        }
    }else
    {
        printf("R307 FINGERPRINT MODULE NOT DETECTED\n");
    }
    vTaskDelete(NULL);
}

void Task_for_wifi_provision(void* parameter){
    init_wifi_provisioning_via_ble();
    vTaskDelete(NULL);
}

void Task_for_mqtt_communicate(void* parameter){
    app_mqtt_start();
    app_mqtt_publish(topic, data, strlen(data));
    vTaskDelete(NULL);    
}







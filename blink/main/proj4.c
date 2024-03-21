#include <stdio.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"


#include <esp_event.h>
#include <esp_system.h>
#include <esp_https_server.h>
#include "driver/i2c.h"



#include "lcd.h"

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

#define PIN_NUM_MISO GPIO_NUM_19
#define PIN_NUM_MOSI GPIO_NUM_23
#define PIN_NUM_CLK  GPIO_NUM_18
#define PIN_NUM_CS   GPIO_NUM_5
#define PIN_NUM_RST  GPIO_NUM_27



void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    lcd_init();
    ESP_LOGI("i2c ", "I2C initialized successfully");
    while(1){
        vTaskDelay(1000/portTICK_PERIOD_MS);
        lcd_put_cur(0,0);


        lcd_send_string("Hello World");
    }
}

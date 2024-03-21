#include "stdio.h"
#include "esp_log.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "output_iot.h"



void output_io_create(gpio_num_t gpio_num){
     gpio_pad_select_gpio(gpio_num);
     // set the GPIO as output // 
     gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
     

     
}

void output_set_level(gpio_num_t gpio_num , type_output type){
   gpio_set_level(gpio_num, type);

}

int x = 1;
void output_toggle_pin(gpio_num_t gpio_num){
   gpio_set_level(gpio_num, 1 - x);
   x = 1 - x;
}



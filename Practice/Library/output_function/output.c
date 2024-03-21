#include "driver/gpio.h"
#include "output.h"


void output_init(int gpio_num){
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num,GPIO_MODE_OUTPUT);
}

void output_high(int gpio_num){
    gpio_set_level(gpio_num,HIGH);
}

void output_low(int gpio_num){
    gpio_set_level(gpio_num,LOW);
}

int output_get_level(int gpio_num){
return gpio_set_level(gpio_num);
}

void output_toggle(int gpio_num){
   int x = gpio_get_level(gpio_num);
   if(x == 1){
    output_low(gpio_num);
   } 
   else{
    output_high(gpio_num);
   }
}
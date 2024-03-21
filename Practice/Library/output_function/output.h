#ifndef __OUTPUT_H_
#define __OUTPUT_H_
#include "driver/gpio.h"

typedef enum LEVEL {
    HIGH = 1,
    LOW = 0
};

void output_init(int gpio_num);
void output_high(int gpio_num);
void output_low(int gpio_num);
int output_get_level(int gpio_num);
void output_toggle(int gpio_num);


#endif
#ifndef __INPUT_IOT_H_
#define __INPUT_IOT_H_

#include "esp_err.h"
#include "hal/gpio_types.h"
#include "driver/uart.h"


typedef void (*input_callback_t) (int,uint64_t);


typedef enum {
    GPIO_INTR_rising= 1,     /*!< GPIO interrupt type : rising edge                  */
    GPIO_INTR_falling = 2,     /*!< GPIO interrupt type : falling edge                 */
    GPIO_INTR_ANY = 3,     /*!< GPIO interrupt type : both rising and falling edge */
    
} type_interrupt;

void input_io_create(gpio_num_t gpio_num, type_interrupt type);
void input_get_level(gpio_num_t gpio_num );
void input_set_callback(void *cb);
void uart_init(uart_port_t uart_num,QueueHandle_t QUEUART, int TXpin_IO, int RXpin_IO,int rts_io_num, int cts_io_num);


#endif
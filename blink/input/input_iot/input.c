#include "stdio.h"
#include "esp_log.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "input_iot.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "string.h"
#include "esp_log.h"
#include "input_iot.h"

// ************************************************* INPUT INTERRUPT*************************************//
input_callback_t inputcall_back = NULL;
 static uint64_t start, end;
 static uint64_t tick;

static void IRAM_ATTR gpio_input_hanler(void *arg){
          int gpio_num = (uint32_t) arg;

          if(gpio_get_level(gpio_num) == 0){
            start = xTaskGetTickCountFromISR();
          }
          else{
            end = xTaskGetTickCountFromISR();
            tick = end - start;
            inputcall_back(gpio_num, tick);
          }
}



void input_io_create(gpio_num_t gpio_num, type_interrupt type){
     gpio_pad_select_gpio(gpio_num);
     gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
     gpio_set_pull_mode(gpio_num,GPIO_PULLUP_ONLY);
     gpio_set_intr_type(gpio_num,type);
     gpio_install_isr_service(0);
     gpio_isr_handler_add(gpio_num,gpio_input_hanler,(void*) gpio_num);
     
}



void input_set_callback(void *cb){
   inputcall_back = cb;
}



// **********************************uart interrupt ***********************************//
/*The process of sending data involves the following steps:

Write data into Tx FIFO buffer

FSM serializes the data

FSM sends the data out

- The process of receiving data is similar, but the steps are reversed:

FSM processes an incoming serial stream and parallelizes it

FSM writes the data into Rx FIFO buffer

Read the data from Rx FIFO buffer
*/
 /* Configure parameters of an UART driver,
     * communication pins and install the driver */





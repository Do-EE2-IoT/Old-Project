#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "output.h"

// initialize variable and task
TaskHandle_t task_for_output;
void Task_for_output(void *parameter);

void app_main(void){
output_init(GPIO_NUM_2);
xTaskCreate(Task_for_output,"task_for_output",1024,NULL,1,&task_for_output);




}

void Task_for_output(void *parameter){
   int x = output_get_level(GPIO_NUM_2);
    printf("%d", x);
    output_toggle(GPIO_NUM_2);
    vTaskDelay(1000/portTICK_RATE_MS);
}
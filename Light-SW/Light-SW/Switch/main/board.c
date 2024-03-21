#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"

#include "iot_button.h"
#include "board.h"

#define TAG "BOARD"

#define BUTTON_IO_NUM           5
#define BUTTON_ACTIVE_LEVEL     0

extern void example_ble_mesh_send_gen_onoff_set(uint8_t button_state);
extern void example_ble_mesh_send_gen_onoff_get(void);
extern int state_button;

static void button_tap_cb(void* arg)
{
    ESP_LOGI(TAG, "tap cb (%s)", (char *)arg);
    state_button = 1 - state_button;
    /*Sending Set message*/
    example_ble_mesh_send_gen_onoff_set(state_button);
    /*Sending Get message*/
    // example_ble_mesh_send_gen_onoff_get();
}

static void board_button_init(void)
{
    button_handle_t btn_handle = iot_button_create(BUTTON_IO_NUM, BUTTON_ACTIVE_LEVEL);
    if (btn_handle) {
        iot_button_set_evt_cb(btn_handle, BUTTON_CB_RELEASE, button_tap_cb, "RELEASE");
    }
}

void board_init(void)
{
    board_button_init();
}

/* board.c - Board-specific hooks */

/*
 * SPDX-FileCopyrightText: 2017 Intel Corporation
 * SPDX-FileContributor: 2018-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "board.h"

static void board_led_init(void)
{
        gpio_reset_pin(GPIO_NUM_2);
        gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
        gpio_set_level(GPIO_NUM_2, LED_OFF);
}

void board_init(void)
{
    board_led_init();
}

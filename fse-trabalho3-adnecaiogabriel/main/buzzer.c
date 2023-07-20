#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

void buzzer(){
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(1000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(1000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
}

void buzzer1(){
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(1000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 1);
}

void buzzer2(){
    gpio_set_level(2, 1);
    vTaskDelay(2000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
}
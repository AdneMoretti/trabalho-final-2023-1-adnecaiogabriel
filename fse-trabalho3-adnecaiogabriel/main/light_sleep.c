#include <time.h>
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "freertos/semphr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "json_parser.h"
# define TAG "LIGHT_SLEEP"
#include "wifi.h"
#include "mqtt.h"
#define GPIO_HALL 15

int magnetic_signal;

void light_sleep_task() {
    ESP_LOGI(TAG, "Entrando em modo light sleep");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    esp_light_sleep_start();

    esp_sleep_wakeup_cause_t causa = esp_sleep_get_wakeup_cause();
    if(causa != ESP_SLEEP_WAKEUP_TIMER){
        magnetic_signal = gpio_get_level(GPIO_HALL);
        send_magnetic_signal(magnetic_signal);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("O %s me acordou !\n", causa == ESP_SLEEP_WAKEUP_TIMER ? "TIMER" : "HALL");
    wifi_start();
}

void wake_up_with_hall() {
    esp_err_t err;
    gpio_wakeup_enable(GPIO_HALL, GPIO_INTR_LOW_LEVEL);
    err = esp_sleep_enable_gpio_wakeup();
    esp_sleep_enable_timer_wakeup(5 * 1000000);
    ESP_ERROR_CHECK(err);
}
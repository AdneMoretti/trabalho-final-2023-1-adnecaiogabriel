#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "json_parser.h"

#define TAG "SOUND"
#define SOUND_GPIO 19

void check_sound() {
   vTaskDelay(2000 / portTICK_PERIOD_MS);
   int sound;
   while (true) {
    sound = gpio_get_level(SOUND_GPIO);
    ESP_LOGI(TAG, "SOUND: %d", sound);
    
    send_sound(&sound);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
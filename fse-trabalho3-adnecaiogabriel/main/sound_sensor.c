#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "json_parser.h"

#define TAG "sound"
#define SOUND_GPIO 19

volatile bool soundAlert = true;

void check_sound() {
   vTaskDelay(2000 / portTICK_PERIOD_MS);
   int sound;
   while (true) {
    sound = gpio_get_level(SOUND_GPIO);
    ESP_LOGI(TAG, "sound: %d", sound);
    
    if (sound == 1) {
      send_sound_alert(&soundAlert);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    // sound = sound/4;
    send_sound_telemetry(&sound);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
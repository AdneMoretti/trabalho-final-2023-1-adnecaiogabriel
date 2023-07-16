#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "json_parser.h"

#include "adc_module.h"

#define TAG "sound"
#define SOUND_GPIO 19

void check_sound() {
   vTaskDelay(2000 / portTICK_PERIOD_MS);
   int sound;
   int soundAlert = false;
   while (true) {
    sound = analogRead(SOUND_GPIO);
    ESP_LOGI(TAG, "sound: %d", sound);
    
    if (sound == 1) {
      soundAlert = true;
      send_sound_alert(&soundAlert);
    }
    else {
      soundAlert = false;
      send_sound_alert(&soundAlert);
    }
    
    // sound = sound/4;
    send_sound_telemetry(&sound);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
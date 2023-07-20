#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "global.h"
#include "json_parser.h"
#include "security.h"
#include "nvs.h"
#include "LED.h"
#define TOUCH_SENSOR 15
#define BUZZER_GPIO 2
#define BUTTON_GPIO 0

#define TAG "SECURITY"

#define ESP_CONFIG_NUMBER CONFIG_ESP_CONFIG_NUMBER

void security(char *tag)
{   
  char *ALARM_TAG = (char *)tag;
  esp_rom_gpio_pad_select_gpio(BUZZER_GPIO);   
  esp_rom_gpio_pad_select_gpio(TOUCH_SENSOR);
  gpio_set_direction(TOUCH_SENSOR, GPIO_MODE_INPUT);
  gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);  

  if (ESP_CONFIG_NUMBER == 1) {
    gpio_set_level(BUZZER_GPIO,0);
  }

  grava_valor_nvs(1,"alarme");
  grava_string_nvs(ALARM_TAG,"tag");

  while (le_valor_nvs("alarme") == 1){
    if (ESP_CONFIG_NUMBER == 1) {
      if (gpio_get_level(TOUCH_SENSOR) == 1 || gpio_get_level(BUTTON_GPIO) == 1) { //SE A LEITURA DO TOUCH_SENSOR FOR IGUAL A HIGH, FAZ
        ESP_LOGI(TAG, "Desligou alarme!");
        send_stop_alarm(ALARM_TAG); //desliga o alarme
        gpio_set_level(BUZZER_GPIO,0);
        grava_valor_nvs(0,"alarme");
        vTaskDelay(100 / portTICK_PERIOD_MS);
      } else {
        gpio_set_level(BUZZER_GPIO, 1); //ACENDE O alarme
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    }
    if (ESP_CONFIG_NUMBER == 2) {
      if (gpio_get_level(BUTTON_GPIO) == 0) { //SE A LEITURA DO TOUCH_SENSOR FOR IGUAL A HIGH, FAZ
        ESP_LOGI(TAG, "Desligou alarme!");
        send_stop_alarm(ALARM_TAG); //desliga o alarme
        grava_valor_nvs(0,"alarme");
        vTaskDelay(100 / portTICK_PERIOD_MS);
      } else {
        flashLEDs(ALARM_TAG);
        grava_valor_nvs(1,"alarme");
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
    }
  }
  vTaskDelete(NULL); // Encerra task
}
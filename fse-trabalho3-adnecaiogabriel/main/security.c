#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "global.h"
#include "security.h"
#include "nvs.h"
#define BUZZER_GPIO 2
#define SENSOR 15

void security()
{
  const char* alarme = "alarme";   
  esp_rom_gpio_pad_select_gpio(BUZZER_GPIO);   
  esp_rom_gpio_pad_select_gpio(SENSOR);
  gpio_set_direction(SENSOR, GPIO_MODE_INPUT);
  gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);  

  gpio_set_level(BUZZER_GPIO,0);
  while (1){
    if(gpio_get_level(SENSOR) == 1){ //SE A LEITURA DO SENSOR FOR IGUAL A HIGH, FAZ
      printf("Desligou alarme");
      gpio_set_level(BUZZER_GPIO, 0); //desliga o alarme
      grava_valor_nvs(0,alarme);
      return;
    }else if(gpio_get_level(SENSOR) == 0){ //SE A LEITURA DO SENSOR FOR IGUAL A LOW, FAZ
      printf("Ligado o Alarme");
      gpio_set_level(BUZZER_GPIO, 1); //ACENDE O alarme
      vTaskDelay(100 / portTICK_PERIOD_MS);
      grava_valor_nvs(1,alarme);
    }
  }
}
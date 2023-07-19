#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "security_sistem.h"
#include "global.h"
#include "security.h"
#define BUZZER_GPIO 2
#define SENSOR 15

void security(int dado)
{
  // Configuração dos pinos dos LEDs 
  esp_rom_gpio_pad_select_gpio(BUZZER_GPIO);   
  esp_rom_gpio_pad_select_gpio(SENSOR);
  // Configura os pinos dos LEDs como Output
  gpio_set_direction(SENSOR, GPIO_MODE_INPUT);
  gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);  

  // Configuração do pino do Botão
  // Configura o pino do Botão como Entrada
  // Configura o resistor de Pulldown para o botão (por padrão a entrada estará em Zero)  
  gpio_set_level(BUZZER_GPIO,0);
  ALARME=dado;
  // Testa o Botão utilizando polling
  while (ALARME==1){
    if(gpio_get_level(SENSOR) == 1){ //SE A LEITURA DO SENSOR FOR IGUAL A HIGH, FAZ
      printf("Desligou alarme");
      gpio_set_level(BUZZER_GPIO, 0); //desliga o alarme
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ALARME=0;
    }else if(gpio_get_level(SENSOR) == 0){ //SE A LEITURA DO SENSOR FOR IGUAL A LOW, FAZ
      printf("Ligado o Alarme");
      gpio_set_level(BUZZER_GPIO, 1); //ACENDE O alarme
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}
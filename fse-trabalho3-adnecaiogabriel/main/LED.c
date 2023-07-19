#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include "esp_err.h"

void flashLEDs(char *alarmTag) {
  int r = 0, g = 0, b = 0;

  switch (hash(alarmTag)) {
    case hash("LOW_TEMPERATURE"):
      // Aviso de luz Azul
      r = 255;
      g = 255;
      b = 0;
      break;
    case hash("HIGH_TEMPERATURE"):
      // Aviso de luz Laranja
      r = 0;
      g = 90;
      b = 255;
      break;
    case hash("LOW_UMIDITY"):
      // Aviso de luz Marrom
      r = 116;
      g = 186;
      b = 236;
      break;
    case hash("HIGH_UMIDITY"):
      // Aviso de luz Roxa
      r = 127;
      g = 255;
      b = 127;
      break;
    case hash("SOUND"):
      // Aviso de cor Vermelha
      r = 0;
      g = 255;
      b = 255;
      break;
    case hash("HALL"):
      // Aviso de cor Branca
      r = 0;
      g = 0;
      b = 0;
      break;
    default:
      // Caso a tag não seja reconhecida, desliga os LEDs
      r = 255;
      g = 255;
      b = 255;
      break;
  }

  ledcAttachPin(LED_PIN_R, 0);
  ledcAttachPin(LED_PIN_G, 1);
  ledcAttachPin(LED_PIN_B, 2);

  ledcSetup(0, 12000, 8);  // Canal 0, frequência de 12 kHz, resolução de 8 bits
  ledcSetup(1, 12000, 8);  // Canal 1, frequência de 12 kHz, resolução de 8 bits
  ledcSetup(2, 12000, 8);  // Canal 2, frequência de 12 kHz, resolução de 8 bits

  ledcWrite(0, r);  // LED vermelho
  ledcWrite(1, g);  // LED verde
  ledcWrite(2, b);  // LED azul

  vTaskDelay(500 / portTICK_PERIOD_MS);  // Aguarda 0,2 segundo

  // Desliga os LEDs
  ledcWrite(0, 255);  // LED vermelho
  ledcWrite(1, 255);  // LED verde
  ledcWrite(2, 255);  // LED azul

  vTaskDelay(500 / portTICK_PERIOD_MS);
}

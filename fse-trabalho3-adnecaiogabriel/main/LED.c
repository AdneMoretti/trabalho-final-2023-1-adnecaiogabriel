#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "driver/adc.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LED_RED_GPIO 23
#define LED_GREEN_GPIO 22
#define LED_BLUE_GPIO 21

void flashLEDs(char *alarmTag) {
  int r = 1, g = 1, b = 1;

  if (strcmp(alarmTag, "LOW_TEMPERATURE") == 0) {
    // Blue light warning
    r = 1;
    g = 1;
    b = 0;
  } else if (strcmp(alarmTag, "HIGH_TEMPERATURE") == 0) {
    // Orange light warning
    r = 0;
    g = 0;
    b = 1;
  } else if (strcmp(alarmTag, "LOW_UMIDITY") == 0) {
    // Brown light warning
    r = 1;
    g = 0;
    b = 0;
  } else if (strcmp(alarmTag, "HIGH_UMIDITY") == 0) {
    // Purple light warning
    r = 0;
    g = 1;
    b = 0;
  } else if (strcmp(alarmTag, "SOUND_ALARM") == 0) {
    // Red light warning
    r = 0;
    g = 1;
    b = 1;
  } else if (strcmp(alarmTag, "HALL") == 0) {
    // White light warning
    r = 0;
    g = 0;
    b = 0;
  } else {
    // Turn off light if not recognized
    r = 1;
    g = 1;
    b = 1;
  }

  gpio_set_level(LED_RED_GPIO, r);
  gpio_set_level(LED_GREEN_GPIO, g);
  gpio_set_level(LED_BLUE_GPIO, b);

  vTaskDelay(500 / portTICK_PERIOD_MS);

  // Desliga os LEDs
  gpio_set_level(LED_RED_GPIO, 1);
  gpio_set_level(LED_GREEN_GPIO, 1);
  gpio_set_level(LED_BLUE_GPIO, 1);

  vTaskDelay(500 / portTICK_PERIOD_MS);
}

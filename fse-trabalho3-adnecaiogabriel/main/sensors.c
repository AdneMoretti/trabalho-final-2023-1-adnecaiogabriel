#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#include "gpio_setup.h"
#include "adc_module.h"

#define ESP_CONFIG_NUMBER CONFIG_ESP_CONFIG_NUMBER

// Configuração dos pinos
#define SENSOR_PIN 19

void setup_analog_sensors()
{
    adc_init(ADC_UNIT_1);

    pinMode(SENSOR_PIN, GPIO_INPUT);
}
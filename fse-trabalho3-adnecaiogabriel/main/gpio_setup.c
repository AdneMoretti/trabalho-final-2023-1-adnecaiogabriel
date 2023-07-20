#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_err.h"

#define LED_RED_GPIO 23
#define LED_GREEN_GPIO 22
#define LED_BLUE_GPIO 21
#define SOUND_GPIO 19
#define HALL_PIN 15
#define DHT11_PIN 4
#define BUZZER_PIN 2
#define BUTTON_GPIO 0

void configure_ESP_BUTTON(void)
{
    esp_rom_gpio_pad_select_gpio(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
}

void configure_BUZZER(void)
{
    esp_rom_gpio_pad_select_gpio(BUZZER_PIN);
    gpio_reset_pin(BUZZER_PIN);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
}

void configure_DHT11(void)
{
    // gpio_set_direction(DHT11_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT11_PIN, 0);
    // ets_delay_us(20 * 1000);
    gpio_set_level(DHT11_PIN, 1);
    // ets_delay_us(40);
    gpio_set_direction(DHT11_PIN, GPIO_MODE_INPUT);
}

void configure_HALL(void)
{
    esp_rom_gpio_pad_select_gpio(HALL_PIN);
    gpio_set_direction(HALL_PIN, GPIO_MODE_INPUT);
}

void configure_LED(void)
{
    esp_rom_gpio_pad_select_gpio(LED_RED_GPIO);
    gpio_set_direction(LED_RED_GPIO, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(LED_GREEN_GPIO);
    gpio_set_direction(LED_GREEN_GPIO, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(LED_BLUE_GPIO);
    gpio_set_direction(LED_BLUE_GPIO, GPIO_MODE_OUTPUT);
    
    gpio_set_level(LED_RED_GPIO, 1);
    gpio_set_level(LED_GREEN_GPIO, 1);
    gpio_set_level(LED_BLUE_GPIO, 1);
}

void configure_SOUND(void)
{
    esp_rom_gpio_pad_select_gpio(SOUND_GPIO);
    gpio_set_direction(SOUND_GPIO, GPIO_MODE_INPUT);
}
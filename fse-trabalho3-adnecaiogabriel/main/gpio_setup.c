#include "driver/gpio.h"

#define LED_RED_GPIO 23
#define LED_GREEN_GPIO 22
#define LED_BLUE_GPIO 21
#define SOUND_GPIO 19
#define BUZZER_GPIO 2

void configure_SOUND(void)
{
    esp_rom_gpio_pad_select_gpio(SOUND_GPIO);
    gpio_set_direction(SOUND_GPIO, GPIO_MODE_INPUT);
}

void configure_BUZZER(void)
{
    esp_rom_gpio_pad_select_gpio(SOUND_GPIO);
    gpio_set_direction(SOUND_GPIO, GPIO_MODE_INPUT);
}

void configure_dht11(void)
{
    esp_rom_gpio_pad_select_gpio(BUZZER_GPIO);
    gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);
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
